
local lexer = require 'lexer'
local parser = require 'parser'
local analyzer = require 'analyzer'
local dump = require 'dump'

local lex = lexer [[
	&id{Statement} = /%a%w*/;
	typed_name = id{Type} id id id;
]]
local parse = parser(lex)
local ast = parse()

print(dump(ast))

local analyze = analyzer(ast)
--[[

analyze [[
	type name yes no
	ok but ddd # adfedfe ef qef
	p t
	q x
]]
--]]

