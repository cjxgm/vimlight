syntax match ppp contained /+++/
hi link ppp Constant

syntax match pp contained /++/
hi link pp Statement

syntax match p contained /+/
hi link p Comment

syntax match pnp /+/ contains=p
syntax match pnp /++/ contains=pp
syntax match pnp /+++/ contains=ppp

