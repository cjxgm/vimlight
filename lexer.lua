
local lexer = function(src)
	local src_len = #src

	local match_pos = 1
	local matchs = {}

	local match = function(pattern)
		matchs = { src:match("^(" .. pattern .. ")()", match_pos) }
		if matchs[1] then
			match_pos = matchs[#matchs]
			return true
		end
	end

	local blanks = function()
		while match_pos <= src_len		-- not eof
			and (match("%s+")			-- space
				or match("#.-[\n]")		-- comment
				or match("#.*"))		-- last comment without newline
		do end
	end

	local lex = function()
		blanks()
		while match_pos <= src_len do
			if match("%a%w*") then		-- identifier
				return 'id', matchs[1]
			elseif match("/") then		-- regex
				local regex = ""
				while true do
					if not match(".-/") then error("wrong regex") end
					regex = regex .. matchs[1]
					if matchs[1]:sub(-2, -2) ~= '\\' then
						break
					end
				end
				return 're', regex:sub(1, -2)
			elseif match("[?*|&{}]") then	-- symbols
				return 'sym', matchs[1]
			else
				error("unknown " .. src:sub(match_pos))
				break
			end
		end
	end

	return lex
end

return lexer

