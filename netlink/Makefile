ifneq ($(KERNELRELEASE),)
	obj-m :=netl.o
else
	KERNELDIR ?=/lib/modules/$(shell uname -r)/build
	PWD :=$(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	gcc client.c -o client
endif

clean:
	-rm *.o *.ko
