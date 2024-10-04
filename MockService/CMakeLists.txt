cmake_minimum_required(VERSION 3.8)

# Set the project name and version
project(AvBufferService VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Linux Binder Dependencies
if (NOT DEFINED LINUX_BINDER_OUT_DIR)
    message(STATUS "Setting LINUX_BINDER_OUT_DIR to default value")
    set(LINUX_BINDER_OUT_DIR "${CMAKE_SOURCE_DIR}/../../../rdk-halif-binder/local")
endif()
message(STATUS "LINUX_BINDER_OUT_DIR set to ${LINUX_BINDER_OUT_DIR}")

set(LINUX_BINDER_BIN_DIR "${LINUX_BINDER_OUT_DIR}/bin/")
message(STATUS "LINUX_BINDER_BIN_DIR set to ${LINUX_BINDER_BIN_DIR}")

set(LINUX_BINDER_LIB_DIR "${LINUX_BINDER_OUT_DIR}/lib/")
message(STATUS "LINUX_BINDER_LIB_DIR set to ${LINUX_BINDER_LIB_DIR}")

set(LINUX_BINDER_INCLUDE_DIR "${LINUX_BINDER_OUT_DIR}/include/")
message(STATUS "LINUX_BINDER_INCLUDE_DIR set to ${LINUX_BINDER_INCLUDE_DIR}")


# AIDL Interface Dependencies
if (NOT DEFINED AIDL_EXAMPLE_GEN_DIR)
    message(STATUS "Setting AIDL_EXAMPLE_GEN_DIR to default value")
    set(AIDL_EXAMPLE_GEN_DIR "${CMAKE_SOURCE_DIR}/../hal/avbuffer")
endif()
message(STATUS "AIDL_EXAMPLE_GEN_DIR set to ${AIDL_EXAMPLE_GEN_DIR}")

if (NOT DEFINED AVBUFFER_AIDL_VERSION)
	message(STATUS "Setting AVBUFFER_AIDL_VERSION to current latest")
	set(AVBUFFER_AIDL_VERSION "${AIDL_EXAMPLE_GEN_DIR}/current")
endif()
message(STATUS "AVBUFFER_AIDL_VERSION set to ${AVBUFFER_AIDL_VERSION}")

# AvBufferService Installation Directories
if (NOT DEFINED CMAKE_INSTALL_LIBDIR)
    message("Set and create cmake install lib directory")
    set(CMAKE_INSTALL_LIBDIR ${CMAKE_INSTALL_PREFIX}/lib)
    file(MAKE_DIRECTORY ${CMAKE_INSTALL_LIBDIR})
    message("cmake install lib dir ${CMAKE_INSTALL_LIBDIR}")
endif ()

if (NOT DEFINED CMAKE_INSTALL_BINDIR)
    message("Set and create cmake install bin directory")
    set(CMAKE_INSTALL_BINDIR ${CMAKE_INSTALL_PREFIX}/bin)
    file(MAKE_DIRECTORY ${CMAKE_INSTALL_BINDIR})
    message("cmake install bin dir ${CMAKE_INSTALL_BINDIR}")
endif ()


#############################################################
# MockAvbuffer Build
#############################################################

# Set Service Directory
 set(AVBUFFER_SERVICE  ${CMAKE_SOURCE_DIR}/MockService)
 

# Set AIDL Source files
set(AVBUFFER_AIDL_SRCS
   
    ${AIDL_EXAMPLE_GEN_DIR}/gen/current/cpp/com/rdk/hal/avbuffer/HeapMetrics.cpp
    ${AIDL_EXAMPLE_GEN_DIR}/gen/current/cpp/com/rdk/hal/avbuffer/IAVBuffer.cpp
    ${AIDL_EXAMPLE_GEN_DIR}/gen/current/cpp/com/rdk/hal/avbuffer/IAVBufferSpaceListener.cpp
    ${AIDL_EXAMPLE_GEN_DIR}/gen/current/cpp/com/rdk/hal/avbuffer/Pool.cpp
    ${AIDL_EXAMPLE_GEN_DIR}/gen/current/cpp/com/rdk/hal/avbuffer/PoolMetrics.cpp
    ${AIDL_EXAMPLE_GEN_DIR}/gen/current/cpp/com/rdk/hal/audiodecoder/IAudioDecoder.cpp
    ${AIDL_EXAMPLE_GEN_DIR}/gen/current/cpp/com/rdk/hal/audiodecoder/IAudioDecoderControllerListener.cpp
    ${AIDL_EXAMPLE_GEN_DIR}/gen/current/cpp/com/rdk/hal/audiodecoder/IAudioDecoderEventListener.cpp
    ${AIDL_EXAMPLE_GEN_DIR}/gen/current/cpp/com/rdk/hal/audiodecoder/IAudioDecoderController.cpp
    ${AIDL_EXAMPLE_GEN_DIR}/gen/current/cpp/com/rdk/hal/audiodecoder/Capabilities.cpp
    ${AIDL_EXAMPLE_GEN_DIR}/gen/current/cpp/com/rdk/hal/audiodecoder/CSDAudioFormat.cpp
    ${AIDL_EXAMPLE_GEN_DIR}/gen/current/cpp/com/rdk/hal/audiodecoder/ChannelType.cpp
    ${AIDL_EXAMPLE_GEN_DIR}/gen/current/cpp/com/rdk/hal/audiodecoder/Codec.cpp
    ${AIDL_EXAMPLE_GEN_DIR}/gen/current/cpp/com/rdk/hal/audiodecoder/ErrorCode.cpp
    ${AIDL_EXAMPLE_GEN_DIR}/gen/current/cpp/com/rdk/hal/audiodecoder/FrameMetadata.cpp
    ${AIDL_EXAMPLE_GEN_DIR}/gen/current/cpp/com/rdk/hal/audiodecoder/PCMFormat.cpp
    ${AIDL_EXAMPLE_GEN_DIR}/gen/current/cpp/com/rdk/hal/audiodecoder/Property.cpp
    ${AIDL_EXAMPLE_GEN_DIR}/gen/current/cpp/com/rdk/hal/videodecoder/IVideoDecoder.cpp
    ${AIDL_EXAMPLE_GEN_DIR}/gen/current/cpp/com/rdk/hal/videodecoder/IVideoDecoderController.cpp
    ${AIDL_EXAMPLE_GEN_DIR}/gen/current/cpp/com/rdk/hal/videodecoder/IVideoDecoderControllerListener.cpp
    ${AIDL_EXAMPLE_GEN_DIR}/gen/current/cpp/com/rdk/hal/videodecoder/IVideoDecoderEventListener.cpp
    ${AIDL_EXAMPLE_GEN_DIR}/gen/current/cpp/com/rdk/hal/videodecoder/CSDVideoFormat.cpp
    ${AIDL_EXAMPLE_GEN_DIR}/gen/current/cpp/com/rdk/hal/videodecoder/Capabilities.cpp
    ${AIDL_EXAMPLE_GEN_DIR}/gen/current/cpp/com/rdk/hal/videodecoder/Codec.cpp
    ${AIDL_EXAMPLE_GEN_DIR}/gen/current/cpp/com/rdk/hal/videodecoder/CodecCapabilities.cpp
    ${AIDL_EXAMPLE_GEN_DIR}/gen/current/cpp/com/rdk/hal/videodecoder/DynamicRange.cpp
    ${AIDL_EXAMPLE_GEN_DIR}/gen/current/cpp/com/rdk/hal/videodecoder/ErrorCode.cpp
    ${AIDL_EXAMPLE_GEN_DIR}/gen/current/cpp/com/rdk/hal/videodecoder/FrameMetadata.cpp
    ${AIDL_EXAMPLE_GEN_DIR}/gen/current/cpp/com/rdk/hal/videodecoder/OperationalMode.cpp
    ${AIDL_EXAMPLE_GEN_DIR}/gen/current/cpp/com/rdk/hal/videodecoder/PixelFormat.cpp
    ${AIDL_EXAMPLE_GEN_DIR}/gen/current/cpp/com/rdk/hal/videodecoder/Property.cpp
    ${AIDL_EXAMPLE_GEN_DIR}/gen/current/cpp/com/rdk/hal/videodecoder/PropertyKVPair.cpp
    ${AIDL_EXAMPLE_GEN_DIR}/gen/current/cpp/com/rdk/hal/videodecoder/ScanType.cpp
    ${AIDL_EXAMPLE_GEN_DIR}/gen/current/cpp/com/rdk/hal/PropertyValue.cpp
)

add_library(mavbuffer SHARED
    ${AVBUFFER_AIDL_SRCS}
    ${AVBUFFER_SERVICE}/AVBuffer.cpp
)

target_compile_options(mavbuffer PRIVATE
    -Wall -Wextra -std=c++17
)

target_include_directories(mavbuffer PRIVATE
    ${AIDL_EXAMPLE_GEN_DIR}/gen/current/cpp/h
    ${LINUX_BINDER_INCLUDE_DIR}

)

# Add the library directory
link_directories(${LINUX_BINDER_LIB_DIR})
find_library(BINDER_LIB  binder PATHS ${LINUX_BINDER_LIB_DIR})
find_library(UTILS_LIB utils PATHS ${LINUX_BINDER_LIB_DIR})

target_link_libraries(mavbuffer PRIVATE ${BINDER_LIB} ${UTILS_LIB})

# Add Service Executable
add_executable(AVBufferService
	${AVBUFFER_SERVICE}/AVBufferService.cpp
)

target_compile_options(AVBufferService PRIVATE
    -Wall -Wextra -std=c++17
)

target_include_directories(AVBufferService PRIVATE
    ${AIDL_EXAMPLE_GEN_DIR}/gen/current/cpp/h
    ${LINUX_BINDER_INCLUDE_DIR}
    
)

target_link_libraries(AVBufferService PRIVATE ${BINDER_LIB} ${UTILS_LIB} mavbuffer)

###########################################################
# Install the library
###########################################################
install(
    TARGETS
    mavbuffer

    LIBRARY
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

###########################################################
# Install the binary
###########################################################
install(
    TARGETS
    AVBufferService

    RUNTIME
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)

