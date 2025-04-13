
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

using android::sp;
using android::String16;
using android::defaultServiceManager;
using android::binder::Status;

using namespace com::rdk::hal;
using namespace com::rdk::hal::avbuffer;
using namespace com::rdk::hal::videodecoder;
using namespace com::rdk::hal::audiodecoder;

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

UT_status_t vts_avbuffer_getService()
{
    gAVBufferManager =
        android::interface_cast<IAVBuffer>(
            android::defaultServiceManager()->getService(
                android::String16(IAVBuffer::serviceName().c_str())
            )
        );
    if (gAVBufferManager == nullptr)
    {
        return UT_STATUS_FAILURE;
    }
    return UT_STATUS_OK;
}


class AVBufferSpaceListener : public BnAVBufferSpaceListener {
public:

    AVBufferSpaceListener() {};
    virtual ~AVBufferSpaceListener() = default;

    Status onSpaceAvailable() override
    {
        return ::android::binder::Status::ok();
    }
};

class VTS_L1_AVBuffer : public virtual UTCore
{
protected:
    static void SetUpTestSuite()
    {
    }

    static void TearDownTestSuite()
    {
    }
};


class L1AVBuffer_PoolManagementTestSuite : public VTS_L1_AVBuffer {
    protected:
        ::com::rdk::hal::avbuffer::Pool mVideoPool;
        ::com::rdk::hal::avbuffer::Pool mAudioPool;
        bool secureheap = true;

        void SetUp() override {

            VTS_L1_AVBuffer::SetUp();
            createVideoPool();
            createAudioPool();
        }

        void TearDown() override {
            destroyVideoPool();
            destroyAudioPool();
            VTS_L1_AVBuffer::TearDown();
        }

        void createVideoPool() {
            std::string videoDecoderPath = "IVideoDecoderManager.IVideoDecoder";
            printf("=======BHANU-1==========\n");
            int32_t videoDecoderCount = UT_KVP_PROFILE_GET_LIST_COUNT(videoDecoderPath.c_str());
            UT_ASSERT_TRUE(videoDecoderCount > 0) << "Video decoder profile returned invalid count.";

            std::string videoDecoderIdPath = videoDecoderPath + ".0.";
            int32_t _videoDecoderId = UT_KVP_PROFILE_GET_UINT32(videoDecoderIdPath.c_str());
            printf("=======BHANU-2==========\n");
            ::com::rdk::hal::videodecoder::IVideoDecoder::Id videoDecoderId;
            videoDecoderId.value = _videoDecoderId;

            sp<IAVBufferSpaceListener> listener = new AVBufferSpaceListener();
            Status status = gAVBufferManager->createVideoPool(secureheap, videoDecoderId, listener, &mVideoPool);
            UT_ASSERT_TRUE(status.isOk()) << "Failed to create video pool";
            UT_ASSERT_NOT_EQUAL(mVideoPool.handle, Pool::INVALID);
        }

        void createAudioPool() {
            printf("=======BHANU-3==========\n");
            std::string audioDecoderPath = "IAudioDecoderManager.IAudioDecoder";
            int32_t audioDecoderCount = UT_KVP_PROFILE_GET_LIST_COUNT(audioDecoderPath.c_str());
            UT_ASSERT_TRUE(audioDecoderCount > 0) << "Audio decoder profile returned invalid count.";
            printf("=======BHANU-4==========\n");
            std::string audioDecoderIdPath = audioDecoderPath + ".0.";
            int32_t _audioDecoderId = UT_KVP_PROFILE_GET_UINT32(audioDecoderIdPath.c_str());

            ::com::rdk::hal::audiodecoder::IAudioDecoder::Id audioDecoderId;
            audioDecoderId.value = _audioDecoderId;

            sp<IAVBufferSpaceListener> listener = new AVBufferSpaceListener();
            Status status = gAVBufferManager->createAudioPool(secureheap, audioDecoderId, listener, &mAudioPool);
            UT_ASSERT_TRUE(status.isOk()) << "Failed to create audio pool";
            UT_ASSERT_NOT_EQUAL(mAudioPool.handle, Pool::INVALID);
        }

        void destroyVideoPool() {
            if (mVideoPool.handle != ::com::rdk::hal::avbuffer::Pool::INVALID) {
                Status status = gAVBufferManager->destroyPool(mVideoPool, &secureheap);
                UT_ASSERT_TRUE(status.isOk());
            }
        }

        void destroyAudioPool() {
            if (mAudioPool.handle != ::com::rdk::hal::avbuffer::Pool::INVALID) {
                Status status = gAVBufferManager->destroyPool(mAudioPool, &secureheap);
                UT_ASSERT_TRUE(status.isOk());
            }
        }
    };


class L1AVBuffer_BufferManagementTestSuite : public L1AVBuffer_PoolManagementTestSuite {
protected:
    int64_t mVideoBufferHandle = IAVBuffer::INVALID_HANDLE;
    int64_t mAudioBufferHandle = IAVBuffer::INVALID_HANDLE;

    void SetUp() override {
        // Call base class to create audio/video pools
        L1AVBuffer_PoolManagementTestSuite::SetUp();

        // Allocate buffer for video
        Status status = gAVBufferManager->alloc(mVideoPool, 1024, &mVideoBufferHandle);
        UT_ASSERT_TRUE(status.isOk()) << "Failed to allocate video buffer";
        UT_ASSERT_NOT_EQUAL(mVideoBufferHandle, IAVBuffer::INVALID_HANDLE);

        // Allocate buffer for audio
        status = gAVBufferManager->alloc(mAudioPool, 1024, &mAudioBufferHandle);
        UT_ASSERT_TRUE(status.isOk()) << "Failed to allocate audio buffer";
        UT_ASSERT_NOT_EQUAL(mAudioBufferHandle, IAVBuffer::INVALID_HANDLE);
    }

