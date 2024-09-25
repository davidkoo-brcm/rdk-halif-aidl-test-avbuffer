#pragma once

#include <binder/IBinder.h>
#include <binder/IInterface.h>
#include <binder/Status.h>
#include <com/rdk/hal/audiodecoder/IAudioDecoder.h>
#include <com/rdk/hal/avbuffer/HeapMetrics.h>
#include <com/rdk/hal/avbuffer/IAVBufferSpaceListener.h>
#include <com/rdk/hal/avbuffer/Pool.h>
#include <com/rdk/hal/avbuffer/PoolMetrics.h>
#include <com/rdk/hal/videodecoder/IVideoDecoder.h>
#include <cstdint>
#include <string>
#include <utils/StrongPointer.h>
#include <vector>

namespace com::rdk::hal::avbuffer {
class HeapMetrics;
class IAVBufferSpaceListener;
class Pool;
class PoolMetrics;
}  // namespace com::rdk::hal::avbuffer
namespace com {
namespace rdk {
namespace hal {
namespace avbuffer {
class IAVBufferDelegator;

class IAVBuffer : public ::android::IInterface {
public:
  typedef IAVBufferDelegator DefaultDelegator;
  DECLARE_META_INTERFACE(AVBuffer)
  static const ::std::string& serviceName();
  enum : int64_t { INVALID_HANDLE = -1L };
  virtual ::android::binder::Status getHeapMetrics(bool secureHeap, ::com::rdk::hal::avbuffer::HeapMetrics* _aidl_return) = 0;
  virtual ::android::binder::Status createVideoPool(bool secureHeap, const ::com::rdk::hal::videodecoder::IVideoDecoder::Id& videoDecoderId, const ::android::sp<::com::rdk::hal::avbuffer::IAVBufferSpaceListener>& listener, ::com::rdk::hal::avbuffer::Pool* _aidl_return) = 0;
  virtual ::android::binder::Status createAudioPool(bool secureHeap, const ::com::rdk::hal::audiodecoder::IAudioDecoder::Id& audioDecoderId, const ::android::sp<::com::rdk::hal::avbuffer::IAVBufferSpaceListener>& listener, ::com::rdk::hal::avbuffer::Pool* _aidl_return) = 0;
  virtual ::android::binder::Status destroyPool(const ::com::rdk::hal::avbuffer::Pool& poolHandle) = 0;
  virtual ::android::binder::Status getPoolMetrics(const ::com::rdk::hal::avbuffer::Pool& poolHandle, ::com::rdk::hal::avbuffer::PoolMetrics* _aidl_return) = 0;
  virtual ::android::binder::Status getAllPoolMetrics(bool secureHeap, ::std::vector<::com::rdk::hal::avbuffer::PoolMetrics>* _aidl_return) = 0;
  virtual ::android::binder::Status alloc(const ::com::rdk::hal::avbuffer::Pool& poolHandle, int32_t size, int64_t* _aidl_return) = 0;
  virtual ::android::binder::Status notifyWhenSpaceAvailable(const ::com::rdk::hal::avbuffer::Pool& poolHandle, int32_t size) = 0;
  virtual ::android::binder::Status trimSize(int64_t bufferHandle, int32_t newSize) = 0;
  virtual ::android::binder::Status free(int64_t bufferHandle) = 0;
  virtual ::android::binder::Status isValid(int64_t bufferHandle, bool* _aidl_return) = 0;
  virtual ::android::binder::Status getAllocList(const ::com::rdk::hal::avbuffer::Pool& poolHandle, ::std::vector<int64_t>* _aidl_return) = 0;
  virtual ::android::binder::Status calculateSHA1(int64_t bufferHandle, ::std::vector<uint8_t>* _aidl_return) = 0;
};  // class IAVBuffer

class IAVBufferDefault : public IAVBuffer {
public:
  ::android::IBinder* onAsBinder() override {
    return nullptr;
  }
  ::android::binder::Status getHeapMetrics(bool /*secureHeap*/, ::com::rdk::hal::avbuffer::HeapMetrics* /*_aidl_return*/) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
  ::android::binder::Status createVideoPool(bool /*secureHeap*/, const ::com::rdk::hal::videodecoder::IVideoDecoder::Id& /*videoDecoderId*/, const ::android::sp<::com::rdk::hal::avbuffer::IAVBufferSpaceListener>& /*listener*/, ::com::rdk::hal::avbuffer::Pool* /*_aidl_return*/) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
  ::android::binder::Status createAudioPool(bool /*secureHeap*/, const ::com::rdk::hal::audiodecoder::IAudioDecoder::Id& /*audioDecoderId*/, const ::android::sp<::com::rdk::hal::avbuffer::IAVBufferSpaceListener>& /*listener*/, ::com::rdk::hal::avbuffer::Pool* /*_aidl_return*/) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
  ::android::binder::Status destroyPool(const ::com::rdk::hal::avbuffer::Pool& /*poolHandle*/) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
  ::android::binder::Status getPoolMetrics(const ::com::rdk::hal::avbuffer::Pool& /*poolHandle*/, ::com::rdk::hal::avbuffer::PoolMetrics* /*_aidl_return*/) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
  ::android::binder::Status getAllPoolMetrics(bool /*secureHeap*/, ::std::vector<::com::rdk::hal::avbuffer::PoolMetrics>* /*_aidl_return*/) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
  ::android::binder::Status alloc(const ::com::rdk::hal::avbuffer::Pool& /*poolHandle*/, int32_t /*size*/, int64_t* /*_aidl_return*/) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
  ::android::binder::Status notifyWhenSpaceAvailable(const ::com::rdk::hal::avbuffer::Pool& /*poolHandle*/, int32_t /*size*/) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
  ::android::binder::Status trimSize(int64_t /*bufferHandle*/, int32_t /*newSize*/) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
  ::android::binder::Status free(int64_t /*bufferHandle*/) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
  ::android::binder::Status isValid(int64_t /*bufferHandle*/, bool* /*_aidl_return*/) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
  ::android::binder::Status getAllocList(const ::com::rdk::hal::avbuffer::Pool& /*poolHandle*/, ::std::vector<int64_t>* /*_aidl_return*/) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
  ::android::binder::Status calculateSHA1(int64_t /*bufferHandle*/, ::std::vector<uint8_t>* /*_aidl_return*/) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
};  // class IAVBufferDefault
}  // namespace avbuffer
}  // namespace hal
}  // namespace rdk
}  // namespace com
