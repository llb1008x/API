#指定生成可加载模块的名称  
obj-m:=hellodemo.o  
  
#实际是指向你自己创建的内核代码树的路径，build是个软连接  
KERDIR=/lib/modules/$(shell uname -r)/build  
  
#编译模块的当前路径  
CURDIR=$(shell pwd)  
  
#make后默认自动跳转到这里执行，make功能比较多，有兴趣的可以网上搜下  
all:  
	make -C $(KERDIR) M=$(CURDIR) modules  
  
#这是模块清理工作，一般会清理掉所有生成的文件  
clean:  
	make -C $(KERDIR) M=$(CURDIR) clean 
