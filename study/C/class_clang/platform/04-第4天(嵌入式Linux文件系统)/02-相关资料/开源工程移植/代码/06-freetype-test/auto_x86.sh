export   C_INCLUDE_PATH=/home/xiuhai/work/platform/crosstool/fs/opensource/freetype/opt_x86/include/:/home/xiuhai/work/platform/crosstool/fs/opensource/freetype/opt_x86/include/freetype2/
gcc -L/home/xiuhai/work/platform/crosstool/fs/opensource/freetype/opt_x86/lib/ -lfreetype -o tx86 freetype_test.c nls_cp936.c
