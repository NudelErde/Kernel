# List of all syscalls
syscall is interrupt `0x80` and has 4 64 bit arguments in registers `RAX` to `RDX`.  
 * `RAX == 1` = Process syscall
   * `RBX == 0x01` = exit, `RCX` = return value
   * `RBX == 0x02` = Get process ID, `RCX` = pointer 64 bit result buffer
   * `RBX == 0x03` = Spawn process, `RCX` = pointer to spawn request struct
   * `RBX == 0x04` = Sleep, `RCX` = time in micro seconds
   * `RBX == 0x05` = malloc, `RCX` = size in bytes, `RDX` = pointer to 64 bit result buffer
   * `RBX == 0x06` = free, `RCX` = pointer
   * `RBX == 0x07` = create shared memory page, `RCX` = pointer to 64 bit result buffer
   * `RBX == 0x08` = invite process to shared memory page, `RCX` = shared page id, `RDX` = process id
   * `RBX == 0x09` = get pointer to shared memory page, `RCX` = shared page id, `RDX` = pointer to 64 bit result buffer
   * `RBX == 0x0A` = free shared memory page, `RCX` = shared page id
   * `RBX == 0x0B` = wait for process finish, `RCX` = pid, `RDX` = pointer to 64 bit result buffer
   * `RBX == 0x0C` = get process arguments, `RCX` = pointer to result pointer
   * `RBX == 0x0D` = get parent pid, `RCX` = pointer to 64 bit result buffer
   * `RBX == 0x10` = get method info in other process, `RCX` = pointer to InterProcessMethodInformationRequest struct
   * `RBX == 0x11` = call inter process method, `RCX` = InterProcessMethodCallTargetDescriptor struct
   * `RBX == 0x12` = register inter process method, `RCX` = InterProcessMethodRegistration struct
   * `RBX == 0x13` = not implemented: remove inter process method, `RCX` = method id
 * `RAX == 2` = filesystem syscall
   * `RBX == 1` = Get filesystem count, `RCX` = pointer to 64 bit result buffer
   * `RBX == 2` = Get System filesystem, `RCX` = pointer to 64 bit result buffer
 * `RAX == 3` = basic IO syscall
   * `RBX == 1` = write data, `RCX` = one data byte
   * `RBX == 2` = read data, `RCX` = pointer to 8 bit result buffer
 * `RAX == 4` = ext4 syscall
   * `RBX == 1` = get inode of path, `RCX` = filesystem, `RDX` = pointer to inodeOfPathRequest struct
   * `RBX == 2` = get sector of inode, `RCX` = filesystem, `RDX` = pointer to sectorOfInodeRequest struct
   * `RBX == 3` = get flags of inode, `RCX` = filesystem, `RDX` = pointer to flagsOfInodeRequest struct
   * `RBX == 4` = get directory entry, `RCX` = filesystem, `RDX` = pointer to directoryEntriesRequest struct
   * `RBX == 5` = get size of file, `RCX` = filesystem, `RDX` = pointer to fileSizeRequest struct
 * `RAX == 5` = pci device control
   * `RBX == 0x01` = get native pci control, `RCX` = bus device function, `RDX` = pointer to buffer //not implemented
   * `RBX == 0x02` = get pci device list, `RCX` = buffer element count, `RDX` = pointer to buffer
   * `RBX == 0x03` = get pci device header, `RCX` = bus device function, `RDX` = pointer to buffer
   * `RBX == 0x04` = allocate memory page, `RCX` = virtual address
   * `RBX == 0x05` = free memory page, `RCX` = virtual address
   * `RBX == 0x06` = virtual to physical address, `RCX` = virtual address, `RDX` = pointer to 64 bit result buffer
   * `RBX == 0x07` = map virtual to physical address, `RCX` = virtual address, `RDX` = physical address
   * `RBX == 0x10` = get driver status, `RCX` = bus device function, `RDX` = pointer to 64 bit result buffer
   * `RBX == 0x11` = driver call 1, `RCX` = bus device function, `RDX` = arg
   * `RBX == 0x12` = driver call 2, `RCX` = bus device function, `RDX` = arg
   * `RBX == 0x13` = driver call 3, `RCX` = bus device function, `RDX` = arg
   * `RBX == 0x14` = driver call 4, `RCX` = bus device function, `RDX` = arg
   * `RBX == 0x15` = driver call 5, `RCX` = bus device function, `RDX` = arg
   * `RBX == 0x16` = driver call 6, `RCX` = bus device function, `RDX` = arg
   * `RBX == 0x17` = driver call 7, `RCX` = bus device function, `RDX` = arg
   * `RBX == 0x18` = driver call 8, `RCX` = bus device function, `RDX` = arg
   * `RBX == 0x19` = driver call 9, `RCX` = bus device function, `RDX` = arg
   * `RBX == 0x1A` = driver call A, `RCX` = bus device function, `RDX` = arg
   * `RBX == 0x1B` = driver call B, `RCX` = bus device function, `RDX` = arg
   * `RBX == 0x1C` = driver call C, `RCX` = bus device function, `RDX` = arg
   * `RBX == 0x1D` = driver call D, `RCX` = bus device function, `RDX` = arg
   * `RBX == 0x1E` = driver call E, `RCX` = bus device function, `RDX` = arg
   * `RBX == 0x1F` = driver call F, `RCX` = bus device function, `RDX` = arg

# Kernel driver interface
 * USB
 * Ethernet
   * 1 = getConnectionStatus()
     * `uint64_t status`
   * 2 = getReceivedPacket()
     * `uint64_t frameSize`
     * `uint8_t buffer[2Ki]`
   * 3 = transmitPacket()
     * `uint64_t success`
     * `uint8_t buffer[2Ki]`
 * VGA
   * 1 = getWidth()
     * `uint64_t width`
   * 2 = getHeight()
     * `uint64_t height`
   * 3 = setMode()
     * `VGA::Mode mode`
   * 4 = drawBuffer()
     * `uint64_t start`
     * `uint64_t bufferSize`
     * `uint8_t buffer[1Ki]`
