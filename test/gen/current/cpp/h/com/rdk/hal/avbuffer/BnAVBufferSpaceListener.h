#pragma once

#include <binder/IInterface.h>
#include <com/rdk/hal/avbuffer/IAVBufferSpaceListener.h>
//#include <binder/Delegate.h>
#include <com/rdk/hal/avbuffer/BnAVBufferSpaceListener.h>


namespace com {
namespace rdk {
namespace hal {
namespace avbuffer {
class BnAVBufferSpaceListener : public ::android::BnInterface<IAVBufferSpaceListener> {
public:
  static constexpr uint32_t TRANSACTION_onSpaceAvailable = ::android::IBinder::FIRST_CALL_TRANSACTION + 0;
  explicit BnAVBufferSpaceListener();
  ::android::status_t onTransact(uint32_t _aidl_code, const ::android::Parcel& _aidl_data, ::android::Parcel* _aidl_reply, uint32_t _aidl_flags) override;
};  // class BnAVBufferSpaceListener

class IAVBufferSpaceListenerDelegator : public BnAVBufferSpaceListener {
public:
  explicit IAVBufferSpaceListenerDelegator(const ::android::sp<IAVBufferSpaceListener> &impl) : _aidl_delegate(impl) {}

  ::android::sp<IAVBufferSpaceListener> getImpl() { return _aidl_delegate; }
  ::android::binder::Status onSpaceAvailable() override {
    return _aidl_delegate->onSpaceAvailable();
  }
private:
  ::android::sp<IAVBufferSpaceListener> _aidl_delegate;
};  // class IAVBufferSpaceListenerDelegator
}  // namespace avbuffer
}  // namespace hal
}  // namespace rdk
}  // namespace com
