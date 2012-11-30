solution "VEGA"
    configurations { "Debug", "Release", "Hybrid" }
    
    project "VisualVega"
        kind "ConsoleApp"
        language "C++"
        
        location "VisualVega"
        targetdir "../."
        
        objdir "../_obj"
        debugdir ".."
        
        files { "VisualVega/VisualVega.cpp", "vega/**.cpp", "vega/**.hpp", "vega/**.h" }
        
        includedirs(os.getenv("BOOST_ROOT") or error("BOOST_ROOT must be defined !"))
        includedirs { "external/freeglut/include", "external/glew/include" }
        
        defines { "FREEGLUT_STATIC", "GLEW_STATIC" }
        
        libdirs { "../_lib" }
        links { "freeglut_static", "glew_static" }
        flags { "FatalWarnings" }
        
        configuration { "windows", "vs*" }
            defines { "WIN32" }
            flags { "NoIncrementalLink", "NoEditAndContinue", "NoPCH" }
            linkoptions{ "/LTCG" }
        
        
        configuration "Debug"
            defines { "_DEBUG" }
            flags { "Symbols" }
            targetname "VisualVega_d"
            
        configuration { "Debug", "vs*" }
            linkoptions { "/NODEFAULTLIB:\"MSVCRT\"" }
            
        configuration { "Hybrid" }
            defines { "NDEBUG" }
            flags { "Symbols" }
            targetname "VisualVega_h"
 
        configuration "Release"
            defines { "NDEBUG" }
            flags { "OptimizeSpeed" }
            targetname "VisualVega"
        
        
        configuration { "Release", "vs*" }
            buildoptions { "/GL" }
            flags {  "NoMinimalRebuild", }
            linkoptions { "/OPT:REF", "/OPT:ICF" }
        