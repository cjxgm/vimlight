
local lexer = require 'lexer'
local parser = require 'parser'
local analyzer = require 'analyzer'
local dump = require 'dump'

local lex = lexer [[
	&id = /%a%w*/;
	&type{Type} = id / /*;    # (/< */ type / *>/)*;
	#typed_name = type / +/ id | /%( */ type / *%) */ id;
	typed_name = type / /+ id;
]]
--[[
local lex = lexer [[
	&id = /%a%w*/;
	# all = id{Type} (/ +/ id)+;
	&rec = id{Type} (/:/ rec /;/)?;
	all = /%( */ rec / *%) */ id | ;
	# all = rec / +/ id | /\( */ rec / *\) */ id;
]]
local parse = parser(lex)
local ast = parse()

print(dump(ast))

local analyze = analyzer(ast)
local colors = analyze [[
	type name
	t(e)t
	type name 
	type name tete
]]

print(dump(colors))
--[[
--]]

