
function setup_configurations()
    configuration "Debug"
        defines { "_DEBUG" }
        flags { "Symbols" }
        
    configuration "Release"
        defines { "NDEBUG" }
        flags { "Optimize" }
end

-- External projects
include("src/external")

-- Main solution
include("src")

-- Tools
include("tools")

newaction {
    trigger     = "backup",
    description = "Create a backup for Vega",
    execute     = function ()
        os.mkdir("_backup")
        os.copyfile("VisualVega.exe", "_backup")
    end
}