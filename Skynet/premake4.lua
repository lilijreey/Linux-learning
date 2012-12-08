--------------------------------------------------------------------------------
--         FILE:  premake.lua
--  DESCRIPTION:  Build Project
--       AUTHOR:  lili (Evan)
--        EMAIL:  <lilijreey@gmail.com>
--      CREATED:  11/07/2012 07:18:54 AM CST
--------------------------------------------------------------------------------
--     premake4 --cc=gcc --os=linux gmake

solution "Skynet"
    configurations { "Debug", "Release" }

    project "SkynetPro" 

    --SharedLib --    A shared library, or DLL.
    --StaticLib --    A static library. 
    --ConsoleApp --    A console, or command-line, executable.
    --WindowedApp --    An application that runs 
    --        in a desktop window. This distinction does not
    --        apply on Linux, but is important on Windows and Mac OS X.
    kind "ConsoleApp"

    --param: C C++ 
    language "C"

    --premake生成的solution 和 project 文件的目录
    location "build"

    defines {"_REENTRANT", "_GNU_SOURCE"}
    files { "src/**.c", "src/**.h"}
    includedirs {"src"} 
    linkoptions {"-ldl", "-rdynamic", "-lm", "-lglib-2.0", "-lpthread"}

    configuration "Debug"
        targetdir "bin/debug"
        --EE defines 定义预处理符号
        defines { "DEBUG" } -- limit to configuration (Debug) 
        -- EE flags premake 自己定义的一套编译选项 
        -- 建议不要使用,用buildoptions 和 linkoptions 
        -- Symbols 包含调试信息
        --flags { "Symbols" , }

        --EE buildoptions 直接把参数传递给编译器不转换
        buildoptions {"-Wall", "-g",
                        "`pkg-config --cflags glib-2.0`"}

        --EE targetdir 指定目标文件的存放目录

    configuration "Release"
        defines { "NDEBUG" }
        buildoptions {"-Wall", "-O2",
                    "-funroll-loops",
                    "-fhosted",
                    "`pkg-config --cflags glib-2.0`"}
        targetdir "bin/release"
