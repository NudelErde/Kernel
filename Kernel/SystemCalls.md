# List of all syscalls
syscall is interrupt `0x80` and has 4 64 bit arguments in registers `RAX` to `RDX`.  
 * `RAX == 1` = Process syscall
   * `RBX == 1` = exit, `RCX` = return value
   * `RBX == 2` = Get process ID, `RCX` = pointer 64 bit result buffer
   * `RBX == 3` = Spawn process, `RCX` = pointer to spawn request struct
   * `RBX == 4` = Sleep, `RCX` = time in micro seconds
   * `RBX == 5` = malloc, `RCX` = size in bytes, `RDX` = pointer to 64 bit result buffer
   * `RBX == 6` = free, `RCX` = pointer
   * `RBX == 7` = create shared memory page, `RCX` = pointer to 64 bit result buffer
   * `RBX == 8` = invite process to shared memory page, `RCX` = shared page id, `RDX` = process id
   * `RBX == 9` = get pointer to shared memory page, `RCX` = shared page id, `RDX` = pointer to 64 bit result buffer
   * `RBX == 10` = free shared memory page, `RCX` = shared page id
   * `RBX == 11` = not implemented: get method id in other process, `RCX` = target pid, `RDX` = pointer to InterProcessMethodInformationRequest struct
   * `RBX == 12` = not implemented: call inter process method, `RCX` = InterProcessMethodCallTargetDescriptor struct, `RDX` = pointer to args
   * `RBX == 13` = not implemented: register inter process method, `RCX` = InterProcessMethodRegistration struct
   * `RBX == 14` = not implemented: remove inter process method, `RCX` = method id
   * `RBX == 15` = wait for process finish, `RCX` = pid, `RDX` = pointer to 64 bit result buffer
   * `RBX == 16` = get process arguments, `RCX` = pointer to result pointer
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