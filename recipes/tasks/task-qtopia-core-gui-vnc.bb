HOMEPAGE = "http://www.trolltech.com"
LICENSE = "MIT"

inherit task

DESCRIPTION_task-qtopia-core-gui-vnc = "Qtopia Core GUI libraries with LinuxFB & VNC graphics drivers"
RDEPENDS_task-qtopia-core-gui-vnc = "\
    task-qtopia-core-gui \
    qtopiacore-plugins-gfx-drivers"
