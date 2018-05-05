#use: ./make2ext4.sh rootfs rootfs.ext4

ROOT_FS=""
OUT_FILE="rootfs.ext4"

if [ ! -d "$1" ];then
echo "$1 this is not a directory!"
exit 0
fi
ROOT_FS="${1%/}"
echo "input: ${ROOT_FS}"

if [ ! -z "$2" ];then
OUT_FILE="$2"
fi
echo "output: ${OUT_FILE}"

rm -rf ${OUT_FILE}
dd if=/dev/zero of=${OUT_FILE} bs=512 count=262144
mke2fs -F -m 0 ${OUT_FILE} -t ext4
mkdir tmpfs
mount -o loop ${OUT_FILE} tmpfs
cp ${ROOT_FS}/* tmpfs/ -arf
umount tmpfs
rm -rf tmpfs

