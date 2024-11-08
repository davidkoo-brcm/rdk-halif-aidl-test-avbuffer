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
#include "/usr/include/c++/11/debug/assertions.h"
#include "/usr/include/assert.h"
#include "AVBuffer.h"



using namespace com::rdk::hal;
using namespace com::rdk::hal::avbuffer;
using namespace com::rdk::hal::videodecoder;
using namespace com::rdk::hal::audiodecoder;
using android::binder::Status;
using std::vector;

sp<IAVBufferSpaceListener> _spacelistener;
/*Macros*/
#define SECURE_HEAP_SIZE 16000000
#define NON_SECURE_HEAP_SIZE 16000000
#define VIDEO_POOL_SIZE 1000000
#define AUDIO_POOL_SIZE 1000000
#define POOL_SIZE 1000000
#define MAX_POOLS 32 
#define MAX_VIDEO_POOLS_PER_HEAP 8 
#define MAX_AUDIO_POOLS_PER_HEAP 8
#define MAX_POOLS_PER_HEAP 16 

/*Keep MAX_POOLS_PER_HEAP as sum of MAX_VIDEO_POOLS_PER_HEAP and MAX_AUDIO_POOLS_PER_HEAP*/
/*Local heap/pool/buffer book-keeping*/
/*The memory hierarchy is from largest unit to smallest, i.e. from heap to pool to buffer*/
/*This book-keeping is purely for mocking, and as a first-cut version at least, we are not*/
/*guaranteeing thread-safety*/
/*But, every other book-keeping functionality of the heap-pool-buffer is mocked as per*/
/*RDK-E HAL AVBuffer specs.*/
/*The following function's valid-arg-bad-return-code paths are not going to be mocked:*/
/*calculateSHA's good arg-bad return code combination is not going to be mocked because it is not*/
/*clear at this point what an UNSUPPORTED OPERATION means*/
/*Note: All book-keeping-related global variable names start with a 'g'*/
int32_t gsecureBytesUsed = 0, gnonsecureBytesUsed = 0;
int32_t gsecureBytesTotal = SECURE_HEAP_SIZE, gnonsecureBytesTotal = NON_SECURE_HEAP_SIZE;

int32_t gnumberOfSecVideoPoolsUsed = 0, gnumberOfNonsecVideoPoolsUsed = 0;
int32_t gnumberOfSecAudioPoolsUsed = 0, gnumberOfNonsecAudioPoolsUsed = 0;
int32_t gnumberOfSecVideoPoolallocs = 0, gnumberOfNonsecVideoPoolallocs = 0;
int32_t gnumberOfSecAudioPoolallocs = 0, gnumberOfNonsecAudioPoolallocs = 0;
int64_t gAllocHandle = 0;


struct buffElement
{
    bool lastBuff;
    int  size;
    int64_t globalAllocHandle;

};

std::vector<std::vector<buffElement>> buffArray (MAX_POOLS);

struct poolElement
{
    int64_t perPoolBytesUsed;
    int64_t perPoolAllocHandle;
};

std::vector<poolElement> poolBuffArray(MAX_POOLS, {0,0});

void onSpaceAvailableCheck(int size);


::android::binder::Status AVBuffer::getHeapMetrics(bool secureHeap, HeapMetrics *_aidl_return) 
{
     std::cout << "getHeapMetrics called with secureHeap=" << secureHeap << std::endl;
     std::cout << "Standard path - no valid arg error path exists" << std::endl;
	        
     _aidl_return->secure = secureHeap;
     if (secureHeap)
     {
         _aidl_return->bytesUsed = gsecureBytesUsed;
    	 _aidl_return->bytesTotal = SECURE_HEAP_SIZE;
     } 
     else
     {
	_aidl_return->bytesUsed = gnonsecureBytesUsed;
        _aidl_return->bytesTotal = NON_SECURE_HEAP_SIZE;
     }
     std::cout << "gsecureBytesUsed" << gsecureBytesUsed << "," << "gnonsecurebytesUsed" << gnonsecureBytesUsed << "," << "SECURE_HEAP_SIZE" << SECURE_HEAP_SIZE << "," << "NON_SECURE_HEAP_SIZE" <<  NON_SECURE_HEAP_SIZE << std::endl;
	    		
     return ::android::binder::Status::ok();
}


