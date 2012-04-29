KERNEL_VERSION	:= `uname -r`
KERNEL_DIR	:= /lib/modules/$(KERNEL_VERSION)/build
INSTALL_MOD_DIR	:= sound

PWD		:= $(shell pwd)

#obj-m		:= pxa-uda1342-i2c.o

obj-m		:= pxa-audio.o

#pxa-uda1342-i2c-obj		:= pxa-uda1342-i2c.o pxa-audio.o uda1342.o


all:
	@echo "Building KoreSound drivers..."
	@(make -C $(KERNEL_DIR) M=$(PWD) CROSS_COMPILE=$(CROSS_COMPILE) modules)


install:
	@echo "Installing KoreSound drivers..."
	@(make -C $(KERNEL_DIR) M=$(PWD) INSTALL_MOD_DIR=$(INSTALL_MOD_DIR) INSTALL_MOD_PATH=$(INSTALL_MOD_PATH) modules_install)

clean:
	-rm -f *.o *.ko .*.cmd .*.flags *.mod.c Module.symvers modules.order
	-rm -rf .tmp_versions

