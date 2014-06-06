
local rule = require 'rule'
local analyzer = require 'analyzer'

local r = rule [[
	comment = /#.*\n/ | /#.*$/

	&id = /%a%w*/
	typed_name = id{Type} id
]]

--[[
local analyze = analyzer(r)

analyze [[
	type name yes no
	ok but ddd # adfedfe ef qef
	p t
	q x
]]
--]]
