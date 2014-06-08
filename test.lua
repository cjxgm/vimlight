
local lexer = require 'lexer'
local parser = require 'parser'
local analyzer = require 'analyzer'
local dump = require 'dump'

local lex = lexer [[
	&id = /%a%w*/;
	&type{Type} = id (/</ type />/)?;
	typed_name = type id | /(/ type /)/ id;
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

