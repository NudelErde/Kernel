#!/bin/sh
echo "Inject" $1 "to" $2

loopDevice=$(losetup -f)
losetup -P $loopDevice ./Kernel/image_file
mkdir -p image
mount ${loopDevice}p1 image
mkdir -p image/$(dirname ${2})
mv $1 image/${2}
ls image
umount image
losetup -d ${loopDevice}