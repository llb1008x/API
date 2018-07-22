#! /bin/sh
#usemsg：sudo sh down_file_for210.sh
#note: makesure you sd card node is /dev/mmcblk0
######################################################################
#block          	data                 	size
#  0             	edu-boot                1M
#  1				Kernel					5M
#  2   				RootFs					128M
#  3				UserData				(SD_size - 134)M
######################################################################
#通过开发板直接下载
dd if=edu-boot.bin of=/dev/mmcblk0 seek=1
dd if=zImage of=/dev/mmcblk0 seek=2048
dd if=rootfs.ext4 of=/dev/mmcblk0 seek=12288

