-- Add the packages required
add_requires("catch2", "boost", "fmt", "spdlog", {system = false})

-- Create the Stellar-Engine-Test project
target("test")

    -- Set the project kind to binary
    set_kind("binary")

    -- Set the target directory to bin/tests
    set_targetdir("../bin")

    -- Set the C++ standard to C++20
    set_languages("cxx20")

    -- Add the source files
    add_files("**.cpp")

    -- Add Stellar-Engine as a dependency
    add_deps("Stellar-Engine")

    -- Add the include directories
    add_includedirs("../includes", "../includes/Core", "../includes/Core/Components", "../includes/Core/Events", "../includes/Core/Systems", {public = true})

    -- Add the packages required
    add_packages("catch2", "boost", "fmt", "spdlog")