::android::binder::Status AVBuffer::createVideoPool(bool secureHeap, const IVideoDecoder::Id& videoDecoderId, const sp<IAVBufferSpaceListener>& listener, Pool* _aidl_return) 
{
    Pool pool;
    std::cout << "Create video pool service " << videoDecoderId.value << std::endl;


    if ((videoDecoderId.value != ::com::rdk::hal::videodecoder::IVideoDecoder::Id::UNDEFINED) & (videoDecoderId.value < 0))
    {
        std::cout << "ILLEGAL ARGUMENT" << videoDecoderId.value << std::endl;
        return Status::fromExceptionCode(Status::EX_ILLEGAL_ARGUMENT);
    }

    else
    {
	std::cout << "Standard path" << std::endl;
        if (secureHeap)
	{
	    if (((gnumberOfSecVideoPoolsUsed * VIDEO_POOL_SIZE) + (gnumberOfSecAudioPoolsUsed * AUDIO_POOL_SIZE) <= SECURE_HEAP_SIZE) & (gnumberOfSecVideoPoolsUsed < MAX_VIDEO_POOLS_PER_HEAP))
	    {
	        pool.handle = gnumberOfSecVideoPoolsUsed;
		gnumberOfSecVideoPoolsUsed++;
		std::cout << "Creating video pool with pool handle " << pool.handle << std::endl;

	    }
	    else 
	    {
		std::cout << "Out of pools !!! gnumberOfSecVideoPoolsUsed " << gnumberOfSecVideoPoolsUsed << "gnumberOfSecAudioPoolsUsed " << gnumberOfSecAudioPoolsUsed << std::endl;
                return Status::fromExceptionCode(Status::EX_SERVICE_SPECIFIC,"HALError::OUT_OF_MEMORY");

	    }

	}
	else
	{
	    if (((gnumberOfNonsecVideoPoolsUsed * VIDEO_POOL_SIZE) + (gnumberOfNonsecAudioPoolsUsed * AUDIO_POOL_SIZE) <= NON_SECURE_HEAP_SIZE) & (gnumberOfNonsecVideoPoolsUsed < MAX_VIDEO_POOLS_PER_HEAP))
            {
     	        pool.handle = gnumberOfNonsecVideoPoolsUsed + MAX_POOLS_PER_HEAP;
		gnumberOfNonsecVideoPoolsUsed++;
            }
            else
            {
                return Status::fromExceptionCode(Status::EX_SERVICE_SPECIFIC,"HALError::OUT_OF_MEMORY");
            }

	}	
    }
    _spacelistener = listener;

    *_aidl_return = pool;
    return ::android::binder::Status::ok();
}

::android::binder::Status
AVBuffer::createAudioPool(bool secureHeap, const IAudioDecoder::Id &audioDecoderId,
                          const sp <IAVBufferSpaceListener> &listener, Pool *_aidl_return) {
    Pool pool;

    std::cout << "createAudioPool called with secureHeap = " << secureHeap <<  "audioDecoderId= " << audioDecoderId.value << std::endl;
    if ((audioDecoderId.value != ::com::rdk::hal::audiodecoder::IAudioDecoder::Id::UNDEFINED) & (audioDecoderId.value < 0))
    {
        return Status::fromExceptionCode(Status::EX_ILLEGAL_ARGUMENT);
    }
    else
    {
        std::cout << "Standard positive path" << std::endl;
	if (secureHeap)
        {
            if (((gnumberOfSecVideoPoolsUsed * VIDEO_POOL_SIZE) + (gnumberOfSecAudioPoolsUsed * AUDIO_POOL_SIZE) <= SECURE_HEAP_SIZE) &(gnumberOfSecAudioPoolsUsed < MAX_AUDIO_POOLS_PER_HEAP))
            {
                pool.handle = gnumberOfSecAudioPoolsUsed;
		gnumberOfSecAudioPoolsUsed++;
            }
            else
            {
                return Status::fromExceptionCode(Status::EX_SERVICE_SPECIFIC,"HALError::OUT_OF_MEMORY");
            }
        }
        else
        {
	    if (((gnumberOfNonsecVideoPoolsUsed * VIDEO_POOL_SIZE) + (gnumberOfNonsecAudioPoolsUsed * AUDIO_POOL_SIZE) <= NON_SECURE_HEAP_SIZE) & (gnumberOfNonsecAudioPoolsUsed < MAX_AUDIO_POOLS_PER_HEAP))
	    {
		pool.handle = gnumberOfNonsecAudioPoolsUsed + MAX_POOLS_PER_HEAP;
		gnumberOfNonsecAudioPoolsUsed++;
            }
	    else
	    {
		return Status::fromExceptionCode(Status::EX_SERVICE_SPECIFIC,"HALError::OUT_OF_MEMORY");

            }
	}
    }
    *_aidl_return = pool;
    return ::android::binder::Status::ok();
}

