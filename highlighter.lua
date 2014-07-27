
local highlighter = function()
	if not vim then vim = { command=print } end

	local last_colors = {}

	local same = function(color1, color2)
		for i=1,#color2 do
			if color1[i] ~= color2[i] then return end
		end
		return true
	end

	local add = function(i, y1, x1, y2, x2, clr)
		vim.command(([[syn region lighterH%d start=+\%%%dl\%%%dc+ end=+\%%%dl\%%%dc+]]):format(i, y1, x1, y2, x2))
		vim.command(("hi link lighterH%d %s"):format(i, clr))
	end

	local del = function(i)
		vim.command("syn clear lighterH" .. i)
	end

	local highlight = function(colors)
		for i,color in pairs(colors) do
			local lcolor = last_colors[i]
			if lcolor then
				if not same(lcolor, color) then
					del(i)
					add(i, table.unpack(color))
				end
			else
				add(i, table.unpack(color))
			end
		end
		for i in pairs(last_colors) do
			if not colors[i] then
				del(i)
			end
		end
		last_colors = colors
	end

	return highlight
end

return highlighter

