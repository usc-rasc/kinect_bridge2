include_directories( "${SNDFILE_INCDIR}" )
link_directories( "${SNDFILE_LIBDIR}" )

include_directories( "${POCO_INCDIR}" )
link_directories( "${POCO_LIBDIR}" )

include_directories( "${PNG_INCDIR}" )
link_directories( "${PNG_LIBDIR}" )

add_definitions( "-std=c++11" )
FILE( GLOB src_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp )
add_library( messages SHARED ${src_files} )
