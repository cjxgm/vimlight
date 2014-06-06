
local hl,hlclear = (require 'highlight')()

local src = function()
	local buf = vim.buffer()
	local s = ""
	for i=1,#buf do s = s .. buf[i] .. "\n" end
	return s
end

local pos2d = function(src, pos)
	local l = 1
	local lastp = 1
	for p in src:gmatch("\n()") do
		if pos >= p then l = l + 1 lastp = p
		else break end
	end
	return l, pos - lastp + 1
end

local matched_region = function(src, pattern)
	local rs = {}
	for m,p in src:gmatch("(" .. pattern .. ")()") do
		local sp = p - m:len()
		local ep = p - 1
		rs[{sp, ep}] = true
	end
	return rs
end

local highlight_regions = function(src, rs, color)
	for r in pairs(rs) do
		local l1, c1 = pos2d(src, r[1])
		local l2, c2 = pos2d(src, r[2])
		hl(l1, c1, l2, c2, color)
	end
end

local highlight_match = function(pattern, color)
	local s = src()
	highlight_regions(s, matched_region(s, pattern), color)
end

local hi = function()
	hlclear()
	highlight_match("%a%d", "Comment")
	hl(1, 3, 1, 5, "Type")
	hl(2, 2, 3, 8, "Comment")
end

return hi


