
local lexer = require 'lexer'
local parser = require 'parser'

return function(src)
	local lex = lexer(src)
	--[[ XXX lexer debugger
	while true do
		local tktp, tkid, tknr = lex()
		if not tktp then break end
		print(tknr, tktp, tkid)
	end
	--]]
	local parse = parser(lex)
	return parse()
end

