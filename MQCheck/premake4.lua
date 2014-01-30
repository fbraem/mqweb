project "MQCheck"
  kind       "ConsoleApp"
  language   "C++"
  targetname "mqcheck"
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
                poco_dir .. "/lib"
            }

    -- Set the include paths
    includedirs { 
                    poco_dir .. "/Foundation/include"
                  , poco_dir .. "/Util/include"
                  , poco_dir .. "/XML/include"
                  , mq_inc
                  , "include"
                  , "../MQ/include"
                }

  configuration "Debug"
    links { 
              "PocoUtild"
            , "PocoXMLd" 
            , "PocoFoundationd"
            , "mqd"
          }
  configuration "Release"
    links { 
              "PocoUtil"
            , "PocoXML" 
            , "PocoFoundation"
            , "mq"
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

