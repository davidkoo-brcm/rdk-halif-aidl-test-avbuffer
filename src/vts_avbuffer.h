/**
 *  If not stated otherwise in this file or this component's LICENSE
 *  file the following copyright and licenses apply:
 *
 *  Copyright 2024 RDK Management
 *
 *  Licensed under the Apache License, Version 2.0 (the License);
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an AS IS BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/**
 * @addtogroup HPK Hardware Porting Kit
 * @{
 * @par The Hardware Porting Kit
 * HPK is the next evolution of the well-defined Hardware Abstraction Layer
 * (HAL), but augmented with more comprehensive documentation and test suites
 * that OEM or SOC vendors can use to self-certify their ports before taking
 * them to RDKM for validation or to an operator for final integration and
 * deployment. The Hardware Porting Kit effectively enables an OEM and/or SOC
 * vendor to self-certify their own Video Accelerator devices, with minimal RDKM
 * assistance.
 *
 */

/**
 * @addtogroup AV_BUFFER AV Buffer Module
 * @{
 */

/**
 * @addtogroup AV_BUFFER_HALTEST AV Buffer HAL Tests
 * @{
 */

/**
 * @defgroup AV_BUFFER_HALTEST_HEADER AV Buffer HAL Tests helper functions Header File
 * @{
 * @parblock
 *
 * ### Common functions/macros required for the AV Buffer module :
 *
 * Common functions/macros required for the AV Buffer module .
 *
 * **Pre-Conditions:**  None @n
 * **Dependencies:** None @n
 *
 * @endparblock
 *
 */

/**
 * @file vts_avbuffer.h
 *
 */

#ifndef __VTS_AVBUFFER_H__
#define __VTS_AVBUFFER_H__

#include <binder/Status.h>
#include <chrono>
#include <thread>
#include <stdio.h>
#include <vector>
#include <ut.h>
#include <ut_kvp_profile.h>
#include <ut_log.h>
#include <ut_control_plane.h>
#include <gtest/gtest.h>
#include <binder/IServiceManager.h>
#include <binder/ProcessState.h>
#include <binder/IPCThreadState.h>
#include <binder/Status.h>
#include <utils/String16.h>
#include <optional>
#include <mutex>
#include <cstring>

#include "com/rdk/hal/avbuffer/IAVBuffer.h"
#include "com/rdk/hal/avbuffer/BnAVBufferSpaceListener.h"
#include "com/rdk/hal/audiodecoder/BpAudioDecoderManager.h"
#include "com/rdk/hal/videodecoder/IVideoDecoderManager.h"

#include "IAVBufferHelper.h"

#define MAX_WAIT_MS 10
#define MAX_FINAL_WAIT_MS 5000

using android::sp;
using android::String16;
using android::defaultServiceManager;
using android::binder::Status;

using namespace com::rdk::hal;
using namespace com::rdk::hal::avbuffer;
using namespace com::rdk::hal::videodecoder;
using namespace com::rdk::hal::audiodecoder;


struct avbuffer_info
{
    ::std::vector<int32_t> videoDecoderId;
    ::std::vector<int32_t> audioDecoderId;
    uint32_t nonSecureHeapBytes;
    uint32_t secureHeapBytes;
    bool supportSecure;
};

#define VIDEODECODER_PATH "IVideoDecoderManager.IVideoDecoder"
#define AUDIODECODER_PATH "IAudioDecoderManager.IAudioDecoder"
#define AVBUFFER_SECURESIZE_PATH "IAVBuffer.secureHeapBytes"
#define AVBUFFER_NONSECURESIZE_PATH "IAVBuffer.nonSecureHeapBytes"

extern avbuffer_info gAVBuffer_hfp;
void* vts_kvp_create_instance(char *fileName);
void vts_kvp_destroy_instance(void *instance);
sp<IAVBuffer> vts_getAVBufferService();

UT_status_t vts_avbuffer_parseAudioDecoderHFP();
UT_status_t VTS_AVBuffer_init();

