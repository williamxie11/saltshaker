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
static AppTimer * timer;
static GBitmap * image;
static Layer * saltImage;

/* --- TEXT LAYERS --- */
static TextLayer * notif_layer;
static TextLayer * sps_layer;
static TextLayer * salt_layer; 

/* --- GAME VARIABLES --- */
static int salt;
static int sps; // "Salt Per Second"
static char * notification; 

static int costGrandpa = 10;   
static int costFactory = 50;
static int costMine = 250;

static int spsGrandpa = 1;
static int spsFactory = 10;
static int spsMine = 100;

/* ------------------------------------------------------- */
/* -                 Salty Game Functions                - */
/* ------------------------------------------------------- */
static bool hasEnoughSalt(int cost) {
  if (salt >= cost) {
    return true;
  }
  return false;
}

// 1 = Salty Grandpa | 2 = Salt Factory | 3 = Salt Mine
// Assumes affordability before function call
static void buy(int i) {
  if (i == costGrandpa) {
    salt = salt - costGrandpa;
    sps = sps + spsGrandpa;
    notification = "Hired a Salty Grandpa!";
  }
  else if (i == costFactory) {
    salt = salt - costFactory;
    sps = sps + spsFactory;
    notification = "Bought a Salt Factory!";
  }
  else if (i == costMine) {
    salt = salt - costMine;
    sps = sps + costMine;
    notification = "Built a Salt Mine!";
  }
  else {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Invalid item. Could not buy.");
  }
}

/* ------------------------------------------------------- */
/* -              Pebble Smartwatch Handlers             - */
/* ------------------------------------------------------- */

static void up_click_handler(ClickRecognizerRef recognizer, void * context) {
  // Adjust notification
  notification = "Bought a Salt Factory!";
  text_layer_set_text(notif_layer, notification);
  text_layer_set_text_color(notif_layer, GColorWhite);
  // Adjust SPS
  text_layer_set_text(sps_layer, "+10");
  text_layer_set_text_color(sps_layer, GColorWhite);
  // Adjust salt amount
  text_layer_set_text(salt_layer, "13371114493");
  text_layer_set_font(salt_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_color(salt_layer, GColorWhite);
}

static void select_click_handler(ClickRecognizerRef recognizer, void * context) {
  // Adjust notification
  text_layer_set_text(notif_layer, "Built a Salt Mine!");
  text_layer_set_text_color(notif_layer, GColorWhite);
  // Adjust SPS
  text_layer_set_text(sps_layer, "+100");
  text_layer_set_text_color(sps_layer, GColorWhite);
  // Adjust salt amount
  text_layer_set_text(salt_layer, "696919691");
  text_layer_set_font(salt_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_color(salt_layer, GColorWhite);
}

static void down_click_handler(ClickRecognizerRef recognizer, void * context) {
  // Adjust notification
  text_layer_set_text(notif_layer, "Hired a Salty Grandpa!");
  text_layer_set_text_color(notif_layer, GColorWhite);
  // Adjust SPS
  text_layer_set_text(sps_layer, "+1");
  text_layer_set_text_color(sps_layer, GColorWhite);
  // Adjust salt amount
  text_layer_set_text(salt_layer, "4222242515");
  text_layer_set_font(salt_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_color(salt_layer, GColorWhite);
}

static void click_config_provider(void * context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void layer_update_callback(Layer * me, GContext * ctx) {
  GRect bounds = image->bounds;
  graphics_draw_bitmap_in_rect(ctx, image, (GRect) { .origin = { 0, 30 }, .size = bounds.size });
}

static void window_load(Window * window) {
  salt = 0; // this needs to be loaded in instead of resetting
  sps = 0; // this needs to be loaded in instead of resetting
  Layer * window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  // Set up the notifications text layer
  notif_layer = text_layer_create((GRect) { .origin = {0, 0}, .size = {bounds.size.w, 20} });
  text_layer_set_background_color(notif_layer, GColorBlack);
  text_layer_set_text_alignment(notif_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(notif_layer));
  // Set up the salt per second text layer
  sps_layer = text_layer_create((GRect) { .origin = {0, bounds.size.h - 40}, .size = {bounds.size.w, 15} });
  text_layer_set_background_color(sps_layer, GColorBlack);
  text_layer_set_text_alignment(sps_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(sps_layer));
  // Set up the salt text layer
  salt_layer = text_layer_create((GRect) { .origin = {0, bounds.size.h - 25}, .size = {bounds.size.w, 25} });
  text_layer_set_background_color(salt_layer, GColorBlack);
  text_layer_set_text_alignment(salt_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(salt_layer));
}

static void window_unload(Window * window) {
  text_layer_destroy(notif_layer);
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

  Layer * window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);

  saltImage = layer_create(bounds);
  layer_set_update_proc(saltImage, layer_update_callback);
  layer_add_child(window_layer, saltImage);

  //accel_data_service_subscribe(0, NULL);
}

static void deinit(void) {
  window_destroy(window);
}

/* ------------------------------------------------------- */

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_SALT);

  app_event_loop();
  gbitmap_destroy(image);
  deinit();
}
