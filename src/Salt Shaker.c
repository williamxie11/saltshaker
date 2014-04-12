/* IT'S SALT SHAKING TIME! */
// @author William Xie
// http://github.com/williamxie11/saltshaker

/* 
Inspired by Cookie Clicker, Salt Shaker gives you the power to command armies 
of salty grandpas and build infinite salt mines to create your salt empire!
*/

#include <pebble.h>

static Window *window;
static TextLayer *text_layer;
static AppTimer *timer;

// Buy Salt Mine
static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Bought a Salt Mine!");
  text_layer_set_text_color(text_layer, GColorWhite);
}

// Intense Shake Manual Button
static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "[SALT INTENSIFIES]");
  text_layer_set_text_color(text_layer, GColorWhite);
}

// Buy Salty Grandpa
static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Bought a Salty Grandpa!");
  text_layer_set_text_color(text_layer, GColorWhite);
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  /*
  GRect bounds = layer_get_bounds(window_layer);

  text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 20 } });
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
  */
  GRect bounds = layer_get_bounds(window_layer);
  text_layer = text_layer_create((GRect) { .origin = {0, 130}, .size = {bounds.size.w, 20} });
  text_layer_set_background_color(text_layer, GColorBlack);
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
}

static void init(void) {
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
  window_set_background_color(window, GColorBlack);

  //accel_data_service_subscribe(0, NULL);
}

/*
static void init(void) {
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload
  });
  window_stack_push(window, true);
  window_set_background_color(window, GColorBlack);

  accel_data_service_subscribe(0, NULL);

  //timer = app_timer_register(ACCEL_STEP_MS, timer_callback, NULL);
}
*/

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
