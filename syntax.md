# vim: ft=dosini
# syntax
## from lexer
id = /\I\i*/
regex = /\/.*\//[escaping \\/]

## for parser
all = syntax*

syntax = contained? name rule
contained = '&'
name = id color?
color = '{' id '}'

rule = branch ('|' branch)*
#branch = atom repeat?
branch = atom
atom = name | regex
#repeat = '?' | '*' | '+'

