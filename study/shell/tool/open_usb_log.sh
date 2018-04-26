#!/bin/bash

#adb root

# adb shell mount -t debugfs none /sys/kernel/debug

   adb shell echo 'file ci13xxx_msm.c +p' > "/sys/kernel/debug/dynamic_debug/control"

   adb shell echo 'file ci13xxx_udc.c +p' > "/sys/kernel/debug/dynamic_debug/control"

   adb shell echo 'file android.c +p' > "/sys/kernel/debug/dynamic_debug/control"

   adb shell echo 'file composite.c +p' > "/sys/kernel/debug/dynamic_debug/control"

   adb shell echo 'file gadget.c +p' > "/sys/kernel/debug/dynamic_debug/control"

   adb shell echo 'file phy-msm-usb.c +p' > "/sys/kernel/debug/dynamic_debug/control"

   adb shell echo 'file u_bam.c +p' > "/sys/kernel/debug/dynamic_debug/control"

   adb shell echo 'file u_smd.c +p' > "/sys/kernel/debug/dynamic_debug/control"

   adb shell echo 'file f_serial.c +p' > "/sys/kernel/debug/dynamic_debug/control"

   adb shell echo 'file f_acm.c +p' > "/sys/kernel/debug/dynamic_debug/control"

   adb shell echo 'file f_fs.c +p' > "/sys/kernel/debug/dynamic_debug/control"

   adb shell echo 'file f_rmnet.c +p' > "/sys/kernel/debug/dynamic_debug/control"

   adb shell echo 'file f_mtp.c +p' > "/sys/kernel/debug/dynamic_debug/control"

   adb shell echo 'file f_mass_storage.c +p' > "/sys/kernel/debug/dynamic_debug/control"

   adb shell echo 'file f_rmnet_smd.c +p' > "/sys/kernel/debug/dynamic_debug/control"

   adb shell echo 'file hcd.c +p' > "/sys/kernel/debug/dynamic_debug/control"

   adb shell echo 'file hub.c +p' > "/sys/kernel/debug/dynamic_debug/control"

   adb shell echo 'file echi-hub.c +p' > "/sys/kernel/debug/dynamic_debug/control"

   adb shell echo 'file echi-hcd.c +p' > "/sys/kernel/debug/dynamic_debug/control"

   adb shell echo 'file ehci-msm.c +p' > "/sys/kernel/debug/dynamic_debug/control"

   adb shell echo 'file echi-msm2.c +p' > "/sys/kernel/debug/dynamic_debug/control"

   adb shell echo 'file ehci-msm-hsic.c +p' > "/sys/kernel/debug/dynamic_debug/control"
