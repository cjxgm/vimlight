
local lexer = require 'lexer'
local parser = require 'parser'

return function(src)
	local lex = lexer(src)
	while true do
		local tkt, tki = lex()
		if not tkt then break end
		print(tkt, tki)
	end
	--[[
	local parse = parser(lex)
	return parse()
	--]]
end

