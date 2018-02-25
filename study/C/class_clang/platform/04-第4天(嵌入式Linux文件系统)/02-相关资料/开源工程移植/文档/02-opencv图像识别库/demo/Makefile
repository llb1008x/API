TARGET = recognise
SRC = Recognise.cpp main.cpp

LIBOPENCV := /home/lijian/opencv/install
LIBLCD := /home/lijian/SVNWorkCopy/rNewCar/trunk/S3C2440/liblcd
CFLAGS = -I$(LIBOPENCV)/include/opencv -I$(LIBLCD)
LDFLAGS = -L$(LIBOPENCV)/lib -L$(LIBLCD) -llcd -lm -lcv -lcvaux -lcxcore -lhighgui -lml -lpthread -ljpeg -lpng -lrt -lcxcore -lz

$(TARGET): $(SRC)
	arm-linux-g++ $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)
	arm-linux-strip $(TARGET)

clean:
	rm -rf $(TARGET)
