workspace "RayTracer"
    configurations { 
        "Debug", 
        "Release" 
    }

    architecture "x64"

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
    
project "RayTracer"
    location "RayTracer"
    kind "ConsoleApp"
    language "C++"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    files
    {
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.h"
    }
    
    includedirs
    {
      "%{prj.name}/src",
      "vendor/GLFW/include",
      "vendor/glad/include",
      "Magenta/Magenta/src",
      "vendor/GLEW/include",
      "vendor/glm",
      "vendor/stb_image",
      "vendor/imgui/",
      "vendor/imgui/backends"
    }
    
    libdirs
    {
      "vendor/GLFW/lib",
      "vendor/GLEW/lib"
    }
    
    links
    {
      "Magenta",
      "glfw3dll",
      "opengl32",
      "glew32",
      "ImGUI"
    }
    
    postbuildcommands
    {
        ("cp ../vendor/GLFW/lib/glfw3.dll ../bin/" .. outputdir .. "/RayTracer"),
        ("cp ../vendor/GLEW/lib/glew32.dll ../bin/" .. outputdir .. "/RayTracer")
    }

    cppdialect "C++20"
    staticruntime "On"
    systemversion "latest"
    
    filter "configurations:Debug"
      defines "SB_DEBUG"
      symbols "On"
    
    filter "configurations:Release"
    defines "SB_BUILD"
    optimize "On"

project "Magenta"
    location "Magenta/Magenta"
    kind "StaticLib"
    language "C++"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    files
    {
        "%{prj.name}/Magenta/src/**.cpp",
        "%{prj.name}/Magenta/src/**.h",
        "vendor/stb_image/**.cpp",
        "vendor/stb_image/**.h"
    }
    
    includedirs
    {
      "%{prj.name}/Magenta/src",
      "vendor/GLFW/include",
      "vendor/glad/include",
      "vendor/GLEW/include",
      "vendor/glm",
      "vendor/stb_image",
      "vendor/imgui/",
      "vendor/imgui/backends",
    }

    libdirs
    {
      "vendor/GLFW/lib",
      "vendor/GLEW/lib"
    }

    links
    {
      "glfw3dll",
      "opengl32",
      "glew32",
      "ImGUI"
    }
    
    cppdialect "C++20"
    staticruntime "On"
    systemversion "latest"

    filter "configurations:Debug"
      defines "MG_DEBUG"
      symbols "On"
    
    filter "configurations:Release"
      defines "MG_BUILD"
      optimize "On"
 
project "ImGUI"
    location "vendor/imgui"
    kind "StaticLib"
    language "C++"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    files
    {
      "vendor/imgui/*.cpp",
      "vendor/imgui/*.h",
      "vendor/imgui/backends/imgui_impl_glfw.cpp",
      "vendor/imgui/backends/imgui_impl_glfw.h",
      "vendor/imgui/backends/imgui_impl_opengl3.cpp",
      "vendor/imgui/backends/imgui_impl_opengl3.h"
    }
    
    includedirs
    {
      "vendor/imgui",
      "vendor/imgui/backends",
      "vendor/GLFW/include",
    }

    libdirs
    {
      "vendor/GLFW/lib",
      "vendor/GLEW/lib"
    }

    links
    {
      "glfw3",
      "opengl32",
      "glew32"
    }
 
    cppdialect "C++20"
    staticruntime "On"
    systemversion "latest"