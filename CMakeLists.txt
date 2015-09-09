cmake_minimum_required(VERSION 2.4.6)

project( kinect_bridge2_server )

#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

include_directories( include )
link_directories( lib )

set( DEPS_DIR "${PROJECT_SOURCE_DIR}/deps" )

if( "${CMAKE_GENERATOR}" MATCHES ".*Win64*" )
    set( BUILD_TYPE x64 )
else()
    set( BUILD_TYPE Win32 )
endif()

# libpng
set( PNG_DEPDIR "${DEPS_DIR}/libpng1617" )
set( PNG_INCDIR "${PNG_DEPDIR}" )

if( ${BUILD_TYPE} STREQUAL x64 )
	set( PNG_DLLDIR "${PNG_DEPDIR}/projects/vstudio/x64/Release" )
	set( PNG_LIBDIR "${PNG_DEPDIR}/projects/vstudio/x64" )
else()
	set( PNG_DLLDIR "${PNG_DEPDIR}/projects/vstudio/Release" )
	set( PNG_LIBDIR "${PNG_DEPDIR}/projects/vstudio" )
endif()

set( PNG_LIBS libpng16 zlib )

# libsndfile
set( SNDFILE_DEPDIR "${DEPS_DIR}/libsndfile" )
set( SNDFILE_INCDIR "${SNDFILE_DEPDIR}/include" )

if( ${BUILD_TYPE} STREQUAL x64 )
	set( SNDFILE_DLLDIR "${SNDFILE_DEPDIR}/bin64" )
	set( SNDFILE_LIBDIR "${SNDFILE_DEPDIR}/lib64" )
else()
	set( SNDFILE_DLLDIR "${SNDFILE_DEPDIR}/bin" )
	set( SNDFILE_LIBDIR "${SNDFILE_DEPDIR}/lib" )
endif()

set( SNDFILE_LIBS libsndfile-1 )

# libpoco
set( POCO_LIBNAMES Foundation Net Util JSON XML )

set( POCO_DEPDIR "${DEPS_DIR}/poco-1.6.0" )
foreach( poco_lib ${POCO_LIBNAMES} )
	set( POCO_INCDIR ${POCO_INCDIR} ${POCO_DEPDIR}/${poco_lib}/include )
#	include_directories( "${POCO_DEPDIR}/${poco_lib}/include" )
	set( POCO_LIBS ${POCO_LIBS} Poco${poco_lib} )
endforeach()

if( ${BUILD_TYPE} STREQUAL x64 )
	set( POCO_DLLDIR "${POCO_DEPDIR}/bin64" )
	set( POCO_LIBDIR "${POCO_DEPDIR}/lib64" )
else()
	set( POCO_DLLDIR "${POCO_DEPDIR}/bin" )
	set( POCO_LIBDIR "${POCO_DEPDIR}/lib" )
endif()

# kinect
set( KINECT_INCDIR "$<1:$(KINECTSDK20_DIR)>/inc" )

if( ${BUILD_TYPE} STREQUAL x64 )
	set( KINECT_LIBDIR "$(KINECTSDK20_DIR)/lib/x64" )
else()
	set( KINECT_LIBDIR "$(KINECTSDK20_DIR)/lib/x86" )
endif()

message( "${KINECT_INCDIR}" )
message( "${KINECT_LIBDIR}" )

set( KINECT_LIBS kinect20 )

message( "${POCO_DLLDIR}" )

add_subdirectory( src )
add_subdirectory( exe )
#add_subdirectory( test-kinect-sdk )
