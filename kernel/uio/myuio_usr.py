#!/usr/bin/python
# -*- coding: utf-8 -*-  

import mmap
import time

UIO="uio0"

uio_map0_addr = 0
uio_map0_size = 0

## 1.得到uio的地址和大小
with open("/sys/class/uio/{}/maps/map0/addr".format(UIO)) as f:
    uio_map0_addr = int(f.read().strip('\n'), 16)

with open("/sys/class/uio/{}/maps/map0/size".format(UIO)) as f:
    uio_map0_size= int(f.read().strip('\n'), 16)

print("uio map0 addr {:x}, size:{}".format(uio_map0_addr, uio_map0_size))

## 2. 映射
with open("/dev/{}".format(UIO), "r+b") as f:
    mm = mmap.mmap(f.fileno(),uio_map0_size, mmap.MAP_SHARED,mmap.PROT_READ|mmap.PROT_WRITE)
    print("mmap uio0 ok");
    print("read mm:{}".format(mm.readline()))
    mm.seek(0)
    mm.write("hello python {}\n".format(time.time()));
    mm[0]='h'


