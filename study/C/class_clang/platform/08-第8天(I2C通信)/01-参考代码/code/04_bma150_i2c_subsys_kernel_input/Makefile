# 若KERNELRELEASE 的内容为 2.6.35.7,则使用内核中的Makefile以及环境直接进行编译驱动模块 
# 若KERNELRELEASE 的内容不是为 2.6.35.7,则下面else部分内容生效 
ifeq ($(KERNELRELEASE),2.6.35.7)

#若make时想使用参数进行编译，则如下:
#使用方式：make t=XXX
TARGET=$(t)
ifneq ($(TARGET),)
obj-m := $(TARGET).o
else
obj-m := driver.o
#若驱动模块只有一个源文件，并且想得到一个ko文件，则如下（打开注释即可）：
#obj-m := XXX.o 

#若驱动模块有多个源文件，并且想得到多个ko文件，则如下（打开注释即可）：
#obj-m := XXX.o YYY.o ...

#若驱动程序有多个源文件，并且只想得到一个ko文件，则如下（打开注释即可）：
#注意：下面两行中的zzz部分内容必须保持一致
#obj-m := ZZZ.o 
#ZZZ-objs := XXX.o YYY.o ... 
endif

else

#若虚拟机的环境变量中没有定义KERNELDIR,则需要如下自行指定内核路径
ifeq ($(KERNELDIR),)
KERNELDIR = /home/sunplusapp-edu/cache/unsp210_linux_2.6.35
endif	

$(PWD) := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules	
	rm -rf *.o *.mod.c *.order *.symvers

clean:
	rm -rf *.ko 
	
#驱动应用测试使用（仅供参考）
app:
	arm-linux-gcc -o t app.c
	
endif 
