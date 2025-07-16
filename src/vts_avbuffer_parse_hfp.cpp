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
 * @defgroup AV_BUFFER_HALTEST_PARSE_HFP AV Buffer HAL Tests parse Hardware feature profile
 * @{
 * @parblock
 *
 * ### Parse HFP functions for AV Buffer :
 *
 * Parse HFP functions required for the module across all vendors.
 *
 * **Pre-Conditions:**  None @n
 * **Dependencies:** None @n
 *
 * @endparblock
 *
 */

/**
 * @file vts_avbuffer_parse_hfp.cpp
 *
 */

#include "vts_avbuffer.h"

avbuffer_info gAVBuffer_hfp;

UT_status_t vts_avbuffer_parseAudioDecoderHFP()
{
    std::string videodecoder_count_path = VIDEODECODER_PATH;
    std::string audiodecoder_count_path = AUDIODECODER_PATH;

    int32_t videoCount = UT_KVP_PROFILE_GET_LIST_COUNT(videodecoder_count_path.c_str());
    int32_t audioCount = UT_KVP_PROFILE_GET_LIST_COUNT(audiodecoder_count_path.c_str());

    if (videoCount <= 0)
    {
        UT_LOG_ERROR("No video decoders found in the HFP");
        return UT_STATUS_FAILURE;
    }
    if (audioCount <= 0)
    {
        UT_LOG_ERROR("No audio decoders found in the HFP");
        return UT_STATUS_FAILURE;
    }

    for (int index = 0; index < videoCount; ++index)
    {
        std::string videodecoderidpath = VIDEODECODER_PATH  "." + std::to_string(index) + ".id";
        int32_t _videoDecoderId = UT_KVP_PROFILE_GET_UINT8(videodecoderidpath.c_str());
        gAVBuffer_hfp.videoDecoderId.push_back(_videoDecoderId);
    }

    std::sort(gAVBuffer_hfp.videoDecoderId.begin(), gAVBuffer_hfp.videoDecoderId.end());

    for (int index = 0; index < audioCount; ++index)
    {
        std::string audiodecoderidpath = AUDIODECODER_PATH  "." + std::to_string(index) + ".id";
        int32_t _audioDecoderId = UT_KVP_PROFILE_GET_UINT8(audiodecoderidpath.c_str());
        gAVBuffer_hfp.audioDecoderId.push_back(_audioDecoderId);
    }

    std::sort(gAVBuffer_hfp.audioDecoderId.begin(), gAVBuffer_hfp.audioDecoderId.end());

    gAVBuffer_hfp.nonSecureHeapBytes = UT_KVP_PROFILE_GET_UINT32(AVBUFFER_NONSECURESIZE_PATH);
    gAVBuffer_hfp.secureHeapBytes = UT_KVP_PROFILE_GET_UINT32(AVBUFFER_SECURESIZE_PATH);

    gAVBuffer_hfp.supportSecure = gAVBuffer_hfp.secureHeapBytes > 0;

    return UT_STATUS_OK;
}

/** @} */ // End of AV_BUFFER_HALTEST_PARSE_HFP
/** @} */ // End of AV_BUFFER_HALTEST
/** @} */ // End of AV_BUFFER
/** @} */ // End of HPK