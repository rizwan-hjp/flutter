// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_VULKAN_VULKAN_PROC_TABLE_H_
#define FLUTTER_VULKAN_VULKAN_PROC_TABLE_H_

#include "flutter/fml/macros.h"
#include "flutter/fml/memory/ref_counted.h"
#include "flutter/fml/memory/ref_ptr.h"
#include "flutter/fml/native_library.h"
#include "third_party/skia/include/core/SkRefCnt.h"
#include "third_party/skia/include/gpu/vk/GrVkBackendContext.h"
#include "vulkan_handle.h"
#include "vulkan_interface.h"

namespace vulkan {

class VulkanProcTable : public fml::RefCountedThreadSafe<VulkanProcTable> {
  FML_FRIEND_REF_COUNTED_THREAD_SAFE(VulkanProcTable);
  FML_FRIEND_MAKE_REF_COUNTED(VulkanProcTable);

 public:
  template <class T>
  class Proc {
   public:
    using Proto = T;

    explicit Proc(T proc = nullptr) : proc_(proc) {}

    ~Proc() { proc_ = nullptr; }

    Proc operator=(T proc) {
      proc_ = proc;
      return *this;
    }

    Proc operator=(PFN_vkVoidFunction proc) {
      proc_ = reinterpret_cast<Proto>(proc);
      return *this;
    }

    explicit operator bool() const { return proc_ != nullptr; }

    operator T() const { return proc_; }  // NOLINT(google-explicit-constructor)

   private:
    T proc_;
  };

  bool HasAcquiredMandatoryProcAddresses() const;

  bool IsValid() const;

  bool AreInstanceProcsSetup() const;

  bool AreDeviceProcsSetup() const;

  bool SetupInstanceProcAddresses(const VulkanHandle<VkInstance>& instance);

  bool SetupDeviceProcAddresses(const VulkanHandle<VkDevice>& device);

  GrVkGetProc CreateSkiaGetProc() const;

#define DEFINE_PROC(name) Proc<PFN_vk##name> name;

  DEFINE_PROC(AcquireNextImageKHR);
  DEFINE_PROC(AllocateCommandBuffers);
  DEFINE_PROC(AllocateMemory);
  DEFINE_PROC(BeginCommandBuffer);
  DEFINE_PROC(BindImageMemory);
  DEFINE_PROC(CmdPipelineBarrier);
  DEFINE_PROC(CreateCommandPool);
  DEFINE_PROC(CreateDebugReportCallbackEXT);
  DEFINE_PROC(CreateDevice);
  DEFINE_PROC(CreateFence);
  DEFINE_PROC(CreateImage);
  DEFINE_PROC(CreateInstance);
  DEFINE_PROC(CreateSemaphore);
  DEFINE_PROC(CreateSwapchainKHR);
  DEFINE_PROC(DestroyCommandPool);
  DEFINE_PROC(DestroyDebugReportCallbackEXT);
  DEFINE_PROC(DestroyDevice);
  DEFINE_PROC(DestroyFence);
  DEFINE_PROC(DestroyImage);
  DEFINE_PROC(DestroyInstance);
  DEFINE_PROC(DestroySemaphore);
  DEFINE_PROC(DestroySurfaceKHR);
  DEFINE_PROC(DestroySwapchainKHR);
  DEFINE_PROC(DeviceWaitIdle);
  DEFINE_PROC(EndCommandBuffer);
  DEFINE_PROC(EnumerateDeviceLayerProperties);
  DEFINE_PROC(EnumerateInstanceExtensionProperties);
  DEFINE_PROC(EnumerateInstanceLayerProperties);
  DEFINE_PROC(EnumeratePhysicalDevices);
  DEFINE_PROC(FreeCommandBuffers);
  DEFINE_PROC(FreeMemory);
  DEFINE_PROC(GetDeviceProcAddr);
  DEFINE_PROC(GetDeviceQueue);
  DEFINE_PROC(GetImageMemoryRequirements);
  DEFINE_PROC(GetInstanceProcAddr);
  DEFINE_PROC(GetPhysicalDeviceFeatures);
  DEFINE_PROC(GetPhysicalDeviceQueueFamilyProperties);
  DEFINE_PROC(QueueSubmit);
  DEFINE_PROC(QueueWaitIdle);
  DEFINE_PROC(ResetCommandBuffer);
  DEFINE_PROC(ResetFences);
  DEFINE_PROC(WaitForFences);
#ifndef TEST_VULKAN_PROCS
#if FML_OS_ANDROID
  DEFINE_PROC(GetPhysicalDeviceSurfaceCapabilitiesKHR);
  DEFINE_PROC(GetPhysicalDeviceSurfaceFormatsKHR);
  DEFINE_PROC(GetPhysicalDeviceSurfacePresentModesKHR);
  DEFINE_PROC(GetPhysicalDeviceSurfaceSupportKHR);
  DEFINE_PROC(GetSwapchainImagesKHR);
  DEFINE_PROC(QueuePresentKHR);
  DEFINE_PROC(CreateAndroidSurfaceKHR);
#endif  // FML_OS_ANDROID
#if OS_FUCHSIA
  DEFINE_PROC(ImportSemaphoreZirconHandleFUCHSIA);
  DEFINE_PROC(GetSemaphoreZirconHandleFUCHSIA);
  DEFINE_PROC(GetMemoryZirconHandleFUCHSIA);
  DEFINE_PROC(CreateBufferCollectionFUCHSIA);
  DEFINE_PROC(DestroyBufferCollectionFUCHSIA);
  DEFINE_PROC(SetBufferCollectionImageConstraintsFUCHSIA);
  DEFINE_PROC(GetBufferCollectionPropertiesFUCHSIA);
#endif  // OS_FUCHSIA
#endif  // TEST_VULKAN_PROCS

#undef DEFINE_PROC

 private:
  fml::RefPtr<fml::NativeLibrary> handle_;
  bool acquired_mandatory_proc_addresses_;
  VulkanHandle<VkInstance> instance_;
  VulkanHandle<VkDevice> device_;

  VulkanProcTable();
  explicit VulkanProcTable(const char* path);
  ~VulkanProcTable();
  bool OpenLibraryHandle(const char* path);
  bool SetupLoaderProcAddresses();
  bool CloseLibraryHandle();
  PFN_vkVoidFunction AcquireProc(
      const char* proc_name,
      const VulkanHandle<VkInstance>& instance) const;
  PFN_vkVoidFunction AcquireProc(const char* proc_name,
                                 const VulkanHandle<VkDevice>& device) const;

  FML_DISALLOW_COPY_AND_ASSIGN(VulkanProcTable);
};

}  // namespace vulkan

#endif  // FLUTTER_VULKAN_VULKAN_PROC_TABLE_H_
