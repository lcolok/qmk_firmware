#pragma once
#include <stdint.h>
#include <stdbool.h>
#define VIA_ENABLE
#define DYNAMIC_KEYMAP_LAYER_COUNT 8
uint32_t timer_read32(void);
uint32_t timer_elapsed32(uint32_t start);
void default_layer_set(uint32_t layer);
