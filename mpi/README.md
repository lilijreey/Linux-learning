MPI
===============================
+ install
   sudo apt-get install mpich2
    安装整个mpi环境和库

+ mpirun
  运行一个mpi程序，
   e.g. mpirun -n <num> <program>
     同时使用N个进程运行

+ 配置mpi节点
  在/mpich/util/machines/machines.LINUX 上写上节点ip

