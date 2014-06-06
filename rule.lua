
local lexer = require 'lexer'
local parser = require 'parser'

return function(src)
	local lex = lexer(src)
	--[[ XXX lexer debugger
	while true do
		local tktp, tkid, tkno = lex()
		if not tktp then break end
		print(tkno, tktp, tkid)
	end
	--]]
	local parse = parser(lex)
	return parse()
end

