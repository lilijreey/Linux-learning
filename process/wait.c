//wait/waitpid 只能等待调用进程的子进程!!!
// pid < -1 的时候还是只能wait 子进程,只不过这些子进程现在的pgrp 是指定的
