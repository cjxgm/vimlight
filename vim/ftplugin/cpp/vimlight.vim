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

au BufWinEnter	* call vimlight#update()
au TextChanged	* call vimlight#update()
au TextChangedI	* call vimlight#update()
au CursorHold	* call vimlight#apply()
au CursorHoldI	* call vimlight#apply()
au CursorMoved	* call vimlight#apply()
au CursorMovedI	* call vimlight#apply()

