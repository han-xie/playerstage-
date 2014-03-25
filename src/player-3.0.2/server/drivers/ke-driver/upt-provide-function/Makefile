CXX=arm-linux-g++
LD=arm-linux-g++
CFLAGS= -Wall -D_DEBUG -D__LINUX__ -DRS485  -c
LDFLAGS = -L ./Lib -lpthread -lmsr -lmad
OBJS=  main.o serial_linux.o speechrecogn.o voice.o capture.o drv-v4l.o grab-ng.o struct-v4l.o struct-dump.o command.o TCPLin.o background.o mp3.o sound.o rs422.o UDPBasic.o
all:MultiFlexNG
MultiFlexNG:$(OBJS) 
	$(LD) $(LDFLAGS) -o MultiFlexNG $(OBJS)
main.o: main.cpp type.h debuglevel.h background.h
	$(CXX) $(CFLAGS) -o main.o main.cpp