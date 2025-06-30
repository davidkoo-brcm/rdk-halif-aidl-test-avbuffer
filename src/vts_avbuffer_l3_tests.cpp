
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

#include "com/rdk/hal/avbuffer/IAVBuffer.h"
#include "com/rdk/hal/avbuffer/BnAVBufferSpaceListener.h"

#include "vts_avbuffer_init.h"
#include "vts_avbuffer.h"
#include "vts_avbuffer_parse_hfp.h"

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

extern sp<IAVBuffer> gAVBufferManager;
static sp<IAVBuffer> sAudioDecoderManager;
static HeapMetrics sHeapMetrics;
static std::vector<IAudioDecoder::Id> sAudioDecoderIds;
static std::vector<IVideoDecoder::Id> sVideoDecoderIds;
static std::vector<sp<IAVBufferSpaceListener>> sSpaceListeners;
static sp<IAVBuffer> sVideoDecoderManager;

UT_status_t vts_audiodecoder_getService()
{
    sAudioDecoderManager =
        android::interface_cast<IAVBuffer>(
            android::defaultServiceManager()->getService(
                android::String16(IAVBuffer::serviceName().c_str())
            )
        );
    if (sAudioDecoderManager == nullptr)
    {
        return UT_STATUS_FAILURE;
    }
    return UT_STATUS_OK;
}

