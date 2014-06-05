local lexer = require 'lexer'
local parser = require 'parser'

lex = lexer [[
# test
&ppp{Constant}	/+++/
&pp {Statement}	/++/
&p  {Comment}	/+/
pnp		ppp | pp | p
]]

parse = parser(lex)
local ss = parse()
local ssname = {}
for _,s in ipairs(ss) do
	ssname[s.name.name] = s
end

local regex = function(rs)
	local re = function(r)
		if r.regex then return r.regex end
		return regex(ssname[r.name].rule)
	end
	if #rs == 1 then return re(rs[1]) end

	local res = {}
	for i=#rs,1,-1 do
		local r = rs[i]
		res[#rs-i+1] = ([[\(%s\)]]):format(regex(r))
	end
	return table.concat(res, [[\|]])
end

local rule = function(r)
	if r.name then
		return ("/%s/ contains=%s"):format(
				regex(ssname[r.name].rule),
				r.name)
	end
	if r.regex then return ("/%s/"):format(r.regex) end
end

local syntax = function(s)
	local st = ""
	local prefix = "syntax match " .. s.name.name
	if s.contained then prefix = prefix .. " contained" end

	for i=#s.rule,1,-1 do
		local r = s.rule[i]
		st = st .. ("%s %s\n"):format(prefix, rule(r))
	end

	if s.name.color then
		st = st .. ("hi link %s %s\n"):format(s.name.name, s.name.color)
	end

	return st
end

for _,s in ipairs(ss) do
	print(syntax(s))
end

