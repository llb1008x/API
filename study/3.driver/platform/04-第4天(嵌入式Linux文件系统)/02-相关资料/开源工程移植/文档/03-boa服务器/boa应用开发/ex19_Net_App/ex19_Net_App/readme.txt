1. 重新烧写kernel(文件zImage)，这个kernel中去掉了触摸屏驱动，因为后边要用到ADC。

2. 拷贝GPIO 和 ADC驱动到开发板上(/toot)目录。s3c2410-gpio.o 和 s3c2410-adc.o
3. 拷贝boa可执行程序 和 boa_server目录 到 开发板上(/root目录)
4. 拷贝login.sh到开地板(/root目录)
5. 重启开发板，即可自动运行boa server。

