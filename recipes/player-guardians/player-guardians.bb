DESCRIPTION = "Player"
LICENSE = "GPL"
DEPENDS = "libtool"

SRC_URI = "file://player-guardians.tar.bz2"
S = "${WORKDIR}/player-guardians-${PV}"

EXTRA_OECONF = "--disable-alldrivers --enable-chemical"
PARALLEL_MAKE = ""
LEAD_SONAME = "libplayer*"
FILES_${PN} = "${bindir} ${libdir}/*.so.${PV}"

inherit autotools

