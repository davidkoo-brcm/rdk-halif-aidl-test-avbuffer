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
 * AVBuffer.cpp
 *
 * Contains the implementation of the AVBuffer interfaces.
 */

#include "AVBuffer.h"

namespace com {
    namespace rdk {
        namespace hal {
            namespace avbuffer {

                std::atomic<uint8_t> AVBuffer::poolHandleCounter(1);
                std::atomic<int64_t> AVBuffer::bufferHandleCounter(1);


                AVBuffer::AVBuffer() {
                    secureHeapSize = SECURE_HEAP_SIZE;
                    nonSecureHeapSize = NON_SECURE_HEAP_SIZE;
                    totalSecureBytesUsed = 0;
                    totalNonSecureBytesUsed = 0;

                    printf("[SERVICE]AVBuffer service initialized\n");
                }

                AVBuffer::~AVBuffer() {
                    printf("[SERVICE]AVBuffer service destroyed\n");
                }

                ::android::binder::Status AVBuffer::getHeapMetrics(bool secureHeap, ::com::rdk::hal::avbuffer::HeapMetrics* _aidl_return)
                {
                    _aidl_return->secure = secureHeap;

                    if (secureHeap)
                    {
                        _aidl_return->bytesUsed = this->totalSecureBytesUsed;
                        _aidl_return->bytesTotal = this->secureHeapSize;
                    }
                    else
                    {
                        _aidl_return->bytesUsed = this->totalNonSecureBytesUsed;
                        _aidl_return->bytesTotal = this->nonSecureHeapSize;
                    }
                    return ::android::binder::Status::ok();
                }

                ::android::binder::Status AVBuffer::createVideoPool(bool secureHeap, const ::com::rdk::hal::videodecoder::IVideoDecoder::Id& videoDecoderId, const ::android::sp<::com::rdk::hal::avbuffer::IAVBufferSpaceListener>& listener, ::com::rdk::hal::avbuffer::Pool* _aidl_return)
                {
                    std::lock_guard<std::mutex> lock(poolMutex);
                    // Check if decoder ID is invalid
                    if ((videoDecoderId.value == ::com::rdk::hal::videodecoder::IVideoDecoder::Id::UNDEFINED) || (videoDecoderId.value < 0))
                    {
                        std::cout << "[SERVICE]ILLEGAL ARGUMENT " << videoDecoderId.value << std::endl;
                        return ::android::binder::Status::fromExceptionCode(::android::binder::Status::EX_ILLEGAL_ARGUMENT);
                    }
                    // Check for available memory and create pool
                    if (secureHeap)
                    {
                        if ((this->secureHeapSize - this->totalSecureBytesUsed) < VIDEO_POOL_SIZE)
                        {
                            std::cout << "Out of secure heap memory!" << std::endl;
                            return ::android::binder::Status::fromExceptionCode(::android::binder::Status::EX_SERVICE_SPECIFIC, "HALError::OUT_OF_MEMORY");
                        }
                        this->totalSecureBytesUsed += VIDEO_POOL_SIZE;
                    }
                    else
                    {
                        if ((this->nonSecureHeapSize - this->totalNonSecureBytesUsed) < VIDEO_POOL_SIZE)
                        {
                            std::cout << "[SERVICE]Out of non-secure heap memory!" << std::endl;
                            return ::android::binder::Status::fromExceptionCode(::android::binder::Status::EX_SERVICE_SPECIFIC, "HALError::OUT_OF_MEMORY");
                        }
                        this->totalNonSecureBytesUsed += VIDEO_POOL_SIZE;
                    }
                    // Create pool handle
                    uint8_t handle = poolHandleCounter.fetch_add(1);
                    Pool pool;
                    pool.handle = handle;
                    *_aidl_return = pool;

                    // Save PoolMetrics and secure flag in internal map
                    PoolMetrics metrics;
                    metrics.poolHandle = pool;
                    metrics.bytesUsed = 0;
                    metrics.bytesTotal = VIDEO_POOL_SIZE;

                    InternalPoolInfo poolInfo;
                    poolInfo.metrics = metrics;
                    poolInfo.isSecure = secureHeap;

                    poolMetricsMap[handle] = poolInfo;

                    std::cout << "[SERVICE] Created " << (secureHeap ? "secure" : "non-secure")
                              << " video pool with handle: " << static_cast<int>(handle) << std::endl;

                    this->spaceListener = listener;
                    return ::android::binder::Status::ok();
                }

