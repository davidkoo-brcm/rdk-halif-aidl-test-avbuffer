
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

#include <ut_kvp_profile.h>
#include <ut.h>
#include <ut_log.h>
#include <ut_control_plane.h>
#include <gtest/gtest.h>
#include <binder/IServiceManager.h>
#include <binder/ProcessState.h>
#include <binder/IPCThreadState.h>
#include <binder/Status.h>
#include <utils/String16.h>
#include <stdio.h>
#include <vector>
#include <optional>
#include <cstring>

#include "com/rdk/hal/avbuffer/IAVBuffer.h"
#include "com/rdk/hal/avbuffer/BnAVBufferSpaceListener.h"
#include "com/rdk/hal/audiodecoder/BpAudioDecoderManager.h"
#include "com/rdk/hal/videodecoder/IVideoDecoderManager.h"

#include "vts_avbuffer_init.h"
#include "vts_avbuffer_parse_hfp.h"
#include "vts_avbuffer.h"

#define MAX_DECODER_COUNT 5

using android::sp;
using android::String16;
using android::defaultServiceManager;
using android::binder::Status;

using namespace com::rdk::hal;
using namespace com::rdk::hal::avbuffer;
using namespace com::rdk::hal::videodecoder;
using namespace com::rdk::hal::audiodecoder;

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

const uint8_t standard_data[256] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
    0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
    0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
    0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
    0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
    0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
    0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
    0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
    0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
    0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
    0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7,
    0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
    0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7,
    0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
    0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7,
    0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
    0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7,
    0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
    0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7,
    0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
    0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7,
    0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF
};
const uint8_t standard_data_sha1[20] = {
    0x6b, 0x86, 0xb2, 0x0b, 0x85, 0xc5, 0x04, 0x03,
    0x14, 0x3e, 0x0a, 0x76, 0x51, 0xd1, 0xe6, 0x7a,
    0x2f, 0x0d, 0x29, 0x16
};
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

class AVBufferSpaceListener : public BnAVBufferSpaceListener
{
private:
        bool isSpaceAvailable = false;

public:

    AVBufferSpaceListener() {};
    virtual ~AVBufferSpaceListener() = default;

    ::android::binder::Status onSpaceAvailable() override
    {
        isSpaceAvailable = true;
        return ::android::binder::Status::ok();
    }

    bool getIsSpaceAvailable()
    {
        if(isSpaceAvailable)
        {
            isSpaceAvailable = false;
            return true;
        }
        return false;
    }
};

extern sp<IAVBuffer> gAVBufferManager;
static sp<IAudioDecoderManager> sAudioDecoderManager;
static HeapMetrics sHeapMetrics;
static std::vector<IAudioDecoder::Id> sAudioDecoderIds;
static std::vector<IVideoDecoder::Id> sVideoDecoderIds;
static std::vector<sp<AVBufferSpaceListener>> sSpaceListeners;
static std::vector<int> sBufferHandles;
static std::vector<Pool> sPoolsCreated;
static sp<IVideoDecoderManager> sVideoDecoderManager;

static bool sSecureHeap;
static bool sAllocateTest;
static bool sNotifyTest;
static bool sTrimTest;
static bool sReadWriteTest;
static bool sSHA1Test;



UT_status_t vts_audiodecoder_getService()
{
    sAudioDecoderManager = android::interface_cast<IAudioDecoderManager>(
            defaultServiceManager()->getService(String16(IAudioDecoderManager::serviceName().c_str())));
    if (sAudioDecoderManager == nullptr)
    {
        return UT_STATUS_FAILURE;
    }
    return UT_STATUS_OK;
}

UT_status_t vts_videodecoder_getService()
{
    sVideoDecoderManager =
        android::interface_cast<IVideoDecoderManager>(
            android::defaultServiceManager()->getService(
                android::String16(IVideoDecoderManager::serviceName().c_str())
            )
        );
    if (sVideoDecoderManager == nullptr)
    {
        return UT_STATUS_FAILURE;
    }
    return UT_STATUS_OK;
}


class VTS_L3_AVBuffer : public virtual UTCore
{
    protected:
        static void SetUpTestSuite()
        {
            UT_ASSERT_EQUAL(vts_audiodecoder_getService(),UT_STATUS_OK);
            AUDIODECODER_MANAGER_GETAUDIODECODERIDS(sAudioDecoderIds);
            UT_ASSERT_EQUAL(vts_videodecoder_getService(),UT_STATUS_OK);
            VIDEODECODER_MANAGER_GETVIDEODECODERIDS(sVideoDecoderIds);
        }

