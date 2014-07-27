so vimlight.vim
call vimlight#new()

au BufNewFile,BufRead * call vimlight#new()
au TextChanged * call vimlight#request()
au TextChangedI * call vimlight#request()
au CursorHold * call vimlight#redraw()
au CursorHoldI * call vimlight#redraw()


