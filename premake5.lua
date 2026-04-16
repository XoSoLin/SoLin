workspace "SoLin"			--工作区
	architecture "x64"		--架构
	startproject "SoLinEditor"	--启动项目

	configurations{			--配置
		"Debug",
		"Release",
		"Dist"
	}

	flags{								--设置编译器选项
		"MultiProcessorCompile"			--多处理器并行编译
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"		--输出目录和中间生成文件目录
IncludeDir = {}														--创建一个表
IncludeDir["GLFW"] = "SoLin/vendor/GLFW/include"					--将表的"GLFW"键索引到此路径
IncludeDir["GLad"] = "SoLin/vendor/GLad/include"
IncludeDir["ImGui"] = "SoLin/vendor/imgui"
IncludeDir["glm"] = "SoLin/vendor/glm"
IncludeDir["stb_image"] = "SoLin/vendor/stb_image"
IncludeDir["entt"] = "SoLin/vendor/entt/include"
IncludeDir["yaml_cpp"] = "SoLin/vendor/yaml-cpp/include"

group "Dependencies"
--[[包含Nut/Nut/vendor/GLFW中的premake文件并合并到这里]]
include "SoLin/vendor/GLFW"
include "SoLin/vendor/GLad"
include "SoLin/vendor/imgui"
include "SoLin/vendor/yaml-cpp"
--[[
--    XXXX
--]]
group ""
---------------------------------------------------------------------------------------

--引擎
project "SoLin"				--项目
	location "SoLin"
	kind "StaticLib"		--类型为静态库
	language "C++"
	staticruntime "on"
	cppdialect "C++17"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")				--输出目录
	objdir ("bin-intermediates/" .. outputdir .. "/%{prj.name}")	--中间目录

	pchheader "slpch.h"												--预编译头文件
	pchsource "SoLin/src/slpch.cpp"									--(实际上编译器不需要 但是VS需要这个.cpp文件)

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	defines{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",	                                                --将IncludeDir表中GLFW键索引的值作为一个库文件
		"%{IncludeDir.GLad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.glm}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.yaml_cpp}"
	}

	links{                           --为SoLin项目(.dll)链接文件
        "GLFW",                                                                 --链接上方项目GLFW
		"GLad",
		"ImGui",
        "yaml",                     --子lua脚本中的项目名叫yaml
        "opengl32.lib"
    }

	filter "system:windows"
		systemversion "latest"		--最高版本
		buildoptions "/utf-8"		--附加命令行

		defines{					--宏声明
			"SL_BUILD_DLL" ,
			"GLFW_INCLUDE_NONE"
		}

		--  //////////////////////////////////////////////////////////////////////
        --  ////  NOW WE USE NUT AS A STATIC LIB, SO DON'T NEED THIS COMMAND  ////
        --  //////////////////////////////////////////////////////////////////////
		--[[postbuildcommands{			--复制时需要有那个exe所在的文件夹，不然dll会一直生成中
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
		}]]

	filter "configurations:Debug"
		defines "SL_DEBUG"
		runtime "Debug"
		--buildoptions "/MDd"			--不开会使得引擎和应用的堆建立在不同内存
		symbols "on"

	filter "configurations:Release"
		defines "SL_RELEASE"
		runtime "Release"
		--buildoptions "/MD"
		optimize "on"

	filter "configurations:Dist"
		defines "SL_DIST"
		runtime "Release"
		--buildoptions "/MD"
		optimize "on"

--应用		--------------------------------------------------------------------------------------

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

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
		systemversion "latest"
		buildoptions "/utf-8"

		defines{
			"SL_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "SL_DEBUG"
		--buildoptions "/MDd"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "SL_RELEASE"
		--buildoptions "/MDd"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "SL_DIST"
		--buildoptions "/MD"
		runtime "Release"
		optimize "on"

--引擎编辑器		--------------------------------------------------------------------------------------

project "SoLinEditor"
	location "SoLinEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

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
		"%{IncludeDir.glm}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.yaml_cpp}"
	}

	links{
		"SoLin"
	}

	filter "system:windows"	
		systemversion "latest"
		buildoptions "/utf-8"

		defines{
		}

	filter "configurations:Debug"
		defines "SL_DEBUG"
		--buildoptions "/MDd"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "SL_RELEASE"
		--buildoptions "/MDd"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "SL_DIST"
		--buildoptions "/MD"
		runtime "Release"
		optimize "on"
