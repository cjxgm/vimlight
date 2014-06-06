
local lexer = function(src)
	local src_len = #src
	local line_nr = 1

	local match_pos = 1
	local matchs = {}

	local match = function(pattern)
		matchs = { src:match("^(" .. pattern .. ")()", match_pos) }
		if matchs[1] then
			match_pos = matchs[#matchs]
			return true
		end
	end

	local blank = function()
		if match("[ \t]+") then			-- space
			return true
		end
		if match("\n+") then			-- new line
			line_nr = line_nr + 1
			return true
		end
		if match("#.-\n") then			-- comment until new line
			line_nr = line_nr + 1
			return true
		end
		if match("#.*$") then			-- comment until eof
			return true
		end
	end

	local blanks = function()
		while blank() do end
	end

	local lex = function()
		blanks()
		while match_pos <= src_len do
			if match("[_%a][_%w]*") then	-- identifier
				return 'id', matchs[1]
			elseif match("/") then		-- regex
				local regex = ""
				while true do
					if not match(".-/") then
						error(("[%d]: regex not ended with /"):format(line_nr))
					end
					regex = regex .. matchs[1]
					if matchs[1]:sub(-2, -2) ~= '\\' then	-- ignore escape [[\/]]
						break
					end
				end
				return 're', regex:sub(1, -2)
			elseif match("[;=?*+|&{}]") then	-- symbols
				return 'sym', matchs[1]
			else
				error(("[%d] unrecognized character \"%s\""):format(
						line_nr, src:sub(match_pos, match_pos)))
				break
			end
		end
	end

	return function()
		-- to make sure lex() is called before reading line_nr
		return (function(tp, id)
			return tp, id, line_nr
		end)(lex())
	end
end

return lexer