        static void TearDownTestSuite()
        {
        }
    public:
        void createAudioPool(IAudioDecoder::Id _decoderId)
        {
            Pool audioPool;
            bool isValidResult = false;
            bool isSpaceAvailable = false;
            int64_t audioBufferHandle;
            uint8_t read_data[256] = {0};
            std::vector<uint8_t> sha1Result;
            sp<AVBufferSpaceListener> listener = new AVBufferSpaceListener();

            AVBUFFER_CREATE_AUDIOPOOL_SUCCESS(gAVBufferManager, sSecureHeap, _decoderId, listener, audioPool);
            AVBUFFER_GETMETRICS(gAVBufferManager,sSecureHeap,sHeapMetrics);
            UT_ASSERT_EQUAL(sHeapMetrics.secure,sSecureHeap);
            UT_ASSERT_GREATER_EQUAL(sHeapMetrics.bytesUsed, 0);
            UT_ASSERT_LESS_EQUAL(sHeapMetrics.bytesUsed, sHeapMetrics.bytesTotal);
            UT_ASSERT_EQUAL(sHeapMetrics.bytesTotal,gAVBuffer_hfp.nonSecureHeapBytes);
            if(sAllocateTest){
                AVBUFFER_ALLOC_SUCCESS(gAVBufferManager,audioPool,(sHeapMetrics.bytesTotal - sHeapMetrics.bytesUsed),audioBufferHandle);
                AVBUFFER_ISVALID_SUCCESS(gAVBufferManager,audioBufferHandle, isValidResult);
                UT_ASSERT_TRUE(isValidResult);
            }
            if(sNotifyTest){
                AVBUFFER_NOTIFYSPACE_SUCCESS(gAVBufferManager,audioPool,(sHeapMetrics.bytesTotal - sHeapMetrics.bytesUsed)/2);
            }
            if(sTrimTest || sNotifyTest){
                AVBUFFER_TRIMSIZE_SUCCESS(gAVBufferManager,audioBufferHandle,(sHeapMetrics.bytesTotal - sHeapMetrics.bytesUsed)/2);
            }
            if(sNotifyTest)
            {
                isSpaceAvailable = false;
                AVBUFFER_WAIT_FOR_SPACEAVAILABLE(gAVBufferManager,listener,isSpaceAvailable);
                UT_ASSERT_TRUE(isSpaceAvailable);
            }
            if(sReadWriteTest)
            {
                std::memcpy((void *)audioBufferHandle,standard_data,sizeof(standard_data));
                std::memcpy(read_data,(void *)audioBufferHandle,sizeof(read_data));
                UT_ASSERT_TRUE((std::memcmp(standard_data,read_data,sizeof(standard_data)) == 0));

            }
            if(sSHA1Test)
            {
                AVBUFFER_CALCULATESHA_SUCCESS(gAVBufferManager,audioBufferHandle,sha1Result);
                for(int i = 0; i< 20; i++)
                {
                    UT_ASSERT_EQUAL(sha1Result[i], standard_data_sha1[i]);
                }
            }

            sPoolsCreated.emplace_back(audioPool);
            sSpaceListeners.emplace_back(listener);
            sBufferHandles.emplace_back(audioBufferHandle);
        }
        void createVideoPool(IVideoDecoder::Id _decoderId)
        {
            Pool videoPool;
            bool isValidResult = false;
            bool isSpaceAvailable = false;
            int64_t videoBufferHandle;
            uint8_t read_data[256] = {0};
            std::vector<uint8_t> sha1Result;
            sp<AVBufferSpaceListener> listener = new AVBufferSpaceListener();

            AVBUFFER_CREATE_VIDEOPOOL_SUCCESS(gAVBufferManager, sSecureHeap, _decoderId, listener, videoPool);
            AVBUFFER_GETMETRICS(gAVBufferManager,sSecureHeap,sHeapMetrics);
            UT_ASSERT_EQUAL(sHeapMetrics.secure,sSecureHeap);
            UT_ASSERT_GREATER_EQUAL(sHeapMetrics.bytesUsed, 0);
            UT_ASSERT_LESS_EQUAL(sHeapMetrics.bytesUsed, sHeapMetrics.bytesTotal);
            UT_ASSERT_EQUAL(sHeapMetrics.bytesTotal,gAVBuffer_hfp.nonSecureHeapBytes);
            if(sAllocateTest){
                AVBUFFER_ALLOC_SUCCESS(gAVBufferManager,videoPool,(sHeapMetrics.bytesTotal - sHeapMetrics.bytesUsed),videoBufferHandle);
                AVBUFFER_ISVALID_SUCCESS(gAVBufferManager,videoBufferHandle, isValidResult);
                UT_ASSERT_TRUE(isValidResult);
            }
            if(sNotifyTest){
                AVBUFFER_NOTIFYSPACE_SUCCESS(gAVBufferManager,videoPool,(sHeapMetrics.bytesTotal - sHeapMetrics.bytesUsed)/2);
            }
            if(sTrimTest || sNotifyTest){
                AVBUFFER_TRIMSIZE_SUCCESS(gAVBufferManager,videoBufferHandle,(sHeapMetrics.bytesTotal - sHeapMetrics.bytesUsed)/2);
            }
            if(sNotifyTest)
            {
                isSpaceAvailable = false;
                AVBUFFER_WAIT_FOR_SPACEAVAILABLE(gAVBufferManager,listener,isSpaceAvailable);
                UT_ASSERT_TRUE(isSpaceAvailable);
            }
            if(sReadWriteTest)
            {
                std::memcpy((void *)videoBufferHandle,standard_data,sizeof(standard_data));
                std::memcpy(read_data,(void *)videoBufferHandle,sizeof(read_data));
                UT_ASSERT_TRUE((std::memcmp(standard_data,read_data,sizeof(standard_data)) == 0));

            }
            if(sSHA1Test)
            {
                AVBUFFER_CALCULATESHA_SUCCESS(gAVBufferManager,videoBufferHandle,sha1Result);
                for(int i = 0; i< 20; i++)
                {
                    UT_ASSERT_EQUAL(sha1Result[i], standard_data_sha1[i]);
                }
            }

            sPoolsCreated.emplace_back(videoPool);
            sSpaceListeners.emplace_back(listener);
            sBufferHandles.emplace_back(videoBufferHandle);
        }
        void destroyPools()
        {
            if(sAllocateTest){
                for(const auto& bufferHandle : sBufferHandles){
                    AVBUFFER_FREE_SUCCESS(gAVBufferManager,bufferHandle);
                }
                sBufferHandles.clear();
            }
            for(const auto& pool : sPoolsCreated){
                AVBUFFER_DESTROY_POOL_SUCCESS(gAVBufferManager,pool);
            }
            sPoolsCreated.clear();
            sSpaceListeners.clear();

        }
};

