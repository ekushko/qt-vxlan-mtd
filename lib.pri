DESTDIR  = $${LIBS_PATH}/$${NAME}

win32:  DLLDESTDIR  = $${BIN_PATH}

#
###-----Version---------------------------------------------------------------------------
#

win32: CONFIG += skip_target_version_ext
unix:  CONFIG += unversioned_libname unversioned_soname

#
###-----Installs----------------------------------------------------------------------------
#

# выполняем install после связывания
win32:  QMAKE_POST_LINK += $$escape_expand(\n\t) mingw32-make install
unix:   QMAKE_POST_LINK += $$escape_expand(\n\t) make install
