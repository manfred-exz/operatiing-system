TARGET = helloworld
KDIR = /usr/src/linux-headers-3.16.0-3-generic
obj-m := $(TARGET).o
default:
	make -C $(KDIR) M=$(PWD) modules
