#include <com/rdk/hal/avbuffer/IAVBufferSpaceListener.h>
#include <com/rdk/hal/avbuffer/BpAVBufferSpaceListener.h>
namespace com {
namespace rdk {
namespace hal {
namespace avbuffer {
DO_NOT_DIRECTLY_USE_ME_IMPLEMENT_META_INTERFACE(AVBufferSpaceListener, "com.rdk.hal.avbuffer.IAVBufferSpaceListener")
}  // namespace avbuffer
}  // namespace hal
}  // namespace rdk
}  // namespace com
#include <com/rdk/hal/avbuffer/BpAVBufferSpaceListener.h>
#include <com/rdk/hal/avbuffer/BnAVBufferSpaceListener.h>
#include <binder/Parcel.h>
#include <android-base/macros.h>

namespace com {
namespace rdk {
namespace hal {
namespace avbuffer {

BpAVBufferSpaceListener::BpAVBufferSpaceListener(const ::android::sp<::android::IBinder>& _aidl_impl)
    : BpInterface<IAVBufferSpaceListener>(_aidl_impl){
}

::android::binder::Status BpAVBufferSpaceListener::onSpaceAvailable() {
  ::android::Parcel _aidl_data;
  _aidl_data.markForBinder(remoteStrong());
  ::android::Parcel _aidl_reply;
  ::android::status_t _aidl_ret_status = ::android::OK;
  ::android::binder::Status _aidl_status;
  _aidl_ret_status = _aidl_data.writeInterfaceToken(getInterfaceDescriptor());
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  _aidl_ret_status = remote()->transact(BnAVBufferSpaceListener::TRANSACTION_onSpaceAvailable, _aidl_data, &_aidl_reply, ::android::IBinder::FLAG_ONEWAY);
  if (UNLIKELY(_aidl_ret_status == ::android::UNKNOWN_TRANSACTION && IAVBufferSpaceListener::getDefaultImpl())) {
     return IAVBufferSpaceListener::getDefaultImpl()->onSpaceAvailable();
  }
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  _aidl_error:
  _aidl_status.setFromStatusT(_aidl_ret_status);
  return _aidl_status;
}

}  // namespace avbuffer
}  // namespace hal
}  // namespace rdk
}  // namespace com
#include <com/rdk/hal/avbuffer/BnAVBufferSpaceListener.h>
#include <binder/Parcel.h>
#include <binder/Stability.h>

namespace com {
namespace rdk {
namespace hal {
namespace avbuffer {

BnAVBufferSpaceListener::BnAVBufferSpaceListener()
{
  ::android::internal::Stability::markVintf(this);
}

::android::status_t BnAVBufferSpaceListener::onTransact(uint32_t _aidl_code, const ::android::Parcel& _aidl_data, ::android::Parcel* _aidl_reply, uint32_t _aidl_flags) {
  ::android::status_t _aidl_ret_status = ::android::OK;
  switch (_aidl_code) {
  case BnAVBufferSpaceListener::TRANSACTION_onSpaceAvailable:
  {
    if (!(_aidl_data.checkInterface(this))) {
      _aidl_ret_status = ::android::BAD_TYPE;
      break;
    }
    ::android::binder::Status _aidl_status(onSpaceAvailable());
  }
  break;
  default:
  {
    _aidl_ret_status = ::android::BBinder::onTransact(_aidl_code, _aidl_data, _aidl_reply, _aidl_flags);
  }
  break;
  }
  if (_aidl_ret_status == ::android::UNEXPECTED_NULL) {
    _aidl_ret_status = ::android::binder::Status::fromExceptionCode(::android::binder::Status::EX_NULL_POINTER).writeOverParcel(_aidl_reply);
  }
  return _aidl_ret_status;
}

}  // namespace avbuffer
}  // namespace hal
}  // namespace rdk
}  // namespace com