UT_status_t vts_videodecoder_getService()
{
    sVideoDecoderManager =
        android::interface_cast<IAVBuffer>(
            android::defaultServiceManager()->getService(
                android::String16(IVideoDecoder::serviceName().c_str())
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
            if(vts_audiodecoder_getService() != UT_STATUS_OK)
            {
                return UT_STATUS_FAILURE;
            }
            AUDIODECODER_MANAGER_GETAUDIODECODERIDS(sAudioDecoderIds);
            if(vts_videodecoder_getService() != UT_STATUS_OK)
            {
                return UT_STATUS_FAILURE;
            }
            VIDEODECODER_MANAGER_GETVIDEODECODERIDS(sVideoDecoderIds);
        }

        static void TearDownTestSuite()
        {
        }
    public:
        void createAudioPoolAndValidate(bool secureHeap, IAudioDecoder::ID _decoderId, Pool audioPool,IAVBufferSpaceListener listener)
        {
            AVBUFFER_CREATE_AUDIOPOOL_SUCCESS(gAVBufferManager, secureHeap, _decoderId, listener, audioPool);
            AVBUFFER_GETMETRICS(gAVBufferManager,secureHeap,sHeapMetrics);
            UT_ASSERT_EQUAL(heapMetrics.secure,secureHeap);
            UT_ASSERT_GREATER_EQUAL(heapMetrics.bytesUsed, 0);
            UT_ASSERT_LESS_EQUAL(heapMetrics.bytesUsed, heapMetrics.bytesTotal);
            UT_ASSERT_EQUAL(heapMetrics.bytesTotal,gAVBuffer_hfp.nonSecureHeapBytes);
            sSpaceListeners.emplace_back(listener);
        }
        void destroyPool(Pool pool)
        {
            AVBUFFER_DESTROY_POOL_SUCCESS(gAVBufferManager,pool);
            sSpaceListeners.clear();
        }
        void createVideoPoolAndValidate(bool secureHeap, IVideoDecoder::ID _decoderId, Pool videoPool,IAVBufferSpaceListener listener)
        {
            AVBUFFER_CREATE_VIDEOPOOL_SUCCESS(gAVBufferManager, secureHeap, _decoderId, listener, videoPool);
            AVBUFFER_GETMETRICS(gAVBufferManager,secureHeap,sHeapMetrics);
            UT_ASSERT_EQUAL(heapMetrics.secure,secureHeap);
            UT_ASSERT_GREATER_EQUAL(heapMetrics.bytesUsed, 0);
            UT_ASSERT_LESS_EQUAL(heapMetrics.bytesUsed, heapMetrics.bytesTotal);
            UT_ASSERT_EQUAL(heapMetrics.bytesTotal,gAVBuffer_hfp.nonSecureHeapBytes);
            sSpaceListeners.emplace_back(listener);
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
    bool secureHeap = false;
    Pool audioPool;
    HeapMetrics heapMetrics;

    for(const auto& _decoderId: sAudioDecoderIds)
    {
        sp<IAVBufferSpaceListener> listener = new AVBudderSpaceListener();
        this->createAudioPoolAndValidate(secureHeap,_decoderId,audioPool,listener);
        this->destroyAPool(audioPool);
    }

}

UT_ADD_TEST(VTS_L3_AVBuffer, seq_nonsec_audiopoolpcm)
{
    IAudioDecoder::Id decoderId = IAudioDecoder::Id::UNDEFINED;
    bool secureHeap = false;
    Pool audioPool;
    sp<IAVBufferSpaceListener> listener = new AVBufferSpaceListener();
    this->createAudioPoolAndValidate(secureHeap,decoderId,audioPool,listener);
    this->destroyAPool(audioPool);

}

UT_ADD_TEST(VTS_L3_AVBuffer, seq_nonsec_audiopool_alloc)
{
    bool secureHeap = false;
    Pool audioPool;
    int64_t audioBufferHandle;


    for(const auto& _decoderId: sAudioDecoderIds)
    {
        sp<IAVBufferSpaceListener> listener = new AVBufferSpaceListener();
        this->createAudioPoolAndValidate(secureHeap,_decoderId,audioPool,listener);
        AVBUFFER_ALLOC_SUCCESS(gAVBufferManager,audioPool,(sHeapMetrics.bytesTotal - sHeapMetrics.used),audioBufferHandle);
        AVBUFFER_FREE_SUCCESS(gAVBufferManager,audioBufferHandle);
        this->destroyAPool(audioPool);
    }

}

UT_ADD_TEST(VTS_L3_AVBuffer, seq_nonsec_audiopool_trim)
{
    bool secureHeap = false;
    Pool audioPool;
    int64_t audioBufferHandle;

    for(const auto& _decoderId: sAudioDecoderIds)
    {
        sp<IAVBufferSpaceListener> listener = new AVBufferSpaceListener();
        this->createAudioPoolAndValidate(secureHeap,_decoderId,audioPool,listener);
        AVBUFFER_ALLOC_SUCCESS(gAVBufferManager,audioPool,(sHeapMetrics.bytesTotal - sHeapMetrics.used),audioBufferHandle);
        AVBUFFER_TRIMSIZE_SUCCESS(gAVBufferManager,audioBufferHandle,(sHeapMetrics.bytesTotal - sHeapMetrics.used)/2);
        AVBUFFER_FREE_SUCCESS(gAVBufferManager,audioBufferHandle);
        this->destroyAPool(audioPool);
    }

}

UT_ADD_TEST(VTS_L3_AVBuffer, seq_nonsec_audiopool_notify)
{
    bool secureHeap = false;
    Pool audioPool;
    int64_t audioBufferHandle;

    for(const auto& _decoderId: sAudioDecoderIds)
    {
        sp<IAVBufferSpaceListener> listener = new AVBufferSpaceListener();
        this->createAudioPoolAndValidate(secureHeap,_decoderId,audioPool,listener);
        AVBUFFER_ALLOC_SUCCESS(gAVBufferManager,audioPool,(sHeapMetrics.bytesTotal - sHeapMetrics.used),audioBufferHandle);
        AVBUFFER_TRIMSIZE_SUCCESS(gAVBufferManager,audioBufferHandle,(sHeapMetrics.bytesTotal - sHeapMetrics.used)/2);
        AVBUFFER_FREE_SUCCESS(gAVBufferManager,audioBufferHandle);
        this->destroyAPool(audioPool);
    }

}

UT_ADD_TEST(VTS_L3_AVBuffer, seq_sec_audiopool)
{
    bool secureHeap = true;
    Pool audioPool;
    HeapMetrics heapMetrics;

    for(const auto& _decoderId: sAudioDecoderIds)
    {
        sp<IAVBufferSpaceListener> listener = new AVBudderSpaceListener();
        this->createAudioPoolAndValidate(secureHeap,_decoderId,audioPool,listener);
        this->destroyAPool(audioPool);
    }

}

UT_ADD_TEST(VTS_L3_AVBuffer, seq_sec_audiopoolpcm)
{
    IAudioDecoder::Id decoderId = IAudioDecoder::Id::UNDEFINED;
    bool secureHeap = true;
    Pool audioPool;
    sp<IAVBufferSpaceListener> listener = new AVBufferSpaceListener();
    this->createAudioPoolAndValidate(secureHeap,decoderId,audioPool,listener);
    this->destroyAPool(audioPool);

}

UT_ADD_TEST(VTS_L3_AVBuffer, seq_sec_audiopool_alloc)
{
    bool secureHeap = true;
    Pool audioPool;
    int64_t audioBufferHandle;


    for(const auto& _decoderId: sAudioDecoderIds)
    {
        sp<IAVBufferSpaceListener> listener = new AVBufferSpaceListener();
        this->createAudioPoolAndValidate(secureHeap,_decoderId,audioPool,listener);
        AVBUFFER_ALLOC_SUCCESS(gAVBufferManager,audioPool,(sHeapMetrics.bytesTotal - sHeapMetrics.used),audioBufferHandle);
        AVBUFFER_FREE_SUCCESS(gAVBufferManager,audioBufferHandle);
        this->destroyAPool(audioPool);
    }

}

UT_ADD_TEST(VTS_L3_AVBuffer, seq_sec_audiopool_trim)
{
    bool secureHeap = true;
    Pool audioPool;
    int64_t audioBufferHandle;

    for(const auto& _decoderId: sAudioDecoderIds)
    {
        sp<IAVBufferSpaceListener> listener = new AVBufferSpaceListener();
        this->createAudioPoolAndValidate(secureHeap,_decoderId,audioPool,listener);
        AVBUFFER_ALLOC_SUCCESS(gAVBufferManager,audioPool,(sHeapMetrics.bytesTotal - sHeapMetrics.used),audioBufferHandle);
        AVBUFFER_TRIMSIZE_SUCCESS(gAVBufferManager,audioBufferHandle,(sHeapMetrics.bytesTotal - sHeapMetrics.used)/2);
        AVBUFFER_FREE_SUCCESS(gAVBufferManager,audioBufferHandle);
        this->destroyAPool(audioPool);
    }

}

UT_ADD_TEST(VTS_L3_AVBuffer, seq_sec_audiopool_notify)
{
    bool secureHeap = false;
    Pool audioPool;
    int64_t audioBufferHandle;
    bool isSpaceAvailable = false;

    for(const auto& _decoderId: sAudioDecoderIds)
    {
        sp<IAVBufferSpaceListener> listener = new AVBufferSpaceListener();
        this->createAudioPoolAndValidate(secureHeap,_decoderId,audioPool,listener);
        AVBUFFER_ALLOC_SUCCESS(gAVBufferManager,audioPool,(sHeapMetrics.bytesTotal - sHeapMetrics.used),audioBufferHandle);
        AVBUFFER_NOTIFYSPACE_SUCCESS(gAVBufferManager,audioPool,(sHeapMetrics.bytesTotal - sHeapMetrics.used));
        AVBUFFER_FREE_SUCCESS(gAVBufferManager,audioBufferHandle);
        isSpaceAvailable = false;
        AVBUFFER_WAIT_FOR_SPACEAVAILABLE(gAVBufferManager,listener, isSpaceAvailable);
        UT_ASSERT_TRUE(isSpaceAvailable);
        this->destroyAPool(audioPool);
    }

}

UT_ADD_TEST(VTS_L3_AVBuffer, con_nonsec_audiopool)
{
    bool secureHeap = false;
    Pool audioPool;

    for(const auto& _decoderId: sAudioDecoderIds)
    {
        sp<IAVBufferSpaceListener> listener = new AVBudderSpaceListener();
        this->createAudioPoolAndValidate(secureHeap,_decoderId,audioPool,listener);
    }
    for(const auto& _decoderId: sAudioDecoderIds)
    {
        this->destroyAPool(audioPool);
    }

}

UT_ADD_TEST(VTS_L3_AVBuffer, con_nonsec_audiopool_alloc)
{
    bool secureHeap = false;
    Pool audioPool;
    int64_t audioBufferHandle[MAX_DECODER_COUNT];
    int count = 0;

    for(const auto& _decoderId: sAudioDecoderIds)
    {
        sp<IAVBufferSpaceListener> listener = new AVBufferSpaceListener();
        this->createAudioPoolAndValidate(secureHeap,_decoderId,audioPool,listener);
    }
    long sizeAvailable = sHeapMetrics.bytesTotal - sHeapMetrics.used;
    for(const auto& _decoderId: sAudioDecoderIds)
    {
        AVBUFFER_ALLOC_SUCCESS(gAVBufferManager,audioPool,(sizeAvailable)/(sAudioDecoderIds.size()),audioBufferHandle[count]);
        count++;
    }
    count = 0;
    for(const auto& _decoderId: sAudioDecoderIds)
    {
        AVBUFFER_FREE_SUCCESS(gAVBufferManager,audioBufferHandle[count]);
        count++;
        this->destroyAPool(audioPool);
    }

}

UT_ADD_TEST(VTS_L3_AVBuffer, con_nonsec_audiopool_trim)
{
    bool secureHeap = false;
    Pool audioPool;
    int64_t audioBufferHandle[MAX_DECODER_COUNT];
    int count = 0;

    for(const auto& _decoderId: sAudioDecoderIds)
    {
        sp<IAVBufferSpaceListener> listener = new AVBufferSpaceListener();
        this->createAudioPoolAndValidate(secureHeap,_decoderId,audioPool,listener);
    }
    long sizeAvailable = sHeapMetrics.bytesTotal - sHeapMetrics.used;
    for(const auto& _decoderId: sAudioDecoderIds)
    {
        AVBUFFER_ALLOC_SUCCESS(gAVBufferManager,audioPool,(sizeAvailable)/(sAudioDecoderIds.size()),audioBufferHandle[count]);
        count++;
    }
    count = 0;
    for(const auto& _decoderId: sAudioDecoderIds)
    {
        AVBUFFER_TRIMSIZE_SUCCESS(gAVBufferManager,audioBufferHandle[count],sizeAvailable/(2*(sAudioDecoderIds.size())));
        count++;
    }
    count = 0;
    for(const auto& _decoderId: sAudioDecoderIds)
    {
        AVBUFFER_FREE_SUCCESS(gAVBufferManager,audioBufferHandle[count]);
        count++;
        this->destroyAPool(audioPool);
    }

}

UT_ADD_TEST(VTS_L3_AVBuffer, con_nonsec_audiopool_notify)
{
    bool secureHeap = false;
    Pool audioPool;
    int64_t audioBufferHandle[MAX_DECODER_COUNT];
    int count = 0;
    bool isSpaceAvailable = false;
    for(const auto& _decoderId: sAudioDecoderIds)
    {
        sp<IAVBufferSpaceListener> listener = new AVBufferSpaceListener();
        this->createAudioPoolAndValidate(secureHeap,_decoderId,audioPool,listener);
    }
    long sizeAvailable = sHeapMetrics.bytesTotal - sHeapMetrics.used;
    for(const auto& _decoderId: sAudioDecoderIds)
    {
        AVBUFFER_ALLOC_SUCCESS(gAVBufferManager,audioPool,(sizeAvailable)/(sAudioDecoderIds.size()),audioBufferHandle[count]);
        count++;
    }
    count = 0;
    for(const auto& _decoderId: sAudioDecoderIds)
    {
        AVBUFFER_NOTIFYSPACE_SUCCESS(gAVBufferManager,audioPool,(sHeapMetrics.bytesTotal - sHeapMetrics.used)/(sAudioDecoderIds.size()));
    }
    count = 0;
    for(const auto& _decoderId: sAudioDecoderIds)
    {
        AVBUFFER_FREE_SUCCESS(gAVBufferManager,audioBufferHandle[count]);
        count++;
        this->destroyAPool(audioPool);
    }
    count = 0;
    for(const auto& _decoderId: sAudioDecoderIds)
    {
        isSpaceAvailable = false;
        AVBUFFER_WAIT_FOR_SPACEAVAILABLE(gAVBufferManager,sSpaceListeners[count], isSpaceAvailable);
        UT_ASSERT_TRUE(isSpaceAvailable);
    }
}

UT_ADD_TEST(VTS_L3_AVBuffer, seq_sec_audiopool)
{
    bool secureHeap = true;
    Pool audioPool;
    HeapMetrics heapMetrics;

    for(const auto& _decoderId: sAudioDecoderIds)
    {
        sp<IAVBufferSpaceListener> listener = new AVBudderSpaceListener();
        this->createAudioPoolAndValidate(secureHeap,_decoderId,audioPool,listener);
        this->destroyAPool(audioPool);
    }

}

UT_ADD_TEST(VTS_L3_AVBuffer, seq_sec_audiopoolpcm)
{
    IAudioDecoder::Id decoderId = IAudioDecoder::Id::UNDEFINED;
    bool secureHeap = true;
    Pool audioPool;
    sp<IAVBufferSpaceListener> listener = new AVBufferSpaceListener();
    this->createAudioPoolAndValidate(secureHeap,decoderId,audioPool,listener);
    this->destroyAPool(audioPool);

}

UT_ADD_TEST(VTS_L3_AVBuffer, seq_sec_audiopool_alloc)
{
    bool secureHeap = true;
    Pool audioPool;
    int64_t audioBufferHandle;


    for(const auto& _decoderId: sAudioDecoderIds)
    {
        sp<IAVBufferSpaceListener> listener = new AVBufferSpaceListener();
        this->createAudioPoolAndValidate(secureHeap,_decoderId,audioPool,listener);
        AVBUFFER_ALLOC_SUCCESS(gAVBufferManager,audioPool,(sHeapMetrics.bytesTotal - sHeapMetrics.used),audioBufferHandle);
        AVBUFFER_FREE_SUCCESS(gAVBufferManager,audioBufferHandle);
        this->destroyAPool(audioPool);
    }

}

UT_ADD_TEST(VTS_L3_AVBuffer, seq_sec_audiopool_trim)
{
    bool secureHeap = true;
    Pool audioPool;
    int64_t audioBufferHandle;

    for(const auto& _decoderId: sAudioDecoderIds)
    {
        sp<IAVBufferSpaceListener> listener = new AVBufferSpaceListener();
        this->createAudioPoolAndValidate(secureHeap,_decoderId,audioPool,listener);
        AVBUFFER_ALLOC_SUCCESS(gAVBufferManager,audioPool,(sHeapMetrics.bytesTotal - sHeapMetrics.used),audioBufferHandle);
        AVBUFFER_TRIMSIZE_SUCCESS(gAVBufferManager,audioBufferHandle,(sHeapMetrics.bytesTotal - sHeapMetrics.used)/2);
        AVBUFFER_FREE_SUCCESS(gAVBufferManager,audioBufferHandle);
        this->destroyAPool(audioPool);
    }

}

UT_ADD_TEST(VTS_L3_AVBuffer, seq_sec_audiopool_notify)
{
    bool secureHeap = false;
    Pool audioPool;
    int64_t audioBufferHandle;

    for(const auto& _decoderId: sAudioDecoderIds)
    {
        sp<IAVBufferSpaceListener> listener = new AVBufferSpaceListener();
        this->createAudioPoolAndValidate(secureHeap,_decoderId,audioPool,listener);
        AVBUFFER_ALLOC_SUCCESS(gAVBufferManager,audioPool,(sHeapMetrics.bytesTotal - sHeapMetrics.used),audioBufferHandle);
        AVBUFFER_NOTIFYSPACE_SUCCESS(gAVBufferManager,audioPool,(sHeapMetrics.bytesTotal - sHeapMetrics.used));
        AVBUFFER_FREE_SUCCESS(gAVBufferManager,audioBufferHandle);
        isSpaceAvailable = false;
        AVBUFFER_WAIT_FOR_SPACEAVAILABLE(gAVBufferManager,listener, isSpaceAvailable);
        UT_ASSERT_TRUE(isSpaceAvailable);
        this->destroyAPool(audioPool);
    }

}

UT_ADD_TEST(VTS_L3_AVBuffer, con_sec_audiopool)
{
    bool secureHeap = true;
    Pool audioPool;

    for(const auto& _decoderId: sAudioDecoderIds)
    {
        sp<IAVBufferSpaceListener> listener = new AVBudderSpaceListener();
        this->createAudioPoolAndValidate(secureHeap,_decoderId,audioPool,listener);
    }
    for(const auto& _decoderId: sAudioDecoderIds)
    {
        this->destroyAPool(audioPool);
    }

}

UT_ADD_TEST(VTS_L3_AVBuffer, con_sec_audiopool_alloc)
{
    bool secureHeap = true;
    Pool audioPool;
    int64_t audioBufferHandle[MAX_DECODER_COUNT];
    int count = 0;

    for(const auto& _decoderId: sAudioDecoderIds)
    {
        sp<IAVBufferSpaceListener> listener = new AVBufferSpaceListener();
        this->createAudioPoolAndValidate(secureHeap,_decoderId,audioPool,listener);
    }
    long sizeAvailable = sHeapMetrics.bytesTotal - sHeapMetrics.used;
    for(const auto& _decoderId: sAudioDecoderIds)
    {
        AVBUFFER_ALLOC_SUCCESS(gAVBufferManager,audioPool,(sizeAvailable)/(sAudioDecoderIds.size()),audioBufferHandle[count]);
        count++;
    }
    count = 0;
    for(const auto& _decoderId: sAudioDecoderIds)
    {
        AVBUFFER_FREE_SUCCESS(gAVBufferManager,audioBufferHandle[count]);
        count++;
        this->destroyAPool(audioPool);
    }

}

UT_ADD_TEST(VTS_L3_AVBuffer, con_sec_audiopool_trim)
{
    bool secureHeap = true;
    Pool audioPool;
    int64_t audioBufferHandle[MAX_DECODER_COUNT];
    int count = 0;

    for(const auto& _decoderId: sAudioDecoderIds)
    {
        sp<IAVBufferSpaceListener> listener = new AVBufferSpaceListener();
        this->createAudioPoolAndValidate(secureHeap,_decoderId,audioPool,listener);
    }
    long sizeAvailable = sHeapMetrics.bytesTotal - sHeapMetrics.used;
    for(const auto& _decoderId: sAudioDecoderIds)
    {
        AVBUFFER_ALLOC_SUCCESS(gAVBufferManager,audioPool,(sizeAvailable)/(sAudioDecoderIds.size()),audioBufferHandle[count]);
        count++;
    }
    count = 0;
    for(const auto& _decoderId: sAudioDecoderIds)
    {
        AVBUFFER_TRIMSIZE_SUCCESS(gAVBufferManager,audioBufferHandle[count],sizeAvailable/(2*(sAudioDecoderIds.size())));
        count++;
    }
    count = 0;
    for(const auto& _decoderId: sAudioDecoderIds)
    {
        AVBUFFER_FREE_SUCCESS(gAVBufferManager,audioBufferHandle[count]);
        count++;
        this->destroyAPool(audioPool);
    }

}

UT_ADD_TEST(VTS_L3_AVBuffer, con_nonsec_audiopool_notify)
{
    bool secureHeap = true;
    Pool audioPool;
    int64_t audioBufferHandle[MAX_DECODER_COUNT];
    int count = 0;
    bool isSpaceAvailable = false;
    for(const auto& _decoderId: sAudioDecoderIds)
    {
        sp<IAVBufferSpaceListener> listener = new AVBufferSpaceListener();
        this->createAudioPoolAndValidate(secureHeap,_decoderId,audioPool,listener);
    }
    long sizeAvailable = sHeapMetrics.bytesTotal - sHeapMetrics.used;
    for(const auto& _decoderId: sAudioDecoderIds)
    {
        AVBUFFER_ALLOC_SUCCESS(gAVBufferManager,audioPool,(sizeAvailable)/(sAudioDecoderIds.size()),audioBufferHandle[count]);
        count++;
    }
    count = 0;
    for(const auto& _decoderId: sAudioDecoderIds)
    {
        AVBUFFER_NOTIFYSPACE_SUCCESS(gAVBufferManager,audioPool,(sHeapMetrics.bytesTotal - sHeapMetrics.used)/(sAudioDecoderIds.size()));
    }
    count = 0;
    for(const auto& _decoderId: sAudioDecoderIds)
    {
        AVBUFFER_FREE_SUCCESS(gAVBufferManager,audioBufferHandle[count]);
        count++;
        this->destroyAPool(audioPool);
    }
    count = 0;
    for(const auto& _decoderId: sAudioDecoderIds)
    {
        isSpaceAvailable = false;
        AVBUFFER_WAIT_FOR_SPACEAVAILABLE(gAVBufferManager,sSpaceListeners[count], isSpaceAvailable);
        UT_ASSERT_TRUE(isSpaceAvailable);
    }
}
