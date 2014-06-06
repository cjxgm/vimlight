
-- (l, c) is (line, column) pair, specifying a coordinate in vim
-- highlight text inside (l1, c1) -> (l2, c2) as color
local tmpname
do
	local n = 0
	tmpname = function()
		n = n + 1
		return "T" .. n
	end
end

local hnames = {}

local highlight_str = function(l1, c1, l2, c2, color)
	local name = tmpname()
	hnames[name] = true
	local s = "syntax match %s /\\%%%dl\\%%%dc.*\\%%%dl\\%%%dc/\n"
	s = s .. "highlight link %s %s\n"
	return s:format(name, l1, c1, l2, c2+1, name, color)
end

local highlight = function(...)
	vim.command(highlight_str(...))
end

local clear = function()
	for n in pairs(hnames) do
		vim.command("syntax clear " .. n)
	end
	hnames = {}
end

return function()
	return highlight, clear
end

