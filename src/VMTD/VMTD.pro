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
QT += network
QT += websockets

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
    VMTD/VMTDController.cpp \
    VMTD/VMTDMethod.cpp \
    VMTD/VMTDSettings.cpp \
    VMTD/VMTDSettingsForm.cpp \
    VMTD/adapter/VMTDAdapterForm.cpp \
    VMTD/adapter/host/VMTDHostAdapterForm.cpp \
    VMTD/adapter/host/VMTDHostClient.cpp \
    VMTD/adapter/host/VMTDHostClientForm.cpp \
    VMTD/adapter/host/VMTDHostServer.cpp \
    VMTD/adapter/host/VMTDHostServerForm.cpp \
    VMTD/adapter/nxapi/VMTDNxApiAdapter.cpp \
    VMTD/adapter/nxapi/VMTDNxApiAdapterForm.cpp \
    VMTD/adapter/nxapi/VMTDNxApiServer.cpp \
    VMTD/adapter/nxapi/VMTDNxApiServerForm.cpp \
    VMTD/device/VMTDDeviceHostDialog.cpp \
    VMTD/device/VMTDDeviceManager.cpp \
    VMTD/device/VMTDDeviceManagerForm.cpp \
    VMTD/device/VMTDDeviceSwitchDialog.cpp \
    VMTD/device/VMTDDeviceTableForm.cpp \
    VMTD/engine/VMTDConfigurator.cpp \
    VMTD/engine/VMTDConfiguratorForm.cpp \
    VMTD/engine/VMTDEngine.cpp \
    VMTD/engine/VMTDEngineForm.cpp \
    VMTD/engine/VMTDGroup.cpp \
    VMTD/engine/VMTDInterface.cpp \
    VMTD/engine/VMTDParticipant.cpp \
    VMTD/engine/VMTDRoute.cpp \
    VMTD/protocol/VMTDClient.cpp \
    VMTD/protocol/VMTDClientForm.cpp \
    VMTD/protocol/VMTDProtocolHandler.cpp \
    VMTD/protocol/VMTDProtocolHandlerForm.cpp \
    VMTD/protocol/VMTDServer.cpp \
    VMTD/protocol/VMTDServerForm.cpp \
    VMTD/protocol/host/VMTDHostProtocolHandler.cpp \
    VMTD/protocol/host/VMTDHostProtocolHandlerForm.cpp \
    VMTD/protocol/nxapi/VMTDNxApiProtocolHandler.cpp \
    VMTD/protocol/nxapi/VMTDNxApiProtocolHandlerForm.cpp

HEADERS += \
    MainWindow.h \
    VMTD/VMTDAlias.h \
    VMTD/VMTDBuildInfo.h \
    VMTD/VMTDComboMap.h \
    VMTD/VMTDController.h \
    VMTD/VMTDDeclareShared.h \
    VMTD/VMTDMethod.h \
    VMTD/VMTDSettings.h \
    VMTD/VMTDSettingsForm.h \
    VMTD/VMTDTemplate.h \
    VMTD/VMTDVarname.h \
    VMTD/adapter/VMTDAdapterForm.h \
    VMTD/adapter/host/VMTDHostAdapterForm.h \
    VMTD/adapter/host/VMTDHostClient.h \
    VMTD/adapter/host/VMTDHostClientForm.h \
    VMTD/adapter/host/VMTDHostServer.h \
    VMTD/adapter/host/VMTDHostServerForm.h \
    VMTD/adapter/nxapi/VMTDNxApiAdapter.h \
    VMTD/adapter/nxapi/VMTDNxApiAdapterForm.h \
    VMTD/adapter/nxapi/VMTDNxApiServer.h \
    VMTD/adapter/nxapi/VMTDNxApiServerForm.h \
    VMTD/device/VMTDDeviceHostDialog.h \
    VMTD/device/VMTDDeviceManager.h \
    VMTD/device/VMTDDeviceManagerForm.h \
    VMTD/device/VMTDDeviceSwitchDialog.h \
    VMTD/device/VMTDDeviceTableForm.h \
    VMTD/engine/VMTDConfigurator.h \
    VMTD/engine/VMTDConfiguratorForm.h \
    VMTD/engine/VMTDEngine.h \
    VMTD/engine/VMTDEngineForm.h \
    VMTD/engine/VMTDGroup.h \
    VMTD/engine/VMTDInterface.h \
    VMTD/engine/VMTDParticipant.h \
    VMTD/engine/VMTDRoute.h \
    VMTD/protocol/VMTDClient.h \
    VMTD/protocol/VMTDClientForm.h \
    VMTD/protocol/VMTDProtocolHandler.h \
    VMTD/protocol/VMTDProtocolHandlerForm.h \
    VMTD/protocol/VMTDServer.h \
    VMTD/protocol/VMTDServerForm.h \
    VMTD/protocol/host/VMTDHostProtocolHandler.h \
    VMTD/protocol/host/VMTDHostProtocolHandlerForm.h \
    VMTD/protocol/nxapi/VMTDNxApiProtocolHandler.h \
    VMTD/protocol/nxapi/VMTDNxApiProtocolHandlerForm.h

FORMS += \
    MainWindow.ui \
    VMTD/VMTDSettingsForm.ui \
    VMTD/adapter/VMTDAdapterForm.ui \
    VMTD/adapter/host/VMTDHostAdapterForm.ui \
    VMTD/adapter/host/VMTDHostClientForm.ui \
    VMTD/adapter/host/VMTDHostServerForm.ui \
    VMTD/adapter/nxapi/VMTDNxApiAdapterForm.ui \
    VMTD/adapter/nxapi/VMTDNxApiServerForm.ui \
    VMTD/device/VMTDDeviceHostDialog.ui \
    VMTD/device/VMTDDeviceManagerForm.ui \
    VMTD/device/VMTDDeviceSwitchDialog.ui \
    VMTD/device/VMTDDeviceTableForm.ui \
    VMTD/engine/VMTDConfiguratorForm.ui \
    VMTD/engine/VMTDEngineForm.ui \
    VMTD/protocol/VMTDClientForm.ui \
    VMTD/protocol/VMTDProtocolHandlerForm.ui \
    VMTD/protocol/VMTDServerForm.ui \
    VMTD/protocol/host/VMTDHostProtocolHandlerForm.ui \
    VMTD/protocol/nxapi/VMTDNxApiProtocolHandlerForm.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
