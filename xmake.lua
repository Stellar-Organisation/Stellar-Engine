set_project("Stellar-Engine")

set_toolchains("clang")

add_rules("mode.debug", "mode.release")

includes("src")
includes("test")

add_headerfiles("includes/(**.hpp)")
