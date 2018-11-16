set (PLUGINS boxfilter medianfilter)
foreach (LIBNAME ${PLUGINS})
    add_library (${LIBNAME} SHARED "${CMAKE_CURRENT_SOURCE_DIR}/${LIBNAME}/${LIBNAME}")
endforeach (LIBNAME)
