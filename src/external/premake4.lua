solution "External"
    configurations { "Debug", "Release" }
    
    
    targetdir "../../_lib"
    objdir "../../_obj"
    
    
if os.get() == "windows" then
    defines { "_WIN32" }
end
    
    configuration "Debug"
        defines { "_DEBUG" }
        flags { "Symbols" }
        
 
    configuration "Release"
        defines { "NDEBUG" }
        flags { "OptimizeSpeed" }
        

    project "freeglut"
        kind "StaticLib"
        language "C"
        
        location "freeglut"
        
        files { "freeglut/src/*.c", "freeglut/include/**.h" }
        includedirs { "freeglut/include" }
        defines { "FREEGLUT_STATIC" }
        
        configuration "Release"
            targetname "freeglut_static"
        
        configuration "Debug"
            targetname "freeglut_static_d"
        
        
    project "glew"
        kind "StaticLib"
        language "C"
        
        location "glew"
        
        files { "glew/src/glew.c", "glew/include/**.h" }
        includedirs { "glew/include" }
        defines { "GLEW_STATIC" }
        
        configuration "Release"
            targetname "glew_static"
        
        configuration "Debug"
            targetname "glew_static_d"
