#include <pebble.h>

// define DEV when in development, for testing purposes
//#define DEV

// define SCREENSHOT when posing for screenshots
//#define SCREENSHOT

// Persistent storage key
#define SETTINGS_KEY 1

// Define our settings struct
typedef struct ClaySettings {
  GColor BackgroundColor;
  GColor HourColor;
  GColor MinuteColor;
  int HourSize;
  bool HourPulse;
  int VibeOffset;
  bool BTPulse;
  int TickSize;
  bool HourMode;
  #ifdef PBL_RGB_BACKLIGHT
  GColor BacklightColor;
  #endif
} ClaySettings;

// An instance of the struct
static ClaySettings settings;

// Main Window
static Window *s_main_window;

// Root (Parent) Window Layer
static Layer *s_window_layer;

// Custom fonts
static GFont s_font_regular_48;

// Global Values:
static bool s_bluetooth_connected;

// Set default settings
static void prv_default_settings() {
  settings.BackgroundColor = GColorLightGray;
  settings.HourColor = GColorWhite;
  settings.MinuteColor = GColorBlack;
  settings.HourSize = 2;
  settings.HourPulse = true;
  settings.VibeOffset = 0;
  settings.BTPulse = true;
  settings.TickSize = 3;
  settings.HourMode = false;
  #ifdef PBL_RGB_BACKLIGHT
  settings.BacklightColor = GColorWhite;
  #endif
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

// Some bit-manipulation for BW displays for gray masks
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

#include <pebble.h>
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
                    8, -12};
/*
static int lilI[] = {0, 1,
                    0, -3,
                    0, 3};

static int lilV[] = {2, 2,
                    -1, -3,
                    0, 3,
                    0, 3,
                    1, -3};

static int lilX[] = {2, 2,
                    -1, -3,
                    1, 3,
                    -1, 3,
                    1, -3};
*/


typedef struct RomanNumeral {
  int Width;
  int NumberOfDigits;
  int* Digits[];
} RomanNumeral;
RomanNumeral _1 =   {0, 1, {I}};
RomanNumeral _2 =   {2, 2, {I, I}};
RomanNumeral _3 =   {4, 3, {I, I, I}};
RomanNumeral _4 =   {6, 2, {I, V}};
RomanNumeral _5 =   {4, 1, {V}};
RomanNumeral _6 =   {6, 2, {V, I}};
RomanNumeral _7 =   {8, 3, {V, I, I}};
RomanNumeral _8 =   {10, 4, {V, I, I, I}};
RomanNumeral _9 =   {6, 2, {I, X}};
RomanNumeral _10 =  {4, 1, {X}};
RomanNumeral _11 =  {6, 2, {X, I}};
RomanNumeral _12 =  {8, 3, {X, I, I}};

RomanNumeral _13 =  {10, 4, {X, I, I, I}};
RomanNumeral _14 =  {12, 3, {X, I, V}};
RomanNumeral _15 =  {10, 2, {X, V}};
RomanNumeral _16 =  {12, 3, {X, V, I}};
RomanNumeral _17 =  {14, 4, {X, V, I, I}};
RomanNumeral _18 =  {16, 5, {X, V, I, I, I}};
RomanNumeral _19 =  {12, 3, {X, I, X}};
RomanNumeral _20 =  {10, 2, {X, X}};
RomanNumeral _21 =  {12, 3, {X, X, I}};
RomanNumeral _22 =  {14, 4, {X, X, I, I}};
RomanNumeral _23 =  {16, 5, {X, X, I, I, I}};
RomanNumeral* Numerals[] = {&_12, &_1, &_2, &_3, &_4, &_5, &_6, &_7, &_8, &_9, &_10, &_11,
                            &_12, &_13, &_14, &_15, &_16, &_17, &_18, &_19, &_20, &_21, &_22, &_23};

