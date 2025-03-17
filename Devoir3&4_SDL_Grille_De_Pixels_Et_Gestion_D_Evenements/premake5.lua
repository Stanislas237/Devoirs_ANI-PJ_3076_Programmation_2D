include "./Scripts/lua/config.lua"

workspace "Prog2D"
    configurations { "Debug", "Release" }
    platforms { "x64" }
	startproject ("Base")
    
    group "Core"
        include "./Base"
    group ""
