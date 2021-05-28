# Kernel

## Building

Use `docker run --rm -v "${pwd}/Kernel:/root/env" [dockername] make build-x86_64` to build the kernel.  
Use `qemu-system-x86_64 -nodefaults -readconfig Kernel/qemuConfig.cfg -L "C:\Program Files\qemu" --serial stdio -smp 4 -device usb-kbd` to run the Kernel in
the default configuration.  
The buildtool is currently windows specific.  
To build a program create a directory and place the .cpp source files in the `src` subdirectory.  
Compile the program and start the VM with `./build.ps1 [directory] [target name]`.  
It should be relatively easy to create programs on non-Windows computers using the commands in `build.ps1`.  
I don't know how reliable the Build Tool works, but it works on my computer most of the time.  
A raw image file at `./Kernel/image_file` with an ext4 filesystem is expected.  
The Kernel tries to start the program at `/startup.elf`

## Kernel features

A full list of syscalls can be found [here](./Kernel/SystemCalls.md). An interface to all implemented syscalls is in the [setup.hpp](./Base/header/setup.hpp).
Kernel feature list:
* x86_64
* paging
* preemptive scheduling
* shared memory pages
* ext4 filesystem reads and writes (writes are not fully implemented and tested)
* execution of elf files