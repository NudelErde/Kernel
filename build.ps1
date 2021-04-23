$progDir = $args[0]
if ($args[1] -ne $null) {
    $progResultName = $args[1]
} else {
    $progResultName = Split-Path -Path ${progDir} -leaf
    $progResultName = "${progResultName}.elf"
}
echo "Build executeable from $progDir and output to $progResultName"
docker run --rm -v "${pwd}:/root/env" myos-buildenv make -C Base program_dir=../$progDir
wsl ./inject.sh ${progDir}/$(Split-Path -Path ${progDir} -leaf).elf $progResultName
qemu-system-x86_64 -cdrom Kernel/dist/kernel.iso -drive file=Kernel/image_file,format=raw -L "C:\Program Files\qemu" -m 6G --serial stdio -boot d -smp 4