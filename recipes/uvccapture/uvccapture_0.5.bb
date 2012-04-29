DESCRIPTION = "UVC Image capture"
PRIORITY = "optional"
SECTION = "Programs"

LICENSE = "GPL"
PR = "r1"

RDEPENDS = "kernel-module-videodev kernel-module-v4l2-common \
kernel-module-v4l1-compat kernel-module-compat-ioctl32 linux-uvc"


SRC_URI = "http://staticwave.ca/source/uvccapture/uvccapture-0.5.tar.bz2 \
file://Makefile.patch;patch=1"

S = "${WORKDIR}/uvccapture-${PV}"


do_compile () {

#unset CFLAGS CPPFLAGS CXXFLAGS LDFLAGS CC LD CPP
#export CFLAGS=$(CFLAGS) -DVERSION="$(PV)"
	make


}



do_install() {
	mkdir -p ${D}${bindir}
    install -m 0755 ${S}/uvccapture ${D}${bindir}
}

