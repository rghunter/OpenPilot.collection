# Libkorebot bitbake recipe
# Nicolas Boichat 2008
DESCRIPTION = "Libkorebot"
SECTION = "libs"
LICENSE = "GPL"

SRC_URI = "http://ftp.k-team.com/korebot/libkorebot/libkorebot-1.11.zip \
	file://libkorebot-1.11-kb1.patch;patch=1"
S = "${WORKDIR}"

FILES_${PN} = "${libdir}/*.so /etc/libkorebot"

# removed; already inside
#FILES_${PN}-dev =+ "${includedir}/korebot"

do_compile() {
	make clean
	TARGET_SYSTEM=korebot-2.6 make -e
	# Using a symlink for the library seems to confuse oe_libinstall
	rm ${S}/build-korebot-2.6/lib/libkorebot.so
	mv ${S}/build-korebot-2.6/lib/libkorebot-${PV}.so ${S}/build-korebot-2.6/lib/libkorebot.so
}

do_install() {
        mkdir -p ${D}${libdir}
        mkdir -p ${D}${includedir}
        install -m 0755 ${S}/build-korebot-2.6/lib/libkorebot.so ${D}${libdir}
        install -m 0755 ${S}/build-korebot-2.6/lib/libkorebot.a ${D}${libdir}
        mkdir -p ${D}${includedir}/korebot
	install -m 0644 ${S}/build-korebot-2.6/include/korebot/* ${D}${includedir}/korebot
	mkdir -p ${D}/etc/libkorebot
	install -m 0644 ${S}/config/*.knc ${D}/etc/libkorebot
}

# Important: allows libkorebot to be used by other packages
do_stage () {
	mkdir -p ${STAGING_INCDIR}/korebot
	install -m 0644 ${S}/build-korebot-2.6/include/korebot/* ${STAGING_INCDIR}/korebot
	oe_libinstall -C ${S}/build-korebot-2.6/lib/ -a -so libkorebot ${STAGING_LIBDIR}
}
