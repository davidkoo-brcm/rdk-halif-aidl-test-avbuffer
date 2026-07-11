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

#ifndef AV_BUFFER_H
#define AV_BUFFER_H

#include <binder/BinderService.h>
#include <optional>
#include <string>
#include <utils/StrongPointer.h>
#include <vector>
#include <set>
#include <mutex>
#include <condition_variable>
#include <future>
#include <chrono>
#include <thread>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <unistd.h>
#include <atomic>

#include "com/rdk/hal/avbuffer/BnAVBuffer.h"
#include "com/rdk/hal/avbuffer/BnAVBufferSpaceListener.h"
#include "com/rdk/hal/videodecoder/IVideoDecoder.h"
#include "com/rdk/hal/audiodecoder/IAudioDecoder.h"


#define NUM_AUDIO_DECODER 2
#define NUM_VIDEO_DECODER 2

namespace com {
    namespace rdk {
        namespace hal {
            namespace avbuffer {

#define SECURE_HEAP_SIZE 16000000
#define NON_SECURE_HEAP_SIZE 16000000
#define VIDEO_POOL_SIZE 1000000
#define AUDIO_POOL_SIZE 1000000
#define POOL_SIZE 1000000

                class AVBuffer : public android::BinderService<AVBuffer>,
                                 public BnAVBuffer {
                private:
                    struct InternalPoolInfo {
                        PoolMetrics metrics;
                        bool isSecure;
                    };

                    struct BufferInfo {
                        Pool poolHandle;
                        int32_t size;
                        bool valid;
                    };

                    long secureHeapSize;
                    long nonSecureHeapSize;
                    long totalSecureBytesUsed;
                    long totalNonSecureBytesUsed;
                    static std::atomic<uint8_t> poolHandleCounter;
                    static std::atomic<int64_t> bufferHandleCounter;
                    // Pool.handle is signed int8_t (1-127 valid); a never-recycled counter
                    // wraps negative after 127 creations. createPool() reuses these first.
                    std::set<uint8_t> freePoolHandles;
                    std::unordered_map<uint8_t, InternalPoolInfo> poolMetricsMap;
                    std::unordered_map<int64_t, BufferInfo> bufferMap;
                    std::unordered_map<uint8_t, std::vector<int64_t>> poolAllocations;
                    android::sp<IAVBufferSpaceListener> spaceListener;

                    std::mutex poolMutex;
                    std::mutex bufferLock;
                    std::condition_variable _cv;

                    // Reuses a freed pool handle if one is available, else mints a new one.
                    uint8_t allocatePoolHandle();

                public:
                    AVBuffer();
                    virtual ~AVBuffer();

                    static char const *getServiceName() { return IAVBuffer::serviceName().c_str(); }

                    ::android::binder::Status getHeapMetrics(bool secureHeap, ::com::rdk::hal::avbuffer::HeapMetrics* _aidl_return) override;
                    ::android::binder::Status createVideoPool(bool secureHeap, const ::com::rdk::hal::videodecoder::IVideoDecoder::Id& videoDecoderId, const ::android::sp<::com::rdk::hal::avbuffer::IAVBufferSpaceListener>& listener, ::com::rdk::hal::avbuffer::Pool* _aidl_return) override;
                    ::android::binder::Status createAudioPool(bool secureHeap, const ::com::rdk::hal::audiodecoder::IAudioDecoder::Id& audioDecoderId, const ::android::sp<::com::rdk::hal::avbuffer::IAVBufferSpaceListener>& listener, ::com::rdk::hal::avbuffer::Pool* _aidl_return) override;
                    ::android::binder::Status destroyPool(const ::com::rdk::hal::avbuffer::Pool& poolHandle, bool* _aidl_return) override;
                    ::android::binder::Status getPoolMetrics(const ::com::rdk::hal::avbuffer::Pool& poolHandle, ::com::rdk::hal::avbuffer::PoolMetrics* _aidl_return) override;
                    ::android::binder::Status getAllPoolMetrics(bool secureHeap, ::std::vector<::com::rdk::hal::avbuffer::PoolMetrics>* _aidl_return) override;
                    ::android::binder::Status alloc(const ::com::rdk::hal::avbuffer::Pool& poolHandle, int32_t size, int64_t* _aidl_return) override;
                    ::android::binder::Status notifyWhenSpaceAvailable(const ::com::rdk::hal::avbuffer::Pool& poolHandle, int32_t size, bool* _aidl_return) override;
                    ::android::binder::Status trimSize(int64_t bufferHandle, int32_t newSize, bool* _aidl_return) override;
                    ::android::binder::Status free(int64_t bufferHandle, bool* _aidl_return) override;
                    ::android::binder::Status isValid(int64_t bufferHandle, bool* _aidl_return) override;
                    ::android::binder::Status getAllocList(const ::com::rdk::hal::avbuffer::Pool& poolHandle, ::std::vector<int64_t>* _aidl_return) override;
                    ::android::binder::Status calculateSHA1(int64_t bufferHandle, ::std::vector<uint8_t>* _aidl_return) override;
                };
            }
        }
    }
}

#endif AV_BUFFER_H
