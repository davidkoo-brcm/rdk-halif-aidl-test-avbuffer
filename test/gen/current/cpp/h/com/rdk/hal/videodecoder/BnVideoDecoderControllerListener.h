#pragma once

#include <binder/IInterface.h>
#include <com/rdk/hal/videodecoder/IVideoDecoderControllerListener.h>
//#include <binder/Delegate.h>
#include <com/rdk/hal/videodecoder/BnVideoDecoderControllerListener.h>


namespace com {
namespace rdk {
namespace hal {
namespace videodecoder {
class BnVideoDecoderControllerListener : public ::android::BnInterface<IVideoDecoderControllerListener> {
public:
  static constexpr uint32_t TRANSACTION_onFrameOutput = ::android::IBinder::FIRST_CALL_TRANSACTION + 0;
  static constexpr uint32_t TRANSACTION_onUserDataOutput = ::android::IBinder::FIRST_CALL_TRANSACTION + 1;
  explicit BnVideoDecoderControllerListener();
  ::android::status_t onTransact(uint32_t _aidl_code, const ::android::Parcel& _aidl_data, ::android::Parcel* _aidl_reply, uint32_t _aidl_flags) override;
};  // class BnVideoDecoderControllerListener

class IVideoDecoderControllerListenerDelegator : public BnVideoDecoderControllerListener {
public:
  explicit IVideoDecoderControllerListenerDelegator(const ::android::sp<IVideoDecoderControllerListener> &impl) : _aidl_delegate(impl) {}

  ::android::sp<IVideoDecoderControllerListener> getImpl() { return _aidl_delegate; }
  ::android::binder::Status onFrameOutput(int64_t nsPresentationTime, int64_t frameBufferHandle, const ::std::optional<::com::rdk::hal::videodecoder::FrameMetadata>& metadata) override {
    return _aidl_delegate->onFrameOutput(nsPresentationTime, frameBufferHandle, metadata);
  }
  ::android::binder::Status onUserDataOutput(int64_t nsPresentationTime, const ::std::vector<uint8_t>& userData) override {
    return _aidl_delegate->onUserDataOutput(nsPresentationTime, userData);
  }
private:
  ::android::sp<IVideoDecoderControllerListener> _aidl_delegate;
};  // class IVideoDecoderControllerListenerDelegator
}  // namespace videodecoder
}  // namespace hal
}  // namespace rdk
}  // namespace com
