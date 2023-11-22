obj-m += test_mod00.o

KDIR = /usr/src/linux-headers-6.2.0-36-generic

all:
	$(MAKE) -C $(KDIR) M=$(shell pwd) modules

clean:
	$(MAKE) -C $(KDIR) M=$(shell pwd) clean
	rm -f *.o