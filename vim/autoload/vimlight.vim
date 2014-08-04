" vimlight: clang syntax highlighter for vim
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
	local root = vim.eval[[expand("<sfile>:h:h")]]
	package.cpath = ("%s;%s/lib/?.so"):format(package.cpath, root)
	vimlight = require 'vimlight'
	local vl = vimlight

	vl.init(root .. "/etc/hlgroup.vimlight")
	vl.done = true

	vl.apply = function(this)
		if this.done then return end
		local result = this.get()
		if result then
			for _,cmd in ipairs(result) do
				vim.command(cmd)
			end
			this.done = true
			return true
		end
	end

	vl.update = function(this)
		if this.done then
			this.done = false
			local src = vim.eval("join(getline(1, '$'), '\n')")
			this.request(src)
		end
	end
END

function vimlight#update()
	lua vimlight:apply()
	lua vimlight:update()
endf

function vimlight#apply()
	lua vimlight:apply()
endf

