# gspcav bitbake recipe
# Julien Tharin 2010
DESCRIPTION = "Gspcav1"
SECTION = "kernel/modules"
LICENSE = "GPL"

SRC_URI = "file://gspcav1-20071224.kb3.tar.bz2"

S = "${WORKDIR}/gspcav1-20071224.kb3"

RRECOMMENDS = "kernel-module-videodev kernel-module-v4l1-compat kernel-module-v4l2-common"

inherit module


do_compile() {
        unset CFLAGS CPPFLAGS CXXFLAGS LDFLAGS
        make -e 'KERNELDIR=${STAGING_KERNEL_DIR}' \
                 #  'CC=${KERNEL_CC}' \
                 #  'LD=${KERNEL_LD}'	
}

do_install() {
	mkdir -p ${D}/lib/modules/${KERNEL_VERSION}/kernel/drivers/usb/media/
	install -m 0644 ${S}/gspca.ko ${D}/lib/modules/${KERNEL_VERSION}/kernel/drivers/usb/media/
   
}


