
local rule = require 'rule'
local analyzer = require 'analyzer'
local dump = require 'dump'

local r = rule [[
	&id = /%a%w*/
	typed_name = id{Type} id
]]

print(dump(r))

--[[
local analyze = analyzer(r)

analyze [[
	type name yes no
	ok but ddd # adfedfe ef qef
	p t
	q x
]]
--]]