/* Status::Exception */
const static ut_control_keyStringMapping_t statusException_mapTable[] =
{
    {"EX_NONE",                  (int32_t) Status::EX_NONE},
    {"EX_SECURITY",              (int32_t) Status::EX_SECURITY},
    {"EX_BAD_PARCELABLE",        (int32_t) Status::EX_BAD_PARCELABLE},
    {"EX_ILLEGAL_ARGUMENT",      (int32_t) Status::EX_ILLEGAL_ARGUMENT},
    {"EX_NULL_POINTER",          (int32_t) Status::EX_NULL_POINTER},
    {"EX_ILLEGAL_STATE",         (int32_t) Status::EX_ILLEGAL_STATE},
    {"EX_NETWORK_MAIN_THREAD",   (int32_t) Status::EX_NETWORK_MAIN_THREAD},
    {"EX_UNSUPPORTED_OPERATION", (int32_t) Status::EX_UNSUPPORTED_OPERATION},
    {"EX_SERVICE_SPECIFIC",      (int32_t) Status::EX_SERVICE_SPECIFIC},
    {"EX_PARCELABLE",            (int32_t) Status::EX_PARCELABLE},
    {"EX_HAS_REPLY_HEADER",      (int32_t) Status::EX_HAS_REPLY_HEADER},
    {"EX_TRANSACTION_FAILED",    (int32_t) Status::EX_TRANSACTION_FAILED},
    {NULL, -1}
};



/* bool */
const static ut_control_keyStringMapping_t bool_mapTable[] =
{
    {"false", (int32_t) false},
    {"true",  (int32_t) true},
    {NULL, -1}
};

#define AVBUFFER_GETMETRICS(avBufferManager, secureHeap, heapMetrics) \
    do { \
        UT_LOG_INFO("Calling getHeapMetrics(IN:secureHeap:[%s], OUT:heapMetrics[])", \
                    UT_Control_GetMapString(bool_mapTable, secureHeap)); \
        Status status = avBufferManager->getHeapMetrics(secureHeap, &heapMetrics); \
        UT_LOG_INFO("Result getHeapMetrics(IN:secureHeap:[%s], OUT:heapMetrics[secure:%s, bytesUsed:%d, bytesTotal:%d]), status:[isOK:%s, exception:%s]", \
                    UT_Control_GetMapString(bool_mapTable, secureHeap), \
                    UT_Control_GetMapString(bool_mapTable, heapMetrics.secure), \
                    heapMetrics.bytesUsed, \
                    heapMetrics.bytesTotal, \
                    UT_Control_GetMapString(bool_mapTable, status.isOk()), \
                    UT_Control_GetMapString(statusException_mapTable, status.exceptionCode())); \
        UT_ASSERT_TRUE(status.isOk()); \
        UT_ASSERT_EQUAL(status.exceptionCode(), Status::EX_NONE); \
    } while (0)

#define AVBUFFER_CREATE_VIDEOPOOL_SUCCESS(avBufferManager, secureHeap, videoDecoderId, listener, videoPool) \
do { \
    UT_LOG_INFO("Calling createVideoPool(IN:secureHeap:[%d], IN:videoDecoderId:[%d], IN:listener:[%p], OUT:videoPool)", \
                 secureHeap, videoDecoderId.value, listener.get()); \
    Status status = avBufferManager->createVideoPool(secureHeap, videoDecoderId, listener, &videoPool); \
    UT_LOG_INFO("Result createVideoPool(IN:secureHeap:[%d], IN:videoDecoderId:[%d], IN:listener:[%p], OUT:videoPool.handle:[%d]), status:[isOK:%s, exception:%s]", \
                 secureHeap, videoDecoderId.value, listener.get(), videoPool.handle, \
                 UT_Control_GetMapString(bool_mapTable, status.isOk()), UT_Control_GetMapString(statusException_mapTable, status.exceptionCode())); \
    UT_ASSERT_TRUE(status.isOk()); \
    UT_ASSERT_NOT_EQUAL(videoPool.handle, Pool::INVALID); \
    UT_ASSERT_EQUAL(status.exceptionCode(), Status::EX_NONE); \
} while (0)

