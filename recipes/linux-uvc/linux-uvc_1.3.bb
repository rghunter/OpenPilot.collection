DESCRIPTION = "Linux UVC driver"
PRIORITY = "optional"
SECTION = "kernel/modules"
MAINTAINER = "Stephane Gauthier <gauthier.stephane-Re5JQEeQqe8AvxtiuMwx3w@xxxxxxxxxxxxxxxx>"
LICENSE = "GPL"
PR = "r0"

RDEPENDS = "kernel-module-videodev kernel-module-v4l2-common \
kernel-module-v4l1-compat kernel-module-compat-ioctl32"


SRC_URI = "file://uvcvideo-r205.tar.bz2"

S = "${WORKDIR}/uvcvideo-r205"

inherit module

#'MODPATH={D}${base_libdir}/modules/${KERNEL_VERSION}/kernel/drivers/net'

do_compile () {
unset CFLAGS CPPFLAGS CXXFLAGS LDFLAGS CC LD CPP
oe_runmake \
'KERNEL_SOURCE=${STAGING_KERNEL_DIR}' \
'KDIR=${STAGING_KERNEL_DIR}' \
'KERNEL_DIR=${STAGING_KERNEL_DIR}' \
'KERNEL_VERSION=${KERNEL_VERSION}' \
'CC=${KERNEL_CC}' \
'LD=${KERNEL_LD}' \
uvcvideo 

}

do_install() {
  install -d ${D}${base_libdir}/modules/${KERNEL_VERSION}/kernel/drivers/usb/media
  install -m 0644 *${KERNEL_OBJECT_SUFFIX} ${D}${base_libdir}/modules/${KERNEL_VERSION}/kernel/drivers/usb/media
}


