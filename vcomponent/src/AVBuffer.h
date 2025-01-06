/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2024 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/** @brief
 * AVBuffer.h
 *
 * The header file contains the AVBuffer service interfaces.
 */

#ifndef _AV_BUFFER_H_
#define _AV_BUFFER_H_

#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>

#include <android/log.h>
#include <binder/BinderService.h>
#include <binder/Status.h>
#include <utils/Errors.h>
#include <utils/Log.h>

#include <utils/String16.h>
#include <utils/Vector.h>

#include "com/rdk/hal/avbuffer/BnAVBuffer.h"
#include "com/rdk/hal/avbuffer/IAVBufferSpaceListener.h"
#include "com/rdk/hal/avbuffer/HeapMetrics.h"
#include "com/rdk/hal/avbuffer/Pool.h"
#include "com/rdk/hal/avbuffer/PoolMetrics.h"
#include "com/rdk/hal/videodecoder/IVideoDecoder.h"
#include "com/rdk/hal/audiodecoder/IAudioDecoder.h"

// libutils
using android::OK;
using android::String8;
using android::String16;
using android::sp;

// libbinder
using android::defaultServiceManager;
using android::binder::Status;

// Generated code
using com::rdk::hal::avbuffer::BnAVBuffer;

// Standard library
using std::string;
using std::unique_ptr;
using std::vector;
using std::mutex;

using namespace com::rdk::hal::avbuffer;
using namespace com::rdk::hal::videodecoder;
using namespace com::rdk::hal::audiodecoder;
using namespace com::rdk::hal;

class AVBuffer : public android::BinderService<AVBuffer>,
                  public BnAVBuffer {
    private:
        friend class BinderService<AVBuffer>;

    AVBuffer() {}
        virtual ~AVBuffer() = default;

        static char const* getServiceName() { return IAVBuffer::serviceName().c_str(); }

    // Implementations of the methods declared in the AIDL interface
    virtual ::android::binder::Status getHeapMetrics(bool secureHeap, HeapMetrics* _aidl_return) override;
    virtual ::android::binder::Status createVideoPool(bool secureHeap, const IVideoDecoder::Id& videoDecoderId, const sp<IAVBufferSpaceListener>& listener, Pool* _aidl_return) override;
    virtual ::android::binder::Status createAudioPool(bool secureHeap, const IAudioDecoder::Id& audioDecoderId, const sp<IAVBufferSpaceListener>& listener, Pool* _aidl_return) override;
    virtual ::android::binder::Status destroyPool(const Pool& poolHandle, bool * t) override;
    virtual ::android::binder::Status getPoolMetrics(const Pool& poolHandle, PoolMetrics* _aidl_return) override;
    virtual ::android::binder::Status getAllPoolMetrics(bool secureHeap, std::vector<PoolMetrics>* _aidl_return) override;
    virtual ::android::binder::Status alloc(const Pool& poolHandle, int size, int64_t* _aidl_return) override;
    virtual ::android::binder::Status notifyWhenSpaceAvailable(const Pool& poolHandle, int size, bool * t) override;
    virtual ::android::binder::Status trimSize(int64_t bufferHandle, int newSize , bool * t) override;
    virtual ::android::binder::Status free(int64_t bufferHandlei , bool * t ) override;
    virtual ::android::binder::Status isValid(int64_t bufferHandle, bool* _aidl_return) override;
    virtual ::android::binder::Status getAllocList(const Pool& poolHandle, std::vector<int64_t>* _aidl_return) override;
    virtual ::android::binder::Status calculateSHA1(int64_t bufferHandle, std::vector<uint8_t>* _aidl_return) override;

};

#endif //_AV_BUFFER_H_
