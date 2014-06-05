
local parser = function(lex)
	local tktype = ''	-- token type
	local tkid   = ''	-- token identity

	local advance = function()
		tktype, tkid = lex()
	end

	local match = function(type, id)
		if type and tktype ~= type then return end
		if id   and tkid   ~= id   then return end
		return true
	end

	local die = function(type, id)
		while tktype do
			print(tktype, tkid)
			advance()
		end
		if type or id then
			local errid = id and ("%q"):format(id) or ''
			local errtype = type and ("[%s]"):format(type) or ''
			error(("error: %s%s expected"):format(errid, errtype))
		else
			error("error: nothing expected, WTF!??")
		end
	end

	local match_or_die = function(type, id)
		return match(type, id) or die(type, id)
	end

	local color = function()
		if not match('sym', '{') then return end
		advance()

		match_or_die('id')
		local color = tkid
		advance()

		match_or_die('sym', '}')
		advance()

		return color
	end

	local name = function()
		if not match('id') then return end
		local t = { name = tkid }
		advance()
		t.color = color()
		return t
	end

	local atom = function()
		local atom = name()
		if atom then return atom end

		match_or_die('re')
		atom = { regex = tkid }
		advance()
		return atom
	end

	local branch = function()
		-- TODO
		return atom()
	end

	local rule = function()
		local rule = {}
		while true do
			local br = branch()
			rule[#rule+1] = br
			if not match('sym', '|') then
				return rule
			end
			advance()
		end
	end

	local syntax = function()
		local t = {}

		if match('sym', '&') then
			t.contained = true
			advance()
		end

		t.name = name()
		if not t.name then
			if t.contained then die('id') end
			return
		end

		t.rule = rule()

		return t
	end

	local parse = function()
		advance()

		local syntaxs = {}
		while true do
			local s = syntax()
			if not s then
				if tktype then die('id') end
				break
			end
			syntaxs[#syntaxs+1] = s
		end

		return syntaxs
	end

	return parse
end

return parser