                ::android::binder::Status AVBuffer::createAudioPool(bool secureHeap, const ::com::rdk::hal::audiodecoder::IAudioDecoder::IAudioDecoder::Id &audioDecoderId, const ::android::sp<IAVBufferSpaceListener> &listener, Pool *_aidl_return)
                {
                    std::lock_guard<std::mutex> lock(poolMutex);
                    // Check if decoder ID is invalid
                    if ((audioDecoderId.value == ::com::rdk::hal::audiodecoder::IAudioDecoder::Id::UNDEFINED) || (audioDecoderId.value < 0))
                    {
                        std::cout << "[SERVICE]ILLEGAL ARGUMENT " << audioDecoderId.value << std::endl;
                        return ::android::binder::Status::fromExceptionCode(::android::binder::Status::EX_ILLEGAL_ARGUMENT);
                    }
                    // Check for available memory and create pool
                    if (secureHeap)
                    {
                        if ((this->secureHeapSize - this->totalSecureBytesUsed) < AUDIO_POOL_SIZE)
                        {
                            std::cout << "[SERVICE]Out of secure heap memory!" << std::endl;
                            return ::android::binder::Status::fromExceptionCode(::android::binder::Status::EX_SERVICE_SPECIFIC, "HALError::OUT_OF_MEMORY");
                        }
                        this->totalSecureBytesUsed += AUDIO_POOL_SIZE;
                    }
                    else
                    {
                        if ((this->nonSecureHeapSize - this->totalNonSecureBytesUsed) < AUDIO_POOL_SIZE)
                        {
                            std::cout << "[SERVICE]Out of non-secure heap memory!" << std::endl;
                            return ::android::binder::Status::fromExceptionCode(::android::binder::Status::EX_SERVICE_SPECIFIC, "HALError::OUT_OF_MEMORY");
                        }
                        this->totalNonSecureBytesUsed += AUDIO_POOL_SIZE;
                    }
                    // Create pool handle
                    uint8_t handle = poolHandleCounter.fetch_add(1);
                    Pool pool;
                    pool.handle = handle;
                    *_aidl_return = pool;

                    // Save PoolMetrics and secure flag in internal map
                    PoolMetrics metrics;
                    metrics.poolHandle = pool;
                    metrics.bytesUsed = 0;
                    metrics.bytesTotal = AUDIO_POOL_SIZE;

                    InternalPoolInfo poolInfo;
                    poolInfo.metrics = metrics;
                    poolInfo.isSecure = secureHeap;

                    poolMetricsMap[handle] = poolInfo;

                    std::cout << "[SERVICE]Created " << (secureHeap ? "secure" : "non-secure")
                              << " audio pool with handle: " << static_cast<int>(handle) << std::endl;

                    this->spaceListener = listener;
                    return ::android::binder::Status::ok();
                }

                ::android::binder::Status AVBuffer::destroyPool(const ::com::rdk::hal::avbuffer::Pool& poolHandle, bool* _aidl_return)
                {
                    auto it = poolMetricsMap.find(poolHandle.handle);
                    if (it == poolMetricsMap.end()) {
                        std::cout << "[SERVICE] Invalid pool handle" << std::endl;
                        *_aidl_return = false;
                        return ::android::binder::Status::ok();
                    }

                    const InternalPoolInfo& poolInfo = it->second;

                    // Check if the pool is empty
                    if (poolInfo.metrics.bytesUsed > 0) {
                        std::cout << "[SERVICE] Pool is not empty. bytesUsed = " << poolInfo.metrics.bytesUsed << std::endl;
                        *_aidl_return = false;
                        return ::android::binder::Status::fromExceptionCode(::android::binder::Status::EX_SERVICE_SPECIFIC, "HALError::NOT_EMPTY");
                    }

                    if (poolInfo.isSecure) {
                        this->totalSecureBytesUsed -= poolInfo.metrics.bytesTotal;
                    } else {
                        this->totalNonSecureBytesUsed -= poolInfo.metrics.bytesTotal;
                    }

                    // Erase pool info from map
                    poolMetricsMap.erase(it);
                    *_aidl_return = true;
                    return ::android::binder::Status::ok();
                }

