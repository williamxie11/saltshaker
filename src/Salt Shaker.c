/* IT'S SALT SHAKING TIME! */
// @author William Xie
// http://github.com/williamxie11/saltshaker

#include <pebble.h>
#include <string.h>

static Window * window;

/* --- TEXT LAYERS --- */
static TextLayer * notif_layer;
static TextLayer * sps_layer;
static TextLayer * salt_layer; 
static TextLayer * grandpa_layer;
static TextLayer * factory_layer;
static TextLayer * mine_layer;

/* --- GAME VARIABLES --- */
static int salt;
static int sps; // "Salt Per Second"
static char * notification; 

// Buffers
static char notification_buffer[30]; // For use in creating notifications with integers
static char salt_buffer[15]; 
static char sps_buffer[25]; 
static char grandpa_buffer[5];
static char factory_buffer[5];
static char mine_buffer[5];

// Cost of items
static int costGrandpa = 10;   
static int costFactory = 1000;
static int costMine = 10000;

// Salt per second of items
static int spsGrandpa = 1;
static int spsFactory = 10;
static int spsMine = 100;

// Total item count
static int countGrandpa = 0;
static int countFactory = 0;
static int countMine = 0;

/* ------------------------------------------------------- */
/* -                 Salty Game Functions                - */
/* ------------------------------------------------------- */

//  1 = Grandpa | 2 = Factory | 3 = Mine
static bool canBuy(int item) {
  if (item == 1 && salt >= costGrandpa) {
    return true;
  }
  else if (item == 2 && salt >= costFactory) {
    return true;
  }
  else if (item == 3 && salt >= costMine) {
    return true;
  }
  else {
    return false;
  }
}

// 1 = Grandpa | 2 = Factory | 3 = Mine
static void buy(int item) {
  // Check to see if player has enough salt to purchase
  if (canBuy(item)) {
    // Buy corresponding item
    if (item == 1) {
      notification = "Hired a Salty Grandpa!";
      salt -= costGrandpa;
      sps += spsGrandpa;
      countGrandpa++;
      costGrandpa *= 1.25; 
    }
    else if (item == 2) {
      notification = "Bought a Salt Factory!";
      salt -= costFactory;
      sps += spsFactory;
      countFactory++;
      costFactory *= 1.25;
    }
    else if (item == 3) {
      notification = "Built a Salt Mine!";
      salt -= costMine;
      sps += spsMine;
      countMine++;
      costMine *= 1.25; 
    }
    else {
      notification = "404 SALT BUY NOT FOUND";
    } 
  }
  else { 
    // Player can't afford item. Display cost.
    int cost = 0;
    char * toBuy;
    if (item == 1) { cost = costGrandpa; toBuy = "Salty Grandpa"; }
    if (item == 2) { cost = costFactory; toBuy = "Salt Factory"; }
    if (item == 3) { cost = costMine; toBuy = "Salt Mine"; }
    snprintf(notification_buffer, 30, "%s: %d Salt", toBuy, cost);
    notification = notification_buffer;
  }
}

/* ------------------------------------------------------- */
/* -              Pebble Smartwatch Handlers             - */
/* ------------------------------------------------------- */

static void updateSalt() {
  snprintf(salt_buffer, 15, "%d", salt);
  text_layer_set_text(salt_layer, salt_buffer);
  text_layer_set_font(salt_layer, fonts_get_system_font(FONT_KEY_DROID_SERIF_28_BOLD));
  text_layer_set_text_color(salt_layer, GColorWhite);
}

