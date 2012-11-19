project "MQ"
  kind "StaticLib"
  language "C++"
  targetname "mq"

  location   ( solution().location )

  configuration "Debug"
    targetsuffix "d"

  configuration { }
  
  files { 
            "src/*.cpp"
          , "include/**.h"
        }
  
  includedirs { 
                  poco_dir .. "/Foundation/include"
                , poco_dir .. "/Util/include"
                , poco_dir .. "/XML/include"
                , "include"
              }  

  configuration "linux"
    includedirs { "/opt/mqm/inc" }

