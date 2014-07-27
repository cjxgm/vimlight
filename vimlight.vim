function vimlight#new()
"	echo "new"
	lua vimlight = (require 'vimint'):new()
endf

function vimlight#add(y1, x1, y2, x2, color)
"	echo "add"
	exec "lua vimlight:add(" . a:y1 . "," . a:x1 . "," . a:y2 . "," . a:x2 ",'" . a:color . "')"
endf

function vimlight#done()
"	echo "done"
	lua vimlight:done()
endf

function vimlight#request()
"	echo "request"
	lua vimlight:request()
	if !exists("s:updatetime")
		let s:updatetime = &updatetime
		set updatetime=1000
	endif
endf

function vimlight#redraw()
	if !exists("s:updatetime")
		let &updatetime = s:updatetime
	endif
	redraw!
endf

