#pragma once

#include <binder/IBinder.h>
#include <binder/IInterface.h>
#include <binder/Status.h>
#include <utils/StrongPointer.h>

namespace com {
namespace rdk {
namespace hal {
namespace avbuffer {
class IAVBufferSpaceListenerDelegator;

class IAVBufferSpaceListener : public ::android::IInterface {
public:
  typedef IAVBufferSpaceListenerDelegator DefaultDelegator;
  DECLARE_META_INTERFACE(AVBufferSpaceListener)
  virtual ::android::binder::Status onSpaceAvailable() = 0;
};  // class IAVBufferSpaceListener

class IAVBufferSpaceListenerDefault : public IAVBufferSpaceListener {
public:
  ::android::IBinder* onAsBinder() override {
    return nullptr;
  }
  ::android::binder::Status onSpaceAvailable() override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
};  // class IAVBufferSpaceListenerDefault
}  // namespace avbuffer
}  // namespace hal
}  // namespace rdk
}  // namespace com
