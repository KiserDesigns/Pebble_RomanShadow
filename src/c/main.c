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
static int s_battery_level;
static bool s_battery_charging;
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

static int I[] = {9,
                    0, -12,
                    0, -9,
                    0, -6,
                    0, -3,
                    0, 0,
                    0, 3,
                    0, 6,
                    0, 9,
                    0, 12};

static int V[] = {17,
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

static int X[] = {17,
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
  int* Digit1;
  int* Digit2;
  int* Digit3;
  int* Digit4;
} RomanNumeral;

RomanNumeral One =     {0, 1, I, 0, 0, 0};
RomanNumeral Two =     {2, 2, I, I, 0, 0};
RomanNumeral Three =   {4, 3, I, I, I, 0};
RomanNumeral Four =    {4, 2, I, V, 0, 0};
RomanNumeral FourAlt = {6, 4, I, I, I, I};
RomanNumeral Five =    {2, 1, V, 0, 0, 0};
RomanNumeral Six =     {4, 2, V, I, 0, 0};
RomanNumeral Seven =   {6, 3, V, I, I, 0};
RomanNumeral Eight =   {8, 4, V, I, I, I};
RomanNumeral Nine =    {4, 2, I, X, 0, 0};
RomanNumeral Ten =     {2, 1, X, 0, 0, 0};
RomanNumeral Eleven =  {4, 2, X, I, 0, 0};
RomanNumeral Twelve =  {6, 3, X, I, I, 0};
  
RomanNumeral* Numerals[] = {&Twelve, &One, &Two, &Three, &Four, &Five, &Six, &Seven, &Eight, &Nine, &Ten, &Eleven};

static int zero[] = {5,
                     -1,-2,
                    1,-2,
                    1,1,
                    0,2,
                    -1,2,
                    -1,-2};
static int one[] = {4,
                    -1,-1,
                    0,-2,
                    0,2,
                    -1,2,
                    1,2};
static int two[] = {6,
                   -1,-2,
                   1,-2,
                   1,0,
                   0,0,
                   -1,1,
                   -1,2,
                   1,2};
static int three[] = {6,
                     -1,-2,
                     1,-2,
                     1,-1,
                     0,0,
                     1,1,
                     1,2,
                     -1,2};
static int four[] = {4,
                    -1,-2,
                    -1,0,
                    1,0,
                    1,-2,
                    1,2};
static int five[] = {6,
                    1,-2,
                    -1,-2,
                    -1,0,
                    1,0,
                    1,1,
                    0,2,
                    -1,2};
static int six[] = {6,
                    1,-2,
                    0,-2,
                    -1,-1,
                    -1,2,
                    1,2,
                    1,0,
                    -1,0};
static int seven[] = {4,
                     -1,-2,
                    1,-2,
                    1,-1,
                    0,0,
                    0,2};
static int eight[] = {8,
                     -1,-2,
                    1,-2,
                    1,-1,
                    -1,1,
                    -1,2,
                    1,2,
                    1,1,
                    -1,-1,
                    -1,-2};
static int nine[] = {6, // number of strokes
                    -1,2, // point 1 of stroke 1
                    0,2, // point 2 of stroke 1, which is point 1 of stroke 2
                    1,1,
                    1,-2,
                    -1,-2,
                    -1,0,
                    1,0}; // last point of the last stroke
static int* numbers[] = {zero, one, two, three, four, five, six, seven, eight, nine};

/* static void draw_date(GContext *ctx, GPoint center, int max_len, int day_of_month) {
  //DateSize = 3, 1-5
  //DateThickness = 5: 1,3,5,7,9
  
  // Calculate Date Digits
  int date_radius = max_len/2;
  int segment_length = settings.DateThickness / 2 + settings.DateSize;
  int digit_width = 2 * segment_length;
  
  int digit, i;
  GPoint digit_center, start, end;
  
  // draw thin background outline
  graphics_context_set_stroke_color(ctx, settings.BackgroundColor);
  graphics_context_set_stroke_width(ctx, settings.DateThickness+4);
    // left digit background
  digit_center = GPoint(center.x + date_radius - digit_width, center.y);
  digit = day_of_month / 10;
  for (i = 1; i < 2*numbers[digit][0]; i = i + 2) {
    start = GPoint(digit_center.x + numbers[digit][i]*segment_length, digit_center.y + numbers[digit][i+1]*segment_length);
    end = GPoint(digit_center.x + numbers[digit][i+2]*segment_length, digit_center.y + numbers[digit][i+3]*segment_length);
    graphics_draw_line(ctx, start, end);
  }
    // right digit background
  digit_center = GPoint(center.x + date_radius + digit_width, center.y);
  digit = day_of_month % 10;
  for (i = 1; i < 2*numbers[digit][0]; i+=2) {
    start = GPoint(digit_center.x + numbers[digit][i]*segment_length, digit_center.y + numbers[digit][i+1]*segment_length);
    end = GPoint(digit_center.x + numbers[digit][i+2]*segment_length, digit_center.y + numbers[digit][i+3]*segment_length);
    graphics_draw_line(ctx, start, end);
  }
  
  // draw digits
  graphics_context_set_stroke_color(ctx, settings.DateColor);
  graphics_context_set_stroke_width(ctx, settings.DateThickness);
    // first digit
  digit_center = GPoint(center.x + date_radius - digit_width, center.y);
  digit = day_of_month / 10;
  for (i = 1; i < 2*numbers[digit][0]; i = i + 2) {
    start = GPoint(digit_center.x + numbers[digit][i]*segment_length, digit_center.y + numbers[digit][i+1]*segment_length);
    end = GPoint(digit_center.x + numbers[digit][i+2]*segment_length, digit_center.y + numbers[digit][i+3]*segment_length);
    graphics_draw_line(ctx, start, end);
  }
    // second digit
  digit_center = GPoint(center.x + date_radius + digit_width, center.y);
  digit = day_of_month % 10;
  for (i = 1; i < 2*numbers[digit][0]; i+=2) {
    start = GPoint(digit_center.x + numbers[digit][i]*segment_length, digit_center.y + numbers[digit][i+1]*segment_length);
    end = GPoint(digit_center.x + numbers[digit][i+2]*segment_length, digit_center.y + numbers[digit][i+3]*segment_length);
    graphics_draw_line(ctx, start, end);
  }
  
  
}
*/

static void window_update_proc(Layer *layer, GContext *ctx) {
  
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  GRect bounds = layer_get_bounds(s_window_layer);
  
  window_set_background_color(s_main_window, settings.BackgroundColor);
  
  // Get Local Time and Day of Month
  int hour = tick_time->tm_hour % 12;
  int minute = tick_time->tm_min;
  
  // double pulse on the top of the hour
  if (minute == 0 && settings.HourPulse){
    vibes_double_pulse();
  }
  
  
  int hour_angle = DEG_TO_TRIGANGLE (30*hour + 0.5*minute);
  int minute_angle = DEG_TO_TRIGANGLE (6*minute);
  
  // Get center of screen and max visible arm length for scaling
  GPoint center = GPoint((bounds.size.w-1)/2, (bounds.size.h-1)/2);
  int max_len = center.x>center.y? center.y : center.x;
  
  // Calculate Hour Hand
  int hour_len = 40;
  GRect hour_rect = GRect ( center.x - hour_len, center.y - hour_len, 2*hour_len+1, 2*hour_len+1);
  GPoint hour_end = gpoint_from_polar(hour_rect, GOvalScaleModeFitCircle, hour_angle);
  
  // Calculate Minute Hand
  int minute_len = 70;
  GRect minute_rect = GRect ( center.x - minute_len, center.y - minute_len, 2*minute_len+1, 2*minute_len+1);
  GPoint minute_end = gpoint_from_polar(minute_rect, GOvalScaleModeFitCircle, minute_angle);

  
  // Draw Hands
    // Thin Background Border
  graphics_context_set_stroke_color(ctx, settings.BackgroundColor);
  graphics_draw_line(ctx, center, hour_end);
  graphics_draw_line(ctx, center, minute_end);
    // Actual Hands
  graphics_context_set_stroke_color(ctx, settings.HourColor);
  graphics_context_set_stroke_width(ctx, settings.HourSize);
  graphics_draw_line(ctx, center, hour_end);
  graphics_context_set_stroke_color(ctx, settings.MinuteColor);
  graphics_draw_line(ctx, center, minute_end);
  

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

  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

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
