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
	vimlight = require 'vimlight'	-- global variable intentionally
	local vl = vimlight

	vl.init(root .. "/etc/hlgroup.vimlight")
	vl.done = true
	vl.modified = true

	vl.apply = function(this)
		if this.done then return end
		local result
		while true do
			result = this.get()
			if result == nil then break end
			for _,cmd in ipairs(result) do
				vim.command(cmd)
			end
			this.done = true
		end
		return this.done
	end

	vl.finish = function(this)
		while not this.done do
			this:apply()
		end
	end

	vl.update = function(this)
		if this.done then
			this.done = false
			this.modified = false
			local src = vim.eval("join(getline(1, '$'), '\n')")
			this.request(src)
		end
	end

	vl.modify = function(this)
		this.modified = true
	end

	vl.rename = function(this)
		local file = vim.eval("expand('%')")
		if file == "" then file = "source.cc" end
		this.name(file)
	end
END

function vimlight#update()
	if &ft != "cpp"
		return
	endif
lua <<END
	vimlight:apply()
	if vimlight.modified then
		vimlight:update()
	end
END
endf

function vimlight#modify()
	lua vimlight:modify()
	call vimlight#update()
endf

function vimlight#rename()
	if &ft != "cpp"
		return
	endif

	syn match cppFunction "\zs\w\+\ze\s*("
	hi def link cppFunction Function
	syn match cppBinNumber "0b[01]\+"
	hi def link cppBinNumber cNumber
	syn match cppNamespaceSep "::"
	hi def link cppNamespaceSep Special

	lua vimlight:rename()
	call vimlight#modify()
endf

function vimlight#finish()
	lua vimlight:finish()
endf

