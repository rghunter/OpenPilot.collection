#package description
DESCRIPTION = "Adaptation to the new i2c port name: from /dev/i2c/0 to /dev/i2c-0"
SECTION = "i2c_update"

#this is optional
LICENSE = "GPLv2"

PN = "i2c-udevrule"
PV = "0.1"

SRC_URI = "file://i2c.rules"

S = "${WORKDIR}"

FILES_${PN} = "/etc/udev/rules.d"

do_install() {
	mkdir -p ${D}/etc/udev/rules.d/
	install -m 0644 ${WORKDIR}/i2c.rules ${D}/etc/udev/rules.d/
}
