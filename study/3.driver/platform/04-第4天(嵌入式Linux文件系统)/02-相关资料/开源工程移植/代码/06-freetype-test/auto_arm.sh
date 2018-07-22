export   C_INCLUDE_PATH=/home/xiuhai/work/platform/crosstool/fs/opensource/freetype/opt_arm/include/:/home/xiuhai/work/platform/crosstool/fs/opensource/freetype/opt_arm/include/freetype2/
arm-linux-gcc -L/home/xiuhai/work/platform/crosstool/fs/opensource/freetype/opt_arm/lib/ -lfreetype -o tarm freetype_test.c nls_cp936.c
