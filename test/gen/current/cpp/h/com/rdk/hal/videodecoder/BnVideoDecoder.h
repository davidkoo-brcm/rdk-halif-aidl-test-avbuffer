#pragma once

#include <binder/IInterface.h>
#include <com/rdk/hal/videodecoder/IVideoDecoder.h>
//#include <binder/Delegate.h>
#include <com/rdk/hal/videodecoder/BnVideoDecoder.h>
#include <com/rdk/hal/videodecoder/BnVideoDecoderController.h>
#include <com/rdk/hal/videodecoder/BnVideoDecoderControllerListener.h>
#include <com/rdk/hal/videodecoder/BnVideoDecoderEventListener.h>


namespace com {
namespace rdk {
namespace hal {
namespace videodecoder {
class BnVideoDecoder : public ::android::BnInterface<IVideoDecoder> {
public:
  static constexpr uint32_t TRANSACTION_getCapabilities = ::android::IBinder::FIRST_CALL_TRANSACTION + 0;
  static constexpr uint32_t TRANSACTION_getProperty = ::android::IBinder::FIRST_CALL_TRANSACTION + 1;
  static constexpr uint32_t TRANSACTION_setProperty = ::android::IBinder::FIRST_CALL_TRANSACTION + 2;
  static constexpr uint32_t TRANSACTION_getPropertyMulti = ::android::IBinder::FIRST_CALL_TRANSACTION + 3;
  static constexpr uint32_t TRANSACTION_getState = ::android::IBinder::FIRST_CALL_TRANSACTION + 4;
  static constexpr uint32_t TRANSACTION_open = ::android::IBinder::FIRST_CALL_TRANSACTION + 5;
  static constexpr uint32_t TRANSACTION_close = ::android::IBinder::FIRST_CALL_TRANSACTION + 6;
  static constexpr uint32_t TRANSACTION_registerEventListener = ::android::IBinder::FIRST_CALL_TRANSACTION + 7;
  static constexpr uint32_t TRANSACTION_unregisterEventListener = ::android::IBinder::FIRST_CALL_TRANSACTION + 8;
  explicit BnVideoDecoder();
  ::android::status_t onTransact(uint32_t _aidl_code, const ::android::Parcel& _aidl_data, ::android::Parcel* _aidl_reply, uint32_t _aidl_flags) override;
};  // class BnVideoDecoder

class IVideoDecoderDelegator : public BnVideoDecoder {
public:
  explicit IVideoDecoderDelegator(const ::android::sp<IVideoDecoder> &impl) : _aidl_delegate(impl) {}

  ::android::sp<IVideoDecoder> getImpl() { return _aidl_delegate; }
  ::android::binder::Status getCapabilities(::com::rdk::hal::videodecoder::Capabilities* _aidl_return) override {
    return _aidl_delegate->getCapabilities(_aidl_return);
  }
  ::android::binder::Status getProperty(::com::rdk::hal::videodecoder::Property property, ::std::optional<::com::rdk::hal::PropertyValue>* _aidl_return) override {
    return _aidl_delegate->getProperty(property, _aidl_return);
  }
  ::android::binder::Status setProperty(::com::rdk::hal::videodecoder::Property property, const ::com::rdk::hal::PropertyValue& propertyValue, bool* _aidl_return) override {
    return _aidl_delegate->setProperty(property, propertyValue, _aidl_return);
  }
  ::android::binder::Status getPropertyMulti(::std::vector<::com::rdk::hal::videodecoder::PropertyKVPair>* propertyKVList, bool* _aidl_return) override {
    return _aidl_delegate->getPropertyMulti(propertyKVList, _aidl_return);
  }
  ::android::binder::Status getState(::com::rdk::hal::State* _aidl_return) override {
    return _aidl_delegate->getState(_aidl_return);
  }
  ::android::binder::Status open(::com::rdk::hal::videodecoder::Codec codec, bool secure, const ::android::sp<::com::rdk::hal::videodecoder::IVideoDecoderControllerListener>& videoDecoderControllerListener, ::android::sp<::com::rdk::hal::videodecoder::IVideoDecoderController>* _aidl_return) override {
    ::android::sp<::com::rdk::hal::videodecoder::IVideoDecoderControllerListenerDelegator> _videoDecoderControllerListener;
    /*if (videoDecoderControllerListener) {
      _videoDecoderControllerListener = ::android::sp<::com::rdk::hal::videodecoder::IVideoDecoderControllerListenerDelegator>::cast(delegate(videoDecoderControllerListener));
    }*/
    auto _status = _aidl_delegate->open(codec, secure, _videoDecoderControllerListener, _aidl_return);
    /*if (*_aidl_return) {
      *_aidl_return = ::android::sp<::com::rdk::hal::videodecoder::IVideoDecoderControllerDelegator>::cast(delegate(*_aidl_return));
    }*/
    return _status;
  }
  ::android::binder::Status close(const ::android::sp<::com::rdk::hal::videodecoder::IVideoDecoderController>& videoDecoderController, bool* _aidl_return) override {
    ::android::sp<::com::rdk::hal::videodecoder::IVideoDecoderControllerDelegator> _videoDecoderController;
    /*if (videoDecoderController) {
      _videoDecoderController = ::android::sp<::com::rdk::hal::videodecoder::IVideoDecoderControllerDelegator>::cast(delegate(videoDecoderController));
    }*/
    return _aidl_delegate->close(_videoDecoderController, _aidl_return);
  }
  ::android::binder::Status registerEventListener(const ::android::sp<::com::rdk::hal::videodecoder::IVideoDecoderEventListener>& videoDecoderEventListener, bool* _aidl_return) override {
    ::android::sp<::com::rdk::hal::videodecoder::IVideoDecoderEventListenerDelegator> _videoDecoderEventListener;
    /*if (videoDecoderEventListener) {
      _videoDecoderEventListener = ::android::sp<::com::rdk::hal::videodecoder::IVideoDecoderEventListenerDelegator>::cast(delegate(videoDecoderEventListener));
    }*/
    return _aidl_delegate->registerEventListener(_videoDecoderEventListener, _aidl_return);
  }
  ::android::binder::Status unregisterEventListener(const ::android::sp<::com::rdk::hal::videodecoder::IVideoDecoderEventListener>& videoDecoderEventListener, bool* _aidl_return) override {
    ::android::sp<::com::rdk::hal::videodecoder::IVideoDecoderEventListenerDelegator> _videoDecoderEventListener;
    /*if (videoDecoderEventListener) {
      _videoDecoderEventListener = ::android::sp<::com::rdk::hal::videodecoder::IVideoDecoderEventListenerDelegator>::cast(delegate(videoDecoderEventListener));
    }*/
    return _aidl_delegate->unregisterEventListener(_videoDecoderEventListener, _aidl_return);
  }
private:
  ::android::sp<IVideoDecoder> _aidl_delegate;
};  // class IVideoDecoderDelegator
}  // namespace videodecoder
}  // namespace hal
}  // namespace rdk
}  // namespace com
