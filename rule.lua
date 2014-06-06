
local lexer = require 'lexer'
local parser = require 'parser'

return function(src)
	local lex = lexer(src)
	local parse = parser(lex)
	return parse()
end

