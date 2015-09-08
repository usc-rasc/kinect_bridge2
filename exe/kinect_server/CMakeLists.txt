# includes for this project
include_directories( ${CMAKE_CURRENT_SOURCE_DIRECTORY} )

include_directories( "${SNDFILE_INCDIR}" )
link_directories( "${SNDFILE_LIBDIR}" )

include_directories( "${POCO_INCDIR}" )
link_directories( "${POCO_LIBDIR}" )

include_directories( "${PNG_INCDIR}" )
link_directories( "${PNG_LIBDIR}" )

include_directories( "${KINECT_INCDIR}" )
link_directories( "${KINECT_LIBDIR}" )

add_executable( test_pack_as test_pack_as.cpp )
target_link_libraries( test_pack_as ${POCO_LIBS} ${PNG_LIBS} ${SNDFILE_LIBS} ${KINECT_LIBS} kinect_common messages atomics )

add_executable( test_kinect_device_RGB test_kinect_device_RGB.cpp )
target_link_libraries( test_kinect_device_RGB ${POCO_LIBS} ${PNG_LIBS} ${SNDFILE_LIBS} ${KINECT_LIBS} kinect_common messages atomics )

add_executable( test_kinect_device_depth test_kinect_device_depth.cpp )
target_link_libraries( test_kinect_device_depth ${POCO_LIBS} ${PNG_LIBS} ${SNDFILE_LIBS} ${KINECT_LIBS} kinect_common messages atomics )

add_executable( test_kinect_device_infrared test_kinect_device_infrared.cpp )
target_link_libraries( test_kinect_device_infrared ${POCO_LIBS} ${PNG_LIBS} ${SNDFILE_LIBS} ${KINECT_LIBS} kinect_common messages atomics )

add_executable( test_kinect_device_audio test_kinect_device_audio.cpp )
target_link_libraries( test_kinect_device_audio ${POCO_LIBS} ${PNG_LIBS} ${SNDFILE_LIBS} ${KINECT_LIBS} kinect_common messages atomics )

add_executable( kinect_logger kinect_logger.cpp )
target_link_libraries( kinect_logger ${POCO_LIBS} ${PNG_LIBS} ${SNDFILE_LIBS} ${KINECT_LIBS} kinect_common messages atomics )

#add_executable( test_kinect_device_RGBA test_kinect_device_RGBA.cpp )
#target_link_libraries( test_kinect_device_RGBA ${POCO_LIBS} ${PNG_LIBS} ${SNDFILE_LIBS} ${KINECT_LIBS} kinect_common messages atomics )
#
#add_executable( test_kinect_device_png test_kinect_device_png.cpp )
#target_link_libraries( test_kinect_device_png ${POCO_LIBS} ${PNG_LIBS} ${SNDFILE_LIBS} ${KINECT_LIBS} kinect_common messages atomics )

add_executable( test_kinect_device_gzip test_kinect_device_gzip.cpp )
target_link_libraries( test_kinect_device_gzip ${POCO_LIBS} ${PNG_LIBS} ${SNDFILE_LIBS} ${KINECT_LIBS} kinect_common messages atomics )

#add_executable( kinect_server kinect_server.cpp kinect_server.rc )
#
## set subsystem to windows since we have a GUI
#set_target_properties( kinect_server PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS" ) 
#
## need unicode for LPCSTR, etc
#ADD_DEFINITIONS(-DUNICODE)
#ADD_DEFINITIONS(-D_UNICODE)
#
## link against all of our libs
#target_link_libraries( kinect_server ${POCO_LIBS} ${PNG_LIBS} ${SNDFILE_LIBS} ${KINECT_LIBS} kinect_common messages atomics )

set( executables
		test_pack_as
		test_kinect_device_RGB
		test_kinect_device_depth
		test_kinect_device_infrared
		test_kinect_device_audio
		kinect_logger
#		test_kinect_device_RGBA
#		test_kinect_device_png
		test_kinect_device_gzip
)

# copy all DLLs alongside our exe
foreach( executable ${executables} )
	file( GLOB png_dlls ${PNG_DLLDIR}/*.dll )
	foreach( dll ${png_dlls} )
		add_custom_command( TARGET ${executable} POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy_if_different
				${dll}
				$<TARGET_FILE_DIR:${executable}> )
	endforeach()

	file( GLOB sndfile_dlls ${SNDFILE_DLLDIR}/*.dll )
	foreach( dll ${sndfile_dlls} )
		add_custom_command( TARGET ${executable} POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy_if_different
				${dll}
				$<TARGET_FILE_DIR:${executable}> )
	endforeach()

	file( GLOB poco_dlls ${POCO_DLLDIR}/*.dll )
	foreach( dll ${poco_dlls} )
		add_custom_command( TARGET ${executable} POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy_if_different
				${dll}
				$<TARGET_FILE_DIR:${executable}> )
	endforeach()
endforeach()
