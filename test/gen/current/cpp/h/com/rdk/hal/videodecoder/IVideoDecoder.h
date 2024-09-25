#pragma once

#include <android/binder_to_string.h>
#include <binder/IBinder.h>
#include <binder/IInterface.h>
#include <binder/Parcel.h>
#include <binder/Status.h>
#include <com/rdk/hal/PropertyValue.h>
#include <com/rdk/hal/State.h>
#include <com/rdk/hal/videodecoder/Capabilities.h>
#include <com/rdk/hal/videodecoder/Codec.h>
#include <com/rdk/hal/videodecoder/IVideoDecoderController.h>
#include <com/rdk/hal/videodecoder/IVideoDecoderControllerListener.h>
#include <com/rdk/hal/videodecoder/IVideoDecoderEventListener.h>
#include <com/rdk/hal/videodecoder/Property.h>
#include <com/rdk/hal/videodecoder/PropertyKVPair.h>
#include <cstdint>
#include <optional>
#include <tuple>
#include <utils/String16.h>
#include <utils/StrongPointer.h>
#include <vector>

namespace com::rdk::hal {
class PropertyValue;
}  // namespace com::rdk::hal
namespace com::rdk::hal::videodecoder {
class Capabilities;
class IVideoDecoderController;
class IVideoDecoderControllerListener;
class IVideoDecoderEventListener;
class PropertyKVPair;
}  // namespace com::rdk::hal::videodecoder
namespace com {
namespace rdk {
namespace hal {
namespace videodecoder {
class IVideoDecoderDelegator;

class IVideoDecoder : public ::android::IInterface {
public:
  typedef IVideoDecoderDelegator DefaultDelegator;
  DECLARE_META_INTERFACE(VideoDecoder)
  class Id : public ::android::Parcelable {
  public:
    int32_t value = 0;
    inline bool operator!=(const Id& rhs) const {
      return std::tie(value) != std::tie(rhs.value);
    }
    inline bool operator<(const Id& rhs) const {
      return std::tie(value) < std::tie(rhs.value);
    }
    inline bool operator<=(const Id& rhs) const {
      return std::tie(value) <= std::tie(rhs.value);
    }
    inline bool operator==(const Id& rhs) const {
      return std::tie(value) == std::tie(rhs.value);
    }
    inline bool operator>(const Id& rhs) const {
      return std::tie(value) > std::tie(rhs.value);
    }
    inline bool operator>=(const Id& rhs) const {
      return std::tie(value) >= std::tie(rhs.value);
    }

    enum : int32_t { UNDEFINED = -1 };
    ::android::Parcelable::Stability getStability() const override { return ::android::Parcelable::Stability::STABILITY_VINTF; }
    ::android::status_t readFromParcel(const ::android::Parcel* _aidl_parcel) final;
    ::android::status_t writeToParcel(::android::Parcel* _aidl_parcel) const final;
    static const ::android::String16& getParcelableDescriptor() {
      static const ::android::StaticString16 DESCRIPTOR (u"com.rdk.hal.videodecoder.IVideoDecoder.Id");
      return DESCRIPTOR;
    }
    inline std::string toString() const {
      std::ostringstream os;
      os << "Id{";
      os << "value: " << ::android::internal::ToString(value);
      os << "}";
      return os.str();
    }
  };  // class Id
  virtual ::android::binder::Status getCapabilities(::com::rdk::hal::videodecoder::Capabilities* _aidl_return) = 0;
  virtual ::android::binder::Status getProperty(::com::rdk::hal::videodecoder::Property property, ::std::optional<::com::rdk::hal::PropertyValue>* _aidl_return) = 0;
  virtual ::android::binder::Status setProperty(::com::rdk::hal::videodecoder::Property property, const ::com::rdk::hal::PropertyValue& propertyValue, bool* _aidl_return) = 0;
  virtual ::android::binder::Status getPropertyMulti(::std::vector<::com::rdk::hal::videodecoder::PropertyKVPair>* propertyKVList, bool* _aidl_return) = 0;
  virtual ::android::binder::Status getState(::com::rdk::hal::State* _aidl_return) = 0;
  virtual ::android::binder::Status open(::com::rdk::hal::videodecoder::Codec codec, bool secure, const ::android::sp<::com::rdk::hal::videodecoder::IVideoDecoderControllerListener>& videoDecoderControllerListener, ::android::sp<::com::rdk::hal::videodecoder::IVideoDecoderController>* _aidl_return) = 0;
  virtual ::android::binder::Status close(const ::android::sp<::com::rdk::hal::videodecoder::IVideoDecoderController>& videoDecoderController, bool* _aidl_return) = 0;
  virtual ::android::binder::Status registerEventListener(const ::android::sp<::com::rdk::hal::videodecoder::IVideoDecoderEventListener>& videoDecoderEventListener, bool* _aidl_return) = 0;
  virtual ::android::binder::Status unregisterEventListener(const ::android::sp<::com::rdk::hal::videodecoder::IVideoDecoderEventListener>& videoDecoderEventListener, bool* _aidl_return) = 0;
};  // class IVideoDecoder

class IVideoDecoderDefault : public IVideoDecoder {
public:
  ::android::IBinder* onAsBinder() override {
    return nullptr;
  }
  ::android::binder::Status getCapabilities(::com::rdk::hal::videodecoder::Capabilities* /*_aidl_return*/) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
  ::android::binder::Status getProperty(::com::rdk::hal::videodecoder::Property /*property*/, ::std::optional<::com::rdk::hal::PropertyValue>* /*_aidl_return*/) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
  ::android::binder::Status setProperty(::com::rdk::hal::videodecoder::Property /*property*/, const ::com::rdk::hal::PropertyValue& /*propertyValue*/, bool* /*_aidl_return*/) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
  ::android::binder::Status getPropertyMulti(::std::vector<::com::rdk::hal::videodecoder::PropertyKVPair>* /*propertyKVList*/, bool* /*_aidl_return*/) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
  ::android::binder::Status getState(::com::rdk::hal::State* /*_aidl_return*/) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
  ::android::binder::Status open(::com::rdk::hal::videodecoder::Codec /*codec*/, bool /*secure*/, const ::android::sp<::com::rdk::hal::videodecoder::IVideoDecoderControllerListener>& /*videoDecoderControllerListener*/, ::android::sp<::com::rdk::hal::videodecoder::IVideoDecoderController>* /*_aidl_return*/) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
  ::android::binder::Status close(const ::android::sp<::com::rdk::hal::videodecoder::IVideoDecoderController>& /*videoDecoderController*/, bool* /*_aidl_return*/) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
  ::android::binder::Status registerEventListener(const ::android::sp<::com::rdk::hal::videodecoder::IVideoDecoderEventListener>& /*videoDecoderEventListener*/, bool* /*_aidl_return*/) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
  ::android::binder::Status unregisterEventListener(const ::android::sp<::com::rdk::hal::videodecoder::IVideoDecoderEventListener>& /*videoDecoderEventListener*/, bool* /*_aidl_return*/) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
};  // class IVideoDecoderDefault
}  // namespace videodecoder
}  // namespace hal
}  // namespace rdk
}  // namespace com
