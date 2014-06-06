
lua hi = require 'vimtest'
au TextChangedI * lua hi()
au TextChanged  * lua hi()

