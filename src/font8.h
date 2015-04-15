#pragma once
#include "pebble.h"
void create_font8();
void destroy_font8();
void draw_font8_ctx(GContext *ctx, int16_t start_x, int16_t start_y, uint8_t chr);
void draw_font8(uint8_t *screen, int16_t x, int16_t y, uint8_t color, uint8_t chr);
void draw_font8_text(uint8_t *screen, int16_t x, int16_t y, uint8_t color, char *str);
void draw_font8_fast(uint8_t *screen, int16_t start_x, int16_t start_y, uint8_t chr);