#define AVBUFFER_CREATE_VIDEOPOOL_FAILURE(avBufferManager, secureHeap, videoDecoderId, listener, videoPool, exception) \
    do { \
        UT_LOG_INFO("Calling createVideoPool(IN:secureHeap:[%d], IN:videoDecoderId:[%d], IN:listener:[%p], OUT:videoPool)", \
            secureHeap, videoDecoderId.value, listener.get()); \
        Status status = avBufferManager->createVideoPool(secureHeap, videoDecoderId, listener, &videoPool); \
        UT_LOG_INFO("Result createVideoPool(IN:secureHeap:[%d], IN:videoDecoderId:[%d], IN:listener:[%p], OUT:videoPool.handle:[%d]), status:[isOK:%s, exception:%s]", \
                    secureHeap, videoDecoderId.value, listener.get(), videoPool.handle, \
                    UT_Control_GetMapString(bool_mapTable, status.isOk()), UT_Control_GetMapString(statusException_mapTable, status.exceptionCode())); \
        UT_ASSERT_FALSE(status.isOk()); \
        UT_ASSERT_EQUAL(status.exceptionCode(), exception); \
    } while (0)

#define AVBUFFER_CREATE_AUDIOPOOL_SUCCESS(avBufferManager, secureHeap, audioDecoderId, listener, audioPool) \
    do { \
        UT_LOG_INFO("Calling createAudioPool(IN:secureHeap:[%d], IN:audioDecoderId:[%d], IN:listener:[%p], OUT:audioPool)", \
                    secureHeap, audioDecoderId.value, listener.get()); \
        Status status = avBufferManager->createAudioPool(secureHeap, audioDecoderId, listener, &audioPool); \
        UT_LOG_INFO("Result createAudioPool(IN:secureHeap:[%d], IN:audioDecoderId:[%d], IN:listener:[%p], OUT:audioPool.handle:[%d]), status:[isOK:%s, exception:%s]", \
                    secureHeap, audioDecoderId.value, listener.get(), audioPool.handle, \
                    UT_Control_GetMapString(bool_mapTable, status.isOk()), UT_Control_GetMapString(statusException_mapTable, status.exceptionCode())); \
        UT_ASSERT_TRUE(status.isOk()); \
        UT_ASSERT_NOT_EQUAL(audioPool.handle, Pool::INVALID); \
        UT_ASSERT_EQUAL(status.exceptionCode(), Status::EX_NONE); \
    } while (0)

#define AVBUFFER_CREATE_AUDIOPOOL_FAILURE(avBufferManager, secureHeap, audioDecoderId, listener, audioPool, exception) \
    do { \
        UT_LOG_INFO("Calling createAudioPool(IN:secureHeap:[%d], IN:audioDecoderId:[%d], IN:listener:[%p], OUT:audioPool)", \
                    secureHeap, audioDecoderId.value, listener.get()); \
        Status status = avBufferManager->createAudioPool(secureHeap, audioDecoderId, listener, &audioPool); \
        UT_LOG_INFO("Result createAudioPool(IN:secureHeap:[%d], IN:audioDecoderId:[%d], IN:listener:[%p], OUT:audioPool.handle:[%d]), status:[isOK:%s, exception:%s]", \
                    secureHeap, audioDecoderId.value, listener.get(), audioPool.handle, \
                    UT_Control_GetMapString(bool_mapTable, status.isOk()), UT_Control_GetMapString(statusException_mapTable, status.exceptionCode())); \
        UT_ASSERT_FALSE(status.isOk()); \
        UT_ASSERT_EQUAL(status.exceptionCode(), exception); \
    } while (0)

#define AVBUFFER_DESTROY_POOL_SUCCESS(avBufferManager, pool) \
    do { \
        bool _aidl_return = false; \
        Status status; \
        UT_LOG_INFO("Calling destroyPool(IN:poolHandle:[%d], OUT:_aidl_return:[])", pool); \
        status = avBufferManager->destroyPool(pool, &_aidl_return); \
        UT_LOG_INFO("Result destroyPool with (IN:poolHandle:[%d], OUT:_aidl_return:[%s]), status:[isOK:%s, exception:%s]", \
                    pool.handle, UT_Control_GetMapString(bool_mapTable, _aidl_return), \
                    UT_Control_GetMapString(bool_mapTable, status.isOk()), \
                    UT_Control_GetMapString(statusException_mapTable, status.exceptionCode())); \
        UT_ASSERT_TRUE(status.isOk()); \
        UT_ASSERT_TRUE(_aidl_return); \
        UT_ASSERT_EQUAL(status.exceptionCode(), Status::EX_NONE); \
    } while (0)

