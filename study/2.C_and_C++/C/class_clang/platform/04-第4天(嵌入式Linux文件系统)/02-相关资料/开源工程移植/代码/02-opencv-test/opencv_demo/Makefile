obj = opencv_test
CC = arm-linux-gcc
src = openCV_test.c
lib = -lcv -lcvaux -lcxcore -lhighgui -ljpeg -lml -lpng12 -lpng -lz -L/home/edu/share/opensource/install/lib/
$(obj): $(src)
	$(CC) -o $(obj) $(src) $(lib)
clean : 
	rm *.o $(obj)
