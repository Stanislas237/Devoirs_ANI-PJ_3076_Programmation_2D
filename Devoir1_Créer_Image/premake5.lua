workspace "2D Project"

configurations { "Debug", "Release" }

project "PixelApp"
    kind "ConsoleApp"
    language "C++"
    files { "**.h", "**.cpp" }