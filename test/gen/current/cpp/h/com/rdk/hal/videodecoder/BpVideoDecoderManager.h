#pragma once

#include <binder/IBinder.h>
#include <binder/IInterface.h>
#include <utils/Errors.h>
#include <com/rdk/hal/videodecoder/IVideoDecoderManager.h>

namespace com {
namespace rdk {
namespace hal {
namespace videodecoder {
class BpVideoDecoderManager : public ::android::BpInterface<IVideoDecoderManager> {
public:
  explicit BpVideoDecoderManager(const ::android::sp<::android::IBinder>& _aidl_impl);
  virtual ~BpVideoDecoderManager() = default;
  ::android::binder::Status getVideoDecoderIds(::std::vector<::com::rdk::hal::videodecoder::IVideoDecoder::Id>* _aidl_return) override;
  ::android::binder::Status getSupportedOperationalModes(::std::vector<::com::rdk::hal::videodecoder::OperationalMode>* _aidl_return) override;
  ::android::binder::Status getVideoDecoder(const ::com::rdk::hal::videodecoder::IVideoDecoder::Id& videoDecoderId, ::android::sp<::com::rdk::hal::videodecoder::IVideoDecoder>* _aidl_return) override;
};  // class BpVideoDecoderManager
}  // namespace videodecoder
}  // namespace hal
}  // namespace rdk
}  // namespace com
