#pragma once

#include <binder/IBinder.h>
#include <binder/IInterface.h>
#include <binder/Status.h>
#include <com/rdk/hal/State.h>
#include <com/rdk/hal/videodecoder/ErrorCode.h>
#include <cstdint>
#include <utils/StrongPointer.h>

namespace com {
namespace rdk {
namespace hal {
namespace videodecoder {
class IVideoDecoderEventListenerDelegator;

class IVideoDecoderEventListener : public ::android::IInterface {
public:
  typedef IVideoDecoderEventListenerDelegator DefaultDelegator;
  DECLARE_META_INTERFACE(VideoDecoderEventListener)
  virtual ::android::binder::Status onDecodeError(::com::rdk::hal::videodecoder::ErrorCode errorCode, int32_t vendorErrorCode) = 0;
  virtual ::android::binder::Status onStateChanged(::com::rdk::hal::State oldState, ::com::rdk::hal::State newState) = 0;
};  // class IVideoDecoderEventListener

class IVideoDecoderEventListenerDefault : public IVideoDecoderEventListener {
public:
  ::android::IBinder* onAsBinder() override {
    return nullptr;
  }
  ::android::binder::Status onDecodeError(::com::rdk::hal::videodecoder::ErrorCode /*errorCode*/, int32_t /*vendorErrorCode*/) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
  ::android::binder::Status onStateChanged(::com::rdk::hal::State /*oldState*/, ::com::rdk::hal::State /*newState*/) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
};  // class IVideoDecoderEventListenerDefault
}  // namespace videodecoder
}  // namespace hal
}  // namespace rdk
}  // namespace com
