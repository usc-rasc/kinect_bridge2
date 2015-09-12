include_directories( "${SNDFILE_INCDIR}" )
link_directories( "${SNDFILE_LIBDIR}" )

include_directories( "${POCO_INCDIR}" )
link_directories( "${POCO_LIBDIR}" )

include_directories( "${PNG_INCDIR}" )
link_directories( "${PNG_LIBDIR}" )

FILE( GLOB executables RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp )

foreach( executable ${executables} )
	get_filename_component( executable_name ${executable} NAME_WE )
	add_definitions( "-std=c++11" )
	add_executable( ${executable_name} ${executable} )
	target_link_libraries( ${executable_name} ${SNDFILE_LIBS} ${POCO_LIBS} ${PNG_LIBS} messages atomics )
endforeach()
