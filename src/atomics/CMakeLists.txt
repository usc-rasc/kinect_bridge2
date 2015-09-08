include_directories( "${POCO_INCDIR}" )
link_directories( "${POCO_LIBDIR}" )

FILE( GLOB src_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp )
add_library( atomics ${src_files} )
