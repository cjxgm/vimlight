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
	if exists("s:updatetime")
		let &updatetime = s:updatetime
		unlet s:updatetime
	endif
	redraw!
endf

function vimlight#request()
"	echo "request"
	if exists("s:updatetime")	" only when modified will this exist
		lua vimlight:request()
	endif
endf

function vimlight#modified()
	if !exists("s:updatetime")
		let s:updatetime = &updatetime
		set updatetime=300
	endif
endf

