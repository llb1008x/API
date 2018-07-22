unexport O_TARGET
unexport obj-y
unexport subdirs
unexport SUBDIRS

ifdef O_TARGET
$(O_TARGET): $(obj-y)
	rm -f $@
	$(LD) -r -o $@ $^
endif # O_TARGET

%.o:%.c
	$(CC) -Wall -c -O2 -o $@ $< $(DIR_INCLUDE) -fno-builtin-printf

%.o:%.S
	$(CC) -Wall -c -O2 -o $@ $< $(DIR_INCLUDE) -fno-builtin-printf
	
#子目录makefile的递归调用:当有新的目录时要在这里加上
SUBDIRS=init drivers/uart
.PHONY:subdirs $(SUBDIRS)
subdirs:$(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@		