#define AVBUFFER_DESTROY_POOL_FAILURE(avBufferManager, pool, exception) \
    do { \
        bool _aidl_return = false; \
        Status status; \
        UT_LOG_INFO("Calling destroyPool(IN:poolHandle:[%d], OUT:_aidl_return:[])", pool.handle); \
        status = avBufferManager->destroyPool(pool, &_aidl_return); \
        UT_LOG_INFO("Result destroyPool with (IN:poolHandle:[%d], OUT:_aidl_return:[%s]), status:[isOK:%s, exception:%s]", \
                    pool.handle, UT_Control_GetMapString(bool_mapTable, _aidl_return), \
                    UT_Control_GetMapString(bool_mapTable, status.isOk()), \
                    UT_Control_GetMapString(statusException_mapTable, status.exceptionCode())); \
        UT_ASSERT_FALSE(_aidl_return); \
        UT_ASSERT_EQUAL(status.exceptionCode(), exception); \
    } while (0)

#define AVBUFFER_GETPOOL_METRICS_SUCCESS(avBufferManager, pool, poolMetrics) \
    do { \
        UT_LOG_INFO("Calling getPoolMetrics(IN:poolHandle:[%d], OUT:PoolMetrics[])", pool.handle); \
        Status status = avBufferManager->getPoolMetrics(pool, &poolMetrics); \
        UT_LOG_INFO("Result getPoolMetrics(IN:poolHandle:[%d], OUT:PoolMetrics[bytesUsed:%d, bytesTotal:%d]), status:[isOK:%s, exception:%s]", \
                    pool.handle, poolMetrics.bytesUsed, poolMetrics.bytesTotal, \
                    UT_Control_GetMapString(bool_mapTable, status.isOk()), UT_Control_GetMapString(statusException_mapTable, status.exceptionCode())); \
        UT_ASSERT_TRUE(status.isOk()); \
        UT_ASSERT_EQUAL(status.exceptionCode(), Status::EX_NONE); \
    } while (0)

#define AVBUFFER_GETPOOL_METRICS_FAILURE(avBufferManager, pool, poolMetrics, exception) \
    do { \
        UT_LOG_INFO("Calling getPoolMetrics(IN:poolHandle:[%d], OUT:PoolMetrics[])", pool.handle); \
        Status status = avBufferManager->getPoolMetrics(pool, &poolMetrics); \
        UT_LOG_INFO("Result getPoolMetrics(IN:poolHandle:[%d], OUT:PoolMetrics[bytesUsed:%d, bytesTotal:%d]), status:[isOK:%s, exception:%s]", \
                    pool.handle, poolMetrics.bytesUsed, poolMetrics.bytesTotal, \
                    UT_Control_GetMapString(bool_mapTable, status.isOk()), UT_Control_GetMapString(statusException_mapTable, status.exceptionCode())); \
        UT_ASSERT_FALSE(status.isOk()); \
        UT_ASSERT_EQUAL(status.exceptionCode(), exception); \
    } while (0)

#define AVBUFFER_GETALLPOOL_METRICS_SUCCESS(avBufferManager, secureHeap, poolMetrics) \
    do { \
        UT_LOG_INFO("Calling getAllPoolMetrics(IN:secureHeap:[%s], OUT:poolMetrics[])", UT_Control_GetMapString(bool_mapTable, secureHeap)); \
        Status status = avBufferManager->getAllPoolMetrics(secureHeap, &poolMetrics); \
        UT_LOG_INFO("Result getAllPoolMetrics(IN:secureHeap:[%s], OUT:poolMetrics[%zu]), status:[isOK:%s, exception:%s]", \
            UT_Control_GetMapString(bool_mapTable, secureHeap), poolMetrics.size(), UT_Control_GetMapString(bool_mapTable, status.isOk()), \
                     UT_Control_GetMapString(statusException_mapTable, status.exceptionCode())); \
        UT_ASSERT_TRUE(status.isOk()); \
        UT_ASSERT_EQUAL(status.exceptionCode(), Status::EX_NONE); \
    } while (0)

