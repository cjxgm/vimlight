local server = (...)
local src    = io.open("/tmp/vimlight.exchange"):read("*a")

local    lexer = require 'lexer'
local   parser = require 'parser'
local analyzer = require 'analyzer'
local     dump = require 'dump'

--[[
	&id = /[a-zA-Z_][a-zA-Z_0-9]*/;
	&noptr_type = id (/</ type_list? />/)?;
	&type_list = type (/,/ type)*;
	&type{Type} = noptr_type /[*&]/*;
	typed_name = /return/ id	# so that return won't be regarded as type
				| type id
	;
--]]
local lex = lexer [[
	&id = /%a%w*/;
	&type{Type} = id;
	typed_name = /return/ id	# so that return won't be regarded as type
				| type id
	;
]]
local parse = parser(lex)

local ast = parse()
--print(dump(ast))

local analyze = analyzer(ast)
local result = analyze(src)
--print(dump(result))

local vim = {}

--os.execute = print
vim.expr = function(expr)
	os.execute(([[vim --servername %s --remote-expr "%s" > /dev/null]])
			:format(server, expr))
end

vim.param = function(...)
	local quote
	quote = function(v, ...)
		if type(v) == 'string' then v = ("'%s'"):format(v) end
		if select('#', ...) == 0 then return v end
		return v, quote(...)
	end
	return table.concat({quote(...)}, ",")
end

vim.call = function(name, ...)
	vim.expr(("vimlight#%s(%s)"):format(name, vim.param(...)))
end

vim.add = function(...)
	vim.call("add", ...)
end

vim.done = function()
	vim.call("done")
end

for _,hl in ipairs(result) do
	vim.add(unpack(hl))
end
vim.done()

