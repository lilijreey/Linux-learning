/**
 * @file     test.c
 *           
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     02/15/2014 10:58:04 AM
 *
 */

// 发送信息
///int MPI_send(void *buf, int len , MPI_Datatype datatype, 
///         int dest, int tag, MPI_Comm comm)
/// @param buf 发送数据的起始地址
/// @param len 发送数据的长度 # 不是字节的数量，是发送数据类型的单位数量
/// @param datatype 发送数据的数据类型 # 可以是MPI预定义的也可是用户自定义的
/// @param dest 目的进程号
/// @param tag 消息标志 # 用于把本机这次发送的消息和一起发送的消息区别开
/// @param comm 通信与

// receive msg
/// int MPI_Recv(void *buf, int cout, MPI_Datatype datatype,
///              int source, int tag, MPI_Comm comm, MPI_Status *status)

#include <mpi.h>
#include <stdio.h>
#include <string.h>


#if 0

int
main(int argc, char *argv[])
{

    int myid, numproces;
    int namelen;

    char processor_name[MPI_MAX_PROCESSOR_NAME];

    ///EE init mpi mpi程序的第一个调用,他完成MPI的初始化工作
    MPI_Init(&argc, &argv);

    //EE 得到当前进程的标示
    ///int MPI_Comm_rank(MPI_Comm comm, int *rank)
    /// @param comm [IN] 该进程所在的通信句柄
    /// @param rank [OUT] 该进程在comm 中的标石
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);


    // EE 通信与包含的进程数量
    /// MPI_Comm_size(MPI_Comm comm, int *size)
    /// @param comm [IN] 通信鱼句柄
    /// @param size [OUT] comm 中的进程数量
    MPI_Comm_size(MPI_COMM_WORLD, &numproces);

    MPI_Get_processor_name(processor_name, &namelen);
    printf("Hello, world %d of %d on %s\n", myid, numproces, processor_name);

    //EE finalize, 它是MPI的最后一个实现语句，否则程序运行结果是不可知的
    MPI_Finalize();
    return 0;
}				/* ----------  end of function main  ---------- */

#endif


// send and recive


int
main(int argc, char *argv[])
{

    char msg[20];
    int myrank;
    MPI_Status status;
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    if (myrank == 0) {
        strcpy(msg, "Hello");
        MPI_Send(msg, strlen(msg), MPI_CHAR, 1, 99, MPI_COMM_WORLD);
        printf("send over\n");
    } else if (myrank == 1) {
        MPI_Recv(msg, 20, MPI_CHAR, 0, 99, MPI_COMM_WORLD, &status);
        printf("\nRecv:%s\n",msg);
    }

    MPI_Finalize();

    return 0;
}				/* ----------  end of function main  ---------- */
