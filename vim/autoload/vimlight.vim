" vimlight. clang semantic highlighter for vim
" vim: noet ts=4 sw=4 sts=0
"
" Copyright: (C) 2014-2015 Giumo Xavier Clanjor (哆啦比猫/兰威举) <cjxgm@126.com>
"   License: The MIT Licence with modification. see LICENSE for details.

" run only once
if exists("g:loaded_vimlight")
	finish
endif
let g:loaded_vimlight = 1

lua <<END
	local root = vim.eval[[expand("<sfile>:h:h")]]
	package.cpath = ("%s;%s/lib/?.so" ):format(package.cpath, root)
	package. path = ("%s;%s/lib/?.lua"):format(package. path, root)
	vimlight = require[[vimlight]](root)	-- global variable intentionally
END

au VimLeave	* call vimlight#leave()

function vimlight#update()
	if &ft != "cpp" && &ft != "c"
		return
	endif
	lua vimlight.fetch()
	lua vimlight.view()
	lua vimlight.update()
endf

function vimlight#modify()
	lua vimlight.modify()
	call vimlight#update()
endf

function vimlight#enter()
	if &ft != "cpp" && &ft != "c"
		return
	endif

	syn match cppBinNumber "0b[01]\+"
	hi def link cppBinNumber cNumber
	syn match cppNamespaceSep "::"
	hi def link cppNamespaceSep Special

	lua vimlight.identify()
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


