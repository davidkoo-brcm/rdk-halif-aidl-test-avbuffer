#pragma once

#include <binder/IBinder.h>
#include <binder/IInterface.h>
#include <binder/Status.h>
#include <com/rdk/hal/videodecoder/FrameMetadata.h>
#include <cstdint>
#include <optional>
#include <utils/StrongPointer.h>
#include <vector>

namespace com::rdk::hal::videodecoder {
class FrameMetadata;
}  // namespace com::rdk::hal::videodecoder
namespace com {
namespace rdk {
namespace hal {
namespace videodecoder {
class IVideoDecoderControllerListenerDelegator;

class IVideoDecoderControllerListener : public ::android::IInterface {
public:
  typedef IVideoDecoderControllerListenerDelegator DefaultDelegator;
  DECLARE_META_INTERFACE(VideoDecoderControllerListener)
  virtual ::android::binder::Status onFrameOutput(int64_t nsPresentationTime, int64_t frameBufferHandle, const ::std::optional<::com::rdk::hal::videodecoder::FrameMetadata>& metadata) = 0;
  virtual ::android::binder::Status onUserDataOutput(int64_t nsPresentationTime, const ::std::vector<uint8_t>& userData) = 0;
};  // class IVideoDecoderControllerListener

class IVideoDecoderControllerListenerDefault : public IVideoDecoderControllerListener {
public:
  ::android::IBinder* onAsBinder() override {
    return nullptr;
  }
  ::android::binder::Status onFrameOutput(int64_t /*nsPresentationTime*/, int64_t /*frameBufferHandle*/, const ::std::optional<::com::rdk::hal::videodecoder::FrameMetadata>& /*metadata*/) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
  ::android::binder::Status onUserDataOutput(int64_t /*nsPresentationTime*/, const ::std::vector<uint8_t>& /*userData*/) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
};  // class IVideoDecoderControllerListenerDefault
}  // namespace videodecoder
}  // namespace hal
}  // namespace rdk
}  // namespace com
