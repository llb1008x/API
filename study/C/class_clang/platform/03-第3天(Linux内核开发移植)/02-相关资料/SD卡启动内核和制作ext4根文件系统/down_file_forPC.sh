#! /bin/sh
#usemsg：sudo sh down_file_forPC.sh
#note: makesure you sd card node is /dev/sdb
######################################################################
#block          	data                 	size
#  0             	edu-boot                1M
#  1				Kernel					5M
#  2   				RootFs					128M
#  3				UserData				(SD_size - 134)M
######################################################################
#通过虚拟机下载镜像到SD卡中
dd iflag=dsync oflag=dsync if=edu-boot.bin of=/dev/sdb seek=1
dd iflag=dsync oflag=dsync if=zImage of=/dev/sdb seek=2048
dd iflag=dsync oflag=dsync if=rootfs.ext4 of=/dev/sdb seek=12288
