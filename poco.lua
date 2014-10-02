-- Did you build POCO statically(true) or dynamically(false)?
poco_static = true

-- Where is POCO?

-- on Linux you can leave the following variables empty when you installed
-- POCO (you ran 'install make')

-- on Windows you need to set these variables before building MQWeb!

poco_dir = ""
poco_lib_dir = poco_dir .. ""

-- A Linux example:
poco_dir = "/home/bronx/Development/poco" -- where did you extract POCO?
poco_lib_dir = poco_dir .. "/static/lib/Linux/i686" -- where are the libraries?

-- A Windows example:
--poco_dir = "c:\\development\\poco-1.5.2-release"
--poco_lib_dir = poco_dir .. "\\lib"

