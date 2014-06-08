
local map = function(f, ...)				-- XXX: put into some module?
	local function map(x, ...)
		if x == nil then return end
		return f(x), map(...)
	end
	return map(...)
end

local array_extend = function(t, t2)		-- XXX: put into some module?
	local i = #t + 1
	for _,v in ipairs(t2) do
		t[i] = v
		i = i + 1
	end
	return t
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




	local ops = {}		-- operations
	local match_pos
	local src, src_len
	local color_marks	-- colorize when matching
	local colors		-- when match success, move color_marks here

	local colorize = function(s, e, color)
		local pos2d = function(pos)	-- convert 1d address into 2d coordinates
			local y = 1
			local lastp = 1
			for p in src:gmatch("\n()") do
				if pos >= p then
					y = y + 1
					lastp = p
				else
					break
				end
			end
			return y, pos - lastp + 1
		end
		local y1, x1 = pos2d(s)
		local y2, x2 = pos2d(e-1)	-- e: pos of the char after the matched
		x2 = x2 + 1
		color_marks[#color_marks+1] = { y1, x1, y2, x2, color }
	end

	local function execute(instr)
		if type(instr) == 'string' then
			local i = rules[instr]
			assert(i, "no such function called " .. instr)
			return execute(i)
		end
		if not instr.op then return true end
		local op = ops[instr.op]
		assert(op, "unknown instruction " .. instr.op)
		return op(table.unpack(instr))
	end

	-- all ops functions return true when success
	ops.seq = function(...)
		local function seq(instr, ...)
			if not instr then return true end
			if not execute(instr) then return end
			return seq(...)
		end
		return seq(...)
	end

	ops.br = function(...)
		local function br(instr, ...)
			if not instr then return end
			if execute(instr) then return true end
			return br(...)
		end
		return br(...)
	end

	ops.call = function(name)
		return execute(name)
	end

	ops.match = function(pattern)
		local pos = select(-1, src:match("^%s*(" .. pattern .. ")()", match_pos))
		if pos then
			match_pos = pos
			return true
		end
	end

	ops.color = function(color, instr)
		-- skip blanks
		match_pos = src:match("^%s*()", match_pos)

		local s = match_pos
		if execute(instr) then
			colorize(s, match_pos, color)
			return true
		end
	end

	local gexecute = function(instr)
		match_pos = 1
		while match_pos <= src_len do
			color_marks = {}
			if execute(instr) then
				array_extend(colors, color_marks)
			else
				match_pos = match_pos + 1
			end
		end
	end

	local analyze = function(source)
		src = source
		src_len = src:len()
		colors = {}
		for _,v in ipairs(exports) do gexecute(v) end
		return colors
	end

	return analyze
end

return analyzer

