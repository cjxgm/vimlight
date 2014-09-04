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
au TextChanged	* call vimlight#modify()
au TextChangedI	* call vimlight#modify()
au CursorHold	* call vimlight#update()
au CursorHoldI	* call vimlight#update()
au CursorMoved	* call vimlight#update()
au CursorMovedI	* call vimlight#update()

