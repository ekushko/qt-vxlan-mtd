------------------------------------------------
#
# Project created by Kushko Evgenii Aleksandrovich
#
#-------------------------------------------------

NAME = VMTD

TEMPLATE  = app
#TEMPLATE  = lib

CONFIG += debug_and_release
CONFIG += build_all

VERSION  = 0.0.0.1

QMAKE_TARGET_PRODUCT      = Moving Target Defense by VXLAN
QMAKE_TARGET_DESCRIPTION  = Moving Target Defense by VXLAN
QMAKE_TARGET_COMPANY      = Kushko E.A.
QMAKE_TARGET_COPYRIGHT    = (C) Kushko E.A.
#RC_ICONS                  = resources/favicon.ico

#
###-----Директивы---------------------------------------------------------------------------
#

DEFINES += VMTD_LIBRARY

DEFINES += VERSION=\\\"$$VERSION\\\"
DEFINES += TARGET=\\\"$$TARGET\\\"

#
###-----Dependecies-------------------------------------------------------------------------
#

QT += core gui
QT += widgets
QT += sql
#QT += network
#QT += websockets

#
###-----common.pri--------------------------------------------------------------------------
#

include( ../../common.pri )

##
####-----Libraries----------------------------------------------------------------------------
##
#

#openssl.path  = $${BIN_PATH}
#
#contains(QT_ARCH, i386) {
#    win32 {
#        INCLUDEPATH += c:/Qt/Tools/OpenSSL/Win_x86/include
#        LIBS        += -Lc:/Qt/Tools/OpenSSL/Win_x86/bin -llibcrypto-1_1
#
#        openssl.files += c:/Qt/Tools/OpenSSL/Win_x86/bin/libcrypto-1_1.dll
#    }
#} else {
#    win32 {
#        INCLUDEPATH += c:/Qt/Tools/OpenSSL/Win_x64/include
#        LIBS        += -Lc:/Qt/Tools/OpenSSL/Win_x64/bin -llibcrypto-1_1-x64
#
#        openssl.files += c:/Qt/Tools/OpenSSL/Win_x64/bin/libcrypto-1_1-x64.dll
#    }
#}
#
#unix:  LIBS += -lcrypto
#
#win32: LIBS += -lDbgHelp

EXTERNAL_LIBNAMELIST  =

for( LIBNAME, EXTERNAL_LIBNAMELIST ) {

    equals( TEMPLATE, "app" ) {
        extdll.path  = $${BIN_PATH}
        win32: extdll.files += $${EXTERNAL_LIBS_PATH}/$${LIBNAME}/$${LIBNAME}$${LIB_SUFFIX}.dll
        unix:  extdll.files += $${EXTERNAL_LIBS_PATH}/$${LIBNAME}/lib$${LIBNAME}$${LIB_SUFFIX}.so
    }

    INCLUDEPATH += $${EXTERNAL_IMPORT_PATH}/$${LIBNAME}

    win32: LIBS += -L$${EXTERNAL_LIBS_PATH}/$${LIBNAME} -l$${LIBNAME}$${LIB_SUFFIX}
    unix:  LIBS += -L$${EXTERNAL_LIBS_PATH}/$${LIBNAME} -L. -l$${LIBNAME}$${LIB_SUFFIX} -Wl,-rpath,lib -Wl,-rpath,.
}

#
###-----Installs----------------------------------------------------------------------------
#

CONFIG( release, debug|release ) {
#    INSTALLS += openssl
#    INSTALLS += dll
#    INSTALLS += extdll
}

#set.path   = $${BIN_PATH}
#set.files  = resources/settings
#INSTALLS  += set

#equals( TEMPLATE, "app" ) {
#    data.path   = $${BIN_PATH}
#    data.files  = files/*
#    INSTALLS   += data
#}

equals( TEMPLATE, "lib" ) {

    message( "__"$${NAME}.pro":  For LIB copy *.h files to ./import directory" )

    incl.path   = $${IMPORT_PATH}/$${NAME}
    incl.files += $$NAME/*.h
    INSTALLS   += incl

    FOLDERSLIST  = \
        device

    for( FOLDER, FOLDERSLIST ) {
        $${FOLDER}.path   = $${IMPORT_PATH}/$${NAME}/$${FOLDER}
        $${FOLDER}.files  = $$NAME/$${FOLDER}/*.h
        INSTALLS         += $${FOLDER}
    }
}

#
###-----app\lib.pri---------------------------------------------------------------------------
#

equals( TEMPLATE, "app" ) {
    include( ../../app.pri )
} else {
    include( ../../lib.pri )
}

#
###-----Files--------------------------------------------------------------------------------
#

CONFIG += c++11

SOURCES += \
    main.cpp \
    MainWindow.cpp \
    VMTD/VMTDBuildInfo.cpp \
    VMTD/VMTDSettings.cpp \
    VMTD/VMTDSettingsForm.cpp \
    VMTD/device/VMTDDeviceHostDialog.cpp \
    VMTD/device/VMTDDeviceManager.cpp \
    VMTD/device/VMTDDeviceManagerForm.cpp \
    VMTD/device/VMTDDeviceSwitchDialog.cpp \
    VMTD/device/VMTDDeviceTableForm.cpp

HEADERS += \
    MainWindow.h \
    VMTD/VMTDBuildInfo.h \
    VMTD/VMTDComboMap.h \
    VMTD/VMTDDeclareShared.h \
    VMTD/VMTDSettings.h \
    VMTD/VMTDSettingsForm.h \
    VMTD/VMTDTemplate.h \
    VMTD/VMTDVarname.h \
    VMTD/device/VMTDDeviceHostDialog.h \
    VMTD/device/VMTDDeviceManager.h \
    VMTD/device/VMTDDeviceManagerForm.h \
    VMTD/device/VMTDDeviceSwitchDialog.h \
    VMTD/device/VMTDDeviceTableForm.h

FORMS += \
    MainWindow.ui \
    VMTD/VMTDSettingsForm.ui \
    VMTD/device/VMTDDeviceHostDialog.ui \
    VMTD/device/VMTDDeviceManagerForm.ui \
    VMTD/device/VMTDDeviceSwitchDialog.ui \
    VMTD/device/VMTDDeviceTableForm.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
