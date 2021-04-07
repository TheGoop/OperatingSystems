# A Kernel Seedling

The module outputs a count of the total number of running processes.

## Building

Run make.

## Running

After building, use "sudo insmod proc_count.ko" to insert the module into the kernel. Then, 
do "cat /proc/count" to run the module.

## Cleaning Up

Do "sudo rmmod proc_count" to remove the module from the kernel, then run "make clean" to 
remove all the generated files.

## Testing

Kernel Version: #1 SMP PREEMPT Wed, 24 Mar 2021 18:53:54 +0000.