static void window_update_proc(Layer *layer, GContext *ctx) {
  
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  GRect bounds = layer_get_unobstructed_bounds(s_window_layer);
  
  window_set_background_color(s_main_window, settings.BackgroundColor);
  #ifdef PBL_RGB_BACKLIGHT
  light_set_color(BacklightColor);
  #endif
  
  // Get Local Time
  int hour = tick_time->tm_hour % (settings.HourMode?24:12);
  int minute = tick_time->tm_min;
  
  #ifdef DEV
  hour = ((tick_time->tm_sec + 60*tick_time->tm_min)/3) % (settings.HourMode?24:12) % 2;
  minute = (tick_time->tm_sec % 12 + 54) % 60;
  hour = 10;
  #endif
  
  // int date = tick_time->tm_mday;
  
  // double pulse on the top of the hour, plus offset
  if ((minute == (60+settings.VibeOffset)%60) && settings.HourPulse){
    vibes_double_pulse();
    #ifdef DEV
    hour = 5;
    #endif
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
  
  // Draw Shadow Elements (minute umbra, ticks, etc)
  graphics_context_set_stroke_width(ctx, thickness + 1);
  
  // For BW displays, draw the shadows in the opposite color of the background if the shadow is supposed to be Gray
  #ifdef PBL_BW
  bool is_white = gcolor_equal(settings.BackgroundColor, GColorWhite);
  if (gcolor_equal(settings.MinuteColor, GColorLightGray)) {
      graphics_context_set_stroke_color(ctx, is_white?GColorBlack:GColorWhite);
  } else {
    graphics_context_set_stroke_color(ctx, settings.MinuteColor);
  }
  #else
  graphics_context_set_stroke_color(ctx, settings.MinuteColor);
  #endif
  
  //Draw Roman Hour Shadow
  if (hour == 0 && settings.HourMode == true){
    start = GPoint(center.x, center.y);
    end = GPoint(center.x + shadow_x, center.y + shadow_y);
    graphics_context_set_stroke_width(ctx, 28*settings.HourSize+1);
    graphics_draw_line(ctx, start, end);
  } else {
    graphics_context_set_stroke_width(ctx, thickness + 1);
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
  }
  
  //Draw Ticks
  if (settings.TickSize > 0) {
    graphics_context_set_stroke_width(ctx, settings.TickSize*2 - 1);
    for (i = 0; i < 12; i++){
      start = GPoint(center.x + cos_lookup(DEG_TO_TRIGANGLE (30*i)) * max_len / TRIG_MAX_RATIO, center.y + sin_lookup(DEG_TO_TRIGANGLE (30*i)) * max_len / TRIG_MAX_RATIO);
      end = GPoint(center.x + cos_lookup(DEG_TO_TRIGANGLE (30*i)) * max_len / 20 * (i%3>0?9:8) / TRIG_MAX_RATIO, center.y + sin_lookup(DEG_TO_TRIGANGLE (30*i)) * max_len / 20 * (i%3>0?9:8) / TRIG_MAX_RATIO);
      graphics_draw_line(ctx, start, end);
    }
  }
  
  //on BW displays, if the shadow is supposed to be Gray, color in a checkboard pattern of the background color across the whole screen
  #ifdef PBL_BW
  if (gcolor_equal(settings.MinuteColor, GColorLightGray)) {
    GBitmap *fb = graphics_capture_frame_buffer_format(ctx, GBitmapFormat1Bit);
    // Iterate over all rows
    for(int i = 0; i < bounds.size.h; i++) {
      // Get this row's range and data
      GBitmapDataRowInfo info = gbitmap_get_data_row_info(fb, i);
      // Iterate over all visible columns
      for(int j = info.min_x; j <= info.max_x; j++) {
        // Apply a gray mask (whiting out a checkerboard on white backgrounds, blacking out on black backgrounds)
        if ( (i+j+(is_white?1:0)) % 2 ){
          set_pixel_color(info, j, is_white?1:0);
        }
      }
    }
    graphics_release_frame_buffer(ctx, fb);
  }
  #endif
  
  // Draw Hour Numeral
  
  graphics_context_set_stroke_color(ctx, settings.HourColor);
  if (hour == 0 && settings.HourMode == true){
    start = GPoint(center.x, center.y);
    end = GPoint(center.x + shadow_x, center.y + shadow_y);
    graphics_context_set_stroke_width(ctx, thickness + 1);
    graphics_draw_circle(ctx, start, 12*settings.HourSize);
  } else {
    graphics_context_set_stroke_width(ctx, thickness + 1);
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
  
  if (settings.TickSize > 0) {
    graphics_context_set_stroke_width(ctx, settings.TickSize*2 - 1);
    i = (minute + 2)/5;
    start = GPoint(center.x + sin_lookup(DEG_TO_TRIGANGLE (30*i)) * max_len / TRIG_MAX_RATIO, center.y - cos_lookup(DEG_TO_TRIGANGLE (30*i)) * max_len / TRIG_MAX_RATIO);
    end = GPoint(center.x + sin_lookup(DEG_TO_TRIGANGLE (30*i)) * max_len / 20 * (i%3>0?9:8) / TRIG_MAX_RATIO, center.y - cos_lookup(DEG_TO_TRIGANGLE (30*i)) * max_len / 20 * (i%3>0?9:8) / TRIG_MAX_RATIO);
    graphics_draw_line(ctx, start, end);
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
  
  Tuple *vibe_offset_t = dict_find(iterator, MESSAGE_KEY_VibeOffset);
  if (vibe_offset_t) {
    settings.VibeOffset = vibe_offset_t->value->int32;
  }
  
  Tuple *bt_pulse_t = dict_find(iterator, MESSAGE_KEY_BTPulse);
  if (bt_pulse_t) {
    settings.BTPulse = bt_pulse_t->value->int32 == 1;
  }
  
  Tuple *tick_size_t = dict_find(iterator, MESSAGE_KEY_TickSize);
  if (tick_size_t) {
    settings.TickSize = tick_size_t->value->int32;
  }
  
  Tuple *hour_mode_t = dict_find(iterator, MESSAGE_KEY_HourMode);
  if (hour_mode_t) {
    settings.HourMode = hour_mode_t->value->int32 == 1;
  }
  
  #ifdef PBL_RGB_BACKLIGHT
  Tuple *light_color_t = dict_find(iterator, MESSAGE_KEY_BacklightColor);
  if (light_color_t) {
    settings.BacklightColor = GColorFromHEX(light_color_t->value->int32);
    light_set_color(BacklightColor);
  }
  
  // Save and apply if any settings were changed
  if ( bg_color_t || hour_color_t || min_color_t || hour_mode_t || vibe_offset_t ||\
      hour_size_t || hour_pulse_t || bt_pulse_t || tick_size_t || light_color_t) {
    prv_save_settings();
    layer_mark_dirty(s_window_layer);
  }
  
  #else
  
  // Save and apply if any settings were changed
  if ( bg_color_t || hour_color_t || min_color_t || hour_mode_t || vibe_offset_t ||\
      hour_size_t || hour_pulse_t || bt_pulse_t || tick_size_t) {
    prv_save_settings();
    layer_mark_dirty(s_window_layer);
  }
  #endif
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

static void prv_unobstructed_will_change(GRect final_unobstructed_screen_area, void *context) {
  layer_mark_dirty(s_window_layer);
  //window_update_proc(Layer *layer, GContext *ctx);
}

static void prv_unobstructed_change(AnimationProgress progress, void *context) {
  layer_mark_dirty(s_window_layer);
  //window_update_proc(Layer *layer, GContext *ctx);
}

static void prv_unobstructed_did_change(void *context) {
  layer_mark_dirty(s_window_layer);
  //window_update_proc(Layer *layer, GContext *ctx);
}

static void main_window_load(Window *window) {
  GRect bounds = layer_get_bounds(window_get_root_layer(window));

  s_window_layer = layer_create(bounds);
  
  layer_set_update_proc(s_window_layer, window_update_proc);
  
  layer_add_child(window_get_root_layer(window), s_window_layer);
  
  UnobstructedAreaHandlers handlers = {
    .will_change = prv_unobstructed_will_change,
    .change = prv_unobstructed_change,
    .did_change = prv_unobstructed_did_change
  };
  unobstructed_area_service_subscribe(handlers, NULL);
  
}

static void main_window_unload(Window *window) {
  
  // Unload custom fonts
  fonts_unload_custom_font(s_font_regular_48);
  
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

  #ifdef DEV
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  #else
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  #endif

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