::android::binder::Status AVBuffer::destroyPool(const Pool &poolHandle) 
{
    PoolMetrics *_aidl_return;
    ::android::binder::Status status;

    std::cout << "destroyPool called with poolHandle = " << poolHandle.handle << std::endl;
    if (poolHandle.handle == ::com::rdk::hal::avbuffer::Pool::INVALID)
    {
        return Status::fromExceptionCode(Status::EX_ILLEGAL_ARGUMENT);
    }
    
    else
    {
        std::cout << "Standard positive path " << std::endl;
	status = getPoolMetrics(poolHandle, _aidl_return);
	if (_aidl_return->bytesUsed != 0)
	{
	    std::cout << "Non-empty pool!!!!" << std::endl;
	    return Status::fromExceptionCode(Status::EX_SERVICE_SPECIFIC,"HALError::NOT_EMPTY");
	}
	    std::cout << "Pool good, can be emptied! " << std::endl;
	    gnumberOfSecVideoPoolsUsed--;
    }

    return ::android::binder::Status::ok();
}

::android::binder::Status
AVBuffer::getPoolMetrics(const Pool &poolHandle, PoolMetrics *_aidl_return) 
{
    std::cout << "getPoolMetrics called with poolHandle=" << poolHandle.handle << std::endl;
    std::cout << "Standard positive path " << std::endl;
    if (poolHandle.handle == ::com::rdk::hal::avbuffer::Pool::INVALID)
    {
        return Status::fromExceptionCode(Status::EX_ILLEGAL_ARGUMENT);
    }

    _aidl_return->poolHandle = poolHandle;
    if (poolHandle.handle < MAX_POOLS_PER_HEAP)
    {
    	_aidl_return->bytesUsed = poolBuffArray[poolHandle.handle].perPoolBytesUsed;
    	_aidl_return->bytesTotal = POOL_SIZE;
    }
    else
    {
	_aidl_return->bytesUsed = poolBuffArray[poolHandle.handle].perPoolBytesUsed;
        _aidl_return->bytesTotal = POOL_SIZE;

    }
    return ::android::binder::Status::ok();
}

::android::binder::Status
AVBuffer::getAllPoolMetrics(bool secureHeap, std::vector <PoolMetrics> *_aidl_return) 
{
    std::cout << "getAllPoolMetrics called with secureHeap=" << secureHeap << std::endl;
    std::cout << "Standard positive path " << std::endl;
    PoolMetrics metrics;
    for (size_t rowIndex = 0; rowIndex < MAX_POOLS; rowIndex++)
    {
        metrics.bytesUsed = poolBuffArray[rowIndex].perPoolBytesUsed; 
  	metrics.bytesTotal = POOL_SIZE;
	_aidl_return->push_back(metrics); 
    }

    return ::android::binder::Status::ok();
}

