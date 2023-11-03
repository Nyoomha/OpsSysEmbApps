obj-m += test_mod.o

KDIR = /usr/src/linux-headers-6.2.0-32-generic

CFLAGS = -D__"KERNEL__"-DMODULE -I$(KDIR)/include -Wall

all:
	$(MAKE) -C $(KDIR) M=$(shell pwed) $(KCONFIG) modules

clean:
	$(MAKE) -C $(KDIR) M=$(shell pwd) clean
	rm -f *.o

	