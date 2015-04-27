" vimlight. clang semantic highlighter for vim
" vim: noet ts=4 sw=4 sts=0
"
" Copyright: (C) 2014 eXerigumo Clanjor(哆啦比猫/兰威举) <cjxgm@126.com>
"   License: The MIT Licence with modification. see LICENSE for details.

" run only once
if exists("g:loaded_vimlight")
	finish
endif
let g:loaded_vimlight = 1

lua <<END
	local make_command_environment = function()
		local highlights = {}
		local env = {}

		local hls = function()
			local id = vim.eval[[bufnr("")]];
			if not highlights[id] then highlights[id] = {} end
			return highlights[id]
		end

		local hls_push = function(hl)
			local hs = hls()
			hs[#hs+1] = hl
		end

		local hls_clear = function()
			local id = vim.eval[[bufnr("")]];
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

		return env
	end

	local make_vimlight = function(root)
		local engine = require 'vimlight_engine'
		engine.init(root .. "/etc/hlgroup.vimlight")

		local default_options = {
			c = "-std=gnu11 -Wall -Wextra",
			cpp = "-std=gnu++14 -Wall -Wextra",
		}
		local cmd_env = make_command_environment()
		local done = true
		local modified = true
		local vl = {}
		local file
		local option

		vl.fetch = function()
			if done then return end
			while true do
				local cmds = engine.get()
				if cmds == nil then break end
				load(cmds, "vimlight-command", 't', cmd_env)()
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
		end

		vl.rename = function()
			file = vim.eval[[expand('%')]]
			local ft = vim.eval[[&ft]]
			if file == "" then file = "source." .. ft end
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
			local y = vim.eval[[getcurpos()]][1]
			local h = vim.eval[[&lines]]
			cmd_env.view(y, h)
		end

		return vl
	end

	local root = vim.eval[[expand("<sfile>:h:h")]]
	package.cpath = ("%s;%s/lib/?.so"):format(package.cpath, root)
	vimlight = make_vimlight(root)	-- global variable intentionally
END

function vimlight#update()
	if &ft != "cpp" && &ft != "c"
		return
	endif
lua <<END
	vimlight.fetch()
	vimlight.view()
	vimlight.update()
END
endf

function vimlight#modify()
	lua vimlight.modify()
	call vimlight#update()
endf

function vimlight#enter()
	if &ft != "cpp" && &ft != "c"
		return
	endif

	syn match cppFunction "\zs\w\+\ze\s*("
	hi def link cppFunction Function
	syn match cppBinNumber "0b[01]\+"
	hi def link cppBinNumber cNumber
	syn match cppNamespaceSep "::"
	hi def link cppNamespaceSep Special

	lua vimlight.rename()
	lua vimlight.reoption()
	lua vimlight.setup()
	call vimlight#modify()
endf

function vimlight#finish()
	lua vimlight.finish()
	lua vimlight.view()
endf

function vimlight#leave()
	lua vimlight.leave()
endf


" user function
function vimlight#option(opt)
	let b:vimlight_option = a:opt
	lua vimlight.reoption()
	lua vimlight.setup()
	call vimlight#modify()
endf

" user function
function vimlight#default_option()
	unlet! b:vimlight_option
	lua vimlight.reoption()
	lua vimlight.setup()
	call vimlight#modify()
endf

au VimLeave	* call vimlight#leave()