/**
 * @brief Test for VTS_L1_AVBuffer to verify heap metrics retrieval for both secure and non-secure heaps.
 *
 * This test checks the functionality of the getHeapMetrics API for both non-secure and secure heap scenarios.
 * It ensures that the API returns the correct heap metrics and validates the expected conditions for both types of heaps.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 001@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize non-secure heap metrics retrieval | secureHeap = false, heapMetrics = uninitialized | Status is OK, heapMetrics.secure is false, bytesUsed >= 0, bytesUsed <= bytesTotal, bytesTotal matches expected value | Should Pass |
 * | 02 | Call getHeapMetrics for non-secure heap | secureHeap = false, heapMetrics = initialized | Status is OK, heapMetrics.secure is false, bytesUsed >= 0, bytesUsed <= bytesTotal, bytesTotal matches expected value | Should Pass |
 * | 03 | Validate non-secure heap metrics | heapMetrics = populated | Assertions for status, secure, bytesUsed, and bytesTotal | Should be successful |
 * | 04 | Initialize secure heap metrics retrieval | secureHeap = value from profile, heapMetrics = uninitialized | Status is OK, heapMetrics.secure is true, bytesUsed >= 0, bytesUsed <= bytesTotal, bytesTotal matches expected value | Should Pass |
 * | 05 | Call getHeapMetrics for secure heap | secureHeap = value from profile, heapMetrics = initialized | Status is OK, heapMetrics.secure is true, bytesUsed >= 0, bytesUsed <= bytesTotal, bytesTotal matches expected value | Should Pass |
 * | 06 | Validate secure heap metrics | heapMetrics = populated | Assertions for status, secure, bytesUsed, and bytesTotal | Should be successful |
 */
UT_ADD_TEST(VTS_L3_AVBuffer, seq_nonsec_audiopool)
{
    sSecureHeap = false;
    sAllocateTest = false;
    sTrimTest = false;
    sNotifyTest = false;
    sReadWriteTest = false;
    sSHA1Test = false;

    for(const auto& _decoderId: sAudioDecoderIds)
    {
        this->createAudioPool(_decoderId);
    }
    this->destroyPools();

}

UT_ADD_TEST(VTS_L3_AVBuffer, seq_nonsec_audiopoolpcm)
{
    sSecureHeap = false;
    sAllocateTest = false;
    sTrimTest = false;
    sNotifyTest = false;
    sReadWriteTest = false;
    sSHA1Test = false;
    IAudioDecoder::Id _decoderId;
    _decoderId.value = IAudioDecoder::Id::UNDEFINED;
    this->createAudioPool(_decoderId);
    this->destroyPools();

}

UT_ADD_TEST(VTS_L3_AVBuffer, seq_nonsec_audiopool_alloc)
{
    sSecureHeap = false;
    sAllocateTest = true;
    sTrimTest = false;
    sNotifyTest = false;
    sReadWriteTest = false;
    sSHA1Test = false;

    for(const auto& _decoderId: sAudioDecoderIds)
    {
        this->createAudioPool(_decoderId);
    }
    this->destroyPools();
}

UT_ADD_TEST(VTS_L3_AVBuffer, seq_nonsec_audiopool_trim)
{
    sSecureHeap = false;
    sAllocateTest = true;
    sTrimTest = true;
    sNotifyTest = false;
    sReadWriteTest = false;
    sSHA1Test = false;

    for(const auto& _decoderId: sAudioDecoderIds)
    {
        this->createAudioPool(_decoderId);
    }
    this->destroyPools();
}

