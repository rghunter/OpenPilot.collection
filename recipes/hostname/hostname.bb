# change hostname Korebot 2 Gumstix

SRC_URI = "file://hostname"

do_install() {
	install -d ${D}${sysconfdir}
	echo ${WORKDIR}	
	install -m 0644 ${WORKDIR}/hostname ${D}${sysconfdir}/hostname
	
}


