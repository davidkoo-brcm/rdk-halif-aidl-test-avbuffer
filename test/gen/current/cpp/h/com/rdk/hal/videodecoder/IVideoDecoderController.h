#pragma once

#include <binder/IBinder.h>
#include <binder/IInterface.h>
#include <binder/Status.h>
#include <com/rdk/hal/videodecoder/CSDVideoFormat.h>
#include <cstdint>
#include <utils/StrongPointer.h>
#include <vector>

namespace com {
namespace rdk {
namespace hal {
namespace videodecoder {
class IVideoDecoderControllerDelegator;

class IVideoDecoderController : public ::android::IInterface {
public:
  typedef IVideoDecoderControllerDelegator DefaultDelegator;
  DECLARE_META_INTERFACE(VideoDecoderController)
  virtual ::android::binder::Status start() = 0;
  virtual ::android::binder::Status stop() = 0;
  virtual ::android::binder::Status decodeBuffer(int64_t nsPresentationTime, int64_t bufferHandle) = 0;
  virtual ::android::binder::Status flush(bool reset) = 0;
  virtual ::android::binder::Status signalDiscontinuity() = 0;
  virtual ::android::binder::Status signalEOS() = 0;
  virtual ::android::binder::Status parseCodecSpecificData(::com::rdk::hal::videodecoder::CSDVideoFormat csdVideoFormat, const ::std::vector<uint8_t>& codecData, bool* _aidl_return) = 0;
};  // class IVideoDecoderController

class IVideoDecoderControllerDefault : public IVideoDecoderController {
public:
  ::android::IBinder* onAsBinder() override {
    return nullptr;
  }
  ::android::binder::Status start() override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
  ::android::binder::Status stop() override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
  ::android::binder::Status decodeBuffer(int64_t /*nsPresentationTime*/, int64_t /*bufferHandle*/) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
  ::android::binder::Status flush(bool /*reset*/) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
  ::android::binder::Status signalDiscontinuity() override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
  ::android::binder::Status signalEOS() override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
  ::android::binder::Status parseCodecSpecificData(::com::rdk::hal::videodecoder::CSDVideoFormat /*csdVideoFormat*/, const ::std::vector<uint8_t>& /*codecData*/, bool* /*_aidl_return*/) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
};  // class IVideoDecoderControllerDefault
}  // namespace videodecoder
}  // namespace hal
}  // namespace rdk
}  // namespace com
