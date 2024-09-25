#include <com/rdk/hal/videodecoder/IVideoDecoderControllerListener.h>
#include <com/rdk/hal/videodecoder/BpVideoDecoderControllerListener.h>
namespace com {
namespace rdk {
namespace hal {
namespace videodecoder {
DO_NOT_DIRECTLY_USE_ME_IMPLEMENT_META_INTERFACE(VideoDecoderControllerListener, "com.rdk.hal.videodecoder.IVideoDecoderControllerListener")
}  // namespace videodecoder
}  // namespace hal
}  // namespace rdk
}  // namespace com
#include <com/rdk/hal/videodecoder/BpVideoDecoderControllerListener.h>
#include <com/rdk/hal/videodecoder/BnVideoDecoderControllerListener.h>
#include <binder/Parcel.h>
#include <android-base/macros.h>

namespace com {
namespace rdk {
namespace hal {
namespace videodecoder {

BpVideoDecoderControllerListener::BpVideoDecoderControllerListener(const ::android::sp<::android::IBinder>& _aidl_impl)
    : BpInterface<IVideoDecoderControllerListener>(_aidl_impl){
}

::android::binder::Status BpVideoDecoderControllerListener::onFrameOutput(int64_t nsPresentationTime, int64_t frameBufferHandle, const ::std::optional<::com::rdk::hal::videodecoder::FrameMetadata>& metadata) {
  ::android::Parcel _aidl_data;
  _aidl_data.markForBinder(remoteStrong());
  ::android::Parcel _aidl_reply;
  ::android::status_t _aidl_ret_status = ::android::OK;
  ::android::binder::Status _aidl_status;
  _aidl_ret_status = _aidl_data.writeInterfaceToken(getInterfaceDescriptor());
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  _aidl_ret_status = _aidl_data.writeInt64(nsPresentationTime);
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  _aidl_ret_status = _aidl_data.writeInt64(frameBufferHandle);
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  _aidl_ret_status = _aidl_data.writeNullableParcelable(metadata);
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  _aidl_ret_status = remote()->transact(BnVideoDecoderControllerListener::TRANSACTION_onFrameOutput, _aidl_data, &_aidl_reply, ::android::IBinder::FLAG_ONEWAY);
  if (UNLIKELY(_aidl_ret_status == ::android::UNKNOWN_TRANSACTION && IVideoDecoderControllerListener::getDefaultImpl())) {
     return IVideoDecoderControllerListener::getDefaultImpl()->onFrameOutput(nsPresentationTime, frameBufferHandle, metadata);
  }
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  _aidl_error:
  _aidl_status.setFromStatusT(_aidl_ret_status);
  return _aidl_status;
}

::android::binder::Status BpVideoDecoderControllerListener::onUserDataOutput(int64_t nsPresentationTime, const ::std::vector<uint8_t>& userData) {
  ::android::Parcel _aidl_data;
  _aidl_data.markForBinder(remoteStrong());
  ::android::Parcel _aidl_reply;
  ::android::status_t _aidl_ret_status = ::android::OK;
  ::android::binder::Status _aidl_status;
  _aidl_ret_status = _aidl_data.writeInterfaceToken(getInterfaceDescriptor());
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  _aidl_ret_status = _aidl_data.writeInt64(nsPresentationTime);
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  _aidl_ret_status = _aidl_data.writeByteVector(userData);
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  _aidl_ret_status = remote()->transact(BnVideoDecoderControllerListener::TRANSACTION_onUserDataOutput, _aidl_data, &_aidl_reply, ::android::IBinder::FLAG_ONEWAY);
  if (UNLIKELY(_aidl_ret_status == ::android::UNKNOWN_TRANSACTION && IVideoDecoderControllerListener::getDefaultImpl())) {
     return IVideoDecoderControllerListener::getDefaultImpl()->onUserDataOutput(nsPresentationTime, userData);
  }
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  _aidl_error:
  _aidl_status.setFromStatusT(_aidl_ret_status);
  return _aidl_status;
}

}  // namespace videodecoder
}  // namespace hal
}  // namespace rdk
}  // namespace com
#include <com/rdk/hal/videodecoder/BnVideoDecoderControllerListener.h>
#include <binder/Parcel.h>
#include <binder/Stability.h>

namespace com {
namespace rdk {
namespace hal {
namespace videodecoder {

BnVideoDecoderControllerListener::BnVideoDecoderControllerListener()
{
  ::android::internal::Stability::markVintf(this);
}

::android::status_t BnVideoDecoderControllerListener::onTransact(uint32_t _aidl_code, const ::android::Parcel& _aidl_data, ::android::Parcel* _aidl_reply, uint32_t _aidl_flags) {
  ::android::status_t _aidl_ret_status = ::android::OK;
  switch (_aidl_code) {
  case BnVideoDecoderControllerListener::TRANSACTION_onFrameOutput:
  {
    int64_t in_nsPresentationTime;
    int64_t in_frameBufferHandle;
    ::std::optional<::com::rdk::hal::videodecoder::FrameMetadata> in_metadata;
    if (!(_aidl_data.checkInterface(this))) {
      _aidl_ret_status = ::android::BAD_TYPE;
      break;
    }
    _aidl_ret_status = _aidl_data.readInt64(&in_nsPresentationTime);
    if (((_aidl_ret_status) != (::android::OK))) {
      break;
    }
    _aidl_ret_status = _aidl_data.readInt64(&in_frameBufferHandle);
    if (((_aidl_ret_status) != (::android::OK))) {
      break;
    }
    _aidl_ret_status = _aidl_data.readParcelable(&in_metadata);
    if (((_aidl_ret_status) != (::android::OK))) {
      break;
    }
    if (auto st = _aidl_data.enforceNoDataAvail(); !st.isOk()) {
      _aidl_ret_status = st.writeToParcel(_aidl_reply);
      break;
    }
    ::android::binder::Status _aidl_status(onFrameOutput(in_nsPresentationTime, in_frameBufferHandle, in_metadata));
  }
  break;
  case BnVideoDecoderControllerListener::TRANSACTION_onUserDataOutput:
  {
    int64_t in_nsPresentationTime;
    ::std::vector<uint8_t> in_userData;
    if (!(_aidl_data.checkInterface(this))) {
      _aidl_ret_status = ::android::BAD_TYPE;
      break;
    }
    _aidl_ret_status = _aidl_data.readInt64(&in_nsPresentationTime);
    if (((_aidl_ret_status) != (::android::OK))) {
      break;
    }
    _aidl_ret_status = _aidl_data.readByteVector(&in_userData);
    if (((_aidl_ret_status) != (::android::OK))) {
      break;
    }
    if (auto st = _aidl_data.enforceNoDataAvail(); !st.isOk()) {
      _aidl_ret_status = st.writeToParcel(_aidl_reply);
      break;
    }
    ::android::binder::Status _aidl_status(onUserDataOutput(in_nsPresentationTime, in_userData));
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

}  // namespace videodecoder
}  // namespace hal
}  // namespace rdk
}  // namespace com
