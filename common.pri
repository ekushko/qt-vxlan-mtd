#
###-----Main---------------------------------------------------------------------------------
#

#SPECIFIED_COPY_UTILITY  = XCOPY

PROJECT_ROOT_PATH  = $${PWD}

unix:  EXTERNAL_ROOT_PATH = /mnt/libs

# Сетевой путь
#win32: EXTERNAL_ROOT_PATH = L:
# Локальный путь
win32: EXTERNAL_ROOT_PATH = C:\Libs


win32:      OS_SUFFIX  = win32
linux-g++:  OS_SUFFIX  = linux

#
###-----Директивы общие----------------------------------------------------------------------
#

DEFINES += QT_MESSAGELOGCONTEXT


#
###-----Flags--------------------------------------------------------------------------------
#

QMAKE_CXXFLAGS_WARN_ON += -Wextra \
#                          -Wno-class-memaccess \
#                          -Wno-int-in-bool-context \
#                          -Wno-delete-non-virtual-dtor \
#                          -Wno-gnu-anonymous-struct \
#                          -Wno-ignored-qualifiers \
#                          -Wno-parentheses \
#                          -Wno-reorder
#                          -Wno-switch \
#                          -Wno-type-limits
#                          -Wno-unused-variable \


#
####-----Config------------------------------------------------------------------------------
#

CONFIG( debug, debug|release ) {
    BUILD_FLAG  = debug
    LIB_SUFFIX  = d
} else {
    BUILD_FLAG  = release
}

equals( TEMPLATE, "lib" ) {
    TARGET  = $${NAME}$${LIB_SUFFIX}
} else {
    TARGET  = $${NAME}
}

CONFIG += c++11
linux-g++:  QMAKE_CXXFLAGS += -std=c++11


#
###-----XCopy--------------------------------------------------------------------------------
#

win32: equals( SPECIFIED_COPY_UTILITY, "XCOPY" ) {

    message( "__common.pri:  Win32 SPECIFIED COPY UTILITY \"XCOPY\"" )

    QMAKE_COPY             = "xcopy /s /e /h /y"
    QMAKE_COPY_DIR         = "xcopy /s /e /h /y /i"
    QMAKE_COPY_FILE        = "xcopy /s /e /h /y"
    QMAKE_INSTALL_DIR      = "xcopy /s /e /h /y /i"
    QMAKE_INSTALL_FILE     = "xcopy /s /e /h /y"
    QMAKE_INSTALL_PROGRAM  = "xcopy /s /e /h /y"
}


#
###-----Directories--------------------------------------------------------------------------
#

CONFIG( debug, debug|release ) {
    BIN_PATH = $${PROJECT_ROOT_PATH}/bin/$${BUILD_FLAG}
} else {
    isEmpty( STATION_NAME ) {
        BIN_PATH = $${PROJECT_ROOT_PATH}/bin/$${NAME}
    } else {
        BIN_PATH = $${PROJECT_ROOT_PATH}/bin/$${STATION_NAME}
    }
}

BUILD_PATH   = $${PROJECT_ROOT_PATH}/build/$${BUILD_FLAG}/$${NAME}

isEmpty( ROOT_NAME ) {
#    message( "ROOT_NAME is EMPTY" )
    IMPORT_PATH = $${PROJECT_ROOT_PATH}/import
} else {
#    message( "ROOT_NAME is" $${ROOT_NAME} )
    IMPORT_PATH = $${PROJECT_ROOT_PATH}/import/$${ROOT_NAME}
}

INCLUDE_PATH = $${PROJECT_ROOT_PATH}/import
EXPORT_PATH  = $${PROJECT_ROOT_PATH}/export
LIBS_PATH    = $${PROJECT_ROOT_PATH}/lib.$${OS_SUFFIX}

EXTERNAL_DATA_PATH   = $${EXTERNAL_ROOT_PATH}/data
EXTERNAL_IMPORT_PATH = $${EXTERNAL_ROOT_PATH}/import
EXTERNAL_LIBS_PATH   = $${EXTERNAL_ROOT_PATH}/lib.$${OS_SUFFIX}

OBJECTS_DIR  = $${BUILD_PATH}/obj
MOC_DIR      = $${BUILD_PATH}/moc
RCC_DIR      = $${BUILD_PATH}/rcc
UI_DIR       = $${BUILD_PATH}/ui

LIBS += -L$${LIBS_PATH}

INCLUDEPATH += $${EXPORT_PATH}
INCLUDEPATH += $${IMPORT_PATH}

#message( "BIN_PATH:   "  $${BIN_PATH}    )
#message( "BUILD_PATH: "  $${BUILD_PATH}  )
#message( "IMPORT_PATH:"  $${IMPORT_PATH} )
#message( "EXPORT_PATH:"  $${EXPORT_PATH} )
#message( "LIBS_PATH:  "  $${LIBS_PATH}   )
#message()
#message( "OBJECTS_DIR:"  $${OBJECTS_DIR} )
#message( "MOC_DIR:    "  $${MOC_DIR}     )
#message( "RCC_DIR:    "  $${RCC_DIR}     )
#message( "UI_DIR:     "  $${UI_DIR}      )
#message()
#message( "LIBS:       "  $${LIBS}        )
#message()
#message( "INCLUDEPATH:"  $${INCLUDEPATH} )
#message()
#return()


#
###-----Check the Environment----------------------------------------------------------------
#

#message()
#message()
#message( "QMAKE_CHK_DIR_EXISTS: "  $$QMAKE_CHK_DIR_EXISTS  )
#message( "QMAKE_COPY:           "  $$QMAKE_COPY            )
#message( "QMAKE_COPY_DIR:       "  $$QMAKE_COPY_DIR        )
#message( "QMAKE_COPY_FILE:      "  $$QMAKE_COPY_FILE       )
#message( "QMAKE_DEL_DIR:        "  $$QMAKE_DEL_DIR         )
#message( "QMAKE_DEL_FILE:       "  $$QMAKE_DEL_FILE        )
#message( "QMAKE_INSTALL_DIR:    "  $$QMAKE_INSTALL_DIR     )
#message( "QMAKE_INSTALL_FILE:   "  $$QMAKE_INSTALL_FILE    )
#message( "QMAKE_INSTALL_PROGRAM:"  $$QMAKE_INSTALL_PROGRAM )
#message( "QMAKE_LINK:           "  $$QMAKE_LINK            )
#message( "QMAKE_LINK_SHLIB:     "  $$QMAKE_LINK_SHLIB      )
#message( "QMAKE_MKDIR:          "  $$QMAKE_MKDIR           )
#message( "QMAKE_MOVE:           "  $$QMAKE_MOVE            )
#message( "QMAKE_QMAKE:          "  $$QMAKE_QMAKE           )
#message( "QMAKE_SYMBOLIC_LINK:  "  $$QMAKE_SYMBOLIC_LINK   )
#return()

