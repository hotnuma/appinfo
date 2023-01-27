TEMPLATE = app
TARGET = appinfo
CONFIG = c99 link_pkgconfig
DEFINES =
INCLUDEPATH =
PKGCONFIG =

PKGCONFIG += tinyc
#PKGCONFIG += gtk+-3.0
#PKGCONFIG = glib-2.0
#PKGCONFIG += dbus-1
#PKGCONFIG += libxml-2.0

SOURCES = \
    0Temp.c \
    application.c \
    main.c \

DISTFILES += \
    install.sh \
    License.txt \
    meson.build \
    Readme.md \

HEADERS += \
    application.h


