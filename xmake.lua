set_project("Stellar-Engine")

set_toolchains("clang")
set_languages("cxx20")

add_rules("mode.debug", "mode.release")

includes("src")
includes("test")

add_headerfiles("includes/(**.hpp)")
