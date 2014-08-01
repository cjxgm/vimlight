local server = (...)

local vim = {}

-- os.execute = print
vim.expr = function(expr)
	os.execute(([[vim --servername %s --remote-expr "%s" > /dev/null]])
			:format(server, expr))
end

vim.call = function(name, args)
	if args == nil then args = "" end
	vim.expr(("vimlight#%s(%s)"):format(name, args))
end

vim.add = function(args)
	vim.call("add", args)
end

vim.done = function()
	vim.call("done")
end

local result = io.popen("./analyzer /tmp/vimlight.exchange", "r")
while true do
	local hl = result:read()
	if hl == nil then break end
	vim.add(hl)
end
vim.done()

