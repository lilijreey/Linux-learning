/**
 * @file     useGlo.c
 *           应用层使用 globalmem
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     06/05/2013 02:33:29 PM
 *
 */


///
//1 insmod globalmem
//2 创建设备节点 mknod  /dev/globalmem c 250 0 
//3 echo 'hello world" > /dev/globalmem
//4 cat /dev/globalmem
