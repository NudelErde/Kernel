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
 * `RAX == 2` = ATA device syscall
   * `RBX == 1` = Get ATA device count, `RCX` = pointer to 64 bit result buffer
   * `RBX == 2` = Get System ATA device, `RCX` = pointer to 64 bit result buffer
 * `RAX == 3` = basic IO syscall
   * `RBX == 1` = write data, `RCX` = one data byte
   * `RBX == 2` = read data, `RCX` = pointer to 8 bit result buffer
 * `RAX == 4` = ext4 syscall
   * `RBX == 1` = get inode of path, `RCX` = device, `RDX` = pointer to inodeOfPathRequest struct
   * `RBX == 2` = get sector of inode, `RCX` = device, `RDX` = pointer to sectorOfInodeRequest struct
   * `RBX == 3` = get flags of inode, `RCX` = device, `RDX` = pointer to flagsOfInodeRequest struct
   * `RBX == 4` = get directory entry, `RCX` = device, `RDX` = pointer to directoryEntriesRequest struct