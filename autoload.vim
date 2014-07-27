so vimlight.vim
call vimlight#new()

au BufNewFile,BufRead * call vimlight#new()
au TextChanged * call vimlight#modified()
au TextChangedI * call vimlight#modified()
au CursorHold * call vimlight#request()
au CursorHoldI * call vimlight#request()


