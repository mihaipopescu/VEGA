-- External projects
include("src/external")


-- Main solution
solution "VEGA"
    configurations { "Debug", "Release" }
    location "src"
    
    project "VisualVega"
        kind "ConsoleApp"
        language "C++"
        
        location "src/VisualVega"
        objdir "_obj"
        debugdir "."
        
        files { "src/VisualVega/VisualVega.cpp", "src/vega/**.cpp", "src/vega/**.hpp", "src/vega/**.h" }
        
        includedirs(os.getenv("BOOST_ROOT") or error("BOOST_ROOT must be defined !"))
        includedirs { "src/external/freeglut/include", "src/external/glew/include" }
        
        defines { "FREEGLUT_STATIC", "GLEW_STATIC" }
        
        libdirs { "_lib" }
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
            
 
        configuration "Release"
            defines { "NDEBUG" }
            flags { "OptimizeSpeed" }
            targetname "VisualVega"
        
        
        configuration { "Release", "vs*" }
            buildoptions { "/GL" }
            flags {  "NoMinimalRebuild", }
            linkoptions { "/OPT:REF", "/OPT:ICF" }
        