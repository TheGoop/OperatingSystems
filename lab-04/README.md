# Hey! I'm Filing Here

This code implements a 1 MiB ext2 file system with 2 directories, 1 regular file, and 1 symbolic link.

## Building

Run make.

## Running

#### Compile and Mount
./ext2-create
mkdir mnt
sudo mount -o loop cs111-base.img mnt

#### Sample Output
>>> ls -ain mnt/                         
total 7
     2 drwxr-xr-x 3    0    0 1024 Jun  6 16:27 .
269730 drwxr-xr-x 3 1000 1000 4096 Jun  6 16:28 ..
    13 lrw-r--r-- 1 1000 1000   11 Jun  6 16:27 hello -> hello-world
    12 -rw-r--r-- 1 1000 1000   12 Jun  6 16:27 hello-world
    11 drwxr-xr-x 2    0    0 1024 Jun  6 16:27 lost+found

## Cleaning up

Run make clean.
