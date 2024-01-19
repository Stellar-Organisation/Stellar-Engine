set_project("Stellar-Engine")

add_requires("llvm >= 16.x", {alias = "llvm"})

set_toolchains("llvm@llvm")
set_languages("cxx20")

add_rules("mode.debug", "mode.release")

includes("src")
includes("test")

add_headerfiles("includes/(**.hpp)")