                ::android::binder::Status AVBuffer::getPoolMetrics(const ::com::rdk::hal::avbuffer::Pool& poolHandle, ::com::rdk::hal::avbuffer::PoolMetrics* _aidl_return)
                {
                    if (poolHandle.handle == ::com::rdk::hal::avbuffer::Pool::INVALID)
                    {
                        return ::android::binder::Status::fromExceptionCode(::android::binder::Status::EX_ILLEGAL_ARGUMENT);
                    }

                    auto it = poolMetricsMap.find(poolHandle.handle);
                    if (it == poolMetricsMap.end()) {
                        std::cout << "[SERVICE] Invalid pool handle: " << static_cast<int>(poolHandle.handle) << std::endl;
                        return ::android::binder::Status::fromExceptionCode(::android::binder::Status::EX_ILLEGAL_ARGUMENT);
                    }

                    const InternalPoolInfo& poolInfo = it->second;
                    *_aidl_return = poolInfo.metrics;
                    return ::android::binder::Status::ok();
                }

                ::android::binder::Status AVBuffer::getAllPoolMetrics(bool secureHeap, ::std::vector<::com::rdk::hal::avbuffer::PoolMetrics>* _aidl_return)
                {
                    _aidl_return->clear();
                    for (const auto& [handle, info] : poolMetricsMap)
                    {
                        if (info.isSecure == secureHeap)
                        {
                            _aidl_return->push_back(info.metrics);
                            std::cout << "[SERVICE]Included pool handle " << static_cast<int>(handle)
                                      << " (used: " << info.metrics.bytesUsed
                                      << ", total: " << info.metrics.bytesTotal << ")" << std::endl;
                        }
                    }
                    return ::android::binder::Status::ok();
                }

                ::android::binder::Status AVBuffer::alloc(const ::com::rdk::hal::avbuffer::Pool& poolHandle, int32_t size, int64_t* _aidl_return)
                {
                    if(size <= 0 || poolHandle.handle == ::com::rdk::hal::avbuffer::Pool::INVALID)
                    {
                        *_aidl_return = ::com::rdk::hal::avbuffer::Pool::INVALID;
                        return ::android::binder::Status::ok();
                    }

                    std::lock_guard<std::mutex> lock(poolMutex);
                    const uint8_t handle = poolHandle.handle;

                    // Check if pool exists
                    auto metricsIt = poolMetricsMap.find(handle);
                    if (metricsIt == poolMetricsMap.end()) {
                        *_aidl_return = ::com::rdk::hal::avbuffer::Pool::INVALID;
                        return ::android::binder::Status::ok();
                    }

                    PoolMetrics& metrics = metricsIt->second.metrics;

                    // Check if there's enough space
                    if ((metrics.bytesTotal - metrics.bytesUsed) < size) {
                        std::cout << "[SERVICE] not enough memory in the pool" << std::endl;
                        return ::android::binder::Status::fromExceptionCode( ::android::binder::Status::EX_SERVICE_SPECIFIC, "HALError::OUT_OF_MEMORY");
                    }
                    // Allocate buffer handle
                    int64_t bufferHandle = bufferHandleCounter.fetch_add(1);
                    // Store buffer info
                    {
                        std::lock_guard<std::mutex> bufLock(bufferLock);
                        bufferMap[bufferHandle] = BufferInfo{
                                .poolHandle = poolHandle,
                                .size = size,
                                .valid = true
                        };
                        poolAllocations[handle].push_back(bufferHandle);
                    }
                    metrics.bytesUsed += size;
                    *_aidl_return = bufferHandle;
                    return ::android::binder::Status::ok();
                }

                ::android::binder::Status AVBuffer::notifyWhenSpaceAvailable(const ::com::rdk::hal::avbuffer::Pool& poolHandle, int32_t size, bool* _aidl_return)
                {
                    if (size <= 0 || size > POOL_SIZE)
                    {
                        *_aidl_return = false;
                        return ::android::binder::Status::fromExceptionCode(::android::binder::Status::EX_ILLEGAL_ARGUMENT);
                    }

                    auto it = poolMetricsMap.find(poolHandle.handle);
                    if (it == poolMetricsMap.end())
                    {
                        *_aidl_return = false;
                        return ::android::binder::Status::fromExceptionCode(::android::binder::Status::EX_ILLEGAL_ARGUMENT);
                    }

                    printf("[SERVICE] notifyWhenSpaceAvailable called with poolHandle=%d, size=%d\n",poolHandle.handle, size);
                    const auto& poolInfo = it->second;
                    long availableSpace = poolInfo.metrics.bytesTotal - poolInfo.metrics.bytesUsed;

                    if (availableSpace >= size)
                    {
                        if (spaceListener != nullptr) {
                            this->spaceListener->onSpaceAvailable();
                        }
                    } else {
                        //TBD
                        std::thread([this]() {
                            std::this_thread::sleep_for(std::chrono::milliseconds(200));  // simulate delay
                            if (spaceListener != nullptr) {
                                this->spaceListener->onSpaceAvailable();
                            }
                        }).detach();
                    }

                    *_aidl_return = true;
                    return ::android::binder::Status::ok();
                }

