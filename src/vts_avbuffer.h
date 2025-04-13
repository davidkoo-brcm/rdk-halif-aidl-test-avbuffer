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

#define AVBUFFER_GETMETRICS(avBufferManager, secureHeap, heapMetrics) \
    do { \
        UT_LOG_INFO("Calling getHeapMetrics(IN:secureHeap:[%s], OUT:heapMetrics[])", \
                    UT_Control_GetMapString(bool_mapTable, secureHeap)); \
        android::binder::Status status = avBufferManager->getHeapMetrics(secureHeap, &heapMetrics); \
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
        UT_ASSERT_EQUAL(videoPool.handle, Pool::INVALID); \
    } while (0)

#define AVBUFFER_CREATE_AUDIOPOOL_SUCCESS(avBufferManager, secureHeap, audioDecoderId, listener, audioPool) \
    do { \
        UT_LOG_INFO("Calling createAudioPool(IN:secureHeap:[%d], IN:audioDecoderId:[%d], IN:listener:[%p], OUT:audioPool)", \
                    secureHeap, audioDecoderId.value, listener.get()); \
        Status status = gAVBufferManager->createAudioPool(secureHeap, audioDecoderId, listener, &audioPool); \
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
        Status status = gAVBufferManager->createAudioPool(secureHeap, audioDecoderId, listener, &audioPool); \
        UT_LOG_INFO("Result createAudioPool(IN:secureHeap:[%d], IN:audioDecoderId:[%d], IN:listener:[%p], OUT:audioPool.handle:[%d]), status:[isOK:%s, exception:%s]", \
                    secureHeap, audioDecoderId.value, listener.get(), audioPool.handle, \
                    UT_Control_GetMapString(bool_mapTable, status.isOk()), UT_Control_GetMapString(statusException_mapTable, status.exceptionCode())); \
        UT_ASSERT_FALSE(status.isOk()); \
        UT_ASSERT_EQUAL(status.exceptionCode(), exception); \
        UT_ASSERT_EQUAL(audioPool.handle, Pool::INVALID); \
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
        UT_LOG_INFO("Calling destroyPool(IN:poolHandle:[%d], OUT:_aidl_return:[])", pool); \
        status = avBufferManager->destroyPool(pool, &_aidl_return); \
        UT_LOG_INFO("Result destroyPool with (IN:poolHandle:[%d], OUT:_aidl_return:[%s]), status:[isOK:%s, exception:%s]", \
                    pool.handle, UT_Control_GetMapString(bool_mapTable, _aidl_return), \
                    UT_Control_GetMapString(bool_mapTable, status.isOk()), \
                    UT_Control_GetMapString(statusException_mapTable, status.exceptionCode())); \
        UT_ASSERT_FALSE(_aidl_return); \
        UT_ASSERT_FALSE(status.isOk()); \
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
        ::android::binder::Status status = avBufferManager->getAllPoolMetrics(secureHeap, &poolMetrics); \
        UT_LOG_INFO("Result getAllPoolMetrics(IN:secureHeap:[false], OUT:poolMetrics[%zu]), status:[isOK:%s, exception:%s]", \
                     poolMetrics.size(), UT_Control_GetMapString(bool_mapTable, status.isOk()), \
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
        UT_LOG_INFO("Result alloc(IN:poolHandle:[%d], IN:size:[%d], OUT:bufferHandle:[0x%X]), status:[isOK:%s, exception:%s]", \
                    pool.handle, size, bufferHandle, UT_Control_GetMapString(bool_mapTable, status.isOk()), \
                    UT_Control_GetMapString(statusException_mapTable, status.exceptionCode())); \
        UT_ASSERT_FALSE(status.isOk()); \
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
        UT_LOG_INFO("Calling free(IN:bufferHandle:[0x%X], OUT:_aidl_return)", bufferHandle); \
        Status status = avBufferManager->free(bufferHandle, &_aidl_return); \
        UT_LOG_INFO("Result free(IN:bufferHandle:[0x%X], OUT:_aidl_return:[%s]), status:[isOK:%s, exception:%s]", \
                    bufferHandle, UT_Control_GetMapString(bool_mapTable, _aidl_return), \
                    UT_Control_GetMapString(bool_mapTable, status.isOk()), \
                    UT_Control_GetMapString(statusException_mapTable, status.exceptionCode())); \
        UT_ASSERT_FALSE(status.isOk()); \
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

#endif //__VTS_AVBUFFER_H__

/** @} */ // End of AV_BUFFER_HALTEST_HEADER
/** @} */ // End of AV_BUFFER_HALTEST
/** @} */ // End of AV_BUFFER
/** @} */ // End of HPK