UT_ADD_TEST(VTS_L3_AVBuffer, seq_nonsec_audiopool_notify)
{
    sSecureHeap = false;
    sAllocateTest = true;
    sTrimTest = false;
    sNotifyTest = true;
    sReadWriteTest = false;
    sSHA1Test = false;
    for(const auto& _decoderId: sAudioDecoderIds)
    {
        this->createAudioPool(_decoderId);
    }
    this->destroyPools();
}

UT_ADD_TEST(VTS_L3_AVBuffer, seq_nonsec_audiopool_readwrite)
{
    sSecureHeap = false;
    sAllocateTest = true;
    sTrimTest = false;
    sNotifyTest = false;
    sReadWriteTest = true;
    sSHA1Test = false;
    for(const auto& _decoderId: sAudioDecoderIds)
    {
        this->createAudioPool(_decoderId);
    }
    this->destroyPools();
}

UT_ADD_TEST(VTS_L3_AVBuffer, seq_nonsec_audiopool_sha1)
{
    sSecureHeap = false;
    sAllocateTest = true;
    sTrimTest = false;
    sNotifyTest = false;
    sReadWriteTest = true;
    sSHA1Test = true;
    for(const auto& _decoderId: sAudioDecoderIds)
    {
        this->createAudioPool(_decoderId);
    }
    this->destroyPools();
}

UT_ADD_TEST(VTS_L3_AVBuffer, seq_sec_audiopool)
{
    sSecureHeap = true;
    sAllocateTest = false;
    sTrimTest = false;
    sNotifyTest = false;
    sReadWriteTest = false;
    sSHA1Test = false;

    for(const auto& _decoderId: sAudioDecoderIds)
    {
        this->createAudioPool(_decoderId);
    }
    this->destroyPools();

}

UT_ADD_TEST(VTS_L3_AVBuffer, seq_sec_audiopoolpcm)
{
    sSecureHeap = true;
    sAllocateTest = false;
    sTrimTest = false;
    sNotifyTest = false;
    sReadWriteTest = false;
    sSHA1Test = false;
    IAudioDecoder::Id _decoderId;
    _decoderId.value = IAudioDecoder::Id::UNDEFINED;
    this->createAudioPool(_decoderId);
    this->destroyPools();

}

UT_ADD_TEST(VTS_L3_AVBuffer, seq_sec_audiopool_alloc)
{
    sSecureHeap = true;
    sAllocateTest = true;
    sTrimTest = false;
    sNotifyTest = false;
    sReadWriteTest = false;
    sSHA1Test = false;

    for(const auto& _decoderId: sAudioDecoderIds)
    {
        this->createAudioPool(_decoderId);
    }
    this->destroyPools();
}

UT_ADD_TEST(VTS_L3_AVBuffer, seq_sec_audiopool_trim)
{
    sSecureHeap = true;
    sAllocateTest = true;
    sTrimTest = true;
    sNotifyTest = false;
    sReadWriteTest = false;
    sSHA1Test = false;

    for(const auto& _decoderId: sAudioDecoderIds)
    {
        this->createAudioPool(_decoderId);
    }
    this->destroyPools();
}

UT_ADD_TEST(VTS_L3_AVBuffer, seq_sec_audiopool_notify)
{
    sSecureHeap = true;
    sAllocateTest = true;
    sTrimTest = false;
    sNotifyTest = true;
    sReadWriteTest = false;
    sSHA1Test = false;
    for(const auto& _decoderId: sAudioDecoderIds)
    {
        this->createAudioPool(_decoderId);
    }
    this->destroyPools();
}

UT_ADD_TEST(VTS_L3_AVBuffer, seq_sec_audiopool_readwrite)
{
    sSecureHeap = true;
    sAllocateTest = true;
    sTrimTest = false;
    sNotifyTest = false;
    sReadWriteTest = true;
    sSHA1Test = false;
    for(const auto& _decoderId: sAudioDecoderIds)
    {
        this->createAudioPool(_decoderId);
    }
    this->destroyPools();
}

UT_ADD_TEST(VTS_L3_AVBuffer, seq_sec_audiopool_sha1)
{
    sSecureHeap = true;
    sAllocateTest = true;
    sTrimTest = false;
    sNotifyTest = false;
    sReadWriteTest = true;
    sSHA1Test = true;
    for(const auto& _decoderId: sAudioDecoderIds)
    {
        this->createAudioPool(_decoderId);
    }
    this->destroyPools();
}

UT_ADD_TEST(VTS_L3_AVBuffer, con_nonsec_audiopool)
{
    sSecureHeap = false;
    sAllocateTest = false;
    sTrimTest = false;
    sNotifyTest = false;
    sReadWriteTest = false;
    sSHA1Test = false;
    std::vector<std::thread> threads;

    for(const auto& _decoderId: sAudioDecoderIds)
    {
        threads.emplace_back([this, _decoderId]() {
            this->createAudioPool(_decoderId);
            });
    }
    for(auto& t : threads)
    {
        if(t.joinable())
            t.join();
    }
    this->destroyPools();
    threads.clear();

}

