poco_dir = os.pathsearch("Poco.h", "/usr/local/include/Poco:/usr/include/Poco")
if poco_dir then
  poco_dir = path.getdirectory(poco_dir)
else
  print "Can't find Poco.h. Please build and install Poco."
  os.exit(1)
end

poco_lib_dir = os.pathsearch("libPocoFoundationd.so", "/usr/local/lib:/usr/lib")

solution "CocoR"
  location ( "../build/" .. _ACTION )
  configurations { "Debug", "Release" }
  platforms { "x32", "x64" }

project "CocoRApp"
  targetname "cocor"
  kind "ConsoleApp"
  language "C++"
  files
  {
    "**.h", "**.cpp"
  }

  libdirs {
              poco_dir .. "/lib"
            , "../../lib"
          }
  includedirs {
                  poco_dir .. "/Foundation/include"
                , poco_dir .. "/Util/include"
                , "."
              }

  libdirs {
              poco_dir .. "/lib"
            , "../../lib"
          }

  configuration "Debug"
    targetsuffix "d"
    links { 
              "PocoFoundationd"
            , "PocoUtild"
          }

  configuration "Release"
    links { 
              "PocoFoundation"
            , "PocoUtil"
          }

