include_directories( "${SNDFILE_INCDIR}" )
link_directories( "${SNDFILE_LIBDIR}" )

include_directories( "${POCO_INCDIR}" )
link_directories( "${POCO_LIBDIR}" )

include_directories( "${PNG_INCDIR}" )
link_directories( "${PNG_LIBDIR}" )

include_directories( "${KINECT_INCDIR}" )
link_directories( "${KINECT_LIBDIR}" )

FILE( GLOB src_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp )
add_library( kinect_common ${src_files} )
