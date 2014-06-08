
local lexer = require 'lexer'
local parser = require 'parser'
local analyzer = require 'analyzer'
local highlighter = require 'highlighter'
local dump = require 'dump'

local lex = lexer [[
	&id = /%a%w*/;
	&type{Type} = id (/</ type />/)*;
	typed_name = /return/ id	# so that return won't be regarded as type
				| type id
				| /%(/ type /%)/ id
	;
]]
local parse = parser(lex)
local ast = parse()
print(dump(ast))

local analyze = analyzer(ast)
local colors = analyze [[
	int add(int a, int b)
	{
		return a + b;
	}

	int main()
	{
		vector<int> a { 1, 2 };
		printf("hello, world! %d\n", add(1, 2));
		return a.size();
	}
]]
print(dump(colors))

local highlight = highlighter()
highlight(colors)


highlight(analyze([[
	int add(int a, int b)
	{
	}

	int main()
	{
		vector<int> a { 1, 2 };
		printf("hello, world! %d\n", add(1, 2));
		return a.size();
	}
]]))

