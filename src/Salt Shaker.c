/* IT'S SALT SHAKING TIME! */
// @author William Xie
// http://github.com/williamxie11/saltshaker

/* 
Inspired by Cookie Clicker, Salt Shaker gives you the power to command armies 
of salty grandpas, build salt farms, and more to create your salt empire!
*/

#include <pebble.h>
#include <string.h>

static Window * window;

/* --- TEXT LAYERS --- */
static TextLayer * notif_layer;
static TextLayer * sps_layer;
static TextLayer * salt_layer; 

/* --- GAME VARIABLES --- */
static int salt;
static int sps; // "Salt Per Second"
static char * notification; 
    
static char salt_buffer[10];
/*
static int costGrandpa = 10;   
static int costFactory = 50;
static int costMine = 250;

static int spsGrandpa = 1;
static int spsFactory = 10;
static int spsMine = 100;
*/

/* ------------------------------------------------------- */
/* -                 Salty Game Functions                - */
/* ------------------------------------------------------- */

// TODO: Actual game functions

/* ------------------------------------------------------- */
/* -              Pebble Smartwatch Handlers             - */
/* ------------------------------------------------------- */

static void up_click_handler(ClickRecognizerRef recognizer, void * context) {
  // Adjust notification
  notification = "Bought a Salt Factory!";
  text_layer_set_text(notif_layer, notification);
  text_layer_set_text_color(notif_layer, GColorWhite);
  /*
  // Adjust SPS
  snprintf(buffer, 10, "%d", sps);
  text_layer_set_text(sps_layer, "sps");
  text_layer_set_font(sps_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_color(sps_layer, GColorWhite);
  */
  // Adjust salt amount
  snprintf(salt_buffer, 10, "%d", salt);
  text_layer_set_text(salt_layer, salt_buffer);
  text_layer_set_font(salt_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_color(salt_layer, GColorWhite);
}

static void select_click_handler(ClickRecognizerRef recognizer, void * context) {
  // Adjust notification
  text_layer_set_text(notif_layer, "Built a Salt Mine!");
  text_layer_set_text_color(notif_layer, GColorWhite);
  // Adjust SPS
  // Adjust salt amount
  snprintf(salt_buffer, 10, "%d", salt);
  text_layer_set_text(salt_layer, salt_buffer);
  text_layer_set_font(salt_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_color(salt_layer, GColorWhite);
}

static void down_click_handler(ClickRecognizerRef recognizer, void * context) {
  // Adjust notification
  text_layer_set_text(notif_layer, "Hired a Salty Grandpa!");
  text_layer_set_text_color(notif_layer, GColorWhite);
  // Adjust SPS
  // Adjust salt amount
  snprintf(salt_buffer, 10, "%d", salt);
  text_layer_set_text(salt_layer, salt_buffer);
  text_layer_set_font(salt_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_color(salt_layer, GColorWhite);
}

static void accel_tap_handler(AccelAxisType axis, int32_t direction) {
  if (axis == 1)
  {
    salt = 999;
  }
}

static void click_config_provider(void * context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window * window) {
  Layer * window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  // Set up the notifications text layer
  notif_layer = text_layer_create((GRect) { .origin = {0, 0}, .size = {bounds.size.w, 20} });
  text_layer_set_background_color(notif_layer, GColorBlack);
  text_layer_set_text_alignment(notif_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(notif_layer));
  // Set up the salt per second text layer
  sps_layer = text_layer_create((GRect) { .origin = {0, bounds.size.h - 60}, .size = {bounds.size.w, 20} });
  text_layer_set_background_color(sps_layer, GColorBlack);
  text_layer_set_text_alignment(sps_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(sps_layer));
  // Set up the salt text layer
  salt_layer = text_layer_create((GRect) { .origin = {0, bounds.size.h - 30}, .size = {bounds.size.w, 30} });
  text_layer_set_background_color(salt_layer, GColorBlack);
  text_layer_set_text_alignment(salt_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(salt_layer));
}

static void window_unload(Window * window) {
  text_layer_destroy(notif_layer);
}

static void init(void) {
  salt = 0; // this needs to be loaded in instead of resetting
  sps = 0; // this needs to be loaded in instead of resetting

  // Load up our window and layers
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
  window_set_background_color(window, GColorBlack);

  // Load up shake detection
  accel_tap_service_subscribe(&accel_tap_handler);
}

static void deinit(void) {
  accel_tap_service_unsubscribe();
  window_destroy(window);
}

/* ------------------------------------------------------- */

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
