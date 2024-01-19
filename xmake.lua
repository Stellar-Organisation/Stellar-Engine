set_project("Stellar-Engine")

add_toolchains("llvm")
set_languages("cxx20")

add_rules("mode.debug", "mode.release")

includes("src")
includes("test")

add_headerfiles("includes/(**.hpp)")
