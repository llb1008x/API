#!/bin/sh
insmod /usr/usb_gadget/g_file_storage.ko file=/dev/mtdblock6 stall=0 removable=1
