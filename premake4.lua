-- Where is MQ?
if os.is("windows") then
 mq_inc = "c:\\development\\MQ\\tools\\c\\include"
elseif os.is("linux") then
 mq_inc = "/opt/mqm/inc"
else
 print "Please set mq_inc to the directory which contains the Websphere MQ header files"
 os.exit(1)
end

-- Where is POCO?

-- only set this when you didn't install POCO (you didn't run 'install make')
--poco_dir = "/home/bronx/Development/PocoTrunk"
--poco_lib_dir = poco_dir .. "/shared/lib/Linux/i686"

poco_dir = "c:\\development\\poco-1.5.2-release"
poco_lib_dir = poco_dir .. "\\lib"

-- Try to find POCO ourselves when the directory is not set
if not poco_dir or #poco_dir == 0 then
  print("Trying to find POCO ourselves ...")
  print("If this is not what you want, set the poco_dir variable in this script and run again")
  if os.is("windows") then
    print "Can't detect POCO install directory on Windows. Please update premake4.lua"
    os.exit(1)
  elseif os.is("linux")  then
    poco_dir     = os.pathsearch("Poco.h", "/usr/local/include/Poco:/usr/include/Poco")
    if poco_dir then
      poco_dir = path.getdirectory(poco_dir)
    else
      print "Can't find Poco.h. Please build and install Poco."
      os.exit(1)
    end
    poco_lib_dir = os.pathsearch("libPocoFoundationd.so", "/usr/local/lib:/usr/lib")
    print("Poco Include Directory: " .. poco_dir)
    print("Poco Library Directory: " .. poco_lib_dir)
    poco_installed = true
  else
    print(os.id() .. " is not yet supported for building MQWeb")
    os.exit(1)
  end
else
  poco_installed = false
end

-- Solution

solution "MQWeb"
  configurations { "Debug", "Release" }
  platforms { "x32", "x64" }

  location ( "build/" .. _ACTION )

  configuration "Debug"
    targetdir "bin/Debug"
    defines { "DEBUG", "_DEBUG" }
    flags { "Symbols" }

  configuration "Release"
    targetdir "bin/Release"

  configuration { "linux", "Release" }
    libdirs { 
              "bin/Release"
            }
  configuration { "linux", "Debug" }
    libdirs { 
              "bin/Debug"
            }

include "MQ"
include "MQWeb"
include "MQCheck"

