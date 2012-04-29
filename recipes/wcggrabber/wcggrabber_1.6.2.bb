DESCRIPTION = "WebCam Go image grabber (wcggrabber) is an image grabber for \
linux that can use the UYVY palette of the Creative WebCam Go."
LICENSE = "GPL"
DEPENDS = ""

SRC_URI = "http://www.unix-ag.uni-kl.de/~auerswal/wcggrabber-1.6.2.tar.bz2 \
	file://wcggrabber-1.6.2-arm.patch;patch=1"
S = "${WORKDIR}/wcggrabber-${PV}"

inherit autotools

do_install() {
	mkdir -p ${D}${bindir}
        install -m 0755 ${S}/wcggrabber ${D}${bindir}
}