#define AVBUFFER_ALLOC_SUCCESS(avBufferManager, pool, size, bufferHandle) \
    do { \
        UT_LOG_INFO("Calling alloc(IN:poolHandle:[%d], IN:size:[%d], OUT:bufferHandle)", pool.handle, size); \
        Status status = avBufferManager->alloc(pool, size, &bufferHandle); \
        UT_LOG_INFO("Result alloc(IN:poolHandle:[%d], IN:size:[%d], OUT:bufferHandle:[0x%X]), status:[isOK:%s, exception:%s]", \
                    pool.handle, size, bufferHandle, UT_Control_GetMapString(bool_mapTable, status.isOk()), \
                    UT_Control_GetMapString(statusException_mapTable, status.exceptionCode())); \
        UT_ASSERT_TRUE(status.isOk()); \
        UT_ASSERT_NOT_EQUAL(bufferHandle, IAVBuffer::INVALID_HANDLE); \
        UT_ASSERT_EQUAL(status.exceptionCode(), Status::EX_NONE); \
    } while (0)

#define AVBUFFER_ALLOC_FAILURE(avBufferManager, pool, size, bufferHandle, exception) \
    do { \
        UT_LOG_INFO("Calling alloc(IN:poolHandle:[%d], IN:size:[%d], OUT:bufferHandle)", pool.handle, size); \
        Status status = avBufferManager->alloc(pool, size, &bufferHandle); \
        UT_LOG_INFO("Result alloc(IN:poolHandle:[%d], IN:size:[%d], OUT:bufferHandle:[0x%08X]), status:[isOK:%s, exception:%s]", \
                    pool.handle, size, bufferHandle, UT_Control_GetMapString(bool_mapTable, status.isOk()), \
                    UT_Control_GetMapString(statusException_mapTable, status.exceptionCode())); \
        UT_ASSERT_EQUAL(bufferHandle, IAVBuffer::INVALID_HANDLE); \
        UT_ASSERT_EQUAL(status.exceptionCode(), exception); \
    } while (0)

#define AVBUFFER_FREE_SUCCESS(avBufferManager, bufferHandle) \
    do { \
        bool _aidl_return = false; \
        UT_LOG_INFO("Calling free(IN:bufferHandle:[0x%X], OUT:_aidl_return)", bufferHandle); \
        Status status = avBufferManager->free(bufferHandle, &_aidl_return); \
        UT_LOG_INFO("Result free(IN:bufferHandle:[0x%X], OUT:_aidl_return:[%s]), status:[isOK:%s, exception:%s]", \
                    bufferHandle, UT_Control_GetMapString(bool_mapTable, _aidl_return), \
                    UT_Control_GetMapString(bool_mapTable, status.isOk()), \
                    UT_Control_GetMapString(statusException_mapTable, status.exceptionCode())); \
        UT_ASSERT_TRUE(status.isOk()); \
        UT_ASSERT_EQUAL(status.exceptionCode(), Status::EX_NONE); \
        UT_ASSERT_TRUE(_aidl_return); \
    } while (0)

