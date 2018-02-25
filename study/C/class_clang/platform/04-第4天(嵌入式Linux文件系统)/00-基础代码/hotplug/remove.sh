MOUNTS=$(mount | grep $1 | cut -d' ' -f3)
umount $MOUNTS
if [ $? -ne 0 ]; then
	cd ..
	umount $MOUNTS
fi

rm -rf $MOUNTS
