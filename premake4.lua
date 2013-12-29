dofile "poco.lua"
dofile "mq.lua"

-- Try to find POCO ourselves when the directory is not set
if not poco_dir or #poco_dir == 0 then
  if os.is("windows") then
    print("Can't detect POCO install directory on Windows. Please update poco.lua and run again.")
    os.exit(1)
  elseif os.is("linux")  then
    print("Trying to find POCO ourselves ...")
    print("If this is not what you want, set the poco_dir variable in poco.lua and run again.")
    poco_dir = os.pathsearch("Poco.h", "/usr/local/include/Poco:/usr/include/Poco")
    if poco_dir then
      poco_dir = path.getdirectory(poco_dir)
    else
      print "Can't find Poco.h. Please build and install Poco."
      os.exit(1)
    end
    poco_lib_dir = os.pathsearch("libPocoFoundationd.so", "/usr/local/lib:/usr/lib")
    print("Ok.")
    print("Poco Include Directory: " .. poco_dir)
    print("Poco Library Directory: " .. poco_lib_dir)
    poco_installed = true
  else
    print("Can't detect POCO install on " .. os.id() .. ". Please update poco.lua and run again.")
    os.exit(1)
  end
else
  -- Check some files to see if everything is configured correctly.
  poco_h = poco_dir .. '/Foundation/include/Poco/Poco.h'
  print("Checking poco_dir variable(" .. poco_dir .. ") ...")
  if ( not os.isfile(poco_h) ) then
    print("Invalid poco_dir set: " .. poco_dir .. ". Couldn't find " .. poco_h);
    os.exit(1);
  else
    print("Ok.")
  end

  poco_installed = false
end

if not mq_inc or #mq_inc == 0 then
  if os.is("windows") then
    mq_inc = "C:\\Program Files\\IBM\\WebSphere MQ\\tools\\c\\include"
  elseif os.is("linux") then
    mq_inc = "/opt/mqm/inc"
  else
    print("Can't detect Websphere MQ install on " .. os.id() .. ". Please update mq.lua and run again.")
    os.exit(1)
  end
else
  -- Check mq file to see if mq is configured correctly
  print("Checking mq_inc variable(" .. mq_inc .. ") ...")
  mq_h = mq_inc .. '/cmqc.h'
  if ( not os.isfile(mq_h) ) then
    print("Invalid mq_inc set: " .. mq_inc .. ". Couldn't find " .. mq_h);
    os.exit(1);
  else
    print("Ok.")
  end
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

