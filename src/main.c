#include "pebble.h"
#include "font8.h"

Window *main_window;
Layer *root_layer;

void root_layer_update(Layer *me, GContext *ctx) {
  GBitmap* framebuffer = graphics_capture_frame_buffer(ctx);
  if(framebuffer) {
    uint8_t* screen = gbitmap_get_data(framebuffer);
    
    #ifdef PBL_COLOR
      draw_font8(screen, 25, 5, GColorCyanARGB8, '0');
      draw_font8(screen, 5, 5, GColorBlackARGB8, 'A');
      draw_font8(screen, 10,10, GColorWhiteARGB8, 'B');
      for(uint8_t i=0; i<10; i++) draw_font8(screen, 10 + i*8, 60, GColorRedARGB8, 65+i);
      for(uint8_t i=0; i<10; i++) draw_font8(screen, 10 + i*8, 80, GColorGreenARGB8, 48+i);
      draw_font8_text(screen, 5, 120, GColorBlackARGB8, "Hello World!\0");
    #else
      draw_font8(screen, 5, 5, GColorBlack, 'A');
      draw_font8(screen, 10, 10, GColorWhite, 'B');
      for(uint8_t i=0; i<10; i++) draw_font8(screen, 10 + i*8, 60, GColorWhite, 48+i);
      for(uint8_t i=0; i<10; i++) draw_font8(screen, 10 + i*8, 80, GColorBlack, 48+i);
      draw_font8_text(screen, 5, 120, GColorBlack, "Hello World!\0");
    #endif
    
    graphics_release_frame_buffer(ctx, framebuffer);
  }
  
  for(uint8_t i=0; i<10; i++)
    draw_font8_ctx(ctx, 10 + i*8, 100, 48+i);
}

// ------------------------------------------------------------------------ //
//  Main Functions
// ------------------------------------------------------------------------ //
static void main_window_load(Window *window) {
  root_layer = window_get_root_layer(window);
  layer_set_update_proc(root_layer, root_layer_update);
}

static void init(void) {
  main_window = window_create();
  window_set_window_handlers(main_window, (WindowHandlers) {
    .load = main_window_load,
  });
  window_set_fullscreen(main_window, true);

  create_font8();
  
  window_stack_push(main_window, true); // Display window
}
  
static void deinit(void) {
  destroy_font8();
  window_destroy(main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}