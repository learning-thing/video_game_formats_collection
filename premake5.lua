-- premake5.lua
workspace "video_game_formats_collection"
   configurations { "Debug", "Release" }
   includedirs { "include", "format_def" }



project "tfss_test_tools"
   kind "ConsoleApp"
   language "C"
   targetdir "build/test/%{cfg.buildcfg}"
   
   newoption {
    trigger     = "test_read",
    description = "creates executable designed to read test"
   }

   newoption {
    trigger     = "test_write",
    description = "creates executable designed to write test"
   }

   newoption {
    trigger     = "test_all",
    description = "creates two executables to test both rw"
   }
      
   files { "**.h", "test.c" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"


project "build_stb_image"
   kind "StaticLib"
    targetdir "build/stb/%{cfg.buildcfg}"
   files { "src/stb_img.c"}

project "tfss_static_tools"
   kind "StaticLib"
   targetdir "build/tfss/static/%{cfg.buildcfg}"
   files { "src/tfss/*.h", "src/tfss/*.c" }
   
   removefiles {"test.c"}
   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
      
project "tfss_shared_tools"
   kind "SharedLib"
   targetdir "build/tfss/shared/%{cfg.buildcfg}"
   
   files { "src/tfss/*.h", "src/tfss/*.c" }
   removefiles {"test.c"}
   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
