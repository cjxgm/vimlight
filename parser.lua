
local parser = function(lex)
	local tktp = ''	-- token type
	local tkid = ''	-- token identity
	local tkno = 0	-- token line no.

	local node = function(tp, tbl)
		tbl.tp = tp
		return tbl
	end

	local advance = function()
		tktp, tkid, tkno = lex()
	end

	local match = function(tp, id)
		if tp and tktp ~= tp then return end
		if id and tkid ~= id then return end
		return true
	end

	local tkdump = function()	-- token dump
		print('token dump:')
		while tktp do		-- token dump
			print('', tkno, tktp, tkid)
			advance()
		end
	end

	local die = function(tp, id)
		local no = tkno
		tkdump()
		if tp or id then
			local errid = id and ( "%q" ):format(id) or ''
			local errtp = tp and ("[%s]"):format(tp) or ''
			error(("error[%d]: %s%s expected"):format(no, errid, errtp))
		else
			error(("error[%d]: nothing expected, WTF!??"):format(no))
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

	local seq = function(try)
		local node_qfac = qfac(try)
		if not node_qfac then return end

		while true do
			local node_qfac2 = qfac(true)
			if node_qfac2 then
				node_qfac2.prev = node_qfac
				node_qfac = node_qfac2
			else
				return node_qfac
			end
		end
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

		return node('syntax', t)
	end

	local all = function()
		advance()

		local t
		while true do
			local node_syntax = syntax(true)
			if node_syntax then
				node_syntax.prev = t
				t = node_syntax
			else
				return t
			end
		end
	end

	local parse = function()
		return all()
	end

	return parse
end

return parser

