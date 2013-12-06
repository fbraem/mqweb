project "MQWeb"
  kind       "ConsoleApp"
  language   "C++"
  targetname "mqweb"
  location   ( solution().location )

  configuration "Debug"
    targetsuffix "d"
  
  configuration { }  

  files { 
            "src/*.cpp"
          , "include/**.h"
        }

  flags {
          "NoPCH"
        }

  configuration "Debug"
    defines { "DEBUG" }
    flags { "Symbols" }
        
  configuration "windows"
      flags { "WinMain" }
	  defines { 
              "_WINDOWS", 
              "_USRDLL", 
              "WIN32", 
              "_CRT_SECURE_NO_DEPRECATE" 
            }

  configuration { "windows", "codelite or codeblock" }
    defines {
              "Foundation_Config_INCLUDED"
            } 

  configuration { }
  
    libdirs { 
                poco_lib_dir
                , "../bin/Debug"
            }

    -- Set the include paths
    includedirs { 
                    poco_dir .. "/Foundation/include"
                  , poco_dir .. "/Util/include"
                  , poco_dir .. "/Net/include"
                  , poco_dir .. "/XML/include"
                  , poco_dir .. "/JSON/include"
                  , "include"
                  , "../MQ/include"
                  , mq_inc
                }

  configuration "Debug"
    links { 
              "mqd"
            , "PocoNetd" 
            , "PocoUtild"
            , "PocoJSONd"
            , "PocoXMLd" 
            , "PocoFoundationd"
          }
  
  configuration "Release"
    links { 
              "mq"
            , "PocoNet" 
            , "PocoUtil"
            , "PocoJSON"
            , "PocoXML" 
            , "PocoFoundation"
          }

  configuration "windows"
    links {   
              "shell32" 
            , "oleaut32"
            , "comdlg32"
            , "comctl32"
            , "rpcrt4"
            , "gdi32"
            , "ole32"
            , "advapi32"
            , "user32"
            , "wsock32" 
            , "iphlpapi"
          }

  configuration "linux"
    links {
              "pthread"
            , "dl"
          }

