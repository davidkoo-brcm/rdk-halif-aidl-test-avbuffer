.PHONY: all

AVBUFFER_AIDL_LIB_NAME := avbufferaidl

AVBUFFER_AIDL_LIB := lib$(AVBUFFER_AIDL_LIB_NAME).so

VIDEODECODER_AIDL_LIB_NAME := videodecoderaidl

VIDEODECODER_AIDL_LIB := lib$(VIDEODECODER_AIDL_LIB_NAME).so

AUDIODECODER_AIDL_LIB_NAME := audiodecoderaidl

AUDIODECODER_AIDL_LIB := lib$(AUDIODECODER_AIDL_LIB_NAME).so

EMULATE_SERVICE_BIN := EmulateAVbufferService

BUILD_DIR ?= $(CURDIR)/../build

AIDL_GEN_DIR ?= $(BUILD_DIR)/avbuffer_gen/current

EMULATE_SERVICE_DIR := $(CURDIR)/src

AVBUFFER_AIDL_SRC += ${AIDL_GEN_DIR}/cpp/com/rdk/hal/avbuffer/HeapMetrics.cpp
AVBUFFER_AIDL_SRC += ${AIDL_GEN_DIR}/cpp/com/rdk/hal/avbuffer/IAVBuffer.cpp
AVBUFFER_AIDL_SRC += ${AIDL_GEN_DIR}/cpp/com/rdk/hal/avbuffer/IAVBufferSpaceListener.cpp
AVBUFFER_AIDL_SRC += ${AIDL_GEN_DIR}/cpp/com/rdk/hal/avbuffer/Pool.cpp
AVBUFFER_AIDL_SRC += ${AIDL_GEN_DIR}/cpp/com/rdk/hal/avbuffer/PoolMetrics.cpp

VIDEODECODER_AIDL_SRC += $(AIDL_GEN_DIR)/cpp/com/rdk/hal/videodecoder/Capabilities.cpp
VIDEODECODER_AIDL_SRC += $(AIDL_GEN_DIR)/cpp/com/rdk/hal/videodecoder/CodecCapabilities.cpp
VIDEODECODER_AIDL_SRC += $(AIDL_GEN_DIR)/cpp/com/rdk/hal/videodecoder/Codec.cpp
VIDEODECODER_AIDL_SRC += $(AIDL_GEN_DIR)/cpp/com/rdk/hal/videodecoder/CSDVideoFormat.cpp
VIDEODECODER_AIDL_SRC += $(AIDL_GEN_DIR)/cpp/com/rdk/hal/videodecoder/DynamicRange.cpp
VIDEODECODER_AIDL_SRC += $(AIDL_GEN_DIR)/cpp/com/rdk/hal/videodecoder/ErrorCode.cpp
VIDEODECODER_AIDL_SRC += $(AIDL_GEN_DIR)/cpp/com/rdk/hal/videodecoder/FrameMetadata.cpp
VIDEODECODER_AIDL_SRC += $(AIDL_GEN_DIR)/cpp/com/rdk/hal/videodecoder/IVideoDecoderController.cpp
VIDEODECODER_AIDL_SRC += $(AIDL_GEN_DIR)/cpp/com/rdk/hal/videodecoder/IVideoDecoderControllerListener.cpp
VIDEODECODER_AIDL_SRC += $(AIDL_GEN_DIR)/cpp/com/rdk/hal/videodecoder/IVideoDecoder.cpp
VIDEODECODER_AIDL_SRC += $(AIDL_GEN_DIR)/cpp/com/rdk/hal/videodecoder/IVideoDecoderEventListener.cpp
VIDEODECODER_AIDL_SRC += $(AIDL_GEN_DIR)/cpp/com/rdk/hal/videodecoder/IVideoDecoderManager.cpp
VIDEODECODER_AIDL_SRC += $(AIDL_GEN_DIR)/cpp/com/rdk/hal/videodecoder/OperationalMode.cpp
VIDEODECODER_AIDL_SRC += $(AIDL_GEN_DIR)/cpp/com/rdk/hal/videodecoder/PixelFormat.cpp
VIDEODECODER_AIDL_SRC += $(AIDL_GEN_DIR)/cpp/com/rdk/hal/videodecoder/Property.cpp
VIDEODECODER_AIDL_SRC += $(AIDL_GEN_DIR)/cpp/com/rdk/hal/videodecoder/PropertyKVPair.cpp
VIDEODECODER_AIDL_SRC += $(AIDL_GEN_DIR)/cpp/com/rdk/hal/videodecoder/ScanType.cpp


COMMON_AIDL_GEN_SRC +=${AIDL_GEN_DIR}/cpp/com/rdk/hal/AVSource.cpp
COMMON_AIDL_GEN_SRC +=${AIDL_GEN_DIR}/cpp/com/rdk/hal/HALError.cpp
COMMON_AIDL_GEN_SRC +=${AIDL_GEN_DIR}/cpp/com/rdk/hal/PropertyValue.cpp
COMMON_AIDL_GEN_SRC +=${AIDL_GEN_DIR}/cpp/com/rdk/hal/State.cpp

