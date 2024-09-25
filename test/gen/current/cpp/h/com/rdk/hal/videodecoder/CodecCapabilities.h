#pragma once

#include <android/binder_to_string.h>
#include <binder/Parcel.h>
#include <binder/Status.h>
#include <com/rdk/hal/videodecoder/Codec.h>
#include <cstdint>
#include <tuple>
#include <utils/String16.h>

namespace com {
namespace rdk {
namespace hal {
namespace videodecoder {
class CodecCapabilities : public ::android::Parcelable {
public:
  ::com::rdk::hal::videodecoder::Codec codec = ::com::rdk::hal::videodecoder::Codec(0);
  int32_t maxFrameRate = 0;
  int32_t maxFrameWidth = 0;
  int32_t maxFrameHeight = 0;
  inline bool operator!=(const CodecCapabilities& rhs) const {
    return std::tie(codec, maxFrameRate, maxFrameWidth, maxFrameHeight) != std::tie(rhs.codec, rhs.maxFrameRate, rhs.maxFrameWidth, rhs.maxFrameHeight);
  }
  inline bool operator<(const CodecCapabilities& rhs) const {
    return std::tie(codec, maxFrameRate, maxFrameWidth, maxFrameHeight) < std::tie(rhs.codec, rhs.maxFrameRate, rhs.maxFrameWidth, rhs.maxFrameHeight);
  }
  inline bool operator<=(const CodecCapabilities& rhs) const {
    return std::tie(codec, maxFrameRate, maxFrameWidth, maxFrameHeight) <= std::tie(rhs.codec, rhs.maxFrameRate, rhs.maxFrameWidth, rhs.maxFrameHeight);
  }
  inline bool operator==(const CodecCapabilities& rhs) const {
    return std::tie(codec, maxFrameRate, maxFrameWidth, maxFrameHeight) == std::tie(rhs.codec, rhs.maxFrameRate, rhs.maxFrameWidth, rhs.maxFrameHeight);
  }
  inline bool operator>(const CodecCapabilities& rhs) const {
    return std::tie(codec, maxFrameRate, maxFrameWidth, maxFrameHeight) > std::tie(rhs.codec, rhs.maxFrameRate, rhs.maxFrameWidth, rhs.maxFrameHeight);
  }
  inline bool operator>=(const CodecCapabilities& rhs) const {
    return std::tie(codec, maxFrameRate, maxFrameWidth, maxFrameHeight) >= std::tie(rhs.codec, rhs.maxFrameRate, rhs.maxFrameWidth, rhs.maxFrameHeight);
  }

  ::android::Parcelable::Stability getStability() const override { return ::android::Parcelable::Stability::STABILITY_VINTF; }
  ::android::status_t readFromParcel(const ::android::Parcel* _aidl_parcel) final;
  ::android::status_t writeToParcel(::android::Parcel* _aidl_parcel) const final;
  static const ::android::String16& getParcelableDescriptor() {
    static const ::android::StaticString16 DESCRIPTOR (u"com.rdk.hal.videodecoder.CodecCapabilities");
    return DESCRIPTOR;
  }
  inline std::string toString() const {
    std::ostringstream os;
    os << "CodecCapabilities{";
    os << "codec: " << ::android::internal::ToString(codec);
    os << ", maxFrameRate: " << ::android::internal::ToString(maxFrameRate);
    os << ", maxFrameWidth: " << ::android::internal::ToString(maxFrameWidth);
    os << ", maxFrameHeight: " << ::android::internal::ToString(maxFrameHeight);
    os << "}";
    return os.str();
  }
};  // class CodecCapabilities
}  // namespace videodecoder
}  // namespace hal
}  // namespace rdk
}  // namespace com
