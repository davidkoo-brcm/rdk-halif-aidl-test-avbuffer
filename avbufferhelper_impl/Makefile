#** *****************************************************************************
# *
# * If not stated otherwise in this file or this component's LICENSE file the
# * following copyright and licenses apply:
# *
# * Copyright 2025 RDK Management
# *
# * Licensed under the Apache License, Version 2.0 (the "License");
# * you may not use this file except in compliance with the License.
# * You may obtain a copy of the License at
# *
# *
# * http://www.apache.org/licenses/LICENSE-2.0
# *
# * Unless required by applicable law or agreed to in writing, software
# * distributed under the License is distributed on an "AS IS" BASIS,
# * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# * See the License for the specific language governing permissions and
# * limitations under the License.
# *
#** ******************************************************************************

# Real IAVBufferHelper implementation, shm-backed, used only by the mock.
RDK_HAL_DIR ?= $(CURDIR)/../../../../rdk-halif-aidl
AVBUFFERHELPER_HEADER_DIR ?= $(RDK_HAL_DIR)/avbuffer/current

CXX ?= g++
CXXFLAGS := -Wall -Wextra -std=c++17 -fPIC -ggdb
INC_FLAGS := -I$(AVBUFFERHELPER_HEADER_DIR)

TARGET_LIB := libavbufferhelper.so
SRC := AVBufferHelperImpl.cpp
OBJ := $(SRC:.cpp=.o)

.PHONY: all clean

all: $(TARGET_LIB)

$(TARGET_LIB): $(OBJ)
	$(CXX) -shared -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INC_FLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET_LIB)