AUDIODECODER_AIDL_SRC += ${AIDL_GEN_DIR}/cpp/com/rdk/hal/audiodecoder/Capabilities.cpp
AUDIODECODER_AIDL_SRC += ${AIDL_GEN_DIR}/cpp/com/rdk/hal/audiodecoder/ChannelType.cpp
AUDIODECODER_AIDL_SRC += ${AIDL_GEN_DIR}/cpp/com/rdk/hal/audiodecoder/Codec.cpp
AUDIODECODER_AIDL_SRC += ${AIDL_GEN_DIR}/cpp/com/rdk/hal/audiodecoder/CSDAudioFormat.cpp
AUDIODECODER_AIDL_SRC += ${AIDL_GEN_DIR}/cpp/com/rdk/hal/audiodecoder/ErrorCode.cpp
AUDIODECODER_AIDL_SRC += ${AIDL_GEN_DIR}/cpp/com/rdk/hal/audiodecoder/FrameMetadata.cpp
AUDIODECODER_AIDL_SRC += ${AIDL_GEN_DIR}/cpp/com/rdk/hal/audiodecoder/IAudioDecoderController.cpp
AUDIODECODER_AIDL_SRC += ${AIDL_GEN_DIR}/cpp/com/rdk/hal/audiodecoder/IAudioDecoderControllerListener.cpp
AUDIODECODER_AIDL_SRC += ${AIDL_GEN_DIR}/cpp/com/rdk/hal/audiodecoder/IAudioDecoder.cpp
AUDIODECODER_AIDL_SRC += ${AIDL_GEN_DIR}/cpp/com/rdk/hal/audiodecoder/IAudioDecoderEventListener.cpp
AUDIODECODER_AIDL_SRC += ${AIDL_GEN_DIR}/cpp/com/rdk/hal/audiodecoder/IAudioDecoderManager.cpp
AUDIODECODER_AIDL_SRC += ${AIDL_GEN_DIR}/cpp/com/rdk/hal/audiodecoder/PCMFormat.cpp
AUDIODECODER_AIDL_SRC += ${AIDL_GEN_DIR}/cpp/com/rdk/hal/audiodecoder/Property.cpp


EMULATE_SERVICE_BIN_SRC := $(EMULATE_SERVICE_DIR)/AVBufferService.cpp
EMULATE_SERVICE_BIN_SRC += $(EMULATE_SERVICE_DIR)/AVBuffer.cpp

EMULATE_SERVICE_INC_FLAGS := -I$(AIDL_GEN_DIR)/h
EMULATE_SERVICE_INC_FLAGS += -I$(BUILD_DIR)/usr/include

EMULATE_SERVICE_LD_LIBS := -lbinder -lutils

EMULATE_SERVICE_LDFLAGS := -L$(BUILD_DIR)/usr/lib
EMULATE_SERVICE_LDFLAGS += -Wl,-rpath-link=$(BUILD_DIR)/usr/lib 

EMULATE_SERVICE_CFLAGS := -Wall -Wextra -std=c++17 -ggdb

AUDIODECODER_AIDL_LIB := $(BUILD_DIR)/usr/lib/$(AUDIODECODER_AIDL_LIB)
AVBUFFER_AIDL_LIB := $(BUILD_DIR)/usr/lib/$(AVBUFFER_AIDL_LIB)
VIDEODECODER_AIDL_LIB := $(BUILD_DIR)/usr/lib/$(VIDEODECODER_AIDL_LIB)
EMULATE_SERVICE_BIN := $(BUILD_DIR)/usr/lib/$(EMULATE_SERVICE_BIN)

all: $(EMULATE_SERVICE_BIN) $(AVBUFFER_AIDL_LIB) $(VIDEODECODER_AIDL_LIB) $(AUDIODECODER_AIDL_LIB)

$(AUDIODECODER_AIDL_LIB): $(AUDIODECODER_AIDL_SRC:%.cpp=%.o) $(COMMON_AIDL_GEN_SRC:%.cpp=%.o)
	g++ -shared $(EMULATE_SERVICE_LDFLAGS) $^ -o $@ $(EMULATE_SERVICE_LD_LIBS)

$(VIDEODECODER_AIDL_LIB): $(VIDEODECODER_AIDL_SRC:%.cpp=%.o) $(COMMON_AIDL_GEN_SRC:%.cpp=%.o)
	g++ -shared $(EMULATE_SERVICE_LDFLAGS) $^ -o $@ $(EMULATE_SERVICE_LD_LIBS)

$(AVBUFFER_AIDL_LIB):$(VIDEODECODER_AIDL_LIB) $(AUDIODECODER_AIDL_LIB)
$(AVBUFFER_AIDL_LIB): $(AVBUFFER_AIDL_SRC:%.cpp=%.o) $(COMMON_AIDL_GEN_SRC:%.cpp=%.o)
	g++ -shared $(EMULATE_SERVICE_LDFLAGS) $^ -o $@ $(EMULATE_SERVICE_LD_LIBS) -l$(VIDEODECODER_AIDL_LIB_NAME) -l$(AUDIODECODER_AIDL_LIB_NAME)


$(EMULATE_SERVICE_BIN): $(VIDEODECODER_AIDL_LIB) $(AUDIODECODER_AIDL_LIB) $(AVBUFFER_AIDL_LIB)
$(EMULATE_SERVICE_BIN): $(EMULATE_SERVICE_BIN_SRC:%.cpp=%.o)
	g++ $(EMULATE_SERVICE_LDFLAGS) \
		$(EMULATE_SERVICE_BIN_SRC:%.cpp=%.o) -o $@ \
		$(EMULATE_SERVICE_LD_LIBS) \
		-lpthread \
		-l$(AVBUFFER_AIDL_LIB_NAME)

%.o: %.cpp
	g++ $(EMULATE_SERVICE_CFLAGS) $(EMULATE_SERVICE_INC_FLAGS) -fPIC -c $< -o $@