#define AVBUFFER_FREE_FAILURE(avBufferManager, bufferHandle, exception) \
    do { \
        bool _aidl_return = false; \
        UT_LOG_INFO("Calling free(IN:bufferHandle:[0x%08X], OUT:_aidl_return)", bufferHandle); \
        Status status = avBufferManager->free(bufferHandle, &_aidl_return); \
        UT_LOG_INFO("Result free(IN:bufferHandle:[0x%08X], OUT:_aidl_return:[%s]), status:[isOK:%s, exception:%s]", \
                    bufferHandle, UT_Control_GetMapString(bool_mapTable, _aidl_return), \
                    UT_Control_GetMapString(bool_mapTable, status.isOk()), \
                    UT_Control_GetMapString(statusException_mapTable, status.exceptionCode())); \
        UT_ASSERT_TRUE(status.isOk()); \
        UT_ASSERT_EQUAL(status.exceptionCode(), exception); \
        UT_ASSERT_FALSE(_aidl_return); \
    } while (0)

    #define AVBUFFER_NOTIFYSPACE_SUCCESS(avBufferManager, pool, size) \
    do { \
        bool _aidl_return = false; \
        UT_LOG_INFO("Calling notifyWhenSpaceAvailable(IN:poolHandle:[%d], IN:size:[%d], OUT:_aidl_return)", pool.handle, size); \
        Status status = avBufferManager->notifyWhenSpaceAvailable(pool, size, &_aidl_return); \
        UT_LOG_INFO("Result notifyWhenSpaceAvailable(IN:poolHandle:[%d], IN:size:[%d], OUT:_aidl_return:[%s]), status:[isOK:%s, exception:%s]", \
                    pool.handle, size, UT_Control_GetMapString(bool_mapTable, _aidl_return), \
                    UT_Control_GetMapString(bool_mapTable, status.isOk()), \
                    UT_Control_GetMapString(statusException_mapTable, status.exceptionCode())); \
        UT_ASSERT_TRUE(status.isOk()); \
        UT_ASSERT_EQUAL(status.exceptionCode(), Status::EX_NONE); \
        UT_ASSERT_TRUE(_aidl_return); \
    } while (0)

#define AVBUFFER_NOTIFYSPACE_FAILURE(avBufferManager, pool, size, exception) \
    do { \
        bool _aidl_return = false; \
        UT_LOG_INFO("Calling notifyWhenSpaceAvailable(IN:poolHandle:[%d], IN:size:[%d], OUT:_aidl_return)", pool.handle, size); \
        Status status = avBufferManager->notifyWhenSpaceAvailable(pool, size, &_aidl_return); \
        UT_LOG_INFO("Result notifyWhenSpaceAvailable(IN:poolHandle:[%d], IN:size:[%d], OUT:_aidl_return:[%s]), status:[isOK:%s, exception:%s]", \
                    pool.handle, size, UT_Control_GetMapString(bool_mapTable, _aidl_return), \
                    UT_Control_GetMapString(bool_mapTable, status.isOk()), \
                    UT_Control_GetMapString(statusException_mapTable, status.exceptionCode())); \
        UT_ASSERT_FALSE(status.isOk()); \
        UT_ASSERT_EQUAL(status.exceptionCode(), exception); \
        UT_ASSERT_FALSE(_aidl_return); \
    } while (0)

#define AVBUFFER_TRIMSIZE_SUCCESS(avBufferManager, bufferHandle, newSize) \
    do { \
        bool _aidl_return = false; \
        UT_LOG_INFO("Calling trimSize with video buffer handle(IN:bufferHandle:[0x%08X], IN:newSize:[%d], OUT:_aidl_return)", bufferHandle, newSize); \
        Status status = avBufferManager->trimSize(bufferHandle, newSize, &_aidl_return); \
        UT_LOG_INFO("Result trimSize(IN:bufferHandle:[0x%08X], IN:newSize:[%d], OUT:_aidl_return:[%s]), status:[isOK:%s, exception:%s]", \
                    bufferHandle, newSize, UT_Control_GetMapString(bool_mapTable, _aidl_return), \
                    UT_Control_GetMapString(bool_mapTable, status.isOk()), UT_Control_GetMapString(statusException_mapTable, status.exceptionCode())); \
        UT_ASSERT_TRUE(status.isOk()); \
        UT_ASSERT_EQUAL(status.exceptionCode(), Status::EX_NONE); \
        UT_ASSERT_TRUE(_aidl_return); \
    } while (0)

#define AVBUFFER_TRIMSIZE_FAILURE(avBufferManager, bufferHandle, newSize, exception) \
    do { \
        bool _aidl_return = false; \
        UT_LOG_INFO("Calling trimSize with video buffer handle(IN:bufferHandle:[%lld], IN:newSize:[%d], OUT:_aidl_return)", bufferHandle, newSize); \
        Status status = avBufferManager->trimSize(bufferHandle, newSize, &_aidl_return); \
        UT_LOG_INFO("Result trimSize(IN:bufferHandle:[%lld], IN:newSize:[%d], OUT:_aidl_return:[%s]), status:[isOK:%s, exception:%s]", \
                    bufferHandle, newSize, UT_Control_GetMapString(bool_mapTable, _aidl_return), \
                    UT_Control_GetMapString(bool_mapTable, status.isOk()), UT_Control_GetMapString(statusException_mapTable, status.exceptionCode())); \
        UT_ASSERT_EQUAL(status.exceptionCode(), exception); \
        UT_ASSERT_FALSE(_aidl_return); \
    } while (0)