UT_ADD_TEST(VTS_L3_AVBuffer, con_nonsec_audiopool_alloc)
{
    sSecureHeap = false;
    sAllocateTest = true;
    sTrimTest = false;
    sNotifyTest = false;
    sReadWriteTest = false;
    sSHA1Test = false;
    std::vector<std::thread> threads;

    for(const auto& _decoderId: sAudioDecoderIds)
    {
        threads.emplace_back([this, _decoderId]() {
            this->createAudioPool(_decoderId);
            });
    }
    for(auto& t : threads)
    {
        if(t.joinable())
            t.join();
    }
    this->destroyPools();
    threads.clear();
}

UT_ADD_TEST(VTS_L3_AVBuffer, con_nonsec_audiopool_trim)
{
    sSecureHeap = false;
    sAllocateTest = true;
    sTrimTest = true;
    sNotifyTest = false;
    sReadWriteTest = false;
    sSHA1Test = false;
    std::vector<std::thread> threads;

    for(const auto& _decoderId: sAudioDecoderIds)
    {
        threads.emplace_back([this, _decoderId]() {
            this->createAudioPool(_decoderId);
            });
    }
    for(auto& t : threads)
    {
        if(t.joinable())
            t.join();
    }
    this->destroyPools();
    threads.clear();
}

UT_ADD_TEST(VTS_L3_AVBuffer, con_nonsec_audiopool_notify)
{
    sSecureHeap = false;
    sAllocateTest = true;
    sTrimTest = false;
    sNotifyTest = true;
    sReadWriteTest = false;
    sSHA1Test = false;
    std::vector<std::thread> threads;

    for(const auto& _decoderId: sAudioDecoderIds)
    {
        threads.emplace_back([this, _decoderId]() {
            this->createAudioPool(_decoderId);
            });
    }
    for(auto& t : threads)
    {
        if(t.joinable())
            t.join();
    }
    this->destroyPools();
    threads.clear();
}

UT_ADD_TEST(VTS_L3_AVBuffer, con_nonsec_audiopool_readwrite)
{
    sSecureHeap = false;
    sAllocateTest = true;
    sTrimTest = false;
    sNotifyTest = false;
    sReadWriteTest = true;
    sSHA1Test = false;
    std::vector<std::thread> threads;

    for(const auto& _decoderId: sAudioDecoderIds)
    {
        threads.emplace_back([this, _decoderId]() {
            this->createAudioPool(_decoderId);
            });
    }
    for(auto& t : threads)
    {
        if(t.joinable())
            t.join();
    }
    this->destroyPools();
    threads.clear();
}

UT_ADD_TEST(VTS_L3_AVBuffer, con_nonsec_audiopool_sha1)
{
    sSecureHeap = false;
    sAllocateTest = true;
    sTrimTest = false;
    sNotifyTest = false;
    sReadWriteTest = true;
    sSHA1Test = true;
    std::vector<std::thread> threads;

    for(const auto& _decoderId: sAudioDecoderIds)
    {
        threads.emplace_back([this, _decoderId]() {
            this->createAudioPool(_decoderId);
            });
    }
    for(auto& t : threads)
    {
        if(t.joinable())
            t.join();
    }
    this->destroyPools();
    threads.clear();
}

UT_ADD_TEST(VTS_L3_AVBuffer, con_sec_audiopool)
{
    sSecureHeap = true;
    sAllocateTest = false;
    sTrimTest = false;
    sNotifyTest = false;
    sReadWriteTest = false;
    sSHA1Test = false;
    std::vector<std::thread> threads;

    for(const auto& _decoderId: sAudioDecoderIds)
    {
        threads.emplace_back([this, _decoderId]() {
            this->createAudioPool(_decoderId);
            });
    }
    for(auto& t : threads)
    {
        if(t.joinable())
            t.join();
    }
    this->destroyPools();
    threads.clear();
}

UT_ADD_TEST(VTS_L3_AVBuffer, con_sec_audiopool_alloc)
{
    sSecureHeap = true;
    sAllocateTest = true;
    sTrimTest = false;
    sNotifyTest = false;
    sReadWriteTest = false;
    sSHA1Test = false;
    std::vector<std::thread> threads;

    for(const auto& _decoderId: sAudioDecoderIds)
    {
        threads.emplace_back([this, _decoderId]() {
            this->createAudioPool(_decoderId);
            });
    }
    for(auto& t : threads)
    {
        if(t.joinable())
            t.join();
    }
    this->destroyPools();
    threads.clear();
}

UT_ADD_TEST(VTS_L3_AVBuffer, con_sec_audiopool_trim)
{
    sSecureHeap = true;
    sAllocateTest = true;
    sTrimTest = true;
    sNotifyTest = false;
    sReadWriteTest = false;
    sSHA1Test = false;
    std::vector<std::thread> threads;

    for(const auto& _decoderId: sAudioDecoderIds)
    {
        threads.emplace_back([this, _decoderId]() {
            this->createAudioPool(_decoderId);
            });
    }
    for(auto& t : threads)
    {
        if(t.joinable())
            t.join();
    }
    this->destroyPools();
    threads.clear();
}

