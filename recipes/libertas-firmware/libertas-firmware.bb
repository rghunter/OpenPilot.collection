DESCRIPTION = "Add firmware for libertas module"
#SECTION = "console/utils"


SRC_URI = "file://libertas_cs.fw \
       file://libertas_cs_helper.fw"


FILES_${PN} = "${base_libdir}/firmware/*.fw"

do_install() {
	# add libertas firmware
	install -d  ${D}${base_libdir}/firmware
	install -m 0644 ${WORKDIR}/libertas_cs.fw	 ${D}${base_libdir}/firmware
	install -m 0644 ${WORKDIR}/libertas_cs_helper.fw	 ${D}${base_libdir}/firmware
}


# Important: allows libkorebot to be used by other packages
#do_stage () {
#	mkdir -p ${STAGING_INCDIR}/korebot
#	install -m 0644 ${S}/build-korebot-2.6/include/korebot/* ${STAGING_INCDIR}/korebot
#	oe_libinstall -C ${S}/build-korebot-2.6/lib/ -a -so libkorebot ${STAGING_LIBDIR}
#}
