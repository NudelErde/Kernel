# BadKernel
I don't know what I do.  
Structure is copy pasted from [davidcallanan](https://github.com/davidcallanan/os-series/tree/master).  
Most of the code is copy pasted from the [osdev](https://wiki.osdev.org) wiki.

## Setup
Docker is used as an build environment. Create the docker image with:  
* `docker build buildenv -t myos-buildenv`

## Build

* Linux: `docker run --rm -v "$pwd":/root/env myos-buildenv make build-x86_64`  
* MacOS: `docker run --rm -v "$PWD":/root/env myos-buildenv make build-x86_64`  
* Windows (CMD): `docker run --rm -v "%cd%":/root/env myos-buildenv make build-x86_64`  
* Windows (PowerShell): `docker run --rm -v "${pwd}:/root/env" myos-buildenv make build-x86_64`  

## Emulate

The operating system can be emulated using [Qemu](https://www.qemu.org/)

 * `qemu-system-x86_64 -cdrom dist/x86_64/kernel.iso`

You should specify a memory size with `-m` and a serial port with `--serial`.  
I don't know why, but my qemu command won't work without `-L "C:\Program Files\qemu"`.