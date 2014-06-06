
local dump
local indentation = "    "
dump = function(tbl, level)
	local s = ""
	level = level or 0
	local indent = string.rep(indentation, level)

	s = s .. "{\n"

	for k,v in pairs(tbl) do
		local kname = tostring(k)
		if type(k) ~= 'string' then kname = ("[%s]"):format(k) end
		local prefix = ("%s%s%s = "):format(indent, indentation, kname)
		local t = type(v)
		if t == "table" then
			s = s .. ("%s%s,\n"):format(prefix, dump(v, level+1))
		elseif t == "string" then
			s = s .. ("%s%q,\n"):format(prefix, v)
		else
			s = s .. ("%s%s,\n"):format(prefix, v)
		end
	end
	s = s .. indent .. "}"
	return s
end

return dump

