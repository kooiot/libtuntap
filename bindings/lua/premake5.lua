-- A solution
workspace "lua-tuntap"
	configurations { "Debug", "Release"}
	location "build"

project "tuntap"
	kind "SharedLib"
	language "C++"
	location "build/lua-tuntap"
	targetprefix ""
	targetdir "bin/%{cfg.buildcfg}"
	dependson { "tuntap-static" }
	cppdialect "C++14"
	defines { "TUNTAP_STATIC_DEFINE" }

	includedirs { 
		"/home/cch/mycode/skynet/3rd/lua/",
		--"/usr/include/lua5.3",
		".",
		"../cpp/",
		"../../",
	}
	files { 
		"../cpp/**.hh",
		"../cpp/**.cc",
		"./src/**.hpp",
		"./src/**.cpp",
		"./sol/**.hpp",
	}

	buildoptions { '-Wpedantic', '-pedantic', '-pedantic-errors', '-Wno-noexcept-type' }

	links { "pthread", "tuntap-static" }

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"

project "tuntap-static"
	kind "StaticLib"
	language "C++"
	location "build/tuntap"
	targetdir "bin/%{cfg.buildcfg}"
	defines { "TUNTAP_STATIC_DEFINE" }

	includedirs { 
		".",
		"../../",
	}
	files { 
		"../../tuntap.h",
		"../../tuntap.c",
		"../../tuntap_log.c",
	}
	if os.istarget("linux") then
		files {
			"../../tuntap-unix.c",
			"../../tuntap-unix-linux.c",
		}
		defines { "Linux" }
	elseif os.istarget('windows') then
		defines { "Windows" }
		files {
			"../../tuntap-windows.c",
		}
	end

	buildoptions { '-Wpedantic', '-pedantic', '-pedantic-errors', "-fPIC" }

	--libdirs { "../../bin" }
	links { "pthread"}
	--linkoptions { "" }

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"


