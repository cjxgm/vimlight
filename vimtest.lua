
local lexer = require 'lexer'
local parser = require 'parser'
local analyzer = require 'analyzer'
local highlighter = require 'highlighter'
local dump = require 'dump'

local lex = lexer [[
	&id = /%a%w*/;
	&type{Type} = id (/</ type />/)*;
	typed_name = /return/ id	# so that return won't be regarded as type
				| type id
				| /%(/ type /%)/ id
	;
]]
local parse = parser(lex)
local ast = parse()
print(dump(ast))
local analyze = analyzer(ast)
local highlight = highlighter()

local src = function()
	local buf = vim.buffer()
	local s = ""
	for i=1,#buf do s = s .. buf[i] .. "\n" end
	return s
end

return function()
	highlight(analyze(src()))
end

