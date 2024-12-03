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

#include <string>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/reboot.h>

#include <binder/IServiceManager.h>
#include <utils/StrongPointer.h>

#include "AVBuffer.h"

#define _bytesTotal 10000

using namespace com::rdk::hal;
using namespace com::rdk::hal::avbuffer;
using namespace com::rdk::hal::videodecoder;
using namespace com::rdk::hal::audiodecoder;
using android::binder::Status;
using std::vector;

sp<IAVBufferSpaceListener> _spacelistener;
int _bytesused = 0;
int _bytesfree = 0;
int newsizetoallocate = 0;

void onSpaceAvailableCheck(int size);

::android::binder::Status AVBuffer::getHeapMetrics(bool secureHeap, HeapMetrics *_aidl_return) 
{
    printf("getHeapMetrics called with secureHeap=%d\n", secureHeap);
    _aidl_return->secure = secureHeap;
    _aidl_return->bytesUsed = 0;
    _aidl_return->bytesTotal = 1000;
    return ::android::binder::Status::ok();
}

::android::binder::Status AVBuffer::createVideoPool(bool secureHeap, const IVideoDecoder::Id& videoDecoderId, const sp<IAVBufferSpaceListener>& listener, Pool* _aidl_return) 
{
    if (videoDecoderId.value < 0) 
    {
        return Status::fromExceptionCode(Status::EX_ILLEGAL_ARGUMENT);
    }

    _spacelistener = listener;

    Pool pool;
    pool.handle = 1; // Dummy pool handle
    *_aidl_return = pool;
    return ::android::binder::Status::ok();
}

::android::binder::Status
AVBuffer::createAudioPool(bool secureHeap, const IAudioDecoder::Id &audioDecoderId,
                          const sp <IAVBufferSpaceListener> &listener, Pool *_aidl_return) {
    printf("createAudioPool called with secureHeap=%d, audioDecoderId=%d\n", secureHeap,
          audioDecoderId.value);
    _aidl_return->handle = 1;
    return ::android::binder::Status::ok();
}

::android::binder::Status AVBuffer::destroyPool(const Pool &poolHandle , bool * t) 
{
    printf("destroyPool called with poolHandle=%d\n", poolHandle.handle);
    return ::android::binder::Status::ok();
}

::android::binder::Status
AVBuffer::getPoolMetrics(const Pool &poolHandle, PoolMetrics *_aidl_return) 
{
    printf("getPoolMetrics called with poolHandle=%d\n", poolHandle.handle);
    _aidl_return->poolHandle = poolHandle;
    _aidl_return->bytesUsed = _bytesused;
    _aidl_return->bytesTotal = _bytesTotal;
    return ::android::binder::Status::ok();
}

::android::binder::Status
AVBuffer::getAllPoolMetrics(bool secureHeap, std::vector <PoolMetrics> *_aidl_return) 
{
    printf("getAllPoolMetrics called with secureHeap=%d\n", secureHeap);
    PoolMetrics metrics;
    metrics.poolHandle.handle = 1;
    metrics.bytesUsed = 0;
    metrics.bytesTotal = 1000;
    _aidl_return->push_back(metrics);
    return ::android::binder::Status::ok();
}

::android::binder::Status AVBuffer::alloc(const Pool &poolHandle, int size, int64_t *_aidl_return) 
{
    if(size <= 0 || poolHandle.handle == ::com::rdk::hal::avbuffer::Pool::INVALID)
    {
	    return Status::fromExceptionCode(Status::EX_ILLEGAL_ARGUMENT);
    }
    _bytesfree = _bytesTotal-_bytesused;  // new logic
    if(size > _bytesfree)
    {
        printf("========== memory not available ===============\n");
        return Status::fromExceptionCode(Status::EX_SERVICE_SPECIFIC);
    }
    
    newsizetoallocate = size;
    _bytesused = _bytesused+size; // new logic
    printf("alloc called with poolHandle=%d, size=%d\n", poolHandle.handle, newsizetoallocate);
    *_aidl_return = 1;
    
    return ::android::binder::Status::ok();
}

::android::binder::Status AVBuffer::notifyWhenSpaceAvailable(const Pool &poolHandle, int size , bool * t) 
{
    if(size > (_bytesTotal - _bytesused))
    {
        std::thread threadonSpaceAvailableCheck(onSpaceAvailableCheck, size);
        threadonSpaceAvailableCheck.detach();
    }
    printf("notifyWhenSpaceAvailable called with poolHandle=%d, size=%d\n", poolHandle.handle, size);
    return ::android::binder::Status::ok();
}

void onSpaceAvailableCheck(int size)
{
    while(size > (_bytesTotal - _bytesused));
    _spacelistener->onSpaceAvailable();   
}

::android::binder::Status AVBuffer::trimSize(int64_t bufferHandle, int newSize , bool * t) 
{
    printf("trimSize called with bufferHandle=%lld, newSize=%d\n", bufferHandle, newSize);
    return ::android::binder::Status::ok();
}

::android::binder::Status AVBuffer::free(int64_t bufferHandle , bool * t)  
{
    _bytesused = _bytesused-newsizetoallocate;
    _bytesfree = _bytesTotal - _bytesused; 
    printf("free called with bufferHandle=%lld\n", bufferHandle);
    return ::android::binder::Status::ok();
}

::android::binder::Status AVBuffer::isValid(int64_t bufferHandle, bool *_aidl_return) 
{
    printf("isValid called with bufferHandle=%lld\n", bufferHandle);
    *_aidl_return = true;
    return ::android::binder::Status::ok();
}

::android::binder::Status
AVBuffer::getAllocList(const Pool &poolHandle, std::vector <int64_t> *_aidl_return) 
{
    printf("getAllocList called with poolHandle=%d\n", poolHandle.handle);
    _aidl_return->push_back(1);
    return ::android::binder::Status::ok();
}

::android::binder::Status
AVBuffer::calculateSHA1(int64_t bufferHandle, std::vector <uint8_t> *_aidl_return) {
    printf("calculateSHA1 called with bufferHandle=%lld\n", bufferHandle);
    _aidl_return->push_back(0);
    return ::android::binder::Status::ok();
}



