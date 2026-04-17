#include <pebble.h>

// Persistent storage key
#define SETTINGS_KEY 1

// Define our settings struct
typedef struct ClaySettings {
  GColor BackgroundColor;
  GColor HourColor;
  GColor MinuteColor;
  int HourSize;
  bool HourPulse;
  bool BTPulse;
} ClaySettings;

// An instance of the struct
static ClaySettings settings;

// Main Window
static Window *s_main_window;

// Root (Parent) Window Layer
static Layer *s_window_layer;

// Global Values:
static bool s_bluetooth_connected;

// Set default settings
static void prv_default_settings() {
  settings.BackgroundColor = GColorLightGray;
  settings.HourColor = GColorWhite;
  settings.MinuteColor = GColorBlack;
  settings.HourSize = 2;
  settings.HourPulse = true;
  settings.BTPulse = true;
}

// Save settings to persistent storage
static void prv_save_settings() {
  persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
}

// Read settings from persistent storage
static void prv_load_settings() {
  // Set defaults first
  prv_default_settings();
  // Then override with any saved values
  persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));
}

//width, number of points, points
static int I[] = {0, 9,
                    0, -12,
                    0, -9,
                    0, -6,
                    0, -3,
                    0, 0,
                    0, 3,
                    0, 6,
                    0, 9,
                    0, 12};
static int V[] = {2, 17,
                    -8, -12,
                    -7, -9,
                    -6, -6,
                    -5, -3,
                    -4, 0,
                    -3, 3,
                    -2, 6,
                    -1, 9,
                    0, 12,
                    1, 9,
                    2, 6,
                    3, 3,
                    4, 0,
                    5, -3,
                    6, -6,
                    7, -9,
                    8, -12};
static int X[] = {2, 17,
                    -8, -12,
                    -6, -9,
                    -4, -6,
                    -2, -3,
                    2, 3,
                    4, 6,
                    6, 9,
                    8, 12,
                    0, 0,
                    -8, 12,
                    -6, 9,
                    -4, 6,
                    -2, 3,
                    2, -3,
                    4, -6,
                    6, -9,
                    8, -12,};
typedef struct RomanNumeral {
  int Width;
  int NumberOfDigits;
  int* Digits[];
} RomanNumeral;
RomanNumeral One =     {0, 1, {I}};
RomanNumeral Two =     {2, 2, {I, I}};
RomanNumeral Three =   {4, 3, {I, I, I}};
RomanNumeral Four =    {6, 2, {I, V}};
RomanNumeral Five =    {4, 1, {V}};
RomanNumeral Six =     {6, 2, {V, I}};
RomanNumeral Seven =   {8, 3, {V, I, I}};
RomanNumeral Eight =   {10, 4, {V, I, I, I}};
RomanNumeral Nine =    {6, 2, {I, X}};
RomanNumeral Ten =     {4, 1, {X}};
RomanNumeral Eleven =  {6, 2, {X, I}};
RomanNumeral Twelve =  {8, 3, {X, I, I}};
RomanNumeral* Numerals[] = {&Twelve, &One, &Two, &Three, &Four, &Five, &Six, &Seven, &Eight, &Nine, &Ten, &Eleven};

#if defined(PBL_BW)
static void byte_set_bit(uint8_t *byte, uint8_t bit, uint8_t value) {
  *byte ^= (-value ^ *byte) & (1 << bit);
}

static void set_pixel_color(GBitmapDataRowInfo info, uint16_t x, uint8_t color) {
  // Find the correct byte, then set the appropriate bit
  uint8_t byte = x / 8;
  uint8_t bit = x % 8; 
  byte_set_bit(&info.data[byte], bit, color);
}
#endif

