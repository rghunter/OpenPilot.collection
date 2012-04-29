DESCRIPTION = "default message of the day"
SECTION = "base"
PRIORITY = "required"
PR = "r1"
LICENSE = "GPL"

SRC_URI = " \
  file://motd \   
  "

do_install () {
	install -d ${D}${sysconfdir}
	install -m 0644 ${WORKDIR}/motd ${D}${sysconfdir}/motd
}

PACKAGES = "${PN}"

