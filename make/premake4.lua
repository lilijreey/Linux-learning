--------------------------------------------------------------------------------
--         FILE:  premake.lua
--  DESCRIPTION:  Build Project
--       AUTHOR:  lili (Evan)
--        EMAIL:  <lilijreey@gmail.com>
--      CREATED:  11/07/2012 07:18:54 AM CST
--------------------------------------------------------------------------------
--  premake 4.0 版本以上使用
--  premake4 默认的配置文件名为 premake4.lua
--  EE linux 下可以使用 这个生成Makefile 文件
--     premake4 --cc=gcc --os=linux gmake

-- premake4 定义的都是函数。只不过不写括号罢了
-- 配置文件是由层次的(作用域）
--  Solutions, projects, configuration

-- EE solution 定义solution 函数
-- 每一个build 都有一个solution 在top level
-- 相当于workspace。是项目的容器(project)
-- 一个solution 定义一个配置集合(configurations) 和多个项目(Project)
--
solution "Skynet"
   -- 这里定义的configurations 是全局的。对所有project 产生影响
   -- EE configurations 定义不同的build设置
   -- 使用 make config=Debug 来选择不同的编译配置
   configurations { "Debug", "Release" }
 
   -- EE project defines one build target
   --  project 生成一个可执行的目标
   -- project to build on binary target.
   -- 必须在定义过solution 和 configurations 后才可以建立
   project "SkynetPro" -- 不指定目标文件的名字，就会一这个为名字

   --
   -- EE kind:  生成的目标文件类型。 每个project 都要指定
   --kind is the project kind identifier, and must be one of:
    --SharedLib --    A shared library, or DLL.
    --StaticLib --    A static library. 
    --ConsoleApp --    A console, or command-line, executable.
    --WindowedApp --    An application that runs 
    --        in a desktop window. This distinction does not
    --        apply on Linux, but is important on Windows and Mac OS X.
      kind "ConsoleApp"

    --EE language: 指定生用的语言。 每个project 都要指定
    --param: C C++ C# 都大写
      language "C"


    --EE location: 指定premake生成的solution 和 project 文件的目录
    --默认和配置文件一个目录(不指定location)
      location "build"

    --EE files 指定原文件
    --files 的参数可以使用通配符
    --*表示在当前目录中查找
    --**表示递归的搜索当前目录和子目录
      files { "src/**.c", "src/**.h"}
      --EE excludes 在files中屏蔽不需要的src文件
 
      defines {"_REENTRANT", "_GNU_SOURCE"}
    --EE links
    --links {"glib-2.0"}
    includedirs {"src"} --生成相应 -I 选项
    --libdirs {..} --like -L
      --EE linkoptions  直接把参数传递给连接器不转换
      linkoptions {"-ldl", "-rdynamic", "-lm", "-lglib-2.0", "-lpthread"}
     --EE targetname 指定目标文件的名字
     --targetname "Goo"
     --
      configuration "Debug"
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
     targetdir "bin/debug"

      configuration "Release"
         defines { "NDEBUG" }
        buildoptions {"-Wall", "-O2",
                      "-funroll-loops",
                      "-fhosted",
                      "`pkg-config --cflags glib-2.0`"}
     targetdir "bin/release"
