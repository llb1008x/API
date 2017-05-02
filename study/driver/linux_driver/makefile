ifneq ($(KERNELRELEASE),)  
	obj-m := cdevdemo.o  
else  
	KERNELDIR = /lib/modules/$(shell uname -r)/build  
	PWD := $(shell pwd)
  
default:  
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules  
	
endif  
  
clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions modules.order  Module.symvers 
	
	

