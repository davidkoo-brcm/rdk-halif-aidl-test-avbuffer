/*------GPT---MODIFIED------*/
#pragma once

#include <array>
#include <binder/Enums.h>
#include <cstdint>
#include <string>

namespace com {
namespace rdk {
namespace hal {
enum class State : int32_t {
  ERROR_UNKNOWN = -1,
  BOOT = 0,
  ACTIVE = 1,
  STANDBY = 2,
  OFF = 3,
  WPS_CONNECTING = 4,
  WPS_CONNECTED = 5,
  WPS_ERROR = 6,
  FULL_SYSTEM_RESET = 7,
  USB_UPGRADE = 8,
  SOFTWARE_DOWNLOAD_ERROR = 9,
};
}  // namespace hal
}  // namespace rdk
}  // namespace com
namespace com {
namespace rdk {
namespace hal {
[[nodiscard]] static inline std::string toString(State val) {
  switch(val) {
  case State::ERROR_UNKNOWN:
    return "ERROR_UNKNOWN";
  case State::BOOT:
    return "BOOT";
  case State::ACTIVE:
    return "ACTIVE";
  case State::STANDBY:
    return "STANDBY";
  case State::OFF:
    return "OFF";
  case State::WPS_CONNECTING:
    return "WPS_CONNECTING";
  case State::WPS_CONNECTED:
    return "WPS_CONNECTED";
  case State::WPS_ERROR:
    return "WPS_ERROR";
  case State::FULL_SYSTEM_RESET:
    return "FULL_SYSTEM_RESET";
  case State::USB_UPGRADE:
    return "USB_UPGRADE";
  case State::SOFTWARE_DOWNLOAD_ERROR:
    return "SOFTWARE_DOWNLOAD_ERROR";
  default:
    return std::to_string(static_cast<int32_t>(val));
  }
}
}  // namespace hal
}  // namespace rdk
}  // namespace com
namespace android {
namespace internal {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++17-extensions"
template <>
constexpr inline std::array<::com::rdk::hal::State, 11> enum_values<::com::rdk::hal::State> = {
  ::com::rdk::hal::State::ERROR_UNKNOWN,
  ::com::rdk::hal::State::BOOT,
  ::com::rdk::hal::State::ACTIVE,
  ::com::rdk::hal::State::STANDBY,
  ::com::rdk::hal::State::OFF,
  ::com::rdk::hal::State::WPS_CONNECTING,
  ::com::rdk::hal::State::WPS_CONNECTED,
  ::com::rdk::hal::State::WPS_ERROR,
  ::com::rdk::hal::State::FULL_SYSTEM_RESET,
  ::com::rdk::hal::State::USB_UPGRADE,
  ::com::rdk::hal::State::SOFTWARE_DOWNLOAD_ERROR,
};
#pragma clang diagnostic pop
}  // namespace internal
}  // namespace android
