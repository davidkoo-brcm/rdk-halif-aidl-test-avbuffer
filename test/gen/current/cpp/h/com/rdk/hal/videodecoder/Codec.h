#pragma once

#include <array>
#include <binder/Enums.h>
#include <cstdint>
#include <string>

namespace com {
namespace rdk {
namespace hal {
namespace videodecoder {
enum class Codec : int32_t {
  MPEG = 1,
  H264 = 2,
  H265 = 3,
  VP9 = 4,
  AV1 = 5,
};
}  // namespace videodecoder
}  // namespace hal
}  // namespace rdk
}  // namespace com
namespace com {
namespace rdk {
namespace hal {
namespace videodecoder {
[[nodiscard]] static inline std::string toString(Codec val) {
  switch(val) {
  case Codec::MPEG:
    return "MPEG";
  case Codec::H264:
    return "H264";
  case Codec::H265:
    return "H265";
  case Codec::VP9:
    return "VP9";
  case Codec::AV1:
    return "AV1";
  default:
    return std::to_string(static_cast<int32_t>(val));
  }
}
}  // namespace videodecoder
}  // namespace hal
}  // namespace rdk
}  // namespace com
namespace android {
namespace internal {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++17-extensions"
template <>
constexpr inline std::array<::com::rdk::hal::videodecoder::Codec, 5> enum_values<::com::rdk::hal::videodecoder::Codec> = {
  ::com::rdk::hal::videodecoder::Codec::MPEG,
  ::com::rdk::hal::videodecoder::Codec::H264,
  ::com::rdk::hal::videodecoder::Codec::H265,
  ::com::rdk::hal::videodecoder::Codec::VP9,
  ::com::rdk::hal::videodecoder::Codec::AV1,
};
#pragma clang diagnostic pop
}  // namespace internal
}  // namespace android