#define AVBUFFER_ISVALID_SUCCESS(avBufferManager, bufferHandle, isValidResult) \
    do { \
        UT_LOG_INFO("Calling isValid(IN:bufferHandle:[0X%08X], OUT:_aidl_return[])", bufferHandle); \
        Status status = avBufferManager->isValid(bufferHandle, &isValidResult); \
        UT_LOG_INFO("Result isValid(IN:bufferHandle:[0X%08X], OUT:_aidl_return:[%s]), status:[isOK:%s, exception:%s]", \
                    bufferHandle, UT_Control_GetMapString(bool_mapTable, isValidResult), \
                    UT_Control_GetMapString(bool_mapTable, status.isOk()), \
                    UT_Control_GetMapString(statusException_mapTable, status.exceptionCode())); \
        UT_ASSERT_TRUE(status.isOk()); \
        UT_ASSERT_EQUAL(status.exceptionCode(), Status::EX_NONE); \
    } while (0)

#define AVBUFFER_GETALLOCLIST_SUCCESS(avBufferManager, poolHandle, allocList) \
    do { \
        UT_LOG_INFO("Calling getAllocList with video pool handle (IN:poolHandle:[%d], OUT:allocList[])", poolHandle.handle); \
        Status status = avBufferManager->getAllocList(poolHandle, &allocList); \
        UT_LOG_INFO("Result getAllocList(IN:poolHandle:[%d], OUT:allocList[size:%zu]), status:[isOK:%s, exception:%s]", \
                    poolHandle.handle, allocList.size(), \
                    UT_Control_GetMapString(bool_mapTable, status.isOk()), \
                    UT_Control_GetMapString(statusException_mapTable, status.exceptionCode())); \
        UT_ASSERT_TRUE(status.isOk()); \
        UT_ASSERT_EQUAL(status.exceptionCode(), Status::EX_NONE); \
    } while (0)

#define AVBUFFER_GETALLOCLIST_FAILURE(avBufferManager, poolHandle, allocList, exception) \
    do { \
        UT_LOG_INFO("Calling getAllocList with video pool handle (IN:poolHandle:[%d], OUT:allocList[])", poolHandle.handle); \
        Status status = avBufferManager->getAllocList(poolHandle, &allocList); \
        UT_LOG_INFO("Result getAllocList(IN:poolHandle:[%d], OUT:allocList[size:%zu]), status:[isOK:%s, exception:%s]", \
                    poolHandle.handle, allocList.size(), \
                    UT_Control_GetMapString(bool_mapTable, status.isOk()), \
                    UT_Control_GetMapString(statusException_mapTable, status.exceptionCode())); \
        UT_ASSERT_FALSE(status.isOk()); \
        UT_ASSERT_EQUAL(status.exceptionCode(), exception); \
        UT_ASSERT_TRUE(allocList.empty()); \
    } while (0)

#define AVBUFFER_CALCULATESHA_SUCCESS(avBufferManager, poolHandle, sha1Result) \
    do { \
        UT_LOG_INFO("Calling calculateSHA1 with video buffer handle (IN:bufferHandle:[%lld], OUT:sha1Result[])", poolHandle); \
        Status status = avBufferManager->calculateSHA1(poolHandle, &sha1Result); \
        UT_LOG_INFO("Result calculateSHA1(IN:bufferHandle:[%lld], OUT:sha1Result[size:%zu]), status:[isOK:%s, exception:%s]", \
                    poolHandle, sha1Result.size(), UT_Control_GetMapString(bool_mapTable, status.isOk()), \
                    UT_Control_GetMapString(statusException_mapTable, status.exceptionCode())); \
        if (status.isOk()) { \
            UT_ASSERT_TRUE(status.isOk()); \
            UT_ASSERT_EQUAL(status.exceptionCode(), Status::EX_NONE); \
            UT_ASSERT_FALSE(sha1Result.empty()); \
        } else { \
            UT_ASSERT_EQUAL(status.exceptionCode(), Status::EX_UNSUPPORTED_OPERATION); \
            UT_ASSERT_TRUE(sha1Result.empty()); \
        } \
    } while (0)