static void window_update_proc(Layer *layer, GContext *ctx) {
  
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  GRect bounds = layer_get_bounds(s_window_layer);
  
  window_set_background_color(s_main_window, settings.BackgroundColor);
  
  // Get Local Time
  int hour = tick_time->tm_hour % 12;
  int minute = tick_time->tm_min;
  
  // double pulse on the top of the hour
  if (minute == 0 && settings.HourPulse){
    vibes_double_pulse();
  }
  
  int minute_angle = DEG_TO_TRIGANGLE (6*minute);
  
  // Get center of screen and max visible arm length for scaling
  GPoint center = GPoint((bounds.size.w-1)/2, (bounds.size.h-1)/2);
  int max_len = 2 * (center.x>center.y ? center.x : center.y);
  int shadow_y = (-cos_lookup(minute_angle) * max_len / TRIG_MAX_RATIO);
  int shadow_x = (sin_lookup(minute_angle) *max_len / TRIG_MAX_RATIO);


  GPoint digit_center, start, end;
  int i, j;
  
  // Calculate the stroke thickness
  int thickness = 4*settings.HourSize;
  graphics_context_set_stroke_width(ctx, thickness + 1);
  
  // Draw Minute Shadow
  #ifdef PBL_BW
  if (gcolor_equal(settings.MinuteColor, GColorLightGray)) {
    if (gcolor_equal(settings.BackgroundColor, GColorWhite)) {
      graphics_context_set_stroke_color(ctx, GColorBlack);
    } else {
      graphics_context_set_stroke_color(ctx, GColorWhite);
    }
  } else {
    graphics_context_set_stroke_color(ctx, settings.MinuteColor);
  }
  #else
  graphics_context_set_stroke_color(ctx, settings.MinuteColor);
  #endif
  
  digit_center = GPoint(center.x - Numerals[hour]->Width * thickness/2, center.y);
  for (i = 0; i<Numerals[hour]->NumberOfDigits; i++){
    digit_center = GPoint(digit_center.x + thickness*(Numerals[hour]->Digits[i][0]), digit_center.y);
    for (j = 2; j <= 2* Numerals[hour]->Digits[i][1]; j = j + 2){
      start = GPoint(digit_center.x + settings.HourSize*Numerals[hour]->Digits[i][j], digit_center.y + settings.HourSize*Numerals[hour]->Digits[i][j+1]);
      end = GPoint(start.x + shadow_x, start.y + shadow_y);
      graphics_draw_line(ctx, start, end);
    }
    digit_center = GPoint(digit_center.x + thickness*(2+Numerals[hour]->Digits[i][0]), digit_center.y);
  }
  
  #ifdef PBL_BW
  if (gcolor_equal(settings.MinuteColor, GColorLightGray)) {
    GBitmap *fb = graphics_capture_frame_buffer_format(ctx, GBitmapFormat1Bit);
    if (gcolor_equal(settings.BackgroundColor, GColorWhite)) {
      // Iterate over all rows
      for(int i = 0; i < bounds.size.h; i++) {
        // Get this row's range and data
        GBitmapDataRowInfo info = gbitmap_get_data_row_info(fb, i);
        // Iterate over all visible columns
        for(int j = info.min_x; j <= info.max_x; j++) {
          // Apply a gray mask (whiting out a checkerboard)
          if ( (i+j+1) % 2 ){
            set_pixel_color(info, j, 1);
          }
        }
      }
    } else {
      // Iterate over all rows
      for(int i = 0; i < bounds.size.h; i++) {
        // Get this row's range and data
        GBitmapDataRowInfo info = gbitmap_get_data_row_info(fb, i);
        // Iterate over all visible columns
        for(int j = info.min_x; j <= info.max_x; j++) {
          // Apply a gray mask (blacking out a checkerboard)
          if ( (i+j) % 2 ){
            set_pixel_color(info, j, 0);
          }
        }
      }
    }
    graphics_release_frame_buffer(ctx, fb);
  }
  #endif
  
  
  // Draw Hour Numeral
  graphics_context_set_stroke_color(ctx, settings.HourColor);
  digit_center = GPoint(center.x - Numerals[hour]->Width * thickness/2, center.y);
  for (i = 0; i<Numerals[hour]->NumberOfDigits; i++){
    digit_center = GPoint(digit_center.x + thickness*(Numerals[hour]->Digits[i][0]), digit_center.y);
    for (j = 2; j < 2* Numerals[hour]->Digits[i][1]; j = j + 2){
      start = GPoint(digit_center.x + settings.HourSize*Numerals[hour]->Digits[i][j], digit_center.y + settings.HourSize*Numerals[hour]->Digits[i][j+1]);
      end = GPoint(digit_center.x + settings.HourSize*Numerals[hour]->Digits[i][j+2], digit_center.y + settings.HourSize*Numerals[hour]->Digits[i][j+3]);
      graphics_draw_line(ctx, start, end);
    }
    digit_center = GPoint(digit_center.x + thickness*(2+Numerals[hour]->Digits[i][0]), digit_center.y);
  }
  

}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  layer_mark_dirty(s_window_layer);
}

