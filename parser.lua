
local parser = function(lex)
	local tktp = ''	-- token type
	local tkid = ''	-- token identity
	local tknr = 0	-- token line number

	local node = function(tp, tbl)
		tbl.tp = tp
		return tbl
	end

	local advance = function()
		tktp, tkid, tknr = lex()
	end

	local match = function(tp, id)
		if tp and tktp ~= tp then return end
		if id and tkid ~= id then return end
		return true
	end

	local tkdump = function()	-- token dump
		print('token dump:')
		while tktp do		-- not eof
			print('', tknr, tktp, tkid)
			advance()
		end
	end

	local die = function(tp, id)
		local nr = tknr		-- tkdump will advance tknr till eof
		tkdump()
		if tp or id then
			local errid = id and ( "%q" ):format(id) or ''
			local errtp = tp and ("[%s]"):format(tp) or ''
			error(("error[%d]: %s%s expected"):format(nr, errid, errtp))
		else
			error(("error[%d]: nothing expected, WTF!??"):format(nr))
		end
	end

	local advance_or_die = function()
		advance()
		if not tktp then
			error("error: unexpected eof")
		end
	end

	local match_or_die = function(tp, id, try)
		if match(tp, id) then return true end
		if try then return end
		die(tp, id)
	end




	local color = function(try)
		if not match_or_die('sym', '{', try) then return end
		advance_or_die()

		match_or_die('id')
		local color = tkid
		advance_or_die()

		match_or_die('sym', '}')
		advance()

		return node('color', { color })
	end

	local name = function(try)
		if not match_or_die('id', nil, try) then return end
		local t = { id = tkid }
		advance()

		t.color = color(true)
		return node('name', t)
	end

	local pattern = function(try)
		if not match_or_die('re', nil, try) then return end
		local t = { id = tkid }
		advance()

		t.color = color(true)
		return node('pattern', t)
	end

	local atom = function(try)
		local t = name(true)
		if t then return t end

		local t = pattern(true)
		if t then return t end

		if not try then die('atom', 'name or pattern') end
	end

	local fac = function(try)
		return atom(try)
	end

	local qfac = function(try)
		return fac(try)
	end

	local function seq(try)
		local function list(try)
			local n = qfac(try)
			if not n then return end
			return n, list(true)
		end
		return node('seq', { list(try) })
	end

	local rule = function(try)
		return seq(try)
	end

	local tmp = function()	-- no need to try
		if match('sym', '&') then
			advance()
			return true
		end
	end

	local syntax = function(try)
		local t = {}
		t.tmp = tmp()

		t.name = name(not t.tmp and try)
		if not t.name then return end

		match_or_die('sym', '=')
		advance_or_die()
		t.rule = rule()
		match_or_die('sym', ';')
		advance()

		return node('syntax', t)
	end

	local function all()
		local function list()
			local n = syntax(true)
			if not n then return end
			return n, list()
		end
		return { list() }
	end

	local parse = function()
		advance()
		return all()
	end

	return parse
end

return parser

