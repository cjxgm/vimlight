local make_vimlight_environment = function()
	local highlights = {}
	local env = {}
	local win_id = 0

	identification = function()
		return vim.eval[[w:vimlight_id]]
	end

	local hls = function()
		local id = identification()
		if not highlights[id] then highlights[id] = {} end
		return highlights[id]
	end

	local hls_push = function(hl)
		local hs = hls()
		hs[#hs+1] = hl
	end

	local hls_clear = function()
		local id = identification()
		highlights[id] = {}
	end

	local match_add = function(highlight)
		local hl = highlight
		if not hl.match then
			local cmd = [==[matchaddpos("%s", [[%d, %d, %d]])]==]
			hl.match = vim.eval(cmd:format(hl.group, hl.y, hl.x, hl.w))
		end
	end

	local match_del = function(highlight)
		local hl = highlight
		if hl.match then
			local cmd = [[matchdelete(%d)]]
			vim.eval(cmd:format(hl.match))
			hl.match = nil
		end
	end

	env.add = function(group, y, x, w)
		hls_push { group = group, y = y, x = x, w = w }
	end

	env.clear = function(i)
		for _,hl in ipairs(hls()) do
			match_del(hl)
		end
		hls_clear()
	end

	env.view = function(y, h)
		local inside = function(value)
			return (value >= y-h and value <= y+h)
		end

		for _,hl in ipairs(hls()) do
			(inside(hl.y) and match_add or match_del)(hl)
		end
	end

	env.identify = function()
		local exists = (vim.eval[[exists("w:vimlight_id")]] ~= 0)
		if exists then return end
		win_id = win_id + 1
		local cmd = [[let w:vimlight_id = %d]]
		vim.command(cmd:format(win_id))
	end

	env.insert_line = function(y, n)
		for _,hl in ipairs(hls()) do
			if hl.y >= y then
				hl.y = hl.y + n
				match_del(hl)
			end
		end
	end

	env.delete_line = function(y, n)
		for _,hl in ipairs(hls()) do
			if hl.y >= y+n then
				hl.y = hl.y - n
				match_del(hl)
			end
		end
	end

	return env
end

return make_vimlight_environment

