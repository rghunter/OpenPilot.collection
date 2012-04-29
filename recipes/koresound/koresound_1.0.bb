DESCRIPTION = "Linux KoreSound"
PRIORITY = "optional"
SECTION = "kernel/modules"
MAINTAINER = "Julien Tharin"
LICENSE = "GPL"
PR = "r0"

RDEPENDS = "kernel-module-sound kernel-module-pxa-audio"


SRC_URI = "file://koresound-1.0.tar.bz2"

S = "${WORKDIR}/koresound-1.0"

inherit module



do_compile () {
unset CFLAGS CPPFLAGS CXXFLAGS LDFLAGS CC LD CPP
oe_runmake \
'KERNEL_SOURCE=${STAGING_KERNEL_DIR}' \
'KDIR=${STAGING_KERNEL_DIR}' \
'KERNEL_DIR=${STAGING_KERNEL_DIR}' \
'KERNEL_VERSION=${KERNEL_VERSION}' \
'CC=${KERNEL_CC}' \
'LD=${KERNEL_LD}' \
all

}

do_install() {
  install -d ${D}${base_libdir}/modules/${KERNEL_VERSION}/kernel/drivers/sound
  install -m 0644 *${KERNEL_OBJECT_SUFFIX} ${D}${base_libdir}/modules/${KERNEL_VERSION}/kernel/drivers/sound
}


