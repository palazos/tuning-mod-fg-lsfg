#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "../vk_dispatch.h"

#ifdef __cplusplus
extern "C" {
#endif

#define VKR_LSFG_MAX_GENERATIONS 3u
#define VKR_LSFG_MAX_TARGETS 7u

// Hidden visibility plus a linker export list left these local, so dlsym failed.
#define VKR_LSFG_API __attribute__((visibility("default")))

typedef struct VkrLsfg VkrLsfg;

// The plugin has its own vkd table. Fill it before vkr_lsfg_create.
VKR_LSFG_API bool vkr_lsfg_bind_dispatch(PFN_vkGetInstanceProcAddr gipa, VkInstance instance);

VKR_LSFG_API VkrLsfg* vkr_lsfg_create(VkDevice device, VkPhysicalDevice physical_device,
                         const char* cache_path, bool allow_fp16);
VKR_LSFG_API void vkr_lsfg_destroy(VkrLsfg* lsfg);

VKR_LSFG_API void vkr_lsfg_configure(VkrLsfg* lsfg, uint32_t multiplier, uint32_t target_rate,
                        float flow_scale, float refresh_rate);

VKR_LSFG_API void vkr_lsfg_set_refresh_rate(VkrLsfg* lsfg, float refresh_rate);

VKR_LSFG_API void vkr_lsfg_set_guest_extent(VkrLsfg* lsfg, uint32_t width, uint32_t height);

VKR_LSFG_API bool vkr_lsfg_needs_rebuild(const VkrLsfg* lsfg, uint32_t width, uint32_t height,
                            VkFormat format);

VKR_LSFG_API bool vkr_lsfg_prepare(VkrLsfg* lsfg, uint32_t width, uint32_t height, VkFormat format);

VKR_LSFG_API uint32_t vkr_lsfg_plan(VkrLsfg* lsfg, uint32_t capacity, uint64_t source_frames);

VKR_LSFG_API void vkr_lsfg_process(VkrLsfg* lsfg, VkCommandBuffer cmd, VkImage source,
                      uint32_t width, uint32_t height, uint32_t generations);

VKR_LSFG_API void vkr_lsfg_generate_into(VkrLsfg* lsfg, VkCommandBuffer cmd, uint32_t generation,
                            uint32_t target_index, VkImage target_image, VkImageView target_view,
                            uint32_t width, uint32_t height);

VKR_LSFG_API void vkr_lsfg_forget_targets(VkrLsfg* lsfg);

VKR_LSFG_API void vkr_lsfg_reset(VkrLsfg* lsfg);

#ifdef __cplusplus
}
#endif