UT_ADD_TEST(VTS_L3_AVBuffer, con_sec_audiopool_notify)
{
    sSecureHeap = true;
    sAllocateTest = true;
    sTrimTest = false;
    sNotifyTest = true;
    sReadWriteTest = false;
    sSHA1Test = false;
    std::vector<std::thread> threads;

    for(const auto& _decoderId: sAudioDecoderIds)
    {
        threads.emplace_back([this, _decoderId]() {
            this->createAudioPool(_decoderId);
            });
    }
    for(auto& t : threads)
    {
        if(t.joinable())
            t.join();
    }
    this->destroyPools();
    threads.clear();
}

UT_ADD_TEST(VTS_L3_AVBuffer, con_sec_audiopool_readwrite)
{
    sSecureHeap = true;
    sAllocateTest = true;
    sTrimTest = false;
    sNotifyTest = false;
    sReadWriteTest = true;
    sSHA1Test = false;
    std::vector<std::thread> threads;

    for(const auto& _decoderId: sAudioDecoderIds)
    {
        threads.emplace_back([this, _decoderId]() {
            this->createAudioPool(_decoderId);
            });
    }
    for(auto& t : threads)
    {
        if(t.joinable())
            t.join();
    }
    this->destroyPools();
    threads.clear();
}

UT_ADD_TEST(VTS_L3_AVBuffer, con_sec_audiopool_sha1)
{
    sSecureHeap = true;
    sAllocateTest = true;
    sTrimTest = false;
    sNotifyTest = false;
    sReadWriteTest = true;
    sSHA1Test = true;
    std::vector<std::thread> threads;

    for(const auto& _decoderId: sAudioDecoderIds)
    {
        threads.emplace_back([this, _decoderId]() {
            this->createAudioPool(_decoderId);
            });
    }
    for(auto& t : threads)
    {
        if(t.joinable())
            t.join();
    }
    this->destroyPools();
    threads.clear();
}

UT_ADD_TEST(VTS_L3_AVBuffer, seq_nonsec_videopool)
{
    sSecureHeap = false;
    sAllocateTest = false;
    sTrimTest = false;
    sNotifyTest = false;
    sReadWriteTest = false;
    sSHA1Test = false;

    for(const auto& _decoderId: sVideoDecoderIds)
    {
        this->createVideoPool(_decoderId);
    }
    this->destroyPools();

}


UT_ADD_TEST(VTS_L3_AVBuffer, seq_nonsec_videopool_alloc)
{
    sSecureHeap = false;
    sAllocateTest = true;
    sTrimTest = false;
    sNotifyTest = false;
    sReadWriteTest = false;
    sSHA1Test = false;

    for(const auto& _decoderId: sVideoDecoderIds)
    {
        this->createVideoPool(_decoderId);
    }
    this->destroyPools();
}

UT_ADD_TEST(VTS_L3_AVBuffer, seq_nonsec_videopool_trim)
{
    sSecureHeap = false;
    sAllocateTest = true;
    sTrimTest = true;
    sNotifyTest = false;
    sReadWriteTest = false;
    sSHA1Test = false;

    for(const auto& _decoderId: sVideoDecoderIds)
    {
        this->createVideoPool(_decoderId);
    }
    this->destroyPools();
}

UT_ADD_TEST(VTS_L3_AVBuffer, seq_nonsec_videopool_notify)
{
    sSecureHeap = false;
    sAllocateTest = true;
    sTrimTest = false;
    sNotifyTest = true;
    sReadWriteTest = false;
    sSHA1Test = false;
    for(const auto& _decoderId: sVideoDecoderIds)
    {
        this->createVideoPool(_decoderId);
    }
    this->destroyPools();
}

UT_ADD_TEST(VTS_L3_AVBuffer, seq_nonsec_videopool_readwrite)
{
    sSecureHeap = false;
    sAllocateTest = true;
    sTrimTest = false;
    sNotifyTest = false;
    sReadWriteTest = true;
    sSHA1Test = false;
    for(const auto& _decoderId: sVideoDecoderIds)
    {
        this->createVideoPool(_decoderId);
    }
    this->destroyPools();
}

UT_ADD_TEST(VTS_L3_AVBuffer, seq_nonsec_videopool_sha1)
{
    sSecureHeap = false;
    sAllocateTest = true;
    sTrimTest = false;
    sNotifyTest = false;
    sReadWriteTest = true;
    sSHA1Test = true;
    for(const auto& _decoderId: sVideoDecoderIds)
    {
        this->createVideoPool(_decoderId);
    }
    this->destroyPools();
}

UT_ADD_TEST(VTS_L3_AVBuffer, seq_sec_videopool)
{
    sSecureHeap = true;
    sAllocateTest = false;
    sTrimTest = false;
    sNotifyTest = false;
    sReadWriteTest = false;
    sSHA1Test = false;

    for(const auto& _decoderId: sVideoDecoderIds)
    {
        this->createVideoPool(_decoderId);
    }
    this->destroyPools();

}