#define AVBUFFER_CALCULATESHA_FAILURE(avBufferManager, poolHandle, sha1Result, exception) \
    do { \
        UT_LOG_INFO("Calling calculateSHA1 with video buffer handle (IN:bufferHandle:[%lld], OUT:sha1Result[])", poolHandle); \
        Status status = avBufferManager->calculateSHA1(poolHandle, &sha1Result); \
        UT_LOG_INFO("Result calculateSHA1(IN:bufferHandle:[%lld], OUT:sha1Result[size:%zu]), status:[isOK:%s, exception:%s]", \
                    poolHandle, sha1Result.size(), UT_Control_GetMapString(bool_mapTable, status.isOk()), \
                    UT_Control_GetMapString(statusException_mapTable, status.exceptionCode())); \
        if(status.exceptionCode() != Status::EX_UNSUPPORTED_OPERATION) { \
            UT_ASSERT_FALSE(status.isOk()); \
            UT_ASSERT_EQUAL(status.exceptionCode(), exception); \
            UT_ASSERT_TRUE(sha1Result.empty()); \
        } \
    } while (0)

#define AVBUFFER_WAIT_FOR_SPACEAVAILABLE(avBufferManager, listener, isSpaceAvailable) \
    int32_t sleep_time = 0; \
    do{\
        do \
        { \
            UT_LOG_INFO("WAIT Started"); \
            std::this_thread::sleep_for(std::chrono::milliseconds(MAX_WAIT_MS)); \
            sleep_time += MAX_WAIT_MS; \
            /* Check the state of the decoder */ \
            isSpaceAvailable = listener->getIsSpaceAvailable(sleep_time); \
            UT_LOG_INFO("WAIT loop"); \
        } while (sleep_time < MAX_FINAL_WAIT_MS && isSpaceAvailable != true);\
    }while(0);

#define AUDIODECODER_MANAGER_GETAUDIODECODERIDS(decoderIds) \
    do { \
        ::android::binder::Status status; \
        UT_LOG_INFO("Calling getAudioDecoderIds(OUT:decoderIds:[])"); \
        status = sAudioDecoderManager->getAudioDecoderIds(&decoderIds); \
        UT_LOG_INFO("Result getAudioDecoderIds(OUT:decoderIds:[size: %d]), status:[isOK:%s, exception:%s]", \
                                decoderIds.size(), UT_Control_GetMapString(bool_mapTable, status.isOk()), \
                                UT_Control_GetMapString(statusException_mapTable, status.exceptionCode())); \
        UT_ASSERT_TRUE(status.isOk()); \
        UT_ASSERT_FALSE(decoderIds.empty()); \
        UT_ASSERT_EQUAL(status.exceptionCode(), Status::EX_NONE); \
    } while (0)

#define VIDEODECODER_MANAGER_GETVIDEODECODERIDS(decoderIds) \
    do { \
        ::android::binder::Status status; \
        UT_LOG_INFO("Calling getVideoDecoderIds(OUT:decoderIds:[])"); \
        status = sVideoDecoderManager->getVideoDecoderIds(&decoderIds); \
        UT_LOG_INFO("Result getVideoDecoderIds(OUT:decoderIds:[size: %d]), status:[isOK:%s, exception:%s]", \
                                decoderIds.size(), UT_Control_GetMapString(bool_mapTable, status.isOk()), \
                                UT_Control_GetMapString(statusException_mapTable, status.exceptionCode())); \
        UT_ASSERT_TRUE(status.isOk()); \
        UT_ASSERT_FALSE(decoderIds.empty()); \
        UT_ASSERT_EQUAL(status.exceptionCode(), Status::EX_NONE); \
    } while (0)

#endif //__VTS_AVBUFFER_H__

/** @} */ // End of AV_BUFFER_HALTEST_HEADER
/** @} */ // End of AV_BUFFER_HALTEST
/** @} */ // End of AV_BUFFER
/** @} */ // End of HPK