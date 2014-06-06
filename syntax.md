# vim: ft=dosini
# syntax
## from lexer
id = /\I\i*/
regex = /\/.*\//[escaping \\/]

## for parser
all = syntax*

syntax = contained name rule
contained = '&'?
name = id color?
pattern = regex color?
atom = name | pattern
color = '{' id '}'

rule = seq ('|' seq)*
seq = case+
case = fac rep?
fac = atom | '(' rule ')'
rep = '?' | '*' | '+'

