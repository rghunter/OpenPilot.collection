DESCRIPTION = "camsource grabs images from a video4linux device and makes them \
available to various plugins for processing or handling."
#SECTION = "console/utils"
LICENSE = "GPL"
DEPENDS = "jpeg libxml2"

SRC_URI = "http://ftp.udc.es/gentoo/distfiles/camsource-0.7.1.tar.gz file://camsource.conf"
S = "${WORKDIR}/camsource-${PV}"

inherit autotools

do_install_append() {
	mkdir -p ${D}${sysconfdir}
	install -m 0644 ${WORKDIR}/camsource.conf ${D}${sysconfdir}
}
