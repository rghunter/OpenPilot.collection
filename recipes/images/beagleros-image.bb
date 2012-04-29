ROS_DEPS = "\
  python \
  log4cxx \
  apr \
  apr-util \
  boost"

WIRELESS_DEPS= "\
  kernel-module-rt2870sta \
  dhcp-server \
  wireless-tools"

APPLICATIONS_DEPS ="\
  vim \
  "

#Angstrom minimalist image
#gives you a small image with ssh access

ANGSTROM_EXTRA_INSTALL ?= ""
DISTRO_SSH_DAEMON ?= "dropbear"

IMAGE_PREPROCESS_COMMAND = "create_etc_timestamp"

IMAGE_INSTALL = "task-boot \
            util-linux-ng-mount util-linux-ng-umount \
            ${ROS_DEPS} \
            ${WIRELESS_DEPS} \
            ${APPLICATIONS_DEPS} \
            ${DISTRO_SSH_DAEMON} \
            ${ANGSTROM_EXTRA_INSTALL} \
            angstrom-version \
            ${SPLASH} \
     "

export IMAGE_BASENAME = "beagleros-image"
IMAGE_LINGUAS = ""

inherit image