::android::binder::Status AVBuffer::alloc(const Pool &poolHandle, int size, int64_t *_aidl_return) 
{
    if(size <= 0 || poolHandle.handle == ::com::rdk::hal::avbuffer::Pool::INVALID)
    {
	    return Status::fromExceptionCode(Status::EX_ILLEGAL_ARGUMENT);
    }
    else
    {
        std::cout << "Alloc with pool handle !!!!!!!!!!!!!!" << poolHandle.handle << std::endl;
	std::cout << "Size of array %d !!!!!!!!!!!!!!!!" <<  buffArray[poolHandle.handle].size() << std::endl;
        std::cout << "Standard positive path " << std::endl;
 	if (poolHandle.handle < MAX_POOLS_PER_HEAP)
	{
	    if (gsecureBytesUsed + size <= POOL_SIZE) 
	    {
                gsecureBytesUsed = gsecureBytesUsed + size;
	        *_aidl_return = gAllocHandle;
		buffArray[poolHandle.handle].push_back({true, size, gAllocHandle});
	        if (buffArray[poolHandle.handle].size() > 1)
		{
		    buffArray[poolHandle.handle][poolBuffArray[poolHandle.handle].perPoolAllocHandle - 1].lastBuff = false;
		}
		std::cout << "Buffer alloc-ed with per-pool handle " << poolBuffArray[poolHandle.handle].perPoolAllocHandle << "gsecureBytesUsed" << gsecureBytesUsed << "size" << size << std::endl;
		poolBuffArray[poolHandle.handle].perPoolBytesUsed = poolBuffArray[poolHandle.handle].perPoolBytesUsed + size;
		poolBuffArray[poolHandle.handle].perPoolAllocHandle++;
		gAllocHandle++;
	    }
	    else
	    {
		return Status::fromExceptionCode(Status::EX_SERVICE_SPECIFIC, "HALError::OUT_OF_MEMORY");
	    }
    	}
    	else
    	{
	    if (gnonsecureBytesUsed + size <= POOL_SIZE)
            {
                gnonsecureBytesUsed = gnonsecureBytesUsed + size;
                *_aidl_return = gAllocHandle;
		buffArray[poolHandle.handle].push_back({true, size, gAllocHandle}); 
                if (buffArray[poolHandle.handle].size() > 1)
                {
                    buffArray[poolHandle.handle][poolBuffArray[poolHandle.handle].perPoolAllocHandle - 1].lastBuff = false;
		}
		    poolBuffArray[poolHandle.handle].perPoolBytesUsed = poolBuffArray[poolHandle.handle].perPoolBytesUsed + size;
                    poolBuffArray[poolHandle.handle].perPoolAllocHandle++;
		    gAllocHandle++;
            }
            else
            {
                return Status::fromExceptionCode(Status::EX_SERVICE_SPECIFIC, "HALError::OUT_OF_MEMORY");
            }
        }
    }
    std::cout << "alloc called with poolHandle= " << poolHandle.handle << "size=" << size << std::endl;
    return ::android::binder::Status::ok();
}

::android::binder::Status AVBuffer::notifyWhenSpaceAvailable(const Pool &poolHandle, int size) 
{
    if (size <= 0)
    	return Status::fromExceptionCode(Status::EX_ILLEGAL_ARGUMENT);
    	
    if (size > (POOL_SIZE - poolBuffArray[poolHandle.handle].perPoolBytesUsed))
    {

	std::cout << "Spawning OnSpaceAvailable thread - waiting for free's " << std::endl;
        std::thread threadonSpaceAvailableCheck(onSpaceAvailableCheck, size);
        threadonSpaceAvailableCheck.detach();
    }
    std::cout << "notifyWhenSpaceAvailable called with poolHandle= " << poolHandle.handle << "size=" << size << std::endl;
    return ::android::binder::Status::ok();
}

void onSpaceAvailableCheck(int size)
{
    while (size > (gsecureBytesTotal - gsecureBytesUsed));
    std::cout << "Got some memory holes, finally ! " << std::endl;
    _spacelistener->onSpaceAvailable();   
}

::android::binder::Status AVBuffer::trimSize(int64_t bufferHandle, int newSize) 
{
    int64_t buffCount = 0;
    std::cout << "trimSize called with bufferHandle= " << bufferHandle << "newSize" << newSize << std::endl;
    if ((bufferHandle < 0) | (newSize <= 0))
    {
        return Status::fromExceptionCode(Status::EX_ILLEGAL_ARGUMENT);
    }
    else
    {
        std::cout << "Standard positive path " << std::endl;
	/*search in 2-d array of both secure, nonsecure heap pools for buffer handle*/
	for (size_t rowIndex = 0; rowIndex < buffArray.size(); rowIndex++) 
	{
	    for (size_t elemIndex = 0; elemIndex < buffArray[rowIndex].size(); elemIndex++)
	    {
	        if (bufferHandle == buffArray[rowIndex][elemIndex].globalAllocHandle)
		{
		    if (buffArray[rowIndex][elemIndex].lastBuff == true)
		    {
		        if (buffArray[rowIndex][elemIndex].size < newSize)
			    return Status::fromExceptionCode(Status::EX_ILLEGAL_ARGUMENT);
			std::cout << "Trimmable buffer found! " << std::endl;
			poolBuffArray[rowIndex].perPoolBytesUsed = poolBuffArray[rowIndex].perPoolBytesUsed - (buffArray[rowIndex][elemIndex].size - newSize);
			buffArray[rowIndex][elemIndex].size = newSize;
			return ::android::binder::Status::ok();
		    }
		}	
	    }
	}
    }
    return Status::fromExceptionCode(Status::EX_ILLEGAL_STATE);
}

