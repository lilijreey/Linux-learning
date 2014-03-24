## Personal Learning GNU autotools notes
GCS (GNU Coding Standards)
FHS (Filesystem Hierarchy Standard)

### 作用
+   主要解决开发者和用户之间的配置不同
+   The primary purpose of the Autotools is to make life simpler for the end user.
+   end user's system doen's need to have the Autotools installed. just
    need a make and bash


### Open source softare ditribution step 
1.  build
2.  test
3.  installation

### 使用流程
1.  写configure.ac 文件
       也可使用automake 生成一个模板来修改
    写 Makefile.am src/Makefile.am 文件

2.  生成模板文件和工具文件
        autoreconf --install 会生成 模板文件
              +-------------------
              |配置文件模板
              |     configure
              |     config.h.in
              |     Makefile.in
              |     src/Makefile.in
              | ----------------
              | 定义第三方宏被 configure.ac 使用
              |     aclocal.m4
              | -----------------
              | 工具脚本
              |     install-sh
              |     depcomp
              |     missing
              | ----------------
              | autotools 缓存文件
              |     autom4te.cache
              +------------------------

3.  生成build 文件 
       run ./configure --args...  这里可以设置很多参数 --prefix=...
          config.status
          Makefile
          src/Makefile
          config.h

4.  Build
      run ./make

5.  Install 
      run ./make install

6.  distribution 打包
      run ./make distcheck
      


### 文件生成顺序
configure.ac -> configure
Makefile.am -> Makfile.in(an Autoconf template)
config.h.in -> config.h
configure -> config.log config.status
automake -> Makefile.in

### 支持语言
C C++ ObjectiveC Fortran Erlang

### autoconf
+   根据configure.ac 生成一个可移植的配置脚本
        e.g. autoconf 
+   提供的工具
    +   autoconf
            Create configure from configure.ac
    +   autoheader generates 
            Create config.h.in from configure.ac
    +   autom4te   
    +   autoreconf
            Run all tools in the right order
    +   autoscan 
            扫描源文件.查看configure.ac 中是要写但是还没有写的宏
              e.g 没有对依赖的头文件进行检测。 使用autoscan 就会
              生成一个 configure.scan 是autoscan 自己认为的合理的
              configure.ac 文件
    +   autoupdate
            更新 configure.ac 中过时的宏
    +   ifnames

### automake
+   创建一个GNU标准可移植的Makefile 文件
    +   automake
        创建一个复杂的Makefile.in file from Makefile.am 
    +   aclocal
            扫描 configure.ac 生成 aclocal.m4

## configuer.ac
 执行的命令后生产一个 confing.log 的文件记录所有configuer.ac 的输出


## config.staus
 用来重新生产产生的文件
  e.g.
  ./config.status --file=src/config.h

## config.log
  记录执行./configuer 后所有的输出和生成的宏

make mostlyclean
make clean
make distclean
make maintainer-clean
