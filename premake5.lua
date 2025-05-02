workspace "video_game_formats_collection"
   configurations { "Debug", "Release" }
   architecture "x86_64"
   startproject "tfss_test_tools"

   -- Output folder: build/{cfg.buildcfg}-{system}-{arch}
   outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

   -- Common include paths
   includedirs { "include", "format_def" }

   newoption {
      trigger     = "test_read",
      description = "creates executable designed to read test"
   }

   newoption {
      trigger     = "test_write",
      description = "creates executable designed to write test"
   }



project "tfss_test_tools"
   kind "ConsoleApp"
   language "C"
   targetdir ("build/bin/" .. outputdir .. "/%{prj.name}")
   objdir ("build/obj/" .. outputdir .. "/%{prj.name}")
   files { "**.h", "test.c" }

   dependson {"tfss_static_tools" }
   links{ "tfss" }


   defines { "TEST_READ" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

project "stb_image"
   kind "StaticLib"
   language "C"
   optimize "On"
   targetdir ("build/bin/" .. outputdir .. "/%{prj.name}")
   objdir ("build/obj/" .. outputdir .. "/%{prj.name}")
   files { "src/stb_img.c" }

project "tfss_static_tools"
   kind "StaticLib"
   language "C"
   targetname "tfss"
   targetdir ("build/bin/" .. outputdir .. "/%{prj.name}")
   objdir ("build/obj/" .. outputdir .. "/%{prj.name}")
   files { "src/tfss/*.h", "src/tfss/*.c" }
   removefiles { "test.c" }
   
   dependson {"stb_image" }
   links{ "stb_image" }
   
   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

project "tfss_shared_tools"
   kind "SharedLib"
   language "C"
   targetname "tfss_shared"
   targetdir ("build/bin/" .. outputdir .. "/%{prj.name}")
   objdir ("build/obj/" .. outputdir .. "/%{prj.name}")
   files { "src/tfss/*.h", "src/tfss/*.c" }
   removefiles { "test.c" }
   dependson {"stb_image" }
   links{ "stb_image" }
   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

