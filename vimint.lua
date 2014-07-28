local highlighter = require 'highlighter'
local vl = {}	-- vl = vimlight

vl.new = function(this)
	return setmetatable({
		highlight = highlighter(vim)
	}, { __index = this })
end

vl.request = function(this)
	if this.colors then
		this.cont = true
	else
		this:spawn()
	end
end

vl.done = function(this)
	this.highlight(this.colors)
	this.colors = nil
	if this.cont then
		this.cont = nil
		this:request()
	end
end

vl.spawn = function(this)
	local server = vim.eval("v:servername")
	assert(server ~= "", "need to start vim with --servername blah")	-- FIXME: explain better

	local src = vim.eval("join(getline(1, '$'), '\n')")

	vim.command("silent w! /tmp/vimlight.exchange")
	this.colors = {}
	os.execute(("lua vimext.lua %q &"):format(server))
end

vl.add = function(this, ...)
	this.colors[#this.colors + 1] = {...}
end

return vl;