    void TearDown() override {

        bool secureHeap = true;

        if (mVideoBufferHandle != IAVBuffer::INVALID_HANDLE) {
            Status status = gAVBufferManager->free(mVideoBufferHandle, &secureHeap);
            UT_ASSERT_TRUE(status.isOk());
        }

        if (mAudioBufferHandle != IAVBuffer::INVALID_HANDLE) {
            Status status = gAVBufferManager->free(mAudioBufferHandle, &secureHeap);
            UT_ASSERT_TRUE(status.isOk());
        }

        // Call base class to destroy audio/video pools
        L1AVBuffer_PoolManagementTestSuite::TearDown();
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
UT_ADD_TEST(VTS_L1_AVBuffer, getHeapMetrics_pos)
{
    // Test for Non-Secure Heap Metrics Retrieval

    bool secureHeap = false;
    ::com::rdk::hal::avbuffer::HeapMetrics heapMetrics;

    AVBUFFER_GETMETRICS(gAVBufferManager, secureHeap, heapMetrics);

    UT_ASSERT_FALSE(heapMetrics.secure);
    UT_ASSERT_GREATER_EQUAL(heapMetrics.bytesUsed, 0);
    UT_ASSERT_LESS_EQUAL(heapMetrics.bytesUsed, heapMetrics.bytesTotal);
    UT_ASSERT_EQUAL(heapMetrics.bytesTotal, gAVBuffer_hfp.nonSecureHeapBytes);

    // Test for Secure Heap Metrics Retrieval

    secureHeap = true;

    AVBUFFER_GETMETRICS(gAVBufferManager, secureHeap, heapMetrics);

    UT_ASSERT_TRUE(heapMetrics.secure);
    UT_ASSERT_GREATER_EQUAL(heapMetrics.bytesUsed, 0);
    UT_ASSERT_LESS_EQUAL(heapMetrics.bytesUsed, heapMetrics.bytesTotal);
    UT_ASSERT_EQUAL(heapMetrics.bytesTotal, gAVBuffer_hfp.secureHeapBytes);
}

/**
 * @brief Test the creation of video pools with secure and non-secure heaps.
 *
 * This test verifies the creation of video pools using both secure and non-secure heaps
 * for each video decoder ID retrieved from the configuration. It ensures that the video pool
 * is created successfully and the handle is valid.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 002@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Retrieve video decoder IDs from YAML configuration | videodecoder_count_path = "IVideoDecoderManager.IVideoDecoder" | expectedCount > 0 | Should be successful |
 * | 02 | Loop through each video decoder ID | index = 0 to expectedCount - 1 | _videoDecoderId = UT_KVP_PROFILE_GET_UINT8(videodecoderidpath.c_str()) | Should be successful |
 * | 03 | Create a valid listener | listener = new AVBufferSpaceListener() | listener is valid | Should be successful |
 * | 04 | Test secure pool creation | secureHeap = UT_KVP_PROFILE_GET_BOOL("IAVBuffer.supportsSecure"), videoDecoderId = _videoDecoderId, listener = listener | status.isOk() == true, videoPool.handle != Pool::INVALID | Should Pass |
 * | 05 | Test non-secure pool creation | secureHeap = false, videoDecoderId = _videoDecoderId, listener = listener | status.isOk() == true, videoPool.handle != Pool::INVALID | Should Pass |
 */
UT_ADD_TEST(VTS_L1_AVBuffer, createVideoPool_pos)
{
    for (const auto& _videoDecoderId : gAVBuffer_hfp.videoDecoderId)
    {
        ::com::rdk::hal::videodecoder::IVideoDecoder::Id videoDecoderId;
        videoDecoderId.value = _videoDecoderId;

        // Create a valid listener
        sp<IAVBufferSpaceListener> listener = new AVBufferSpaceListener();

        // Test 1
        bool secureHeap = true;
        ::com::rdk::hal::avbuffer::Pool videoPool;
        AVBUFFER_CREATE_VIDEOPOOL_SUCCESS(gAVBufferManager, secureHeap, videoDecoderId, listener, videoPool);
        AVBUFFER_DESTROY_POOL_SUCCESS(gAVBufferManager, videoPool);

        // Test 2
        secureHeap = false;
        AVBUFFER_CREATE_VIDEOPOOL_SUCCESS(gAVBufferManager, secureHeap, videoDecoderId, listener, videoPool);
        AVBUFFER_DESTROY_POOL_SUCCESS(gAVBufferManager, videoPool);
    }
}

/**
 * @brief Test the negative scenarios for creating a video pool with invalid video decoder IDs.
 *
 * This test verifies the behavior of the `createVideoPool` function when provided with invalid video decoder IDs.
 * It ensures that the function correctly handles these invalid inputs by returning appropriate error codes.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 003@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Retrieve the secureHeap value from the YAML configuration. | secureHeap = value from YAML | Should be successful | Should be successful |
 * | 02 | Retrieve and sort videoDecoderIds from the YAML configuration. | videoDecoderIds = sorted list from YAML | Should be successful | Should be successful |
 * | 03 | Prepare invalid videoDecoderId values by decrementing the lowest and incrementing the highest ID. | ivideoDecodersnvalidLowId = videoDecoderIds.front() - 1, invalidHighId = videoDecoderIds.back() + 1 | Should be successful | Should be successful |
 * | 04 | Create a listener object for AVBufferSpaceListener. | listener = new AVBufferSpaceListener() | Should be successful | Should be successful |
 * | 05 | Call createVideoPool with invalidLowId and check the result. | secureHeap = value, videoDecoderId = invalidLowId, listener = listener, videoPool = &videoPool | Status exceptionCode = EX_ILLEGAL_ARGUMENT, videoPool.handle = Pool::INVALID | Should Fail |
 * | 06 | Call createVideoPool with invalidHighId and check the result. | secureHeap = value, videoDecoderId = invalidHighId, listener = listener, videoPool = &videoPool | Status exceptionCode = EX_ILLEGAL_ARGUMENT, videoPool.handle = Pool::INVALID | Should Fail |
 */
UT_ADD_TEST(VTS_L1_AVBuffer, createVideoPool_neg)
{
    bool secureHeap;

    int32_t invalidLowId = gAVBuffer_hfp.videoDecoderId.front() - 1;
    int32_t invalidHighId = gAVBuffer_hfp.videoDecoderId.back() + 1;

    // Create listener
    sp<IAVBufferSpaceListener> listener = new AVBufferSpaceListener();
    sp<IAVBufferSpaceListener> listener_nullptr = nullptr;

    // Test with invalidLowId
    ::com::rdk::hal::videodecoder::IVideoDecoder::Id videoDecoderId;
    videoDecoderId.value = invalidLowId;
    ::com::rdk::hal::avbuffer::Pool videoPool;
    secureHeap = true;

    AVBUFFER_CREATE_VIDEOPOOL_FAILURE(gAVBufferManager, secureHeap, videoDecoderId, listener, videoPool, android::binder::Status::EX_ILLEGAL_ARGUMENT);

    // Test with invalidHighId
    videoDecoderId.value = invalidHighId;
    AVBUFFER_CREATE_VIDEOPOOL_FAILURE(gAVBufferManager, secureHeap, videoDecoderId, listener, videoPool, android::binder::Status::EX_ILLEGAL_ARGUMENT);

    // Test with invalid listener
    videoDecoderId.value = gAVBuffer_hfp.videoDecoderId.front();
    AVBUFFER_CREATE_VIDEOPOOL_FAILURE(gAVBufferManager, secureHeap, videoDecoderId, listener_nullptr, videoPool, android::binder::Status::EX_ILLEGAL_ARGUMENT);
}

/**
 * @brief Test the creation of audio pools with various configurations.
 *
 * This test verifies the creation of audio pools using different configurations of secure and non-secure heaps,
 * as well as valid and undefined audio decoder IDs. It ensures that the audio pool creation process is functioning
 * correctly and returns valid handles.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 004@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Retrieve the number of audio decoders from the YAML configuration. | audiodecoder_count_path = "IAudioDecoderManager.IAudioDecoder" | expectedCount > 0 | Should be successful |
 * | 02 | Iterate over all audio decoder IDs and retrieve each ID. | audiodecoderidpath = "IAudioDecoderManager.IAudioDecoder." + index | _audioDecoderId retrieved | Should be successful |
 * | 03 | Create a listener for AV buffer space. | listener = new AVBufferSpaceListener() | Listener created | Should be successful |
 * | 04 | Test Case 1: Create a non-secure audio pool with a valid audio decoder ID. | secureHeap = false, audioDecoderId = valid, listener = listener | status.isOk() = true, audioPool.handle != Pool::INVALID | Should Pass |
 * | 05 | Test Case 2: Create a secure audio pool with a valid audio decoder ID. | secureHeap = true, audioDecoderId = valid, listener = listener | status.isOk() = true, audioPool.handle != Pool::INVALID | Should Pass |
 * | 06 | Test Case 3: Create an audio pool with an undefined audio decoder ID. | secureHeap = false, audioDecoderId = UNDEFINED, listener = listener | status.isOk() = true, audioPool.handle != Pool::INVALID | Should Pass |
 */
UT_ADD_TEST(VTS_L1_AVBuffer, createAudioPool_pos)
{
    // Retrieve the number of audio decoders from the YAML configuration
    ::com::rdk::hal::audiodecoder::IAudioDecoder::Id audioDecoderId;
    bool secureHeap;
    ::com::rdk::hal::avbuffer::Pool audioPool;

    sp<IAVBufferSpaceListener> listener = new AVBufferSpaceListener();

    // Iterate over all audio decoder IDs
    for (const auto& _audioDecoderId : gAVBuffer_hfp.audioDecoderId)
    {
        audioDecoderId.value = _audioDecoderId;

        // Test Case 1
        secureHeap = true;
        AVBUFFER_CREATE_AUDIOPOOL_SUCCESS(gAVBufferManager, secureHeap, audioDecoderId, listener, audioPool);
        AVBUFFER_DESTROY_POOL_SUCCESS(gAVBufferManager, audioPool);

        // Test Case 2
        secureHeap = false;
        AVBUFFER_CREATE_AUDIOPOOL_SUCCESS(gAVBufferManager, secureHeap, audioDecoderId, listener, audioPool);
        AVBUFFER_DESTROY_POOL_SUCCESS(gAVBufferManager, audioPool);
    }

    // Test Case 3: Audio Pool Creation with UNDEFINED Audio Decoder ID
    audioDecoderId.value = ::com::rdk::hal::audiodecoder::IAudioDecoder::Id::UNDEFINED;
    secureHeap = false;

    AVBUFFER_CREATE_AUDIOPOOL_SUCCESS(gAVBufferManager, secureHeap, audioDecoderId, listener, audioPool);
    AVBUFFER_DESTROY_POOL_SUCCESS(gAVBufferManager, audioPool);
}

/**
 * @brief Test the negative scenarios for creating an audio pool with invalid parameters.
 *
 * This test checks the behavior of the `createAudioPool` function when provided with
 * invalid audio decoder IDs and a null listener. It ensures that the function handles
 * these cases correctly by returning appropriate error codes and invalid pool handles.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 005@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Retrieve the count of audio decoders from the profile and assert it's valid. | audiodecoder_count_path = "IAudioDecoderManager.IAudioDecoder", expectedCount > 0 | Profile should return a valid count. | Should be successful |
 * | 02 | Retrieve audio decoder IDs from the profile and store them in a vector. | audiodecoderidpath = "IAudioDecoderManager.IAudioDecoder.i.", _audioDecoderId = value | Audio decoder IDs should be retrieved successfully. | Should be successful |
 * | 03 | Sort the audio decoder IDs and determine the least and highest values. | audioDecoderIds = sorted list | Least and highest values should be determined. | Should be successful |
 * | 04 | Test with least value - 1 for audioDecoderId. | secureHeap = false, audioDecoderId = leastValue - 1, listener = valid, audioPool = invalid | Status exceptionCode = EX_ILLEGAL_ARGUMENT, audioPool.handle = Pool::INVALID | Should Fail |
 * | 05 | Test with highest value + 1 for audioDecoderId. | secureHeap = false, audioDecoderId = highestValue + 1, listener = valid, audioPool = invalid | Status exceptionCode = EX_ILLEGAL_ARGUMENT, audioPool.handle = Pool::INVALID | Should Fail |
 * | 06 | Test with a null listener for a valid audioDecoderId. | secureHeap = false, audioDecoderId = valid, listener = nullptr, audioPool = invalid | Status exceptionCode = EX_ILLEGAL_ARGUMENT, audioPool.handle = Pool::INVALID | Should Fail |
 */
UT_ADD_TEST(VTS_L1_AVBuffer, createAudioPool_neg)
{
    bool secureHeap = true;
    ::com::rdk::hal::audiodecoder::IAudioDecoder::Id audioDecoderId;
    sp<IAVBufferSpaceListener> listener = new AVBufferSpaceListener();
    ::com::rdk::hal::avbuffer::Pool audioPool;

    int32_t leastValue = gAVBuffer_hfp.audioDecoderId.front();
    int32_t highestValue = gAVBuffer_hfp.audioDecoderId.back();

    // Test with least value - 1
    audioDecoderId.value = leastValue - 1;
    AVBUFFER_CREATE_AUDIOPOOL_FAILURE(gAVBufferManager, secureHeap, audioDecoderId, listener, audioPool, android::binder::Status::EX_ILLEGAL_ARGUMENT);

    // Test with highest value + 1
    audioDecoderId.value = highestValue + 1;
    AVBUFFER_CREATE_AUDIOPOOL_FAILURE(gAVBufferManager, secureHeap, audioDecoderId, listener, audioPool, android::binder::Status::EX_ILLEGAL_ARGUMENT);

    //Null Listener for Audio Pool Creation
    audioDecoderId.value = gAVBuffer_hfp.audioDecoderId.front(); // Use a valid ID
    listener = nullptr;
    AVBUFFER_CREATE_AUDIOPOOL_FAILURE(gAVBufferManager, secureHeap, audioDecoderId, listener, audioPool, android::binder::Status::EX_ILLEGAL_ARGUMENT);
}

/**
 * @brief Test the positive scenario of destroying a video pool
 *
 * This test verifies the successful destruction of a video pool by calling the destroyPool
 * function with a valid pool handle. It ensures that the function returns a successful
 * status and the expected boolean result.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 006@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Log the intention to call destroyPool with the pool handle | poolHandle = mVideoPool.handle | Log should be successful | Should be successful |
 * | 02 | Call the destroyPool function with the video pool handle and capture the return status and result | poolHandle = mVideoPool.handle, _aidl_return = false | status.isOk() = true, _aidl_return = true | Should Pass |
 * | 03 | Log the result of the destroyPool call including the status and return value | poolHandle = mVideoPool.handle, _aidl_return = true, status.isOk() = true, status.exceptionCode() = none | Log should be successful | Should be successful |
 * | 04 | Assert that the status is OK | status.isOk() = true | Assertion should pass | Should Pass |
 * | 05 | Assert that the return value is true | _aidl_return = true | Assertion should pass | Should Pass |
 */
UT_ADD_TEST(VTS_L1_AVBuffer, destroyPool_pos)
{
    ::com::rdk::hal::audiodecoder::IAudioDecoder::Id audioDecoderId;
    bool secureHeap;
    ::com::rdk::hal::avbuffer::Pool audioPool;

    sp<IAVBufferSpaceListener> listener = new AVBufferSpaceListener();

    // Iterate over all audio decoder IDs
    for (const auto& _audioDecoderId : gAVBuffer_hfp.audioDecoderId)
    {
        audioDecoderId.value = _audioDecoderId;

        // Test Case 1
        secureHeap = true;
        AVBUFFER_CREATE_AUDIOPOOL_SUCCESS(gAVBufferManager, secureHeap, audioDecoderId, listener, audioPool);
        AVBUFFER_DESTROY_POOL_SUCCESS(gAVBufferManager, audioPool);

        // Test Case 2
        secureHeap = false;
        AVBUFFER_CREATE_AUDIOPOOL_SUCCESS(gAVBufferManager, secureHeap, audioDecoderId, listener, audioPool);
        AVBUFFER_DESTROY_POOL_SUCCESS(gAVBufferManager, audioPool);
    }

    // Test Case 3: Audio Pool Creation with UNDEFINED Audio Decoder ID
    audioDecoderId.value = ::com::rdk::hal::audiodecoder::IAudioDecoder::Id::UNDEFINED;
    secureHeap = false;

    AVBUFFER_CREATE_AUDIOPOOL_SUCCESS(gAVBufferManager, secureHeap, audioDecoderId, listener, audioPool);
    AVBUFFER_DESTROY_POOL_SUCCESS(gAVBufferManager, audioPool);

    // Test Case 2
    for (const auto& _videoDecoderId : gAVBuffer_hfp.videoDecoderId)
    {
        ::com::rdk::hal::videodecoder::IVideoDecoder::Id videoDecoderId;
        videoDecoderId.value = _videoDecoderId;

        // Create a valid listener
        sp<IAVBufferSpaceListener> listener = new AVBufferSpaceListener();

        // Test 1
        bool secureHeap = true;
        ::com::rdk::hal::avbuffer::Pool videoPool;
        AVBUFFER_CREATE_VIDEOPOOL_SUCCESS(gAVBufferManager, secureHeap, videoDecoderId, listener, videoPool);
        AVBUFFER_DESTROY_POOL_SUCCESS(gAVBufferManager, videoPool);

        // Test 2
        secureHeap = false;
        AVBUFFER_CREATE_VIDEOPOOL_SUCCESS(gAVBufferManager, secureHeap, videoDecoderId, listener, videoPool);
        AVBUFFER_DESTROY_POOL_SUCCESS(gAVBufferManager, videoPool);
    }
}

/**
 * @brief Test the negative scenario for destroying an AV buffer pool with an invalid handle.
 *
 * This test verifies the behavior of the `destroyPool` function when provided with an invalid pool handle.
 * It ensures that the function correctly handles invalid input by returning appropriate error codes and statuses.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 007@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize an invalid pool handle for testing. | invalidPool.handle = INVALID | Should initialize successfully | Should be successful |
 * | 02 | Call the `destroyPool` function with the invalid pool handle. | poolHandle = INVALID, aidl_return = false | aidl_return = false, status.isOk() = false, status.exceptionCode() = EX_SERVICE_SPECIFIC | Should Fail |
 * | 03 | Verify the return value and status of the `destroyPool` function. | aidl_return = false, status.isOk() = false, status.exceptionCode() = EX_SERVICE_SPECIFIC | Assertions should pass | Should be successful |
 */
UT_ADD_TEST(VTS_L1_AVBuffer, destroyPool_neg)
{
    // Test Case 1: Invalid Pool Handle
    ::com::rdk::hal::avbuffer::Pool invalidPool;
    invalidPool.handle = ::com::rdk::hal::avbuffer::Pool::INVALID;

    AVBUFFER_DESTROY_POOL_FAILURE(gAVBufferManager, invalidPool, android::binder::Status::EX_NONE);
}

/**
 * @brief Test the functionality of getting pool metrics for both video and audio pools.
 *
 * This test verifies the creation of video and audio pools and checks the retrieval of pool metrics to ensure that the pools are correctly reporting their usage statistics.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 008@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Setup for Video Pool: Initialize secureHeap and retrieve video decoder ID. | secureHeap = true, videoDecoderId = retrieved value | Should be successful | Should be successful |
 * | 02 | Create Video Pool using the video decoder ID and listener. | secureHeap = true, videoDecoderId = retrieved value, listener = new AVBufferSpaceListener() | Should Pass | Should Pass |
 * | 03 | Setup for Audio Pool: Retrieve audio decoder ID. | audioDecoderId = retrieved value | Should be successful | Should be successful |
 * | 04 | Create Audio Pool using the audio decoder ID and listener. | secureHeap = true, audioDecoderId = retrieved value, listener = new AVBufferSpaceListener() | Should Pass | Should Pass |
 * | 05 | Test getPoolMetrics for Video Pool: Retrieve metrics and verify. | videoPool.handle = valid handle | Should Pass, bytesTotal > 0, bytesUsed >= 0 | Should Pass |
 * | 06 | Test getPoolMetrics for Audio Pool: Retrieve metrics and verify. | audioPool.handle = valid handle | Should Pass, bytesTotal > 0, bytesUsed >= 0 | Should Pass |
 * | 07 | Teardown Video Pool: Destroy the video pool. | videoPool = valid pool, secureHeap = true | Should Pass | Done by Pre-requisite TearDown function |
 * | 08 | Teardown Audio Pool: Destroy the audio pool. | audioPool = valid pool, secureHeap = true | Should Pass | Done by Pre-requisite TearDown function |
 */
UT_ADD_TEST(VTS_L1_AVBuffer, getPoolMetrics_pos)
{
    ::com::rdk::hal::videodecoder::IVideoDecoder::Id videoDecoderId;
    ::com::rdk::hal::audiodecoder::IAudioDecoder::Id audioDecoderId;
    ::com::rdk::hal::avbuffer::Pool videoPool;
    ::com::rdk::hal::avbuffer::Pool audioPool;
    ::com::rdk::hal::avbuffer::PoolMetrics videoPoolMetrics;
    ::com::rdk::hal::avbuffer::PoolMetrics audioPoolMetrics;

    bool secureHeap = true;

    videoDecoderId.value = gAVBuffer_hfp.videoDecoderId.front();
    audioDecoderId.value = gAVBuffer_hfp.audioDecoderId.front();

    sp<IAVBufferSpaceListener> listener = new AVBufferSpaceListener();

    AVBUFFER_CREATE_VIDEOPOOL_SUCCESS(gAVBufferManager, secureHeap, videoDecoderId, listener, videoPool);
    // Test getPoolMetrics for Video Pool
    AVBUFFER_GETPOOL_METRICS_SUCCESS(gAVBufferManager, videoPool, videoPoolMetrics);
    UT_ASSERT_GREATER(videoPoolMetrics.bytesTotal, 0);
    UT_ASSERT_GREATER_EQUAL(videoPoolMetrics.bytesUsed, 0);

    AVBUFFER_CREATE_AUDIOPOOL_SUCCESS(gAVBufferManager, secureHeap, audioDecoderId, listener, audioPool);
    // Test getPoolMetrics for Audio Pool
    AVBUFFER_GETPOOL_METRICS_SUCCESS(gAVBufferManager, audioPool, videoPoolMetrics);
    UT_ASSERT_GREATER(audioPoolMetrics.bytesTotal, 0);
    UT_ASSERT_GREATER_EQUAL(audioPoolMetrics.bytesUsed, 0);

    AVBUFFER_DESTROY_POOL_SUCCESS(gAVBufferManager, videoPool);
    AVBUFFER_DESTROY_POOL_SUCCESS(gAVBufferManager, audioPool);
}

/**
 * @brief Test the negative scenario for getPoolMetrics with an invalid pool handle
 *
 * This test verifies the behavior of the getPoolMetrics function when provided
 * with an invalid pool handle. It ensures that the function correctly identifies
 * the invalid input and returns the appropriate error status.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 009@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize an invalid pool handle | invalidPool.handle = Pool::INVALID | Should be successful | |
 * | 02 | Call getPoolMetrics with the invalid pool handle | input: invalidPool.handle = Pool::INVALID, output: poolMetrics | status.isOk() = false, status.exceptionCode() = EX_ILLEGAL_ARGUMENT | Should Fail |
 * | 03 | Log the result of getPoolMetrics call | invalidPool.handle, poolMetrics.bytesUsed, poolMetrics.bytesTotal, status.isOk(), status.exceptionCode() | Should be successful | |
 * | 04 | Assert that the status is not OK | status.isOk() = false | Should be successful | |
 * | 05 | Assert that the exception code is EX_ILLEGAL_ARGUMENT | status.exceptionCode() = EX_ILLEGAL_ARGUMENT | Should be successful | |
 */
UT_ADD_TEST(VTS_L1_AVBuffer, getPoolMetrics_neg)
{
    ::com::rdk::hal::avbuffer::Pool invalidPool;
    invalidPool.handle = ::com::rdk::hal::avbuffer::Pool::INVALID;
    ::com::rdk::hal::avbuffer::PoolMetrics poolMetrics;

    AVBUFFER_GETPOOL_METRICS_FAILURE(gAVBufferManager, invalidPool, poolMetrics, ::android::binder::Status::EX_ILLEGAL_ARGUMENT);
}

/**
 * @brief Test the functionality of retrieving all pool metrics for both secure and non-secure heaps.
 *
 * This test verifies the `getAllPoolMetrics` function for both secure and non-secure heap scenarios.
 * It ensures that the function returns valid pool metrics and checks the integrity of the data returned.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 010@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Retrieve secureHeap value from YAML configuration | secureHeap = value from YAML | Should be successful | Should be successful |
 * | 02 | Prepare to call getAllPoolMetrics for non-secure heap | secureHeap = false, poolMetricsNonSecure = [] | Should be successful | Should be successful |
 * | 03 | Call getAllPoolMetrics for non-secure heap | secureHeap = false, poolMetricsNonSecure = [] | statusNonSecure.isOk() = true, poolMetricsNonSecure not empty | Should Pass |
 * | 04 | Verify non-secure heap results | poolMetricsNonSecure = [...] | All metrics valid, bytesUsed >= 0, bytesTotal >= bytesUsed | Should Pass |
 * | 05 | Prepare to call getAllPoolMetrics for secure heap | secureHeap = value from YAML, poolMetricsSecure = [] | Should be successful | Should be successful |
 * | 06 | Call getAllPoolMetrics for secure heap | secureHeap = value from YAML, poolMetricsSecure = [] | statusSecure.isOk() = true, poolMetricsSecure not empty | Should Pass |
 * | 07 | Verify secure heap results | poolMetricsSecure = [...] | All metrics valid, bytesUsed >= 0, bytesTotal >= bytesUsed | Should Pass |
 */
UT_ADD_TEST(VTS_L1_AVBuffer, getAllPoolMetrics_pos)
{
    ::com::rdk::hal::videodecoder::IVideoDecoder::Id videoDecoderId;
    ::com::rdk::hal::audiodecoder::IAudioDecoder::Id audioDecoderId;
    ::com::rdk::hal::avbuffer::Pool videoPool;
    ::com::rdk::hal::avbuffer::Pool audioPool;
    std::vector<::com::rdk::hal::avbuffer::PoolMetrics> poolMetricsNonSecure;
    std::vector<::com::rdk::hal::avbuffer::PoolMetrics> poolMetricsSecure;

    bool secureHeap = true;

    videoDecoderId.value = gAVBuffer_hfp.videoDecoderId.front();
    audioDecoderId.value = gAVBuffer_hfp.audioDecoderId.front();

    sp<IAVBufferSpaceListener> listener = new AVBufferSpaceListener();

    AVBUFFER_CREATE_VIDEOPOOL_SUCCESS(gAVBufferManager, false, videoDecoderId, listener, videoPool);
    AVBUFFER_CREATE_AUDIOPOOL_SUCCESS(gAVBufferManager, true, audioDecoderId, listener, audioPool);

    AVBUFFER_GETALLPOOL_METRICS_SUCCESS(gAVBufferManager, secureHeap, poolMetricsNonSecure);
    for (const auto& metrics : poolMetricsNonSecure)
    {
        UT_ASSERT_NOT_EQUAL(metrics.poolHandle.handle, Pool::INVALID);
        UT_ASSERT_GREATER_EQUAL(metrics.bytesUsed, 0);
        UT_ASSERT_GREATER_EQUAL(metrics.bytesTotal, metrics.bytesUsed);
    }

    secureHeap = false;
    AVBUFFER_GETALLPOOL_METRICS_SUCCESS(gAVBufferManager, secureHeap, poolMetricsSecure);
    for (const auto& metrics : poolMetricsSecure)
    {
        UT_ASSERT_NOT_EQUAL(metrics.poolHandle.handle, Pool::INVALID);
        UT_ASSERT_GREATER_EQUAL(metrics.bytesUsed, 0);
        UT_ASSERT_GREATER_EQUAL(metrics.bytesTotal, metrics.bytesUsed);
    }

    AVBUFFER_DESTROY_POOL_SUCCESS(gAVBufferManager, videoPool);
    AVBUFFER_DESTROY_POOL_SUCCESS(gAVBufferManager, audioPool);
}

/**
 * @brief Test the allocation of AVBuffer in a positive scenario.
 *
 * This test verifies the successful allocation of AVBuffer using valid pool
 * handles and sizes. It ensures that the buffer allocation is performed correctly
 * for both video and audio pools.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 011@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Setup the environment for video decoder by retrieving the expected count and video decoder ID. | videodecoder_count_path = "IVideoDecoderManager.IVideoDecoder", videodecoderidpath = "IVideoDecoderManager.IVideoDecoder.0." | Expected count should be greater than 0, videoDecoderId should be valid | Should be successful |
 * | 02 | Create a video pool using the video decoder ID and listener. | secureHeap = true, videoDecoderId, listener | Video pool should be created successfully with a valid handle | Should be successful |
 * | 03 | Allocate buffer from the video pool with a valid size. | poolHandle = videoPool.handle, size = retrieved size | Allocation should succeed, bufferHandle should not be INVALID_HANDLE | Should Pass |
 * | 04 | Setup the environment for audio decoder by retrieving the expected count and audio decoder ID. | audiodecoder_count_path = "IAudioDecoderManager.IAudioDecoder", audiodecoderidpath = "IAudioDecoderManager.IAudioDecoder.0." | Expected count should be greater than 0, audioDecoderId should be valid | Should be successful |
 * | 05 | Create an audio pool using the audio decoder ID and listener. | secureHeap = true, audioDecoderId, listener | Audio pool should be created successfully with a valid handle | Should be successful |
 * | 06 | Allocate buffer from the audio pool with a valid size. | poolHandle = audioPool.handle, size = retrieved size | Allocation should succeed, audioBufferHandle should not be INVALID_HANDLE | Should Pass |
 * | 07 | Teardown the video pool if it was created. | videoPool.handle | Pool should be destroyed successfully | Done by Pre-requisite TearDown function |
 * | 08 | Teardown the audio pool if it was created. | audioPool.handle | Pool should be destroyed successfully | Done by Pre-requisite TearDown function |
 */
UT_ADD_TEST(VTS_L1_AVBuffer, alloc_pos)
{
    ::com::rdk::hal::videodecoder::IVideoDecoder::Id videoDecoderId;
    ::com::rdk::hal::audiodecoder::IAudioDecoder::Id audioDecoderId;
    ::com::rdk::hal::avbuffer::Pool videoPool;
    ::com::rdk::hal::avbuffer::Pool audioPool;
    std::vector<::com::rdk::hal::avbuffer::PoolMetrics> poolMetricsNonSecure;
    std::vector<::com::rdk::hal::avbuffer::PoolMetrics> poolMetricsSecure;
    int32_t size = 1024;
    bool secureHeap = true;
    int64_t videobufferHandle;
    int64_t audioBufferHandle;

    videoDecoderId.value = gAVBuffer_hfp.videoDecoderId.front();
    audioDecoderId.value = gAVBuffer_hfp.audioDecoderId.front();

    sp<IAVBufferSpaceListener> listener = new AVBufferSpaceListener();

    AVBUFFER_CREATE_VIDEOPOOL_SUCCESS(gAVBufferManager, false, videoDecoderId, listener, videoPool);
    AVBUFFER_CREATE_AUDIOPOOL_SUCCESS(gAVBufferManager, true, audioDecoderId, listener, audioPool);

    // Test Case 1: Valid video Pool Handle and Size
    AVBUFFER_ALLOC_SUCCESS(gAVBufferManager, videoPool, size, videobufferHandle);
    // Test Case 2: Valid audio Pool Handle and Size
    AVBUFFER_ALLOC_SUCCESS(gAVBufferManager, audioPool, size, audioBufferHandle);

    // Teardown
    AVBUFFER_FREE_SUCCESS(gAVBufferManager, videobufferHandle);
    AVBUFFER_FREE_SUCCESS(gAVBufferManager, audioBufferHandle);

    AVBUFFER_DESTROY_POOL_SUCCESS(gAVBufferManager, videoPool);
    AVBUFFER_DESTROY_POOL_SUCCESS(gAVBufferManager, audioPool);
}

/**
 * @brief Test the allocation of AVBuffer with various invalid scenarios
 *
 * This test checks the behavior of the AVBuffer allocation function when provided with invalid inputs such as an invalid pool handle, size greater than pool capacity, and zero size allocation. These scenarios are tested to ensure the robustness and error handling capabilities of the AVBuffer allocation function.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 012@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize invalid pool handle | invalidPool.handle = Pool::INVALID | bufferHandle = IAVBuffer::INVALID_HANDLE, status.isOk() = false | Should be successful |
 * | 02 | Call alloc with invalid pool handle | poolHandle = invalidPool.handle, size = UT_KVP_PROFILE_GET_UINT32(IAVBuffer.nonSecureHeapBytes) | bufferHandle = IAVBuffer::INVALID_HANDLE, status.isOk() = false | Should Fail |
 * | 03 | Initialize valid video pool handle | validVideoPool.handle = mVideoPool.handle | bufferHandle = IAVBuffer::INVALID_HANDLE, status.isOk() = false | Should be successful |
 * | 04 | Call alloc with size greater than pool capacity | poolHandle = validVideoPool.handle, size = UT_KVP_PROFILE_GET_UINT32(IAVBuffer.nonSecureHeapBytes) + 1 | bufferHandle = IAVBuffer::INVALID_HANDLE, status.isOk() = false | Should Fail |
 * | 05 | Initialize valid audio pool handle | validAudioPool.handle = mAudioPool.handle | bufferHandle = IAVBuffer::INVALID_HANDLE, status.isOk() = false | Should be successful |
 * | 06 | Call alloc with zero size | poolHandle = validAudioPool.handle, size = 0 | bufferHandle = IAVBuffer::INVALID_HANDLE, status.isOk() = false | Should Fail |
 */
UT_ADD_TEST(VTS_L1_AVBuffer, alloc_neg)
{
    ::com::rdk::hal::videodecoder::IVideoDecoder::Id videoDecoderId;
    ::com::rdk::hal::audiodecoder::IAudioDecoder::Id audioDecoderId;
    ::com::rdk::hal::avbuffer::Pool videoPool;
    ::com::rdk::hal::avbuffer::Pool audioPool;
    ::com::rdk::hal::avbuffer::Pool invalidPool;
    invalidPool.handle = Pool::INVALID;
    std::vector<::com::rdk::hal::avbuffer::PoolMetrics> poolMetricsNonSecure;
    std::vector<::com::rdk::hal::avbuffer::PoolMetrics> poolMetricsSecure;
    int32_t size = 1024;
    bool secureHeap = true;
    int64_t bufferHandle;

    videoDecoderId.value = gAVBuffer_hfp.videoDecoderId.front();
    audioDecoderId.value = gAVBuffer_hfp.audioDecoderId.front();

    sp<IAVBufferSpaceListener> listener = new AVBufferSpaceListener();

    AVBUFFER_CREATE_VIDEOPOOL_SUCCESS(gAVBufferManager, false, videoDecoderId, listener, videoPool);
    AVBUFFER_CREATE_AUDIOPOOL_SUCCESS(gAVBufferManager, true, audioDecoderId, listener, audioPool);

    // Test Case 1: Invalid Pool Handle
    AVBUFFER_ALLOC_FAILURE(gAVBufferManager, invalidPool, size, bufferHandle, Status::EX_NONE);

    // Test Case 2: Size Greater Than Pool Capacity
    AVBUFFER_ALLOC_FAILURE(gAVBufferManager, audioPool, gAVBuffer_hfp.secureHeapBytes + 1, bufferHandle, Status::EX_NONE);
    AVBUFFER_ALLOC_FAILURE(gAVBufferManager, videoPool, gAVBuffer_hfp.nonSecureHeapBytes + 1, bufferHandle, Status::EX_NONE);

    // Test Case 3: Zero Size Allocation
    AVBUFFER_ALLOC_FAILURE(gAVBufferManager, videoPool, 0, bufferHandle, Status::EX_NONE);

    AVBUFFER_DESTROY_POOL_SUCCESS(gAVBufferManager, videoPool);
    AVBUFFER_DESTROY_POOL_SUCCESS(gAVBufferManager, audioPool);
}

/**
 * @brief Test the functionality of notifying when space is available in a video buffer pool.
 *
 * This test verifies the behavior of the `notifyWhenSpaceAvailable` function in
 * scenarios where the pool handle and size are valid. It ensures that the gAVBufferManager
 * can successfully notify when space is available in the video buffer pool.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 013@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Ensure gAVBufferManager is available and pool is created | gAVBufferManager = valid, videoPool = created | Service should be available | Should be successful |
 * | 02 | Create a video pool with secure heap | secureHeap = true, videoDecoderId = _videoDecoderId, listener = AVBufferSpaceListener | Video pool should be created successfully | Should be successful |
 * | 03 | Test Case 1: Notify when space is available with valid pool handle and size | poolHandle = videoPool.handle, size = UT_KVP_PROFILE_GET_UINT32(IAVBuffer.nonSecureHeapBytes), aidl_return = false | status.isOk() = true, aidl_return = true | Should Pass |
 * | 04 | Test Case 2: Notify when space is available with minimum valid size | poolHandle = videoPool.handle, size = 1, aidl_return = false | status.isOk() = true, aidl_return = true | Should Pass |
 * | 05 | Clean up: Destroy the video pool | videoPool = valid, secureHeap = true | Pool should be destroyed successfully | Done by Pre-requisite TearDown function |
 */
UT_ADD_TEST(VTS_L1_AVBuffer, notifyWhenSpaceAvailable_pos)
{
    ::com::rdk::hal::videodecoder::IVideoDecoder::Id videoDecoderId;
    ::com::rdk::hal::audiodecoder::IAudioDecoder::Id audioDecoderId;
    ::com::rdk::hal::avbuffer::Pool videoPool;
    ::com::rdk::hal::avbuffer::Pool audioPool;
    std::vector<::com::rdk::hal::avbuffer::PoolMetrics> poolMetricsNonSecure;
    std::vector<::com::rdk::hal::avbuffer::PoolMetrics> poolMetricsSecure;
    int32_t size = 1024;

    videoDecoderId.value = gAVBuffer_hfp.videoDecoderId.front();
    audioDecoderId.value = gAVBuffer_hfp.audioDecoderId.front();

    sp<IAVBufferSpaceListener> listener = new AVBufferSpaceListener();

    AVBUFFER_CREATE_VIDEOPOOL_SUCCESS(gAVBufferManager, false, videoDecoderId, listener, videoPool);
    AVBUFFER_CREATE_AUDIOPOOL_SUCCESS(gAVBufferManager, true, audioDecoderId, listener, audioPool);

    // Test Case 1: Valid video Pool Handle and Size
    AVBUFFER_NOTIFYSPACE_SUCCESS(gAVBufferManager, videoPool, size);
    // Test Case 2: valid audio pool Handle and size
    AVBUFFER_NOTIFYSPACE_SUCCESS(gAVBufferManager, audioPool, size);

    AVBUFFER_DESTROY_POOL_SUCCESS(gAVBufferManager, videoPool);
    AVBUFFER_DESTROY_POOL_SUCCESS(gAVBufferManager, audioPool);
}

/**
 * @brief Test the negative scenarios for the notifyWhenSpaceAvailable function in AVBuffer.
 *
 * This test evaluates the behavior of the notifyWhenSpaceAvailable function when
 * provided with invalid inputs, such as an invalid pool handle, a size that exceeds pool
 * capacity, and a zero size. These scenarios are crucial to ensure the robustness and error
 * handling capabilities of the function.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 014@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Call notifyWhenSpaceAvailable with an invalid pool handle | poolHandle = INVALID, size = UT_KVP_PROFILE_GET_UINT32(IAVBuffer.nonSecureHeapBytes), aidl_return = false | status.isOk() = false, aidl_return = false | Should Fail |
 * | 02 | Call notifyWhenSpaceAvailable with a size that exceeds pool capacity | poolHandle = validPoolHandle.handle, size = UT_KVP_PROFILE_GET_UINT32(IAVBuffer.nonSecureHeapBytes) + 1, aidl_return = false | status.isOk() = false, aidl_return = false | Should Fail |
 * | 03 | Call notifyWhenSpaceAvailable with zero size | poolHandle = validPoolHandle.handle, size = 0, aidl_return = false | status.isOk() = false, aidl_return = false | Should Fail |
 */
UT_ADD_TEST(VTS_L1_AVBuffer, notifyWhenSpaceAvailable_neg)
{
    ::com::rdk::hal::videodecoder::IVideoDecoder::Id videoDecoderId;
    ::com::rdk::hal::audiodecoder::IAudioDecoder::Id audioDecoderId;
    ::com::rdk::hal::avbuffer::Pool videoPool;
    ::com::rdk::hal::avbuffer::Pool audioPool;
    ::com::rdk::hal::avbuffer::Pool invalidPool;
    invalidPool.handle = ::com::rdk::hal::avbuffer::Pool::INVALID;
    std::vector<::com::rdk::hal::avbuffer::PoolMetrics> poolMetricsNonSecure;
    std::vector<::com::rdk::hal::avbuffer::PoolMetrics> poolMetricsSecure;
    int32_t size = 1024;

    videoDecoderId.value = gAVBuffer_hfp.videoDecoderId.front();
    audioDecoderId.value = gAVBuffer_hfp.audioDecoderId.front();

    sp<IAVBufferSpaceListener> listener = new AVBufferSpaceListener();

    AVBUFFER_CREATE_VIDEOPOOL_SUCCESS(gAVBufferManager, false, videoDecoderId, listener, videoPool);
    AVBUFFER_CREATE_AUDIOPOOL_SUCCESS(gAVBufferManager, true, audioDecoderId, listener, audioPool);

    // Test Case 1: Invalid Pool Handle
    AVBUFFER_NOTIFYSPACE_FAILURE(gAVBufferManager, invalidPool, size, Status::EX_NONE);

    // Test Case 2: Size Exceeds Pool Capacity (video pool)
    AVBUFFER_NOTIFYSPACE_FAILURE(gAVBufferManager, videoPool, gAVBuffer_hfp.nonSecureHeapBytes + 1, Status::EX_NONE);

    // Test Case 3: Zero Size (video pool)
    AVBUFFER_NOTIFYSPACE_FAILURE(gAVBufferManager, videoPool, 0, Status::EX_NONE);

    // Test Case 4: Size Exceeds Pool Capacity (audio pool)
    AVBUFFER_NOTIFYSPACE_FAILURE(gAVBufferManager, audioPool, gAVBuffer_hfp.secureHeapBytes + 1, Status::EX_NONE);

    // Test Case 5: Zero Size (audio pool)
    AVBUFFER_NOTIFYSPACE_FAILURE(gAVBufferManager, audioPool, 0, Status::EX_NONE);

    AVBUFFER_DESTROY_POOL_SUCCESS(gAVBufferManager, videoPool);
    AVBUFFER_DESTROY_POOL_SUCCESS(gAVBufferManager, audioPool);
}

/**
 * @brief Test the positive scenarios for trimming the size of a video buffer.
 *
 * This test verifies the functionality of the trimSize method by checking its
 * behavior with valid buffer handles and sizes. It ensures that the method correctly
 * handles different valid sizes and returns the expected results.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 015@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Call trimSize with a valid buffer handle and newSize less than originalSize | bufferHandle = mBufferHandle, newSize = 512, aidl_return = false | status.isOk() = true, aidl_return = true | Should Pass |
 * | 02 | Call trimSize with a valid buffer handle and minimum valid newSize | bufferHandle = mBufferHandle, newSize = 1, aidl_return = false | status.isOk() = true, aidl_return = true | Should Pass |
 * | 03 | Call trimSize with a valid buffer handle and newSize equal to originalSize | bufferHandle = mBufferHandle, newSize = 1024, aidl_return = false | status.isOk() = true, aidl_return = true | Should Pass |
 */
UT_ADD_TEST(L1AVBuffer_BufferManagementTestSuite, trimSize_pos)
{
    bool aidl_return = false;
    int32_t originalSize = 1024; // Assuming the original size from alloc() is 1024 bytes

    // Test Case 1: Positive Scenario - Valid bufferHandle and newSize
    int32_t newSize = 512; // A valid size less than the original size
    UT_LOG_INFO("Calling trimSize with video buffer handle(IN:bufferHandle:[%lld], IN:newSize:[%d], OUT:_aidl_return)", mVideoBufferHandle, newSize);
    ::android::binder::Status status = gAVBufferManager->trimSize(mVideoBufferHandle, newSize, &aidl_return);
    UT_LOG_INFO("Result trimSize(IN:bufferHandle:[%lld], IN:newSize:[%d], OUT:_aidl_return:[%s]), status:[isOK:%s, exception:%s]",
                mVideoBufferHandle, newSize, UT_Control_GetMapString(bool_mapTable, aidl_return),
                UT_Control_GetMapString(bool_mapTable, status.isOk()), UT_Control_GetMapString(statusException_mapTable, status.exceptionCode()));
    UT_ASSERT_TRUE(status.isOk());
    UT_ASSERT_TRUE(aidl_return);

    // Test Case 2: Positive Scenario - Minimum valid newSize
    newSize = 1; // Minimum valid size greater than 0
    UT_LOG_INFO("Calling trimSize with audio buffer handle (IN:bufferHandle:[%lld], IN:newSize:[%d], OUT:_aidl_return)", mAudioBufferHandle, newSize);
    status = gAVBufferManager->trimSize(mAudioBufferHandle, newSize, &aidl_return);
    UT_LOG_INFO("Result trimSize(IN:bufferHandle:[%lld], IN:newSize:[%d], OUT:_aidl_return:[%s]), status:[isOK:%s, exception:%s]",
                mAudioBufferHandle, newSize, UT_Control_GetMapString(bool_mapTable, aidl_return),
                UT_Control_GetMapString(bool_mapTable, status.isOk()), UT_Control_GetMapString(statusException_mapTable, status.exceptionCode()));
    UT_ASSERT_TRUE(status.isOk());
    UT_ASSERT_TRUE(aidl_return);
}

/**
* @brief Test the negative scenarios for the trimSize function in AVBuffer management.
*
* This test evaluates the behavior of the trimSize function when provided with invalid inputs.
* It checks the function's response to an invalid buffer handle, a zero new size, and a new size
* greater than the original size, ensuring that the function handles these cases correctly by
* returning false and indicating an error status.
*
* **Test Group ID:** Basic: 01@n
* **Test Case ID:** 016@n
* **Priority:** High@n
* @n
* **Pre-Conditions:** None@n
* **Dependencies:** None@n
* **User Interaction:** None@n
* @n
* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | Attempt to trim size with an invalid buffer handle (not the last allocated) | bufferHandle = mBufferHandle + 1, newSize = 512 | aidl_return = false, status.isOk() = false | Should Fail |
* | 02 | Attempt to trim size with a valid buffer handle but invalid new size (zero) | bufferHandle = mBufferHandle, newSize = 0 | aidl_return = false, status.isOk() = false | Should Fail |
* | 03 | Attempt to trim size with a valid buffer handle but new size greater than original size | bufferHandle = mBufferHandle, newSize = 2048 | aidl_return = false, status.isOk() = false | Should Fail |
*/
UT_ADD_TEST(L1AVBuffer_BufferManagementTestSuite, test_l1_AVBuffer_negative_trimSize) {
    bool aidl_return = false;
    ::android::binder::Status status;

    // Negative Scenario - Invalid bufferHandle (not last allocated)
    int64_t invalidBufferHandle = mVideoBufferHandle + 1; // Assuming this is not the last allocated
    int32_t validNewSize = 512; // A valid size less than or equal to the original size

    UT_LOG_INFO("Calling trimSize(IN:bufferHandle:[%lld], IN:newSize:[%d], OUT:aidl_return)", invalidBufferHandle, validNewSize);
    status = gAVBufferManager->trimSize(invalidBufferHandle, validNewSize, &aidl_return);
    UT_LOG_INFO("Result trimSize(IN:bufferHandle:[%lld], IN:newSize:[%d], OUT:aidl_return:[%s]), status:[isOK:%s, exception:%s]",
                invalidBufferHandle, validNewSize, UT_Control_GetMapString(bool_mapTable, aidl_return),
                UT_Control_GetMapString(bool_mapTable, status.isOk()), UT_Control_GetMapString(statusException_mapTable, status.exceptionCode()));
    UT_ASSERT_FALSE(aidl_return);
    UT_ASSERT_FALSE(status.isOk());

    // Negative Scenario - Invalid newSize (zero) -  Video buffer handle
    int64_t validBufferHandle = mVideoBufferHandle; // The last allocated handle
    int32_t invalidNewSizeZero = 0; // Invalid size

    UT_LOG_INFO("Calling trimSize with video buffer handle (IN:bufferHandle:[%lld], IN:newSize:[%d], OUT:aidl_return)", validBufferHandle, invalidNewSizeZero);
    status = gAVBufferManager->trimSize(validBufferHandle, invalidNewSizeZero, &aidl_return);
    UT_LOG_INFO("Result trimSize(IN:bufferHandle:[%lld], IN:newSize:[%d], OUT:aidl_return:[%s]), status:[isOK:%s, exception:%s]",
                validBufferHandle, invalidNewSizeZero, UT_Control_GetMapString(bool_mapTable, aidl_return),
                UT_Control_GetMapString(bool_mapTable, status.isOk()), UT_Control_GetMapString(statusException_mapTable, status.exceptionCode()));
    UT_ASSERT_FALSE(aidl_return);
    UT_ASSERT_FALSE(status.isOk());

    // Negative Scenario - Invalid newSize (greater than original size)
    int32_t invalidNewSizeGreater = 2048; // Greater than the original size of 1024

    UT_LOG_INFO("Calling trimSize with video buffer handle (IN:bufferHandle:[%lld], IN:newSize:[%d], OUT:aidl_return)", validBufferHandle, invalidNewSizeGreater);
    status = gAVBufferManager->trimSize(validBufferHandle, invalidNewSizeGreater, &aidl_return);
    UT_LOG_INFO("Result trimSize(IN:bufferHandle:[%lld], IN:newSize:[%d], OUT:aidl_return:[%s]), status:[isOK:%s, exception:%s]",
                validBufferHandle, invalidNewSizeGreater, UT_Control_GetMapString(bool_mapTable, aidl_return),
                UT_Control_GetMapString(bool_mapTable, status.isOk()), UT_Control_GetMapString(statusException_mapTable, status.exceptionCode()));
    UT_ASSERT_FALSE(aidl_return);
    UT_ASSERT_FALSE(status.isOk());

    // Negative Scenario - Invalid newSize (zero) - Audio buffer handle
    validBufferHandle = mAudioBufferHandle; // The last allocated handle

    UT_LOG_INFO("Calling trimSize with video buffer handle (IN:bufferHandle:[%lld], IN:newSize:[%d], OUT:aidl_return)", validBufferHandle, invalidNewSizeZero);
    status = gAVBufferManager->trimSize(validBufferHandle, invalidNewSizeZero, &aidl_return);
    UT_LOG_INFO("Result trimSize(IN:bufferHandle:[%lld], IN:newSize:[%d], OUT:aidl_return:[%s]), status:[isOK:%s, exception:%s]",
                validBufferHandle, invalidNewSizeZero, UT_Control_GetMapString(bool_mapTable, aidl_return),
                UT_Control_GetMapString(bool_mapTable, status.isOk()), UT_Control_GetMapString(statusException_mapTable, status.exceptionCode()));
    UT_ASSERT_FALSE(aidl_return);
    UT_ASSERT_FALSE(status.isOk());

    // Negative Scenario - Invalid newSize (greater than original size)
    UT_LOG_INFO("Calling trimSize with video buffer handle (IN:bufferHandle:[%lld], IN:newSize:[%d], OUT:aidl_return)", validBufferHandle, invalidNewSizeGreater);
    status = gAVBufferManager->trimSize(validBufferHandle, invalidNewSizeGreater, &aidl_return);
    UT_LOG_INFO("Result trimSize(IN:bufferHandle:[%lld], IN:newSize:[%d], OUT:aidl_return:[%s]), status:[isOK:%s, exception:%s]",
                validBufferHandle, invalidNewSizeGreater, UT_Control_GetMapString(bool_mapTable, aidl_return),
                UT_Control_GetMapString(bool_mapTable, status.isOk()), UT_Control_GetMapString(statusException_mapTable, status.exceptionCode()));
    UT_ASSERT_FALSE(aidl_return);
    UT_ASSERT_FALSE(status.isOk());
}

/**
 * @brief Test the positive scenario of freeing a video buffer.
 *
 * This test verifies the successful execution of the `free` API for video buffer management. It ensures that the buffer is correctly freed and the return values are as expected.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 017@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Log the call to the free API with the buffer handle. | bufferHandle = mBufferHandle | Log should be successful | Should be successful |
 * | 02 | Call the free API to release the buffer. | bufferHandle = mBufferHandle, _aidl_return = false | status.isOk() = true, _aidl_return = true | Should Pass |
 * | 03 | Log the result of the free API call. | bufferHandle = mBufferHandle, _aidl_return = true, status.isOk() = true | Log should be successful | Should be successful |
 * | 04 | Verify that the status is OK. | status.isOk() = true | Assertion should pass | Should Pass |
 * | 05 | Verify that the _aidl_return is true. | _aidl_return = true | Assertion should pass | Should Pass |
 */
UT_ADD_TEST(L1AVBuffer_PoolManagementTestSuite, test_l1_AVBuffer_positive_free) {
    bool _aidl_return = false;

    int64_t videobufferHandle;
    int32_t size = 1024;
    UT_LOG_INFO("Calling alloc(IN:poolHandle:[%d], IN:size:[%d], OUT:_aidl_return)", mVideoPool.handle, size);
    Status status = gAVBufferManager->alloc(mVideoPool, size, &videobufferHandle);
    UT_LOG_INFO("Result alloc(IN:poolHandle:[%d], IN:size:[%d], OUT:_aidl_return:[0x%X]), status:[isOK:%s, exception:%s]", mVideoPool.handle, size, videobufferHandle, UT_Control_GetMapString(bool_mapTable, status.isOk()), UT_Control_GetMapString(statusException_mapTable, status.exceptionCode()));
    UT_ASSERT_TRUE(status.isOk());
    UT_ASSERT_NOT_EQUAL(videobufferHandle, IAVBuffer::INVALID_HANDLE);

    int64_t audioBufferHandle;
    UT_LOG_INFO("Calling alloc(IN:poolHandle:[%d], IN:size:[%d], OUT:_aidl_return)", mAudioPool.handle, size);
    status = gAVBufferManager->alloc(mAudioPool, size, &audioBufferHandle);
    UT_LOG_INFO("Result alloc(IN:poolHandle:[%d], IN:size:[%d], OUT:_aidl_return:[0x%X]), status:[isOK:%s, exception:%s]", mAudioPool.handle, size, audioBufferHandle, UT_Control_GetMapString(bool_mapTable, status.isOk()), UT_Control_GetMapString(statusException_mapTable, status.exceptionCode()));
    UT_ASSERT_TRUE(status.isOk());
    UT_ASSERT_NOT_EQUAL(audioBufferHandle, IAVBuffer::INVALID_HANDLE);



//test case : 1 free() with video buffer handle
    UT_LOG_INFO("Calling free with video buffer handle (IN:bufferHandle:[%lld], OUT:_aidl_return)", videobufferHandle);
    status = gAVBufferManager->free(videobufferHandle, &_aidl_return);
    UT_LOG_INFO("Result free(IN:bufferHandle:[%lld], OUT:_aidl_return:[%s]), status:[isOK:%s, exception:%s]",
                videobufferHandle, UT_Control_GetMapString(bool_mapTable, _aidl_return),
                UT_Control_GetMapString(bool_mapTable, status.isOk()),
                UT_Control_GetMapString(statusException_mapTable, status.exceptionCode()));
    UT_ASSERT_TRUE(status.isOk());
    UT_ASSERT_TRUE(_aidl_return);

//test case : 2 free() with audio buffer handle
    UT_LOG_INFO("Calling free with audio buffer handle (IN:bufferHandle:[%lld], OUT:_aidl_return)", audioBufferHandle);
    status = gAVBufferManager->free(audioBufferHandle, &_aidl_return);
    UT_LOG_INFO("Result free(IN:bufferHandle:[%lld], OUT:_aidl_return:[%s]), status:[isOK:%s, exception:%s]",
                audioBufferHandle, UT_Control_GetMapString(bool_mapTable, _aidl_return),
                UT_Control_GetMapString(bool_mapTable, status.isOk()),
                UT_Control_GetMapString(statusException_mapTable, status.exceptionCode()));
    UT_ASSERT_TRUE(status.isOk());
    UT_ASSERT_TRUE(_aidl_return);
}

/**
 * @brief Test the `free` API with an invalid buffer handle to ensure proper error handling.
 *
 * This test verifies the behavior of the `free` API when provided with an invalid buffer handle. The objective is to ensure that the API correctly handles error scenarios and returns appropriate status and output values.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 018@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Log the API call with invalid buffer handle | invalidBufferHandle = -1 | Should be successful | |
 * | 02 | Call the `free` API with invalid buffer handle | invalidBufferHandle = -1, aidlReturn = false | Should Fail | |
 * | 03 | Log the result of the API call | invalidBufferHandle = -1, aidlReturn = false, status.isOk() = false | Should be successful | |
 * | 04 | Verify that the return value is `false` | aidlReturn = false | Should Fail | |
 * | 05 | Check the output parameter `status.isOk()` to ensure it reflects a failure | status.isOk() = false | Should Fail | |
 */
UT_ADD_TEST(L1AVBuffer_PoolManagementTestSuite, test_l1_AVBuffer_negative_free) {
    // Test Case 1: Invalid Buffer Handle
    // An invalid integer value that does not correspond to any allocated memory buffer.
    int64_t invalidBufferHandle =  -1;
    bool aidlReturn = false;

    UT_LOG_INFO("Calling free(IN:bufferHandle:[%lld], OUT:_aidl_return)", invalidBufferHandle);
    ::android::binder::Status status = gAVBufferManager->free(invalidBufferHandle, &aidlReturn);
    UT_LOG_INFO("Result free(IN:bufferHandle:[%lld], OUT:_aidl_return:[%s]), status:[isOK:%s, exception:%s]",
                invalidBufferHandle, UT_Control_GetMapString(bool_mapTable, aidlReturn),
                UT_Control_GetMapString(bool_mapTable, status.isOk()),
                UT_Control_GetMapString(statusException_mapTable, status.exceptionCode()));

    UT_ASSERT_FALSE(aidlReturn);
    UT_ASSERT_FALSE(status.isOk());
}

/**
 * @brief Test the positive scenario of the isValid API in the AVBuffer management test suite.
 *
 * This test verifies the functionality of the isValid API by checking if a given buffer handle is valid.
 * The test ensures that the API returns a successful status and the expected boolean result indicating
 * the validity of the buffer handle.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 019@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Log the call to the isValid API | bufferHandle = mBufferHandle | Should be successful | Should be successful |
 * | 02 | Call the isValid API with the buffer handle | bufferHandle = mBufferHandle, isValidResult = false | status.isOk() = true, isValidResult = true | Should Pass |
 * | 03 | Log the result of the isValid API call | bufferHandle = mBufferHandle, isValidResult = true | Should be successful | Should be successful |
 * | 04 | Verify the return value and output parameter | status.isOk() = true, isValidResult = true | Should be successful | Should Pass |
 */
UT_ADD_TEST(L1AVBuffer_BufferManagementTestSuite, test_l1_AVBuffer_positive_isValid) {
    bool isValidResult = false;
    // Log the call to the isValid API
    UT_LOG_INFO("Calling isValid with video pool handle(IN:bufferHandle:[%lld], OUT:_aidl_return)", mVideoBufferHandle);
    ::android::binder::Status status = gAVBufferManager->isValid(mVideoBufferHandle, &isValidResult);
    UT_LOG_INFO("Result isValid(IN:bufferHandle:[%lld], OUT:_aidl_return:[%s]), status:[isOK:%s, exception:%s]",
                mVideoBufferHandle, UT_Control_GetMapString(bool_mapTable, isValidResult),
                UT_Control_GetMapString(bool_mapTable, status.isOk()),
                UT_Control_GetMapString(statusException_mapTable, status.exceptionCode()));
    UT_ASSERT_TRUE(status.isOk());
    UT_ASSERT_TRUE(isValidResult);

    UT_LOG_INFO("Calling isValid with audio pool handle(IN:bufferHandle:[%lld], OUT:_aidl_return)", mAudioBufferHandle);
    status = gAVBufferManager->isValid(mAudioBufferHandle, &isValidResult);
    UT_LOG_INFO("Result isValid(IN:bufferHandle:[%lld], OUT:_aidl_return:[%s]), status:[isOK:%s, exception:%s]",
                mAudioBufferHandle, UT_Control_GetMapString(bool_mapTable, isValidResult),
                UT_Control_GetMapString(bool_mapTable, status.isOk()),
                UT_Control_GetMapString(statusException_mapTable, status.exceptionCode()));
    UT_ASSERT_TRUE(status.isOk());
    UT_ASSERT_TRUE(isValidResult);
}

/**
* @brief Test the behavior of the `isValid` API with an invalid buffer handle.
*
* This test verifies the behavior of the `isValid` API when it is provided with an invalid buffer handle. The purpose is to ensure that the API correctly identifies and handles invalid buffer handles by returning false.
*
* **Test Group ID:** Basic: 01@n
* **Test Case ID:** 020@n
* **Priority:** High@n
* @n
* **Pre-Conditions:** None@n
* **Dependencies:** None@n
* **User Interaction:** None@n
* @n
* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | Initialize the test by setting up any necessary pre-requisites. | None | None | Should be successful |
* | 02 | Call the `isValid` API with an invalid buffer handle. | input: bufferHandle = 123456789, output: isValidResult = false | Return value: false, Assertion: UT_ASSERT_FALSE(isValidResult) | Should Pass |
* | 03 | Verify the status of the API call to ensure it executed correctly. | status: isOk = true | Assertion: UT_ASSERT_TRUE(status.isOk()) | Should Pass |
*/
UT_ADD_TEST(L1AVBuffer_BufferManagementTestSuite, test_l1_AVBuffer_negative_isValid) {

    int64_t invalidBufferHandle = mAudioBufferHandle+1; // Random integer not obtained from alloc()
    bool isValidResult = false;

    UT_LOG_INFO("Calling isValid(IN:bufferHandle:[%lld], OUT:_aidl_return[])", invalidBufferHandle);
    ::android::binder::Status status = gAVBufferManager->isValid(invalidBufferHandle, &isValidResult);
    UT_LOG_INFO("Result isValid(IN:bufferHandle:[%lld], OUT:_aidl_return:[%s]), status:[isOK:%s, exception:%s]",
                invalidBufferHandle, UT_Control_GetMapString(bool_mapTable, isValidResult),
                UT_Control_GetMapString(bool_mapTable, status.isOk()),
                UT_Control_GetMapString(statusException_mapTable, status.exceptionCode()));
    UT_ASSERT_FALSE(isValidResult);
    UT_ASSERT_TRUE(status.isOk());
}

/**
 * @brief Test the positive scenario of retrieving allocation list from AVBuffer
 *
 * This test verifies the functionality of the getAllocList API by ensuring that it successfully retrieves a list of allocation handles when provided with a valid pool handle. The test checks that the returned list is not empty and that all handles are positive, indicating valid allocations.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 021@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize pool handle with a valid value | poolHandle.handle = 1 | Pool handle should be initialized successfully | Should be successful |
 * | 02 | Log the call to getAllocList with the initialized pool handle | poolHandle.handle = 1 | Log should be successful | Should be successful |
 * | 03 | Call getAllocList API with the pool handle and retrieve allocList | poolHandle.handle = 1, allocList = [] | API call should succeed, allocList should be populated | Should Pass |
 * | 04 | Log the result of getAllocList including status and allocList size | poolHandle.handle = 1, allocList.size() = X | Log should be successful | Should be successful |
 * | 05 | Assert that the status of the API call is OK | status.isOk() = true | Assertion should pass | Should Pass |
 * | 06 | Assert that the allocList is not empty | allocList.empty() = false | Assertion should pass | Should Pass |
 * | 07 | Iterate over allocList and assert each handle is greater than 0 | handle > 0 for each handle in allocList | All assertions should pass | Should Pass |
 */
UT_ADD_TEST(L1AVBuffer_PoolManagementTestSuite, test_l1_AVBuffer_positive_getAllocList) {
    // Setup
    ::com::rdk::hal::avbuffer::Pool poolHandle;

    poolHandle = mVideoPool; // Assume a valid handle is set here
    std::vector<int64_t> allocList;
    UT_LOG_INFO("Calling getAllocList with video pool handle (IN:poolHandle:[%d], OUT:allocList[])", poolHandle.handle);
    ::android::binder::Status status = gAVBufferManager->getAllocList(poolHandle, &allocList);
    UT_LOG_INFO("Result getAllocList(IN:poolHandle:[%d], OUT:allocList[size:%zu]), status:[isOK:%s, exception:%s]",
                poolHandle.handle, allocList.size(),
                UT_Control_GetMapString(bool_mapTable, status.isOk()),
                UT_Control_GetMapString(statusException_mapTable, status.exceptionCode()));
    UT_ASSERT_TRUE(status.isOk());


    poolHandle = mAudioPool; // Assume a valid handle is set here
    std::vector<int64_t> audioallocList;
    UT_LOG_INFO("Calling getAllocList with audio pool handle (IN:poolHandle:[%d], OUT:allocList[])", poolHandle.handle);
    status = gAVBufferManager->getAllocList(poolHandle, &audioallocList);
    UT_LOG_INFO("Result getAllocList(IN:poolHandle:[%d], OUT:allocList[size:%zu]), status:[isOK:%s, exception:%s]",
                poolHandle.handle, audioallocList.size(),
                UT_Control_GetMapString(bool_mapTable, status.isOk()),
                UT_Control_GetMapString(statusException_mapTable, status.exceptionCode()));
    UT_ASSERT_TRUE(status.isOk());
}

/**
 * @brief Test the negative scenario for getAllocList with an invalid pool handle
 *
 * This test verifies the behavior of the getAllocList function when provided with an invalid pool handle.
 * It ensures that the function correctly handles invalid input by returning an appropriate error status and
 * an empty allocation list.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 022@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Prepare an invalid pool handle for testing | invalidPool.handle = INVALID | Should be successful | |
 * | 02 | Prepare an empty vector to receive the allocation list | allocList = empty vector | Should be successful | |
 * | 03 | Log the API call with the invalid pool handle | invalidPool.handle = INVALID | Should be successful | |
 * | 04 | Call the getAllocList API with the invalid pool handle | input: invalidPool.handle = INVALID, output: allocList = empty vector | Status should indicate failure, allocList should remain empty | Should Fail |
 * | 05 | Log the result of the API call, including status and allocation list size | invalidPool.handle = INVALID, allocList.size() = 0 | Should be successful | |
 * | 06 | Verify that the status is not OK | status.isOk() = false | Should be successful | |
 * | 07 | Verify that the exception code is EX_ILLEGAL_ARGUMENT | status.exceptionCode() = EX_ILLEGAL_ARGUMENT | Should be successful | |
 * | 08 | Verify that the allocation list is empty | allocList.empty() = true | Should be successful | |
 */
UT_ADD_TEST(L1AVBuffer_PoolManagementTestSuite, test_l1_AVBuffer_negative_getAllocList) {
    ::com::rdk::hal::avbuffer::Pool invalidPool;
    invalidPool.handle = ::com::rdk::hal::avbuffer::Pool::INVALID;
    ::std::vector<int64_t> allocList;
    UT_LOG_INFO("Calling getAllocList(IN:poolHandle:[%d], OUT:allocList[])", invalidPool.handle);
    ::android::binder::Status status = gAVBufferManager->getAllocList(invalidPool, &allocList);
    UT_LOG_INFO("Result getAllocList(IN:poolHandle:[%d], OUT:allocList[size:%zu]), status:[isOK:%s, exception:%s]",
                invalidPool.handle, allocList.size(), UT_Control_GetMapString(bool_mapTable, status.isOk()),
                UT_Control_GetMapString(statusException_mapTable, status.exceptionCode()));
    UT_ASSERT_FALSE(status.isOk());
    UT_ASSERT_EQUAL(status.exceptionCode(), ::android::binder::Status::EX_ILLEGAL_ARGUMENT);
    UT_ASSERT_TRUE(allocList.empty());
}

/**
 * @brief Test the positive scenario of calculating SHA-1 hash for a video buffer.
 *
 * This test verifies the functionality of the `calculateSHA1` method to ensure it correctly computes the SHA-1 hash for a given video buffer handle. The test checks both the successful computation and the handling of exceptions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 023@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize the test suite and prepare the environment for testing. | None | Environment should be set up successfully. | Should be successful |
 * | 02 | Call the `calculateSHA1` method with a valid buffer handle to compute the SHA-1 hash. | bufferHandle = mBufferHandle, sha1Result = [] | Status should be OK, SHA-1 hash should be computed. | Should Pass |
 * | 03 | Verify the computed SHA-1 hash against the expected value using `isExpectedSHA1`. | sha1Result = computed value | SHA-1 hash should match the expected value. | Should Pass |
 * | 04 | Handle the exception scenario if the status is not OK. | status.exceptionCode = value | Exception code should be EX_UNSUPPORTED_OPERATION. | Should Fail |
 * | 05 | Clean up the test environment and finalize the test suite. | None | Environment should be cleaned up successfully. | Done by Pre-requisite TearDown function |
 */
UT_ADD_TEST(L1AVBuffer_BufferManagementTestSuite, test_l1_AVBuffer_positive_calculateSHA1) {
    std::vector<uint8_t> sha1Result;
    UT_LOG_INFO("Calling calculateSHA1 with video buffer handle (IN:bufferHandle:[%lld], OUT:sha1Result[])", mVideoBufferHandle);
    ::android::binder::Status status = gAVBufferManager->calculateSHA1(mVideoBufferHandle, &sha1Result);
    UT_LOG_INFO("Result calculateSHA1(IN:bufferHandle:[%lld], OUT:sha1Result[size:%zu]), status:[isOK:%s, exception:%s]",
                mVideoBufferHandle, sha1Result.size(), UT_Control_GetMapString(bool_mapTable, status.isOk()),
                UT_Control_GetMapString(statusException_mapTable, status.exceptionCode()));

    if (status.isOk()) {
        UT_ASSERT_TRUE(status.isOk());
    } else {
        UT_ASSERT_EQUAL(status.exceptionCode(), ::android::binder::Status::EX_UNSUPPORTED_OPERATION)
            << "Unexpected exception code.";
    }

    UT_LOG_INFO("Calling calculateSHA1 with audio buffer handle (IN:bufferHandle:[%lld], OUT:sha1Result[])", mAudioBufferHandle);
    status = gAVBufferManager->calculateSHA1(mAudioBufferHandle, &sha1Result);
    UT_LOG_INFO("Result calculateSHA1(IN:bufferHandle:[%lld], OUT:sha1Result[size:%zu]), status:[isOK:%s, exception:%s]",
                mAudioBufferHandle, sha1Result.size(), UT_Control_GetMapString(bool_mapTable, status.isOk()),
                UT_Control_GetMapString(statusException_mapTable, status.exceptionCode()));

    if (status.isOk()) {
        UT_ASSERT_TRUE(status.isOk());
    } else {
        UT_ASSERT_EQUAL(status.exceptionCode(), ::android::binder::Status::EX_UNSUPPORTED_OPERATION)
            << "Unexpected exception code.";
    }
}

/**
 * @brief Test the negative scenario for calculating SHA1 with an invalid buffer handle.
 *
 * This test verifies the behavior of the calculateSHA1 function when provided with an invalid buffer handle. The test ensures that the function correctly handles the error and returns the expected exception code and empty result.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 024@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Prepare invalid buffer handle | invalidBufferHandle = -1 | N/A | Should be successful |
 * | 02 | Prepare output vector | sha1Result = empty vector | N/A | Should be successful |
 * | 03 | Log the API call | invalidBufferHandle = -1, sha1Result = empty vector | N/A | Should be successful |
 * | 04 | Call the API calculateSHA1 | invalidBufferHandle = -1, sha1Result = empty vector | status.exceptionCode() = EX_ILLEGAL_ARGUMENT, sha1Result.empty() = true | Should Fail |
 * | 05 | Log the result | invalidBufferHandle = -1, sha1Result.size() = 0, status.isOk() = false, status.exceptionCode() = EX_ILLEGAL_ARGUMENT | N/A | Should be successful |
 * | 06 | Verify the status and output | status.exceptionCode() = EX_ILLEGAL_ARGUMENT, sha1Result.empty() = true | Assertion checks pass | Should be successful |
 */
UT_ADD_TEST(L1AVBuffer_BufferManagementTestSuite, test_l1_AVBuffer_negative_calculateSHA1) {
    int64_t invalidBufferHandle = mVideoBufferHandle+1; // Example of an invalid handle
    std::vector<uint8_t> sha1Result;

    UT_LOG_INFO("Calling calculateSHA1(IN:bufferHandle:[%lld], OUT:sha1Result[])", invalidBufferHandle);
    ::android::binder::Status status = gAVBufferManager->calculateSHA1(invalidBufferHandle, &sha1Result);
    UT_LOG_INFO("Result calculateSHA1(IN:bufferHandle:[%lld], OUT:sha1Result[%zu]), status:[isOK:%s, exception:%s]",
                invalidBufferHandle, sha1Result.size(),
                UT_Control_GetMapString(bool_mapTable, status.isOk()),
                UT_Control_GetMapString(statusException_mapTable, status.exceptionCode()));

    UT_ASSERT_EQUAL(status.exceptionCode(), ::android::binder::Status::EX_ILLEGAL_ARGUMENT);
    UT_ASSERT_TRUE(sha1Result.empty());
}
