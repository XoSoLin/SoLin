workspace "SoLin"			--工作区
	architecture "x64"		--架构
	startproject "Sandbox"	--启动项目

	configurations{			--配置
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"		--输出目录和中间生成文件目录
IncludeDir = {}														--创建一个表
IncludeDir["GLFW"] = "SoLin/vendor/GLFW/include"					--将表的"GLFW"键索引到此路径
IncludeDir["GLad"] = "SoLin/vendor/GLad/include"
IncludeDir["ImGui"] = "SoLin/vendor/imgui"
IncludeDir["glm"] = "SoLin/vendor/glm"

--[[包含Nut/Nut/vendor/GLFW中的premake文件并合并到这里]]
include "SoLin/vendor/GLFW"
include "SoLin/vendor/GLad"
include "SoLin/vendor/imgui"
--[[
--    XXXX
--]]
---------------------------------------------------------------------------------------

--引擎
project "SoLin"				--项目
	location "SoLin"
	kind "SharedLib"		--类型为动态库
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")				--输出目录
	objdir ("bin-intermediates/" .. outputdir .. "/%{prj.name}")	--中间目录

	pchheader "slpch.h"												--预编译头文件
	pchsource "SoLin/src/slpch.cpp"									--(实际上编译器不需要 但是VS需要这个.cpp文件)

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	includedirs{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",	                                                --将IncludeDir表中GLFW键索引的值作为一个库文件
		"%{IncludeDir.GLad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links{                           --为SoLin项目(.dll)链接文件
        "GLFW",                                                                 --链接上方项目GLFW
		"GLad",
		"ImGui",
        "opengl32.lib"
    }

	filter "system:windows"
		cppdialect "C++17"
		--staticruntime "On"			--是否静态链接运行时库（dll属性的文件需要打开）
		systemversion "latest"		--最高版本
		buildoptions "/utf-8"		--附加命令行

		defines{					--宏声明
			"SL_PLATFORM_WINDOWS",
			"SL_BUILD_DLL" ,
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands{			--复制时需要有那个exe所在的文件夹，不然dll会一直生成中
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
		}

	filter "configurations:Debug"
		defines "SL_DEBUG"
		runtime "Release"
		buildoptions "/MDd"			--不开会使得引擎和应用的堆建立在不同内存
		symbols "On"

	filter "configurations:Release"
		defines "SL_RELEASE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "SL_DIST"
		buildoptions "/MD"
		optimize "On"

--应用		--------------------------------------------------------------------------------------

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-intermediates/" .. outputdir .. "/%{prj.name}")

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs{
		"SoLin/vendor/spdlog/include",
		"SoLin/src",
		"SoLin/vendor",
		"%{IncludeDir.glm}"
	}

	links{
		"SoLin"
	}

	filter "system:windows"
		cppdialect "C++17"	
		systemversion "latest"
		buildoptions "/utf-8"

		defines{
			"SL_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "SL_DEBUG"
		buildoptions "/MDd"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "SL_RELEASE"
		buildoptions "/MDd"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "SL_DIST"
		buildoptions "/MD"
		runtime "Release"
		optimize "On"