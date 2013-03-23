CONFIG      += designer plugin debug_and_release
TARGET      = $$qtLibraryTarget(openchartplugin)
TEMPLATE    = lib

HEADERS     = openchartplugin.h
SOURCES     = openchartplugin.cpp
RESOURCES   = icons.qrc
LIBS        += -L. 

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS    += target
CONFIG+=static
include(openchart.pri)
