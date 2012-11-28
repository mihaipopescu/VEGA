solution "VegaTools"
    configurations { "Debug", "Release" }
    
    setup_configurations()
    
    local boost_dir = os.getenv("BOOST_ROOT") or error("BOOST_ROOT must be defined !")
    
    targetdir "../."
    objdir "../_obj"
    
    project "PrecisionRecall"
        kind "ConsoleApp"
        language "C++"
        
        files
        {
            "precall3d.cpp",
            boost_dir .. "/libs/program_options/src/**.cpp",
        }
        
        includedirs(boost_dir)
        
        defines { "BOOST_ALL_NO_LIB" }
        flags { "FatalWarnings" }

        
    project "raw2df3"
        kind "ConsoleApp"
        language "C++"
        
        
        files
        {
            "raw2df3.cpp",
        }
        
        flags { "FatalWarnings" }
