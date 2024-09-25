#pragma once

#include <binder/IInterface.h>
#include <com/rdk/hal/videodecoder/IVideoDecoderEventListener.h>
//#include <binder/Delegate.h>
#include <com/rdk/hal/videodecoder/BnVideoDecoderEventListener.h>


namespace com {
namespace rdk {
namespace hal {
namespace videodecoder {
class BnVideoDecoderEventListener : public ::android::BnInterface<IVideoDecoderEventListener> {
public:
  static constexpr uint32_t TRANSACTION_onDecodeError = ::android::IBinder::FIRST_CALL_TRANSACTION + 0;
  static constexpr uint32_t TRANSACTION_onStateChanged = ::android::IBinder::FIRST_CALL_TRANSACTION + 1;
  explicit BnVideoDecoderEventListener();
  ::android::status_t onTransact(uint32_t _aidl_code, const ::android::Parcel& _aidl_data, ::android::Parcel* _aidl_reply, uint32_t _aidl_flags) override;
};  // class BnVideoDecoderEventListener

class IVideoDecoderEventListenerDelegator : public BnVideoDecoderEventListener {
public:
  explicit IVideoDecoderEventListenerDelegator(const ::android::sp<IVideoDecoderEventListener> &impl) : _aidl_delegate(impl) {}

  ::android::sp<IVideoDecoderEventListener> getImpl() { return _aidl_delegate; }
  ::android::binder::Status onDecodeError(::com::rdk::hal::videodecoder::ErrorCode errorCode, int32_t vendorErrorCode) override {
    return _aidl_delegate->onDecodeError(errorCode, vendorErrorCode);
  }
  ::android::binder::Status onStateChanged(::com::rdk::hal::State oldState, ::com::rdk::hal::State newState) override {
    return _aidl_delegate->onStateChanged(oldState, newState);
  }
private:
  ::android::sp<IVideoDecoderEventListener> _aidl_delegate;
};  // class IVideoDecoderEventListenerDelegator
}  // namespace videodecoder
}  // namespace hal
}  // namespace rdk
}  // namespace com