UT_ADD_TEST(VTS_L3_AVBuffer, seq_sec_videopool_alloc)
{
    sSecureHeap = true;
    sAllocateTest = true;
    sTrimTest = false;
    sNotifyTest = false;
    sReadWriteTest = false;
    sSHA1Test = false;

    for(const auto& _decoderId: sVideoDecoderIds)
    {
        this->createVideoPool(_decoderId);
    }
    this->destroyPools();
}

UT_ADD_TEST(VTS_L3_AVBuffer, seq_sec_videopool_trim)
{
    sSecureHeap = true;
    sAllocateTest = true;
    sTrimTest = true;
    sNotifyTest = false;
    sReadWriteTest = false;
    sSHA1Test = false;

    for(const auto& _decoderId: sVideoDecoderIds)
    {
        this->createVideoPool(_decoderId);
    }
    this->destroyPools();
}

UT_ADD_TEST(VTS_L3_AVBuffer, seq_sec_videopool_notify)
{
    sSecureHeap = true;
    sAllocateTest = true;
    sTrimTest = false;
    sNotifyTest = true;
    sReadWriteTest = false;
    sSHA1Test = false;
    for(const auto& _decoderId: sVideoDecoderIds)
    {
        this->createVideoPool(_decoderId);
    }
    this->destroyPools();
}

UT_ADD_TEST(VTS_L3_AVBuffer, seq_sec_videopool_readwrite)
{
    sSecureHeap = true;
    sAllocateTest = true;
    sTrimTest = false;
    sNotifyTest = false;
    sReadWriteTest = true;
    sSHA1Test = false;
    for(const auto& _decoderId: sVideoDecoderIds)
    {
        this->createVideoPool(_decoderId);
    }
    this->destroyPools();
}

UT_ADD_TEST(VTS_L3_AVBuffer, seq_sec_videopool_sha1)
{
    sSecureHeap = true;
    sAllocateTest = true;
    sTrimTest = false;
    sNotifyTest = false;
    sReadWriteTest = true;
    sSHA1Test = true;
    for(const auto& _decoderId: sVideoDecoderIds)
    {
        this->createVideoPool(_decoderId);
    }
    this->destroyPools();
}

UT_ADD_TEST(VTS_L3_AVBuffer, con_nonsec_videopool)
{
    sSecureHeap = false;
    sAllocateTest = false;
    sTrimTest = false;
    sNotifyTest = false;
    sReadWriteTest = false;
    sSHA1Test = false;
    std::vector<std::thread> threads;

    for(const auto& _decoderId: sVideoDecoderIds)
    {
        threads.emplace_back([this, _decoderId]() {
            this->createVideoPool(_decoderId);
            });
    }
    for(auto& t : threads)
    {
        if(t.joinable())
            t.join();
    }
    this->destroyPools();
    threads.clear();

}

UT_ADD_TEST(VTS_L3_AVBuffer, con_nonsec_videopool_alloc)
{
    sSecureHeap = false;
    sAllocateTest = true;
    sTrimTest = false;
    sNotifyTest = false;
    sReadWriteTest = false;
    sSHA1Test = false;
    std::vector<std::thread> threads;

    for(const auto& _decoderId: sVideoDecoderIds)
    {
        threads.emplace_back([this, _decoderId]() {
            this->createVideoPool(_decoderId);
            });
    }
    for(auto& t : threads)
    {
        if(t.joinable())
            t.join();
    }
    this->destroyPools();
    threads.clear();
}

UT_ADD_TEST(VTS_L3_AVBuffer, con_nonsec_videopool_trim)
{
    sSecureHeap = false;
    sAllocateTest = true;
    sTrimTest = true;
    sNotifyTest = false;
    sReadWriteTest = false;
    sSHA1Test = false;
    std::vector<std::thread> threads;

    for(const auto& _decoderId: sVideoDecoderIds)
    {
        threads.emplace_back([this, _decoderId]() {
            this->createVideoPool(_decoderId);
            });
    }
    for(auto& t : threads)
    {
        if(t.joinable())
            t.join();
    }
    this->destroyPools();
    threads.clear();
}

UT_ADD_TEST(VTS_L3_AVBuffer, con_nonsec_videopool_notify)
{
    sSecureHeap = false;
    sAllocateTest = true;
    sTrimTest = false;
    sNotifyTest = true;
    sReadWriteTest = false;
    sSHA1Test = false;
    std::vector<std::thread> threads;

    for(const auto& _decoderId: sVideoDecoderIds)
    {
        threads.emplace_back([this, _decoderId]() {
            this->createVideoPool(_decoderId);
            });
    }
    for(auto& t : threads)
    {
        if(t.joinable())
            t.join();
    }
    this->destroyPools();
    threads.clear();
}

