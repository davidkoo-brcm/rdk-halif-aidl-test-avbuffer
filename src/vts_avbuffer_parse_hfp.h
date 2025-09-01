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
 * @defgroup AV_BUFFER_HALTEST_PARSE_HFP_HEADER AV Buffer HAL Tests parse Hardware feature profile Header File
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
 * @file vts_avbuffer_parse_hfp.h
 *
 */

#ifndef __VTS_AVBUFFER_PARSE_HFP_H__
#define __VTS_AVBUFFER_PARSE_HFP_H__

#include <stdio.h>
#include <vector>

#include <ut.h>
#include <ut_kvp_profile.h>
#include <ut_log.h>

typedef struct avbuffer
{
    ::std::vector<int32_t> videoDecoderId;
    ::std::vector<int32_t> audioDecoderId;
    uint32_t nonSecureHeapBytes;
    uint32_t secureHeapBytes;
    bool supportSecure;
    bool supportSHA1;
}avbuffer_t;

#define VIDEODECODER_PATH "IVideoDecoderManager.IVideoDecoder"
#define AUDIODECODER_PATH "IAudioDecoderManager.IAudioDecoder"
#define AVBUFFER_SECURESIZE_PATH "IAVBuffer.secureHeapBytes"
#define AVBUFFER_NONSECURESIZE_PATH "IAVBuffer.nonSecureHeapBytes"
#define AVBUFFER_SHA1_PATH "IAVBuffer.supportsSHA1"

extern avbuffer_t gAVBuffer_hfp;

UT_status_t vts_avbuffer_parseAudioDecoderHFP();

#endif //__VTS_AVBUFFER_PARSE_HFP_H__

/** @} */ // End of AV_BUFFER_HALTEST_PARSE_HFP_HEADER
/** @} */ // End of AV_BUFFER_HALTEST
/** @} */ // End of AV_BUFFER
/** @} */ // End of HPK