DESTDIR  = $${BIN_PATH}

linux-g++: QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN/../../lib.$${OS_SUFFIX}/


#
###-----Deploy------------------------------------------------------------------------------
#

win32 {
    CONFIG( release, debug|release ) {
        #message( "__app.pri:  For Win32 APP|Release Deploy" )

        DEPLOY_COMMAND  = windeployqt --no-translations --no-opengl-sw --no-system-d3d-compiler

        DEPLOY_TARGET  = $$shell_quote($$shell_path($${BIN_PATH}/$${TARGET}.exe))

        #warning( $${DEPLOY_COMMAND} $${DEPLOY_TARGET} )

        QMAKE_POST_LINK += $${DEPLOY_COMMAND} $${DEPLOY_TARGET}
    }
}


#
###-----Installs----------------------------------------------------------------------------
#

win32:  QMAKE_POST_LINK += $$escape_expand(\n\t) mingw32-make install
unix:   QMAKE_POST_LINK += $$escape_expand(\n\t) make install
