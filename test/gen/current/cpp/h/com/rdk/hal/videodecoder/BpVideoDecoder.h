#pragma once

#include <binder/IBinder.h>
#include <binder/IInterface.h>
#include <utils/Errors.h>
#include <com/rdk/hal/videodecoder/IVideoDecoder.h>

namespace com {
namespace rdk {
namespace hal {
namespace videodecoder {
class BpVideoDecoder : public ::android::BpInterface<IVideoDecoder> {
public:
  explicit BpVideoDecoder(const ::android::sp<::android::IBinder>& _aidl_impl);
  virtual ~BpVideoDecoder() = default;
  ::android::binder::Status getCapabilities(::com::rdk::hal::videodecoder::Capabilities* _aidl_return) override;
  ::android::binder::Status getProperty(::com::rdk::hal::videodecoder::Property property, ::std::optional<::com::rdk::hal::PropertyValue>* _aidl_return) override;
  ::android::binder::Status setProperty(::com::rdk::hal::videodecoder::Property property, const ::com::rdk::hal::PropertyValue& propertyValue, bool* _aidl_return) override;
  ::android::binder::Status getPropertyMulti(::std::vector<::com::rdk::hal::videodecoder::PropertyKVPair>* propertyKVList, bool* _aidl_return) override;
  ::android::binder::Status getState(::com::rdk::hal::State* _aidl_return) override;
  ::android::binder::Status open(::com::rdk::hal::videodecoder::Codec codec, bool secure, const ::android::sp<::com::rdk::hal::videodecoder::IVideoDecoderControllerListener>& videoDecoderControllerListener, ::android::sp<::com::rdk::hal::videodecoder::IVideoDecoderController>* _aidl_return) override;
  ::android::binder::Status close(const ::android::sp<::com::rdk::hal::videodecoder::IVideoDecoderController>& videoDecoderController, bool* _aidl_return) override;
  ::android::binder::Status registerEventListener(const ::android::sp<::com::rdk::hal::videodecoder::IVideoDecoderEventListener>& videoDecoderEventListener, bool* _aidl_return) override;
  ::android::binder::Status unregisterEventListener(const ::android::sp<::com::rdk::hal::videodecoder::IVideoDecoderEventListener>& videoDecoderEventListener, bool* _aidl_return) override;
};  // class BpVideoDecoder
}  // namespace videodecoder
}  // namespace hal
}  // namespace rdk
}  // namespace com
