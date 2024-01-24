set_project("Stellar-Engine")

if is_plat("windows") then
    add_toolchains("msvc")
else
    add_toolchains("clang")
end

set_languages("cxx20")
add_rules("mode.debug", "mode.release")

includes("src")
includes("test")

add_headerfiles("includes/(**.hpp)")
