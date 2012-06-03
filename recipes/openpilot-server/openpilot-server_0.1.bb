DESCRIPTION = "OpenPilot SPI datalink server"
AUTHOR = "James Cotton"
LICENSE = "GPLv3"
LIC_FILES_CHKSUM = "file://COPYING;md5=f27defe1e96c2e1ecd4e0c9be8967949"
PR = "r1"
SRC_URI = "git://github.com/peabody124/op_overo.git;protocol=git"
SRCREV = "master"

S = "${WORKDIR}/git"

do_compile () {
  ${CC} ${CFLAGS} -I${S}/server -I${S}/server/uavobjects ${LDFLAGS} -o ${S}/server/spi_server ${S}/server/*.c ${S}/server/uavobjects/*.c
}

do_install () {
  install -d ${D}${bindir}/
  install -m 0755 ${S}/server/spi_server ${D}${bindir}/
}
FILES_${PN} = "${bindir}/spi_server"