UT_ADD_TEST(VTS_L3_AVBuffer, con_nonsec_videopool_readwrite)
{
    sSecureHeap = false;
    sAllocateTest = true;
    sTrimTest = false;
    sNotifyTest = false;
    sReadWriteTest = true;
    sSHA1Test = false;
    std::vector<std::thread> threads;

    for(const auto& _decoderId: sVideoDecoderIds)
    {
        threads.emplace_back([this, _decoderId]() {
            this->createVideoPool(_decoderId);
            });
    }
    for(auto& t : threads)
    {
        if(t.joinable())
            t.join();
    }
    this->destroyPools();
    threads.clear();
}

UT_ADD_TEST(VTS_L3_AVBuffer, con_nonsec_videopool_sha1)
{
    sSecureHeap = false;
    sAllocateTest = true;
    sTrimTest = false;
    sNotifyTest = false;
    sReadWriteTest = true;
    sSHA1Test = true;
    std::vector<std::thread> threads;

    for(const auto& _decoderId: sVideoDecoderIds)
    {
        threads.emplace_back([this, _decoderId]() {
            this->createVideoPool(_decoderId);
            });
    }
    for(auto& t : threads)
    {
        if(t.joinable())
            t.join();
    }
    this->destroyPools();
    threads.clear();
}

UT_ADD_TEST(VTS_L3_AVBuffer, con_sec_videopool)
{
    sSecureHeap = true;
    sAllocateTest = false;
    sTrimTest = false;
    sNotifyTest = false;
    sReadWriteTest = false;
    sSHA1Test = false;
    std::vector<std::thread> threads;

    for(const auto& _decoderId: sVideoDecoderIds)
    {
        threads.emplace_back([this, _decoderId]() {
            this->createVideoPool(_decoderId);
            });
    }
    for(auto& t : threads)
    {
        if(t.joinable())
            t.join();
    }
    this->destroyPools();
    threads.clear();
}

UT_ADD_TEST(VTS_L3_AVBuffer, con_sec_videopool_alloc)
{
    sSecureHeap = true;
    sAllocateTest = true;
    sTrimTest = false;
    sNotifyTest = false;
    sReadWriteTest = false;
    sSHA1Test = false;
    std::vector<std::thread> threads;

    for(const auto& _decoderId: sVideoDecoderIds)
    {
        threads.emplace_back([this, _decoderId]() {
            this->createVideoPool(_decoderId);
            });
    }
    for(auto& t : threads)
    {
        if(t.joinable())
            t.join();
    }
    this->destroyPools();
    threads.clear();
}

UT_ADD_TEST(VTS_L3_AVBuffer, con_sec_videopool_trim)
{
    sSecureHeap = true;
    sAllocateTest = true;
    sTrimTest = true;
    sNotifyTest = false;
    sReadWriteTest = false;
    sSHA1Test = false;
    std::vector<std::thread> threads;

    for(const auto& _decoderId: sVideoDecoderIds)
    {
        threads.emplace_back([this, _decoderId]() {
            this->createVideoPool(_decoderId);
            });
    }
    for(auto& t : threads)
    {
        if(t.joinable())
            t.join();
    }
    this->destroyPools();
    threads.clear();
}

UT_ADD_TEST(VTS_L3_AVBuffer, con_sec_videopool_notify)
{
    sSecureHeap = true;
    sAllocateTest = true;
    sTrimTest = false;
    sNotifyTest = true;
    sReadWriteTest = false;
    sSHA1Test = false;
    std::vector<std::thread> threads;

    for(const auto& _decoderId: sVideoDecoderIds)
    {
        threads.emplace_back([this, _decoderId]() {
            this->createVideoPool(_decoderId);
            });
    }
    for(auto& t : threads)
    {
        if(t.joinable())
            t.join();
    }
    this->destroyPools();
    threads.clear();
}

UT_ADD_TEST(VTS_L3_AVBuffer, con_sec_videopool_readwrite)
{
    sSecureHeap = true;
    sAllocateTest = true;
    sTrimTest = false;
    sNotifyTest = false;
    sReadWriteTest = true;
    sSHA1Test = false;
    std::vector<std::thread> threads;

    for(const auto& _decoderId: sVideoDecoderIds)
    {
        threads.emplace_back([this, _decoderId]() {
            this->createVideoPool(_decoderId);
            });
    }
    for(auto& t : threads)
    {
        if(t.joinable())
            t.join();
    }
    this->destroyPools();
    threads.clear();
}

UT_ADD_TEST(VTS_L3_AVBuffer, con_sec_videopool_sha1)
{
    sSecureHeap = true;
    sAllocateTest = true;
    sTrimTest = false;
    sNotifyTest = false;
    sReadWriteTest = true;
    sSHA1Test = true;
    std::vector<std::thread> threads;

    for(const auto& _decoderId: sVideoDecoderIds)
    {
        threads.emplace_back([this, _decoderId]() {
            this->createVideoPool(_decoderId);
            });
    }
    for(auto& t : threads)
    {
        if(t.joinable())
            t.join();
    }
    this->destroyPools();
    threads.clear();
}