                ::android::binder::Status AVBuffer::trimSize(int64_t bufferHandle, int32_t newSize, bool* _aidl_return)
                {
                    if (newSize <= 0) {
                        *_aidl_return = false;
                        return ::android::binder::Status::ok();
                    }

                    std::lock_guard<std::mutex> lock(bufferLock);

                    auto it = bufferMap.find(bufferHandle);
                    if (it == bufferMap.end() || !it->second.valid) {
                        *_aidl_return = false;
                        return ::android::binder::Status::ok();
                    }

                    BufferInfo& info = it->second;
                    const uint8_t poolHandle = info.poolHandle.handle;

                    auto& allocList = poolAllocations[poolHandle];
                    if (allocList.empty() || allocList.back() != bufferHandle) {
                        return ::android::binder::Status::fromExceptionCode(::android::binder::Status::EX_ILLEGAL_STATE);
                    }

                    if (newSize > info.size) {
                        *_aidl_return = false;
                        return ::android::binder::Status::ok();
                    }
                    {
                        std::lock_guard<std::mutex> poolLock(poolMutex);
                        auto poolIt = poolMetricsMap.find(poolHandle);
                        if (poolIt != poolMetricsMap.end()) {
                            poolIt->second.metrics.bytesUsed -= (info.size - newSize);
                        }
                    }

                    info.size = newSize;
                    *_aidl_return = true;
                    return ::android::binder::Status::ok();
                }

                ::android::binder::Status AVBuffer::free(int64_t bufferHandle, bool* _aidl_return)
                {
                    std::lock_guard <std::mutex> lock(bufferLock);
                    auto it = bufferMap.find(bufferHandle);
                    if (it == bufferMap.end() || !it->second.valid) {
                        *_aidl_return = false;
                        return ::android::binder::Status::ok();
                    }

                    BufferInfo &info = it->second;
                    const uint8_t poolHandle = info.poolHandle.handle;
                    {
                        std::lock_guard <std::mutex> poolLock(poolMutex);
                        auto poolIt = poolMetricsMap.find(poolHandle);
                        if (poolIt != poolMetricsMap.end()) {
                            poolIt->second.metrics.bytesUsed -= info.size;
                        }
                    }

                    // Mark invalid and remove from allocations list
                    info.valid = false;

                    auto &vec = poolAllocations[poolHandle];
                    vec.erase(std::remove(vec.begin(), vec.end(), bufferHandle), vec.end());

                    *_aidl_return = true;
                    return ::android::binder::Status::ok();
                }

                ::android::binder::Status AVBuffer::getAllocList(const Pool& poolHandle, std::vector<int64_t>* _aidl_return)
                {
                    std::lock_guard<std::mutex> lock(poolMutex);
                    if (poolMetricsMap.find(poolHandle.handle) == poolMetricsMap.end())
                    {
                        return ::android::binder::Status::fromExceptionCode(::android::binder::Status::EX_ILLEGAL_ARGUMENT);
                    }
                    std::lock_guard<std::mutex> bufLock(bufferLock);
                    *_aidl_return = poolAllocations[poolHandle.handle];
                    return ::android::binder::Status::ok();
                }

                ::android::binder::Status AVBuffer::calculateSHA1(int64_t bufferHandle, ::std::vector<uint8_t>* _aidl_return) {
                    std::lock_guard<std::mutex> lock(bufferLock);
                    auto it = bufferMap.find(bufferHandle);
                    if (it == bufferMap.end() || !it->second.valid) {
                        return ::android::binder::Status::fromExceptionCode(::android::binder::Status::EX_ILLEGAL_ARGUMENT);
                    }
                    // SHA-1 support is not implemented in this build.
                    return ::android::binder::Status::fromExceptionCode(::android::binder::Status::EX_UNSUPPORTED_OPERATION);
                }

                ::android::binder::Status AVBuffer::isValid(int64_t bufferHandle, bool* _aidl_return)
                {
                    std::lock_guard<std::mutex> lock(bufferLock);
                    auto it = bufferMap.find(bufferHandle);
                    *_aidl_return = (it != bufferMap.end() && it->second.valid);
                    return ::android::binder::Status::ok();
                }
            }
        }
    }
}