static void updateSPS() {
  snprintf(sps_buffer, 25, "+%d / 10 sec", sps);
  text_layer_set_text(sps_layer, sps_buffer);
  text_layer_set_font(sps_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  text_layer_set_text_color(sps_layer, GColorWhite);
}

// 1 = Grandpa | 2 = Factory | 3 = Mine
static void updateCount(int item) {
  if (item == 1) {
    snprintf(grandpa_buffer, 5, "%d", countGrandpa);
    text_layer_set_text(grandpa_layer, grandpa_buffer);
    text_layer_set_font(grandpa_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
    text_layer_set_text_color(grandpa_layer, GColorWhite);
  }
  else if (item == 2) {
    snprintf(factory_buffer, 5, "%d", countFactory);
    text_layer_set_text(factory_layer, factory_buffer);
    text_layer_set_font(factory_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
    text_layer_set_text_color(factory_layer, GColorWhite);
  }
  else if (item == 3) {
    snprintf(mine_buffer, 5, "%d", countMine);
    text_layer_set_text(mine_layer, mine_buffer);
    text_layer_set_font(mine_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
    text_layer_set_text_color(mine_layer, GColorWhite);
  }
  else {
    //
  }
}

static void up_click_handler(ClickRecognizerRef recognizer, void * context) {
  buy(1);
  text_layer_set_text(notif_layer, notification);
  text_layer_set_text_color(notif_layer, GColorWhite);
  updateSalt();
  updateSPS();
  updateCount(1);
}

static void select_click_handler(ClickRecognizerRef recognizer, void * context) {
  buy(2);
  text_layer_set_text(notif_layer, notification);
  text_layer_set_text_color(notif_layer, GColorWhite);
  updateSalt();
  updateSPS();
  updateCount(2);
}

static void down_click_handler(ClickRecognizerRef recognizer, void * context) {
  buy(3);
  text_layer_set_text(notif_layer, notification);
  text_layer_set_text_color(notif_layer, GColorWhite);
  updateSalt();
  updateSPS();
  updateCount(3);
}

static void accel_data_handler(AccelData * data, uint32_t num_samples) {
  // update salt based on batch updates

  // Generate a random salt shaking message!
  int r = rand() % 5; // 20% probability of changing message
  if (r == 0) notification = "[SALT INTENSIFIES]";
  else if (r == 1) notification = "Blood pressure rising!";
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
  // Set up the salt text layer
  salt_layer = text_layer_create((GRect) { .origin = {0, 20}, .size = {bounds.size.w, 40} });
  text_layer_set_background_color(salt_layer, GColorBlack);
  text_layer_set_text_alignment(salt_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(salt_layer));
  // Set up the sps text layer
  sps_layer = text_layer_create((GRect) { .origin = {0, 60}, .size = {bounds.size.w, 20} });
  text_layer_set_background_color(sps_layer, GColorBlack);
  text_layer_set_text_alignment(sps_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(sps_layer));
  // Set up Grandpa Count
  grandpa_layer = text_layer_create((GRect) { .origin = {bounds.size.w/2, 90}, .size = {bounds.size.w/2, 20} });
  text_layer_set_background_color(grandpa_layer, GColorBlack);
  text_layer_set_text_alignment(grandpa_layer, GTextAlignmentLeft);
  layer_add_child(window_layer, text_layer_get_layer(grandpa_layer));
  // Set up Factory Count
  factory_layer = text_layer_create((GRect) { .origin = {bounds.size.w/2, 110}, .size = {bounds.size.w/2, 20} });
  text_layer_set_background_color(factory_layer, GColorBlack);
  text_layer_set_text_alignment(factory_layer, GTextAlignmentLeft);
  layer_add_child(window_layer, text_layer_get_layer(factory_layer));
  // Set up Mine Count
  mine_layer = text_layer_create((GRect) { .origin = {bounds.size.w/2, 130}, .size = {bounds.size.w/2, 20} });
  text_layer_set_background_color(mine_layer, GColorBlack);
  text_layer_set_text_alignment(mine_layer, GTextAlignmentLeft);
  layer_add_child(window_layer, text_layer_get_layer(mine_layer));
}

static void window_unload(Window * window) {
  text_layer_destroy(notif_layer);
  text_layer_destroy(salt_layer);
  text_layer_destroy(sps_layer);
  text_layer_destroy(grandpa_layer);
  text_layer_destroy(factory_layer);
  text_layer_destroy(mine_layer);
}

/* ------------------------------------------------------- */

static void init(void) {
  notification = "Getting salty aren't we?";
  salt = 123456; // this needs to be loaded in instead of resetting
  sps = 0; // this needs to be loaded in instead of resetting
  countGrandpa = countFactory = countMine = 0; // this needs to be loaded in instead of resetting

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
  accel_data_service_subscribe(10, &accel_data_handler);
}

static void deinit(void) {
  accel_data_service_unsubscribe();
  window_destroy(window);
}

/* ------------------------------------------------------- */

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
