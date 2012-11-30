solution "VegaTools"
    configurations { "Debug", "Release" }
    
    setup_configurations()
    
    local boost_dir = os.getenv("BOOST_ROOT") or error("BOOST_ROOT must be defined !")
    
    
    project "PrecisionRecall"
        kind "ConsoleApp"
        language "C++"
        
        targetdir "../."
        objdir "../_obj"
        
        files
        {
            "precall3d.cpp",
            "df3volume.cpp",
            "df3volume.h",
            boost_dir .. "/libs/program_options/src/**.cpp",
        }
        
        includedirs(boost_dir)
        
        defines { "BOOST_ALL_NO_LIB" }
        flags { "FatalWarnings" }

        
    project "raw2df3"
        kind "ConsoleApp"
        language "C++"
        
        targetdir "../."
        objdir "../_obj"
        
        files
        {
            "raw2df3.cpp",
            "df3volume.cpp",
            "df3volume.h",
        }
        
        flags { "FatalWarnings" }

        
        
    project "thseg"
        kind "ConsoleApp"
        language "C++"
        
        targetdir "../."
        objdir "../_obj"
        
        files
        {
            "thseg.cpp",
            "df3volume.cpp",
            "df3volume.h",
        }
        
        flags { "FatalWarnings" }