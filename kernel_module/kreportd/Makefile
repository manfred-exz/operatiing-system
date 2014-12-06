TARGET = kreportd
KDIR = /usr/src/linux-headers-3.16.0-3-generic
obj-m := $(TARGET).o
default:
	make -C $(KDIR) M=$(PWD) modules
clean:
	rm -rf *.o *.cmd *.mod.c *.order *.symvers .*.o .*.cmd
