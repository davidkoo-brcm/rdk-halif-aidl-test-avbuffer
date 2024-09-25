#pragma once

#include <binder/IBinder.h>
#include <binder/IInterface.h>
#include <binder/Status.h>
#include <com/rdk/hal/videodecoder/IVideoDecoder.h>
#include <com/rdk/hal/videodecoder/OperationalMode.h>
#include <optional>
#include <string>
#include <utils/StrongPointer.h>
#include <vector>

namespace com::rdk::hal::videodecoder {
class IVideoDecoder;
}  // namespace com::rdk::hal::videodecoder
namespace com {
namespace rdk {
namespace hal {
namespace videodecoder {
class IVideoDecoderManagerDelegator;

class IVideoDecoderManager : public ::android::IInterface {
public:
  typedef IVideoDecoderManagerDelegator DefaultDelegator;
  DECLARE_META_INTERFACE(VideoDecoderManager)
  static const ::std::string& serviceName();
  virtual ::android::binder::Status getVideoDecoderIds(::std::vector<::com::rdk::hal::videodecoder::IVideoDecoder::Id>* _aidl_return) = 0;
  virtual ::android::binder::Status getSupportedOperationalModes(::std::vector<::com::rdk::hal::videodecoder::OperationalMode>* _aidl_return) = 0;
  virtual ::android::binder::Status getVideoDecoder(const ::com::rdk::hal::videodecoder::IVideoDecoder::Id& videoDecoderId, ::android::sp<::com::rdk::hal::videodecoder::IVideoDecoder>* _aidl_return) = 0;
};  // class IVideoDecoderManager

class IVideoDecoderManagerDefault : public IVideoDecoderManager {
public:
  ::android::IBinder* onAsBinder() override {
    return nullptr;
  }
  ::android::binder::Status getVideoDecoderIds(::std::vector<::com::rdk::hal::videodecoder::IVideoDecoder::Id>* /*_aidl_return*/) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
  ::android::binder::Status getSupportedOperationalModes(::std::vector<::com::rdk::hal::videodecoder::OperationalMode>* /*_aidl_return*/) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
  ::android::binder::Status getVideoDecoder(const ::com::rdk::hal::videodecoder::IVideoDecoder::Id& /*videoDecoderId*/, ::android::sp<::com::rdk::hal::videodecoder::IVideoDecoder>* /*_aidl_return*/) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
};  // class IVideoDecoderManagerDefault
}  // namespace videodecoder
}  // namespace hal
}  // namespace rdk
}  // namespace com
