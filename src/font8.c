#include "font8.h"
static GBitmap *font8_bmp;
static uint8_t *font8;
// ------------------------------------------------------------------------------------------------------------------------------------------------ //
void create_font8() {
  font8_bmp = gbitmap_create_with_resource(RESOURCE_ID_FONT8);
  font8 = gbitmap_get_data(font8_bmp);
}

void destroy_font8() {
  gbitmap_destroy(font8_bmp);
}

void draw_font8_ctx(GContext *ctx, int16_t start_x, int16_t start_y, uint8_t chr) {
    uint8_t *row = font8 + (chr&3) + ((chr&252)*8);  // point to top row (8 pixels) of tile
    for(uint32_t y=0; y<8; y++, row+=4) {   // row+=4 increments to next row
      for(uint32_t x=0; x<8; x++) {
#ifdef PBL_BW
        if(*row & (1<<x))    // *row = 8 bits of row.  This masks the desired pixel
#else
        if(*row & (128>>x))  // *row = 8 bits of row.  This masks the desired pixel
#endif
          graphics_context_set_stroke_color(ctx, GColorWhite);
        else
          graphics_context_set_stroke_color(ctx, GColorBlack);
        graphics_draw_pixel(ctx, GPoint(start_x+x,start_y+y));
      }
    }
}

void draw_font8(uint8_t *screen, int16_t start_x, int16_t start_y, uint8_t color, uint8_t chr) {
#ifdef PBL_BW
  if(color==GColorBlack) {
    for(uint32_t y=0; y<8; y++) {
      uint16_t y_addr = (start_y + y) * 20;
      uint8_t row = font8[(chr&3) + ((chr&252)*8) + (y*4)];
      for(uint32_t x=0; x<8; x++) {

        // How this works
        //  uint16_t x_addr = ((start_x+x) >> 3);
        //  uint8_t x_bit = (start_x+x)&7;
        //  uint8_t pixel = (row>>x)&1;
        //  screen[y_addr + x_addr] |=   pixel << x_bit;  // White Pixel
        //  screen[y_addr + x_addr] &= ~(pixel << x_bit); // Black Pixel

        //screen[y_addr + ((start_x+x) >> 3)] |=  1 << ((start_x+x)&7); // Uncomment this line out for White Background instead of clear
        screen[y_addr + ((start_x+x) >> 3)] &= ~(((row>>x)&1) << ((start_x+x)&7)); // Black Pixel
      }
    }
  } else
  if(color==GColorWhite) {
    for(uint32_t y=0; y<8; y++) {
      uint16_t y_addr = (start_y + y) * 20;
      uint8_t row = font8[(chr&3) + ((chr&252)*8) + (y*4)];
      for(uint32_t x=0; x<8; x++) {
        //screen[y_addr + ((start_x+x) >> 3)] &= ~(1 << ((start_x+x)&7)); // Uncomment this line out for Black Background instead of clear
        screen[y_addr + ((start_x+x) >> 3)] |=  (((row>>x)&1) << ((start_x+x)&7)); // White Pixel
      }
    }
  }
#else // PBL_COLOR
  uint8_t *row = font8 + (chr&3) + ((chr&252)*8);
  for(uint32_t y=0; y<8; y++, row+=4) {
    uint8_t *addr = screen + ((start_y+y)*144) + start_x;
    for(uint32_t x=0; x<8; x++, addr++) {
      //if(*row & (128>>x)) screen[addr] = color;   // less confusing version of below line
        *addr = (*row & (128>>x)) ? color : *addr;  //if fontpixel doesn't = 0 then screenpixel=color, else screenpixel=screenpixel (i.e. doesn't change, clear tile background)
      //*addr = (*row & (128>>x)) ? foreground_color : background_color;  //easily modifiable for two-color
    }
  }
#endif
}

void draw_font8_text(uint8_t *screen, int16_t x, int16_t y, uint8_t color, char *str) {
  uint8_t strpos=0;
  while(str[strpos]>0) {
    draw_font8(screen, x, y, color, str[strpos]);
    x+=8; strpos++;
  }
}