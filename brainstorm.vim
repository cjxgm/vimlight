syntax match abcde rex contained

&type /\I\i*/
&name /\I\i*/
&blanks /\s\+/
&pointer /\*\|&/
&ptr_or_not			blanks* pointer blanks*


&ppp{Constant}		/+++/
&pp{Statement}	/++/
&p {Comment}	/+/
pp_or_p		ppp | pp | p

syntax match p contained /+/
hi link p Comment
syntax match pp contained /++/
hi link pp Statement
syntax match ppp contained /+++/
hi link ppp Constant
syntax match pp_or_p /+/ contains=p
syntax match pp_or_p /++/ contains=pp
syntax match pp_or_p /+++/ contains=ppp

syntax match ptr_or_not contained /\(\(\s\+\)\|\(\s\+\)*\(\*\|&\)\(\s\+\)*\)/ contains=T1
T1 

typename type{Type} blanks name{Comment}

syntax match name_T1 /\I\i*/ contained
hi link name_T1 Comment
syntax match blanks_T1 /\s\+/ nextgroup=name_T1 contained
hi link blanks_T1 blanks
syntax match type_T1 /\I\i*/ nextgroup=blanks_T1 contained
hi link type_T1 Type
syntax match typename /\I\i*\s\+\I\i*/ contains=type_T1

