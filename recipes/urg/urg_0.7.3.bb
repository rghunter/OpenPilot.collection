DESCRIPTION = "Urg"
LICENSE = "GPL"
DEPENDS = "libsdl1.2-dev"

SRC_URI = "file://urg-0.7.3.zip"
S = "${WORKDIR}/urg-${PV}"

#EXTRA_OECONF = "--disable-alldrivers --enable-chemical"

PARALLEL_MAKE = ""

#LEAD_SONAME = "libplayer*"
FILES_${PN} = "${bindir} ${libdir}/*.so.${PV}"

inherit autotools

