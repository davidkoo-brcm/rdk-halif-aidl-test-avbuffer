#pragma once

#include <binder/IBinder.h>
#include <binder/IInterface.h>
#include <utils/Errors.h>
#include <com/rdk/hal/avbuffer/IAVBufferSpaceListener.h>

namespace com {
namespace rdk {
namespace hal {
namespace avbuffer {
class BpAVBufferSpaceListener : public ::android::BpInterface<IAVBufferSpaceListener> {
public:
  explicit BpAVBufferSpaceListener(const ::android::sp<::android::IBinder>& _aidl_impl);
  virtual ~BpAVBufferSpaceListener() = default;
  ::android::binder::Status onSpaceAvailable() override;
};  // class BpAVBufferSpaceListener
}  // namespace avbuffer
}  // namespace hal
}  // namespace rdk
}  // namespace com
