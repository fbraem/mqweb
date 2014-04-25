project "MQDictionary"
  kind       "ConsoleApp"
  language   "C++"
  targetname "mqdict"
  location   ( solution().location )

  configuration "Debug"
    targetsuffix "d"
  
  configuration { }  

  files { 
            "src/*.cpp",
            "../MQWeb/src/Dictionary.cpp"
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
                  , poco_dir .. "/XML/include"
                  , poco_dir .. "/JSON/include"
                  , poco_dir .. "/Data/include"
                  , poco_dir .. "/Data/SQLite/include"
                  , "../MQ/include"
                  , "../MQWeb/include"
                  , mq_inc
                }

  configuration "Debug"
    links {   "PocoUtild"
            , "PocoJSONd"
            , "PocoXMLd" 
            , "PocoDataSQLited"
            , "PocoDatad"
            , "PocoFoundationd"
            , "mqd"
          }
  
  configuration "Release"
    links {   "PocoUtil"
            , "PocoJSON"
            , "PocoXML" 
            , "PocoDataSQLite"
            , "PocoData"
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

if ( poco_static ) then
  configuration "linux"
    links {
              "pthread"
            , "dl"
          }
end
