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

	vl.update = function()
		if vl.done then
			vl.done = false
			local src = vim.eval("join(getline(1, '$'), '\n')")
			vl.request(src)
		end

		local result = vl.get()
		if result then
			for _,cmd in ipairs(result) do
				vim.command(cmd)
			end
			vl.done = true
		end
	end
END

function vimlight#update()
	echo "update"
	lua vimlight.update()
endf

