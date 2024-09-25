#pragma once

#include <binder/IBinder.h>
#include <binder/IInterface.h>
#include <utils/Errors.h>
#include <com/rdk/hal/videodecoder/IVideoDecoderController.h>

namespace com {
namespace rdk {
namespace hal {
namespace videodecoder {
class BpVideoDecoderController : public ::android::BpInterface<IVideoDecoderController> {
public:
  explicit BpVideoDecoderController(const ::android::sp<::android::IBinder>& _aidl_impl);
  virtual ~BpVideoDecoderController() = default;
  ::android::binder::Status start() override;
  ::android::binder::Status stop() override;
  ::android::binder::Status decodeBuffer(int64_t nsPresentationTime, int64_t bufferHandle) override;
  ::android::binder::Status flush(bool reset) override;
  ::android::binder::Status signalDiscontinuity() override;
  ::android::binder::Status signalEOS() override;
  ::android::binder::Status parseCodecSpecificData(::com::rdk::hal::videodecoder::CSDVideoFormat csdVideoFormat, const ::std::vector<uint8_t>& codecData, bool* _aidl_return) override;
};  // class BpVideoDecoderController
}  // namespace videodecoder
}  // namespace hal
}  // namespace rdk
}  // namespace com
