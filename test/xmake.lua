-- Add the packages required
add_requires("catch2", "boost", "fmt", "spdlog", {system = true})

-- Create the Stellar-Engine-Test project
target("test")

    -- Set the project kind to binary
    set_kind("binary")

    -- Set the target directory to bin/tests
    set_targetdir("../bin")

    -- Check if the platform is Windows
    if is_plat("windows") then
        -- Set the C++ standard to C++20 for MSVC
        add_cxxflags("/std:c++20", {force = true})
    else
        -- For other platforms (e.g., Linux)
        add_cxxflags("-std=c++20", "-lstdc++")
    end

    -- Add the source files
    add_files("**.cpp")

    -- Add Stellar-Engine as a dependency
    add_deps("Stellar-Engine")

    -- Add the include directories
    add_includedirs("../includes", "../includes/Core", "../includes/Core/Components", "../includes/Core/Events", "../includes/Core/Systems", {public = true})

    -- Add the packages required
    add_packages("catch2", "boost", "fmt", "spdlog")
