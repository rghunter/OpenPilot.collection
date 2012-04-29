DESCRIPTION = "Player"
LICENSE = "GPL"
DEPENDS = "libtool"

#PV = "0.1"
PR = "r0"



SRC_URI = "http://ovh.dl.sourceforge.net/sourceforge/playerstage/player-2.1.1.tar.bz2\
#SRC_URI = "file://player-2.1.1.tar.bz2\
 file://chemical.patch;patch=1"

S = "${WORKDIR}/player-2.1.1"

EXTRA_OECONF = "--disable-alldrivers  --enable-chemical  --enable-urglaser"
PARALLEL_MAKE = ""
LEAD_SONAME = "libplayer*"
FILES_${PN} = "${bindir} ${libdir}/*.so.${PV}"

inherit autotools

