local make_vimlight = function(root)
	local engine = require 'vimlight_engine'
	engine.init(root .. "/etc/hlgroup.vimlight")

	local default_options = {
		c = "-std=gnu11 -Wall -Wextra",
		cpp = "-std=gnu++14 -Wall -Wextra",
	}
	local env = require[[vimlight_environment]]()
	local done = true
	local modified = false
	local vl = {}
	local file
	local option
	local nline = 1

	vl.fetch = function()
		if done then return end
		while true do
			local cmds = engine.get()
			if cmds == nil then break end
			load(cmds, "vimlight-command", 't', env)()
			done = true
		end
		return done
	end

	vl.finish = function()
		while not done do
			vl.fetch()
		end
	end

	vl.update = function()
		if not modified then return end
		if done then
			done = false
			modified = false
			local src = vim.eval[[join(getline(1, '$'), "\n")]]
			engine.request(src)
		end
	end

	vl.modify = function()
		modified = true

		-- detect line insertion/deletion, then shift the highlight down/up
		local n = vim.eval[[line('$')]]
		if n ~= nline then
			local y = vim.eval[[line('.')]]
			local d = math.abs(n - nline) ; -- semicolon required here to avoid parsing ambiguity
			(n > nline and env.insert_line or env.delete_line)(y, d)
			nline = n
		end
	end

	vl.rename = function()
		file = vim.eval[[expand('%')]]
		if file == "" then file = "source" end
		file = file .. "." .. vim.eval[[&ft]];
	end

	vl.reoption = function()
		local opt = vim.eval[[exists("b:vimlight_option") ? b:vimlight_option : ""]]
		if opt == "" then opt = nil end
		local ft = vim.eval[[&ft]]
		local default = default_options[ft] or ""
		option = opt or default
	end

	vl.setup = function()
		engine.setup(file, option)
	end

	vl.leave = function()
		engine.exit()
		engine = {}
	end

	vl.view = function()
		local y = vim.eval[[line('.')]]
		local h = vim.eval[[&lines]]
		env.view(y, h)
	end

	vl.identify = function()
		env.identify()
	end

	return vl
end

return make_vimlight

