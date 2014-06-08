
local map = function(f, ...)		-- XXX: put into some module?
	local function map(x, ...)
		if x == nil then return end
		return f(x), map(...)
	end
	return map(...)
end

local analyzer = function(ast)
	local rules = {}
	local exports = {}
	local rule_action = {}

	local tmpname
	do
		t = 0
		tmpname = function()
			t = t + 1
			return "T" .. t
		end
	end


	local rule = function(r)
		local action = rule_action[r.tp]
		assert(action, "cannot deal with " .. r.tp)
		return action(r)
	end

	local syntax = function(syn)
		local name = syn.name.id
		local instr = rule(syn.rule)
		if syn.name.color then
			instr = { op='color', syn.name.color[1], instr }
		end
		rules[name] = instr
		if not syn.tmp then exports[#exports+1] = name end
	end

	---- rule actions
	rule_action.seq = function(r)
		if #r == 1 then return rule(r[1]) end
		return { op='seq', map(rule, table.unpack(r)) }
	end

	rule_action.rule = function(r)
		if #r == 1 then return rule(r[1]) end
		return { op='br', map(rule, table.unpack(r)) }
	end

	rule_action.name = function(r)
		local instr = { op='call', r.id }
		if r.color then instr = { op='color', r.color[1], instr } end
		return instr
	end

	rule_action.pattern = function(r)
		local instr = { op='match', r.id }
		if r.color then instr = { op='color', r.color[1], instr } end
		return instr
	end

	rule_action['?'] = function(r)
		return { op='br', rule(r[1]), {} }
	end

	rule_action['*'] = function(r)
		local t = tmpname()
		rules[t] = {
			op='br',
			{
				op='seq',
				rule(r[1]),
				{ op='call', t }
			},
			{}
		}
		return { op='call', t }
	end

	rule_action['+'] = function(r)
		local t = tmpname()
		local instr = rule(r[1])
		rules[t] = {
			op='br',
			{
				op='seq',
				instr,
				{ op='call', t }
			},
			{}
		}
		instr = { op='seq', instr, { op='call', t} }
		return instr
	end

	-- convert ast to instructions
	for _,syn in ipairs(ast) do syntax(syn) end

	--DEBUG
	local dump = require 'dump'
	print(dump(exports))
	print(dump(rules))

	local analyze = function(src)
	end

	return analyze
end

return analyzer

