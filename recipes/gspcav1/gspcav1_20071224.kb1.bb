# gspcav bitbake recipe
# Julien Tharin 2009
DESCRIPTION = "Gspcav1"
SECTION = "kernel/modules"
LICENSE = "GPL"

SRC_URI = "http://mxhaard.free.fr/spca50x/Download/gspcav1-20071224.tar.gz \
	file://gspcav1-20071224-kb1.patch;patch=1 \
	file://Makefile.patch;patch=1 \
	file://quickcamE2500.diff;patch=1"

S = "${WORKDIR}/gspcav1-20071224"

RRECOMMENDS = "kernel-module-videodev kernel-module-v4l1-compat kernel-module-v4l2-common"

inherit module


do_compile() {
        unset CFLAGS CPPFLAGS CXXFLAGS LDFLAGS
        make -e 'KERNELDIR=${STAGING_KERNEL_DIR}' \
                 #  'CC=${KERNEL_CC}' \
                  # 'LD=${KERNEL_LD}'	
}

do_install() {
	mkdir -p ${D}/lib/modules/${KERNEL_VERSION}/kernel/drivers/usb/media/
	install -m 0644 ${S}/gspca.ko ${D}/lib/modules/${KERNEL_VERSION}/kernel/drivers/usb/media/
   
}