static void bluetooth_callback(bool connected) {
  s_bluetooth_connected = connected;
  
  if (!connected && settings.BTPulse) {
    vibes_short_pulse();
  }
}
             
// AppMessage received handler
static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  
  // Check for Clay settings data
  Tuple *bg_color_t = dict_find(iterator, MESSAGE_KEY_BackgroundColor);
  if (bg_color_t) {
    settings.BackgroundColor = GColorFromHEX(bg_color_t->value->int32);
    window_set_background_color(s_main_window, settings.BackgroundColor);
  }
  
  Tuple *hour_color_t = dict_find(iterator, MESSAGE_KEY_HourColor);
  if (hour_color_t) {
    settings.HourColor = GColorFromHEX(hour_color_t->value->int32);
  }
  
  Tuple *min_color_t = dict_find(iterator, MESSAGE_KEY_MinuteColor);
  if (min_color_t) {
    settings.MinuteColor = GColorFromHEX(min_color_t->value->int32);
  }

  Tuple *hour_size_t = dict_find(iterator, MESSAGE_KEY_HourSize);
  if (hour_size_t) {
    settings.HourSize = hour_size_t->value->int32;
  }

  Tuple *hour_pulse_t = dict_find(iterator, MESSAGE_KEY_HourPulse);
  if (hour_pulse_t) {
    settings.HourPulse = hour_pulse_t->value->int32 == 1;
  }
  
  Tuple *bt_pulse_t = dict_find(iterator, MESSAGE_KEY_BTPulse);
  if (bt_pulse_t) {
    settings.BTPulse = bt_pulse_t->value->int32 == 1;
  }

  // Save and apply if any settings were changed
  if ( bg_color_t || hour_color_t || min_color_t || \
      hour_size_t || hour_pulse_t || bt_pulse_t) {
    prv_save_settings();
    layer_mark_dirty(s_window_layer);

  }
}
static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}
static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}
static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void main_window_load(Window *window) {
  GRect bounds = layer_get_bounds(window_get_root_layer(window));

  s_window_layer = layer_create(bounds);
  
  layer_set_update_proc(s_window_layer, window_update_proc);
  
  layer_add_child(window_get_root_layer(window), s_window_layer);
}

static void main_window_unload(Window *window) {
  layer_destroy(s_window_layer);
}

static void init() {
  // Load settings before creating UI
  prv_load_settings();

  s_main_window = window_create();
  window_set_background_color(s_main_window, settings.BackgroundColor);
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  window_stack_push(s_main_window, true);

  layer_mark_dirty(s_window_layer);

  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);

  connection_service_subscribe((ConnectionHandlers) {
    .pebble_app_connection_handler = bluetooth_callback
  });

  // Register AppMessage callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);

  // Open AppMessage
  const int inbox_size = 256;
  const int outbox_size = 256;
  app_message_open(inbox_size, outbox_size);
}

static void deinit() {
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
