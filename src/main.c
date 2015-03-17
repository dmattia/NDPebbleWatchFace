#include <pebble.h>

static Window * s_main_window;
static BitmapLayer * s_background_layer;
static GBitmap * s_background_bitmap;
static TextLayer * s_hour;
static TextLayer * s_minute;

static void update_time() {
	time_t temp = time(NULL);
	struct tm *tick_time = localtime(&temp);
	static char split_buffer_left[] = "00";
	static char split_buffer_right[] = "00";
	
	if(clock_is_24h_style()) {
		strftime(split_buffer_left, sizeof("00"), "%H", tick_time);
	} else {
		strftime(split_buffer_left, sizeof("00"), "%I", tick_time);
	}
	strftime(split_buffer_right, sizeof("00"), "%M", tick_time);

	
	text_layer_set_text(s_hour, split_buffer_left);
	text_layer_set_text(s_minute, split_buffer_right);
}

static void main_window_load(Window * window) {
	//Create hour layer
	s_hour = text_layer_create(GRect(0,5,63,34));
	text_layer_set_text_color(s_hour, GColorWhite);
	text_layer_set_font(s_hour, fonts_get_system_font(FONT_KEY_BITHAM_34_MEDIUM_NUMBERS));
	text_layer_set_background_color(s_hour, GColorClear);
	text_layer_set_text_alignment(s_hour, GTextAlignmentCenter);
	
	//Create minute layer
	s_minute = text_layer_create(GRect(72, 5, 72, 34));
	text_layer_set_text_color(s_minute, GColorWhite);
	text_layer_set_font(s_minute, fonts_get_system_font(FONT_KEY_BITHAM_34_MEDIUM_NUMBERS));
	text_layer_set_background_color(s_minute, GColorClear);
	text_layer_set_text_alignment(s_minute, GTextAlignmentCenter);
	
	//Create bitmap
	s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_GOLDEN_DOME);
	s_background_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
	bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);

	//add layers to window
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_hour));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_minute));
}

static void main_window_unload(Window * window) {
	bitmap_layer_destroy(s_background_layer);
	gbitmap_destroy(s_background_bitmap);
	text_layer_destroy(s_hour);
}

static void tick_handler(struct tm *tick_time, TimeUnits unitsChanged) {
	update_time();
}

static void init() {
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
  .load = main_window_load,
  .unload = main_window_unload
  });
  window_stack_push(s_main_window,true);
	tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit() {
  window_destroy(s_main_window);
} 
  
int main() {
  init();
  app_event_loop();
  deinit();
}