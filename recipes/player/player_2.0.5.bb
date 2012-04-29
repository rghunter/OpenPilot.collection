DESCRIPTION = "Player"
LICENSE = "GPL"
DEPENDS = ""

SRC_URI = "http://ovh.dl.sourceforge.net/sourceforge/playerstage/player-2.0.5.tar.bz2"
S = "${WORKDIR}/player-${PV}"

EXTRA_OECONF = "--disable-alldrivers"
PARALLEL_MAKE = ""
LEAD_SONAME = "libplayer*"
FILES_${PN} = "${bindir} ${libdir}/*.so.${PV}"

inherit autotools

