DESCRIPTION = "ktgrab grabs images from a video4linux device and makes them \
available to various plugins for processing or handling."
#SECTION = "console/utils"
LICENSE = "GPL"
DEPENDS = "jpeg"

SRC_URI = "file://libfg-0.3.1-kb1.tar.bz2"
S = "${WORKDIR}/libfg-0.3.1-kb1"

inherit autotools

do_install () {
    install -d ${D}${bindir}/
    install -m 0755 ${S}/ktgrab ${D}${bindir}/
}

FILES_${PN} = "${bindir}/ktgrab"
