#pragma once

#include <stddef.h>
#include "esp_heap_caps.h"
#include "soc/soc_caps.h"

#if SOC_CACHE_INTERNAL_MEM_VIA_L1CACHE
#include "esp_cache_private.h"
#endif

static inline void *hwss_cache_dma_alloc(size_t size){
    size_t align=4;

#if SOC_CACHE_INTERNAL_MEM_VIA_L1CACHE
    esp_cache_get_alignment(MALLOC_CAP_DMA,&align);
#endif
    return heap_caps_aligned_alloc(align,size,MALLOC_CAP_DMA);
}
