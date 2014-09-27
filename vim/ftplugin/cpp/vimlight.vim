" vimlight: clang syntax highlighter for vim
" vim: noet ts=4 sw=4 sts=0
"
" Copyright: (C) 2014 eXerigumo Clanjor(哆啦比猫/兰威举) <cjxgm@126.com>
"   License: The MIT Licence with modification. see LICENSE for details.

" run only once
if exists("b:did_vimlight")
	finish
endif
let b:did_vimlight = 1

au BufEnter		* call vimlight#rename()
au BufLeave		* call vimlight#finish()
au TextChanged	* call vimlight#modify()
au TextChangedI	* call vimlight#modify()
au CursorHold	* call vimlight#update()
au CursorHoldI	* call vimlight#update()
au CursorMoved	* call vimlight#update()
au CursorMovedI	* call vimlight#update()

syn match cppFunction "\zs\w\+\ze\s*("
hi def link cppFunction Function

syn match cppBinNumber "0b[01]\+"
hi def link cppBinNumber cNumber

syn match cppNamespaceSep "::"
hi def link cppNamespaceSep Special

