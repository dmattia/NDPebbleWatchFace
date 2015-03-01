#include <pebble.h>

static Window * s_main_window;
static TextLayer * s_split_time_layer_left;
static TextLayer * s_split_time_layer_right;
static TextLayer * s_nd_layer;
static GFont s_time_font;
static GFont s_text_font;
static BitmapLayer * s_background_layer;
static GBitmap * s_background_bitmap;

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

	
	text_layer_set_text(s_split_time_layer_left, split_buffer_left);
	text_layer_set_text(s_split_time_layer_right, split_buffer_right);
}

static void main_window_load(Window * window) {
	//Create layer for clock with space in the middle
	s_split_time_layer_left = text_layer_create(GRect(2, 5, 72, 50));
	text_layer_set_background_color(s_split_time_layer_left, GColorClear);
	text_layer_set_text_color(s_split_time_layer_left, GColorBlack);
	
	s_split_time_layer_right = text_layer_create(GRect(72, 5, 60, 50));
	text_layer_set_background_color(s_split_time_layer_right, GColorClear);
	text_layer_set_text_color(s_split_time_layer_right, GColorBlack);
	
	//Create layer for Notre Dame
	s_nd_layer = text_layer_create(GRect(0,82,140,140));
	text_layer_set_background_color(s_nd_layer, GColorClear);
	text_layer_set_text_color(s_nd_layer, GColorWhite);
	text_layer_set_text(s_nd_layer, "Notre Dame");
	
	//Create bitmap
	s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_DOME_WB);
	s_background_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
	bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
	
	update_time();
	
	//load fonts
	s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_LOBSTER_NUMBERS_80));
	s_text_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_BIRD_40));

	//set fonts and font styles
	text_layer_set_font(s_nd_layer, s_text_font);
	text_layer_set_font(s_split_time_layer_left, fonts_get_system_font(FONT_KEY_BITHAM_42_MEDIUM_NUMBERS));
	text_layer_set_font(s_split_time_layer_right, fonts_get_system_font(FONT_KEY_BITHAM_42_MEDIUM_NUMBERS));
	text_layer_set_text_alignment(s_split_time_layer_left, GTextAlignmentLeft);
	text_layer_set_text_alignment(s_split_time_layer_right, GTextAlignmentRight);
	text_layer_set_overflow_mode(s_nd_layer, GTextOverflowModeWordWrap);
	text_layer_set_text_alignment(s_nd_layer, GTextAlignmentCenter);
	
	//APP_LOG(APP_LOG_LEVEL_INFO, "%s", text_layer_get_text(s_split_time_layer_right));
	
	//add layers to window
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_split_time_layer_left));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_split_time_layer_right));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_nd_layer));
}

static void main_window_unload(Window * window) {
	fonts_unload_custom_font(s_time_font);
	fonts_unload_custom_font(s_text_font);
	bitmap_layer_destroy(s_background_layer);
	gbitmap_destroy(s_background_bitmap);
	text_layer_destroy(s_split_time_layer_left);
	text_layer_destroy(s_split_time_layer_right);
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