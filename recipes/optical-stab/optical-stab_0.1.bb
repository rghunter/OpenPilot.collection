DESCRIPTION="gst plugin to handle optical flow"
HOMEPAGE="http://github.com/rghunter/OpticalQuad"
PRIORITY="optional"
LICENSE="GPL"
PR="r0"

SRC_URI="git://github.com/rghunter/OpticalQuad.git;protocol=git"
SRCREV="master"
S = "${WORKDIR}/git"

inherit autotools