::android::binder::Status AVBuffer::free(int64_t bufferHandle)  
{
    if (bufferHandle < 0) 
    {
        return Status::fromExceptionCode(Status::EX_ILLEGAL_ARGUMENT);
    }

    std::cout << "free called with bufferHandle= " << bufferHandle <<std::endl;
    
    for (size_t rowIndex = 0; rowIndex < MAX_POOLS; rowIndex++)
    {
    	for (size_t elemIndex = 0; elemIndex < buffArray[rowIndex].size(); elemIndex++)
        {
            if (bufferHandle == buffArray[rowIndex][elemIndex].globalAllocHandle)
            {
	        std::cout << " Global alloc handle found within pool " << bufferHandle << std::endl;
		if (elemIndex < MAX_POOLS_PER_HEAP)
		{
                    gsecureBytesUsed = gsecureBytesUsed - buffArray[rowIndex][poolBuffArray[rowIndex].perPoolAllocHandle-1].size;
              	}
                else
                    gnonsecureBytesUsed = gnonsecureBytesUsed - buffArray[rowIndex][poolBuffArray[rowIndex].perPoolAllocHandle-1].size;
		poolBuffArray[rowIndex].perPoolBytesUsed = poolBuffArray[rowIndex].perPoolBytesUsed - buffArray[rowIndex][elemIndex].size;
                buffArray[rowIndex][elemIndex].lastBuff == false;
		buffArray[rowIndex][elemIndex].size = 0;
	        buffArray[rowIndex][elemIndex].globalAllocHandle = -1;	
		return ::android::binder::Status::ok();
                } 
	}
    }
    /*ideally should return a different (not EX_ILLEGAL_ARGUMENT)  exception here*/
    return Status::fromExceptionCode(Status::EX_ILLEGAL_ARGUMENT);
}

::android::binder::Status AVBuffer::isValid(int64_t bufferHandle, bool *_aidl_return) 
{
    std::cout << "isValid called with bufferHandle=" << bufferHandle << std::endl;
    std::cout << "Standard positive path " << std::endl;

    if ((bufferHandle > 0) & (bufferHandle <= gAllocHandle))
    /*check if size is 0, and if this is freed*/
    {
	for (size_t rowIndex = 0; rowIndex < MAX_POOLS; rowIndex++)
    	{
	   for (size_t elemIndex = 0; elemIndex < buffArray[rowIndex].size(); elemIndex++)
           {
               if (bufferHandle == buffArray[rowIndex][elemIndex].globalAllocHandle)
               {	
                   std::cout << " Global alloc handle found within pool " << bufferHandle << std::endl;
		   if (buffArray[rowIndex][elemIndex].globalAllocHandle != -1)
		       *_aidl_return = true;	
    		   else 
		       *_aidl_return = false;
	       }
	   }
	}
    }
    else
	*_aidl_return = false;
    return ::android::binder::Status::ok();
}

::android::binder::Status
AVBuffer::getAllocList(const Pool &poolHandle, std::vector <int64_t> *_aidl_return) 
{
    std::cout << "getAllocList called with poolHandle= " << poolHandle.handle << std::endl;
    if(poolHandle.handle == ::com::rdk::hal::avbuffer::Pool::INVALID)
    {
            return Status::fromExceptionCode(Status::EX_ILLEGAL_ARGUMENT);
    }
   
    size_t rowIndex = poolHandle.handle; 

    std::cout << "getAllocList with poolHandle = " << poolHandle.handle << std::endl;
    for (size_t elemIndex = 0; elemIndex < buffArray[rowIndex].size(); elemIndex++)
    {
	if (buffArray[rowIndex][elemIndex].globalAllocHandle != -1)
	{
		_aidl_return->push_back(buffArray[rowIndex][elemIndex].globalAllocHandle);	
	}
    }
    return ::android::binder::Status::ok();
}

::android::binder::Status
AVBuffer::calculateSHA1(int64_t bufferHandle, std::vector <uint8_t> *_aidl_return) {
    std::cout << "calculateSHA1 called with bufferHandle= " << bufferHandle << std::endl;
    if (bufferHandle < 0)
    {
        return Status::fromExceptionCode(Status::EX_ILLEGAL_ARGUMENT);
    }
    else
    {
        std::cout << "DEBUG BUILD - standard positive path " << std::endl;
    }
    _aidl_return->push_back(0);
    return ::android::binder::Status::ok();
}



