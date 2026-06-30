#include <pebble.h>

// define DEV and/or FORCE_BACKLIGHT when in development, for testing purposes
//#define DEV
//#define FORCE_BACKLIGHT

// define SCREENSHOT when posing for screenshots
//#define SCREENSHOT

// Persistent storage key
#define SETTINGS_KEY 2

// Define schedule struct
typedef struct ScheduleEntry {
  bool Enable;
  int Hour;
  int Minute;
  GColor BackgroundColor;
  GColor HourColor;
  GColor MinuteColor;
  GColor BacklightColor;
} ScheduleEntry;

#define SCHEDULE_ENTRIES 10

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
  GColor BacklightColor;
  ScheduleEntry Schedule[SCHEDULE_ENTRIES];
  int VibeStart;
  int VibeEnd;
  bool NumeralType;
  bool HourBorder;
} ClaySettings;

// An instance of the struct
static ClaySettings settings;

// Current colors based on settings or schedule.
static GColor curr_bg_color;
static GColor curr_hr_color;
static GColor curr_min_color;
static GColor curr_bl_color;

#ifdef SCREENSHOT
static ClaySettings screen1 = {
  GColorLightGray, GColorWhite, GColorBlack,
  2, false, 0, false, 3, false, GColorWhite
};

static ClaySettings screen2 = {
  GColorBlack, GColorWhite, GColorLightGray,
  3, false, 0, false, 0, false, GColorWhite
};

static ClaySettings screen3 = {
  GColorWhite, GColorBlack, GColorLightGray,
  1, false, 0, false, 5, true, GColorWhite
};

static ClaySettings screen4 = {
  GColorDukeBlue, GColorIcterine, GColorCyan,
  2, false, 0, false, 0, false, GColorWhite
};

static ClaySettings screen5 = {
  GColorMintGreen, GColorBulgarianRose, GColorJaegerGreen,
  2, false, 0, false, 7, true, GColorWhite
};
#endif

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
  settings.HourSize = 20;
  settings.HourPulse = true;
  settings.VibeOffset = 0;
  settings.BTPulse = true;
  settings.TickSize = 3;
  settings.HourMode = false;
  settings.BacklightColor = GColorWhite;
  ScheduleEntry temp = {false, 0, 0, GColorLightGray, GColorWhite, GColorBlack, GColorWhite};
  for (int i = 0; i < SCHEDULE_ENTRIES; i++) {
    settings.Schedule[i] = temp;
  }
  settings.VibeStart = 8;
  settings.VibeEnd = 20;
  settings.NumeralType = 0;
  settings.HourBorder = false;
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
  
  // Upgrade from old settings versions
  if (settings.HourSize < 10){ // Version 2.1 updated scaling from integer 1-3 to 1.0-4.0*10 (10-40)
    settings.HourSize = settings.HourSize * 10;
    prv_save_settings();
  }
}


//Apply the current scheduled or settings color to curr_bg_color, curr_hr_color, curr_min_color, and (EMERY) curr_bl_color
static void apply_schedule_colors() {
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  int hour = tick_time->tm_hour;
  int minute = tick_time->tm_min;
  
  #ifdef DEV
  hour = 1;
  minute = (tick_time->tm_sec) % 60;
  #endif
  
  #ifdef SCREENSHOT
  if (tick_time->tm_sec == 58 || tick_time->tm_sec == 03) {
    settings = screen3; }
  if (tick_time->tm_sec == 59) {
    settings = screen2; }
  if (tick_time->tm_sec == 0) {
    settings = screen1; }
  if (tick_time->tm_sec == 01) {
    settings = screen4; }
  if (tick_time->tm_sec == 02) {
    settings = screen5; }
  #endif
  
  for (int i = 0; i < SCHEDULE_ENTRIES; i++) {
    if ((settings.Schedule[i].Enable) && \
        (settings.Schedule[i].Hour == hour) && \
        (settings.Schedule[i].Minute == minute) ){
      if (gcolor_equal(settings.Schedule[i].BackgroundColor, GColorBlack) && \
          gcolor_equal(settings.Schedule[i].HourColor, GColorBlack) && \
          gcolor_equal(settings.Schedule[i].MinuteColor, GColorBlack)) {
        curr_bg_color = settings.BackgroundColor;
        curr_hr_color = settings.HourColor;
        curr_min_color = settings.MinuteColor;
        #ifdef PBL_RGB_BACKLIGHT
        curr_bl_color = settings.BacklightColor;
        #endif
      } else {
        curr_bg_color = settings.Schedule[i].BackgroundColor;
        curr_hr_color = settings.Schedule[i].HourColor;
        curr_min_color = settings.Schedule[i].MinuteColor;
        #ifdef PBL_RGB_BACKLIGHT
        curr_bl_color = settings.Schedule[i].BacklightColor;
        #endif
      }
    }
  }
  window_set_background_color(s_main_window, curr_bg_color);
}

// On launch [init()] (or at any time), find the correct schedule "window" to be in, and apply those to curr_*
void bootstrap_schedule_colors() {
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  int hour = tick_time->tm_hour;
  int minute = tick_time->tm_min;
  
  #ifdef DEV
  hour = 1;
  minute = (tick_time->tm_sec) % 60;
  #endif
  
  int max_time = -1;
  int max_index = -1;
  
  //loop through to find the latest time in the day 
  for (int i = 0; i < SCHEDULE_ENTRIES; i++){
    if (settings.Schedule[i].Enable && ((60 * settings.Schedule[i].Hour + settings.Schedule[i].Minute) > max_time)){
        max_index = i;
        max_time = 60 * settings.Schedule[i].Hour + settings.Schedule[i].Minute;
    }
  }
  // If no schedule, set to main Visuals settings.
  // Else, set the colors as if it is about to turn midnight (the latest time applies), then find the right schedule.
  if (max_index == -1) {
    curr_bg_color = settings.BackgroundColor;
    curr_hr_color = settings.HourColor;
    curr_min_color = settings.MinuteColor;
    #ifdef PBL_RGB_BACKLIGHT
    curr_bl_color = settings.BacklightColor;
    #endif
  } else {
    curr_bg_color = settings.Schedule[max_index].BackgroundColor;
    curr_hr_color = settings.Schedule[max_index].HourColor;
    curr_min_color = settings.Schedule[max_index].MinuteColor;
    #ifdef PBL_RGB_BACKLIGHT
    curr_bl_color = settings.Schedule[max_index].BacklightColor;
    #endif
    
    int curr_applied_time = -1;
    // step through the schedule list.
    // If the schedule time is greater than or equal to the current applied time, consider it (only move time forward).
    // If the current local time is greater than or equal to the schedule time, apply it, and store the applied time.
    
    for (int i = 0; i < SCHEDULE_ENTRIES; i++){
      if (settings.Schedule[i].Enable && ((60 * settings.Schedule[i].Hour + settings.Schedule[i].Minute) >= curr_applied_time)){
        if ((60 * hour + minute) >= (60 * settings.Schedule[i].Hour + settings.Schedule[i].Minute)){
          curr_applied_time = 60 * settings.Schedule[i].Hour + settings.Schedule[i].Minute;
          if (gcolor_equal(settings.Schedule[i].BackgroundColor, GColorBlack) && \
              gcolor_equal(settings.Schedule[i].HourColor, GColorBlack) && \
              gcolor_equal(settings.Schedule[i].MinuteColor, GColorBlack)) {
            curr_bg_color = settings.BackgroundColor;
            curr_hr_color = settings.HourColor;
            curr_min_color = settings.MinuteColor;
            #ifdef PBL_RGB_BACKLIGHT
            curr_bl_color = settings.BacklightColor;
            #endif
          } else {
            curr_bg_color = settings.Schedule[i].BackgroundColor;
            curr_hr_color = settings.Schedule[i].HourColor;
            curr_min_color = settings.Schedule[i].MinuteColor;
            #ifdef PBL_RGB_BACKLIGHT
            curr_bl_color = settings.Schedule[i].BacklightColor;
            #endif
          }
        }
      }
    }
  }
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

static int __0[] = {2,25,
                    0,12,
                    -2,11,
                    -4,10,
                    -6,9,
                    -8,8,
                    -8,4,
                    -8,0,
                    -8,-4,
                    -8,-8,
                    -6,-9,
                    -4,-10,
                    -2,-11,
                    0,-12,
                    2,-11,
                    4,-10,
                    6,-9,
                    8,-8,
                    8,-4,
                    8,0,
                    8,4,
                    8,8,
                    6,9,
                    4,10,
                    2,11,
                    0,12};
static int __1[] = {2,11,
                    -8,-8,
                    -6,-9,
                   -4,-10,
                    -2,-11,
                   0,-12,
                   0,-8,
                   0,-4,
                   0,0,
                   0,4,
                   0,8,
                   0,12};
static int __2[] = {2,24,
                    -8,-8,
                    -6,-9,
                    -4,-10,
                    -2,-11,
                    0,-12,
                    2,-11,
                    4,-10,
                    6,-9,
                    8,-8,
                    8,-4,
                    8,0,
                    6,1,
                    4,2,
                    2,3,
                    0,4,
                    -2,5,
                    -4,6,
                    -6,7,
                    -8,8,
                    -8,12,
                    -4,12,
                    0,12,
                    4,12,
                    8,12};
static int __3[] = {2,27,
                    -8,8,
                    -6,9,
                    -4,10,
                    -2,11,
                    0,12,
                    2,11,
                    4,10,
                    6,9,
                    8,8,
                    8,4,
                    6,3,
                    4,2,
                    2,1,
                    0,0,
                    2,-1,
                    4,-2,
                    6,-3,
                    8,-4,
                    8,-8,
                    6,-9,
                    4,-10,
                    2,-11,
                    0,-12,
                    -2,-11,
                    -4,-10,
                    -6,-9,
                    -8,-8};
static int __4[] = {2,15,
                    -4,-12,
                    -5,-9,
                    -6,-6,
                    -7,-3,
                    -8,0,
                    -4,0,
                    0,0,
                    4,0,
                    8,0,
                    8,-12,
                    8,-8,
                    8,-4,
                    8,4,
                    8,8,
                    8,12};
static int __5[] = {2,25,
                    8,-12,
                    4,-12,
                    0,-12,
                    -4,-12,
                    -5,-9,
                    -6,-6,
                    -7,-3,
                    -8,0,
                    -4,0,
                    -2,-1,
                    0,-2,
                    2,-1,
                    4,0,
                    6,1,
                    8,2,
                    8,5,
                    8,8,
                    6,9,
                    4,10,
                    2,11,
                    0,12,
                    -2,11,
                    -4,10,
                    -6,9,
                    -8,8};
static int __6[] = {2,31,
                    8,-8,
                    6,-9,
                    4,-10,
                    2,-11,
                    0,-12,
                    -2,-11,
                    -4,-10,
                    -6,-9,
                    -8,-8,
                    -8,-5,
                    -8,-1,
                    -8,5,
                    -8,8,
                    -6,9,
                    -4,10,
                    -2,11,
                    0,12,
                    2,11,
                    4,10,
                    6,9,
                    8,8,
                    8,5,
                    8,2,
                    6,1,
                    4,0,
                    2,-1,
                    0,-2,
                    -2,-1,
                    -4,0,
                    -6,1,
                    -8,2};
static int __7[] = {2,13,
                    -8,-12,
                    -4,-12,
                    0,-12,
                    4,-12,
                    8,-12,
                    6,-9,
                    4,-6,
                    2,-3,
                    0,0,
                    -2,3,
                    -4,6,
                    -4,9,
                    -4,12};
static int __8[] = {2,38,
                    0,-2,
                    2,-3,
                    4,-4,
                    6,-5,
                    8,-6,
                    8,-8,
                    6,-9,
                    4,-10,
                    2,-11,
                    0,-12,
                    -2,-11,
                    -4,-10,
                    -6,-9,
                    -8,-8,
                    -8,-6,
                    -6,-5,
                    -4,-4,
                    -2,-3,
                    2,-1,
                    4,0,
                    6,1,
                    8,2,
                    8,5,
                    8,8,
                    6,9,
                    4,10,
                    2,11,
                    0,12,
                    -2,11,
                    -4,10,
                    -6,9,
                    -8,8,
                    -8,5,
                    -8,2,
                    -6,1,
                    -4,0,
                    -2,-1,
                    0,-2};
static int __9[] = {2,31,
                    -8,8,
                    -6,9,
                    -4,10,
                    -2,11,
                    -0,12,
                    2,11,
                    4,10,
                    6,9,
                    8,8,
                    8,5,
                    8,1,
                    8,-5,
                    8,-8,
                    6,-9,
                    4,-10,
                    2,-11,
                    0,-12,
                    -2,-11,
                    -4,-10,
                    -6,-9,
                    -8,-8,
                    -8,-5,
                    -8,-2,
                    -6,-1,
                    -4,0,
                    -2,1,
                    -0,2,
                    2,1,
                    4,0,
                    6,-1,
                    8,-2};
static int* Arabic[] = {__0,__1,__2,__3,__4,__5,__6,__7,__8,__9};

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
  
  window_set_background_color(s_main_window, curr_bg_color);
  #ifdef PBL_RGB_BACKLIGHT
  light_set_color(curr_bl_color);
  #endif
  
  // Get Local Time
  int hour = tick_time->tm_hour;
  int minute = tick_time->tm_min;
  
  #ifdef DEV
  hour = (tick_time->tm_sec) % 24;
  hour = 5;
  minute = (tick_time->tm_sec/2) % 60;
  #endif
  
  #ifdef SCREENSHOT
  hour = 19 % (settings.HourMode?24:12);
  minute = (20+tick_time->tm_sec)%60;
  if (minute == 23) {minute = 18;}
  #endif
  
  // int date = tick_time->tm_mday;
  
  // double pulse on the top of the hour, plus offset
  if ((minute == (60+settings.VibeOffset)%60) && settings.HourPulse){
    // check that we are within the VibeStart and VibeEnd bounds
    if (((hour + (settings.VibeOffset<0?1:0))%24 >= settings.VibeStart) &&\
       ((hour + (settings.VibeOffset<0?1:0))%24 <= settings.VibeEnd)){
      vibes_double_pulse();
      #ifdef DEV
      hour = 5;
      #endif
    }
  }
  
  //clock_is_24h_style() would usually be used instead of explicit settings.
  //But 24h mode looks weird in roman numerals, even if that's what is set for the rest of the watch
  hour = hour % (settings.HourMode?24:12);
  
  int minute_angle = DEG_TO_TRIGANGLE (6*minute);
  
  // Get center of screen and max visible arm length for scaling
  GPoint center = GPoint((bounds.size.w-1)/2, (bounds.size.h-1)/2);
  int max_len = 2 * (center.x>center.y ? center.x : center.y);
  int shadow_y = (-cos_lookup(minute_angle) * max_len / TRIG_MAX_RATIO);
  int shadow_x = (sin_lookup(minute_angle) *max_len / TRIG_MAX_RATIO);


  GPoint digit_center = GPointZero;
  GPoint start = GPointZero;
  GPoint end = GPointZero;
  int i, j;
  
  // Calculate the stroke thickness
  int thickness = 4*settings.HourSize/10;
  
  // Draw Shadow Elements (minute umbra, ticks, etc)

  
  // For BW displays, draw the shadows in the opposite color of the background if the shadow is supposed to be Gray
  #ifdef PBL_BW
  bool is_white = gcolor_equal(curr_bg_color, GColorWhite);
  if (gcolor_equal(curr_min_color, GColorLightGray)) {
      graphics_context_set_stroke_color(ctx, is_white?GColorBlack:GColorWhite);
  } else {
    graphics_context_set_stroke_color(ctx, curr_min_color);
  }
  #else
  graphics_context_set_stroke_color(ctx, curr_min_color);
  #endif
  
  //Draw Roman Hour Shadow
  if (settings.NumeralType == 0){
    graphics_context_set_stroke_width(ctx, thickness + 1);
    if (hour == 0 && settings.HourMode == true){
      start = GPoint(center.x, center.y);
      end = GPoint(center.x + shadow_x, center.y + shadow_y);
      graphics_context_set_stroke_width(ctx, 28*settings.HourSize/10+1);
      graphics_draw_line(ctx, start, end);
    } else {
      graphics_context_set_stroke_width(ctx, thickness + 1);
      digit_center = GPoint(center.x - Numerals[hour]->Width * thickness/2, center.y);
      
      for (i = 0; i<Numerals[hour]->NumberOfDigits; i++){
        digit_center = GPoint(digit_center.x + thickness*(Numerals[hour]->Digits[i][0]), digit_center.y);
        for (j = 2; j <= 2* Numerals[hour]->Digits[i][1]; j = j + 2){
          start = GPoint(digit_center.x + settings.HourSize*Numerals[hour]->Digits[i][j]/10, digit_center.y + settings.HourSize*Numerals[hour]->Digits[i][j+1]/10);
          end = GPoint(start.x + shadow_x, start.y + shadow_y);
          graphics_draw_line(ctx, start, end);
        }
        digit_center = GPoint(digit_center.x + thickness*(2+Numerals[hour]->Digits[i][0]), digit_center.y);
      }
    }
  }
  
  //Draw Arabic Hour Shadow
  if (settings.NumeralType == 1){
    graphics_context_set_stroke_width(ctx, thickness + 1);
    if (hour>=10){
      //Draw the first digit, then 10's digit
      int d1 = hour<20?1:0;
      digit_center = GPoint(center.x - (((hour%10==1?2:3) - d1) * thickness ), center.y);
      for (i = 2; i <= 2* Arabic[2-d1][1]; i = i + 2){
        start = GPoint(digit_center.x + settings.HourSize*Arabic[2-d1][i]/10, digit_center.y + settings.HourSize*Arabic[2-d1][i+1]/10);
        end = GPoint(start.x + shadow_x, start.y + shadow_y);
        graphics_draw_line(ctx, start, end);
      }
      digit_center = GPoint(digit_center.x + thickness * (6 - (2*d1)), center.y);
    } else {
    //Draw single-digit hour
      digit_center = GPoint(center.x, center.y);
      if (hour%10==1){
        digit_center = GPoint(center.x + thickness, center.y);
      }
    }
    for (i = 2; i <= 2* Arabic[hour%10][1]; i = i + 2){
      start = GPoint(digit_center.x + settings.HourSize*Arabic[hour%10][i]/10, digit_center.y + settings.HourSize*Arabic[hour%10][i+1]/10);
      end = GPoint(start.x + shadow_x, start.y + shadow_y);
      graphics_draw_line(ctx, start, end);
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
  if (gcolor_equal(curr_min_color, GColorLightGray)) {
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
  
  
  // draw border for hour digit in background color if the background color is not the same as the hour numeral
  if (settings.HourBorder && !gcolor_equal(curr_hr_color, curr_bg_color)){
    #ifdef PBL_BW
    graphics_context_set_stroke_color(ctx,gcolor_equal(curr_hr_color, GColorWhite)?GColorBlack:GColorWhite);
    #else
    graphics_context_set_stroke_color(ctx, curr_bg_color);
    #endif
  
    // Draw Hour Numeral
    if (settings.NumeralType == 0){
      graphics_context_set_stroke_width(ctx, thickness + 3);
      if (hour == 0 && settings.HourMode == true){
        start = GPoint(center.x, center.y);
        end = GPoint(center.x + shadow_x, center.y + shadow_y);
        graphics_draw_circle(ctx, start, 12*settings.HourSize/10);
      } else {
        digit_center = GPoint(center.x - Numerals[hour]->Width * thickness/2, center.y);
        for (i = 0; i<Numerals[hour]->NumberOfDigits; i++){
          digit_center = GPoint(digit_center.x + thickness*(Numerals[hour]->Digits[i][0]), digit_center.y);
          for (j = 2; j < 2* Numerals[hour]->Digits[i][1]; j = j + 2){
            start = GPoint(digit_center.x + settings.HourSize*Numerals[hour]->Digits[i][j]/10, digit_center.y + settings.HourSize*Numerals[hour]->Digits[i][j+1]/10);
            end = GPoint(digit_center.x + settings.HourSize*Numerals[hour]->Digits[i][j+2]/10, digit_center.y + settings.HourSize*Numerals[hour]->Digits[i][j+3]/10);
            graphics_draw_line(ctx, start, end);
          }
          digit_center = GPoint(digit_center.x + thickness*(2+Numerals[hour]->Digits[i][0]), digit_center.y);
        }
      }
    }
    
   //Draw Arabic Hour Numeral
    if (settings.NumeralType == 1){
      graphics_context_set_stroke_width(ctx, thickness + 3);
      if (hour>=10){
        //Draw the first digit, then 10's digit
        int d1 = hour<20?1:0;
        digit_center = GPoint(center.x - (((hour%10==1?2:3) - d1) * thickness ), center.y);
        for (i = 2; i < 2* Arabic[2-d1][1]; i = i + 2){
          start = GPoint(digit_center.x + settings.HourSize*Arabic[2-d1][i]/10, digit_center.y + settings.HourSize*Arabic[2-d1][i+1]/10);
          end = GPoint(digit_center.x + settings.HourSize*Arabic[2-d1][i+2]/10, digit_center.y + settings.HourSize*Arabic[2-d1][i+3]/10);
          graphics_draw_line(ctx, start, end);
        }
        digit_center = GPoint(digit_center.x + thickness * (6 - (2*d1)), center.y);
      } else {
      //Draw single-digit hour
        digit_center = GPoint(center.x, center.y);
        if (hour%10==1){
          digit_center = GPoint(center.x + thickness, center.y);
        }
      }
      for (i = 2; i < 2* Arabic[hour%10][1]; i = i + 2){
        start = GPoint(digit_center.x + settings.HourSize*Arabic[hour%10][i]/10, digit_center.y + settings.HourSize*Arabic[hour%10][i+1]/10);
        end = GPoint(digit_center.x + settings.HourSize*Arabic[hour%10][i+2]/10, digit_center.y + settings.HourSize*Arabic[hour%10][i+3]/10);
        graphics_draw_line(ctx, start, end);
      }
    }
  }
  
  if (settings.HourBorder && gcolor_equal(curr_bg_color, curr_hr_color)){
    #ifdef PBL_BW
    graphics_context_set_stroke_color(ctx,gcolor_equal(curr_hr_color, GColorWhite)?GColorBlack:GColorWhite);
    #else
    graphics_context_set_stroke_color(ctx, curr_min_color);
    #endif
  } else {
    graphics_context_set_stroke_color(ctx, curr_hr_color);
  }
  
  // Draw Hour Numeral
  if (settings.NumeralType == 0){
    graphics_context_set_stroke_width(ctx, thickness + 1);
    if (hour == 0 && settings.HourMode == true){
      start = GPoint(center.x, center.y);
      end = GPoint(center.x + shadow_x, center.y + shadow_y);
      graphics_draw_circle(ctx, start, 12*settings.HourSize/10);
    } else {
      digit_center = GPoint(center.x - Numerals[hour]->Width * thickness/2, center.y);
      for (i = 0; i<Numerals[hour]->NumberOfDigits; i++){
        digit_center = GPoint(digit_center.x + thickness*(Numerals[hour]->Digits[i][0]), digit_center.y);
        for (j = 2; j < 2* Numerals[hour]->Digits[i][1]; j = j + 2){
          start = GPoint(digit_center.x + settings.HourSize*Numerals[hour]->Digits[i][j]/10, digit_center.y + settings.HourSize*Numerals[hour]->Digits[i][j+1]/10);
          end = GPoint(digit_center.x + settings.HourSize*Numerals[hour]->Digits[i][j+2]/10, digit_center.y + settings.HourSize*Numerals[hour]->Digits[i][j+3]/10);
          graphics_draw_line(ctx, start, end);
        }
        digit_center = GPoint(digit_center.x + thickness*(2+Numerals[hour]->Digits[i][0]), digit_center.y);
      }
    }
  }
  
 //Draw Arabic Hour Numeral
  if (settings.NumeralType == 1){
    graphics_context_set_stroke_width(ctx, thickness + 1);
    if (hour>=10){
      //Draw the first digit, then 10's digit
      int d1 = hour<20?1:0;
      digit_center = GPoint(center.x - (((hour%10==1?2:3) - d1) * thickness ), center.y);
      for (i = 2; i < 2* Arabic[2-d1][1]; i = i + 2){
        start = GPoint(digit_center.x + settings.HourSize*Arabic[2-d1][i]/10, digit_center.y + settings.HourSize*Arabic[2-d1][i+1]/10);
        end = GPoint(digit_center.x + settings.HourSize*Arabic[2-d1][i+2]/10, digit_center.y + settings.HourSize*Arabic[2-d1][i+3]/10);
        graphics_draw_line(ctx, start, end);
      }
      digit_center = GPoint(digit_center.x + thickness * (6 - (2*d1)), center.y);
    } else {
    //Draw single-digit hour
      digit_center = GPoint(center.x, center.y);
      if (hour%10==1){
        digit_center = GPoint(center.x + thickness, center.y);
      }
    }
    for (i = 2; i < 2* Arabic[hour%10][1]; i = i + 2){
      start = GPoint(digit_center.x + settings.HourSize*Arabic[hour%10][i]/10, digit_center.y + settings.HourSize*Arabic[hour%10][i+1]/10);
      end = GPoint(digit_center.x + settings.HourSize*Arabic[hour%10][i+2]/10, digit_center.y + settings.HourSize*Arabic[hour%10][i+3]/10);
      graphics_draw_line(ctx, start, end);
    }
  }
  
  
  // draw smaller hour digit if the hour color is same as the background numeral
  if (settings.HourBorder && gcolor_equal(curr_hr_color, curr_bg_color)){
    graphics_context_set_stroke_color(ctx, curr_hr_color);
  
    // Draw Hour Numeral
    if (settings.NumeralType == 0){
      graphics_context_set_stroke_width(ctx, thickness - 1);
      if (hour == 0 && settings.HourMode == true){
        start = GPoint(center.x, center.y);
        end = GPoint(center.x + shadow_x, center.y + shadow_y);
        graphics_draw_circle(ctx, start, 12*settings.HourSize/10);
      } else {
        digit_center = GPoint(center.x - Numerals[hour]->Width * thickness/2, center.y);
        for (i = 0; i<Numerals[hour]->NumberOfDigits; i++){
          digit_center = GPoint(digit_center.x + thickness*(Numerals[hour]->Digits[i][0]), digit_center.y);
          for (j = 2; j < 2* Numerals[hour]->Digits[i][1]; j = j + 2){
            start = GPoint(digit_center.x + settings.HourSize*Numerals[hour]->Digits[i][j]/10, digit_center.y + settings.HourSize*Numerals[hour]->Digits[i][j+1]/10);
            end = GPoint(digit_center.x + settings.HourSize*Numerals[hour]->Digits[i][j+2]/10, digit_center.y + settings.HourSize*Numerals[hour]->Digits[i][j+3]/10);
            graphics_draw_line(ctx, start, end);
          }
          digit_center = GPoint(digit_center.x + thickness*(2+Numerals[hour]->Digits[i][0]), digit_center.y);
        }
      }
    }
    
   //Draw Arabic Hour Numeral
    if (settings.NumeralType == 1){
      graphics_context_set_stroke_width(ctx, thickness - 1);
      if (hour>=10){
        //Draw the first digit, then 10's digit
        int d1 = hour<20?1:0;
        digit_center = GPoint(center.x - (((hour%10==1?2:3) - d1) * thickness ), center.y);
        for (i = 2; i < 2* Arabic[2-d1][1]; i = i + 2){
          start = GPoint(digit_center.x + settings.HourSize*Arabic[2-d1][i]/10, digit_center.y + settings.HourSize*Arabic[2-d1][i+1]/10);
          end = GPoint(digit_center.x + settings.HourSize*Arabic[2-d1][i+2]/10, digit_center.y + settings.HourSize*Arabic[2-d1][i+3]/10);
          graphics_draw_line(ctx, start, end);
        }
        digit_center = GPoint(digit_center.x + thickness * (6 - (2*d1)), center.y);
      } else {
      //Draw single-digit hour
        digit_center = GPoint(center.x, center.y);
        if (hour%10==1){
          digit_center = GPoint(center.x + thickness, center.y);
        }
      }
      for (i = 2; i < 2* Arabic[hour%10][1]; i = i + 2){
        start = GPoint(digit_center.x + settings.HourSize*Arabic[hour%10][i]/10, digit_center.y + settings.HourSize*Arabic[hour%10][i+1]/10);
        end = GPoint(digit_center.x + settings.HourSize*Arabic[hour%10][i+2]/10, digit_center.y + settings.HourSize*Arabic[hour%10][i+3]/10);
        graphics_draw_line(ctx, start, end);
      }
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
  apply_schedule_colors();
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
  
  Tuple *vibe_start_t = dict_find(iterator, MESSAGE_KEY_VibeStart);
  if (vibe_start_t) {
    settings.VibeStart = vibe_start_t->value->int32;
  }
  
  Tuple *vibe_end_t = dict_find(iterator, MESSAGE_KEY_VibeEnd);
  if (vibe_end_t) {
    settings.VibeEnd = vibe_end_t->value->int32;
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
  
  Tuple *numeral_type_t = dict_find(iterator, MESSAGE_KEY_NumeralType);
  if (numeral_type_t) {
    settings.NumeralType = numeral_type_t->value->int32 == 1;
  }
  
  Tuple *hour_border_t = dict_find(iterator, MESSAGE_KEY_HourBorder);
  if (hour_border_t) {
    settings.HourBorder = hour_border_t->value->int32 == 1;
  }
  
  #ifdef PBL_RGB_BACKLIGHT
  Tuple *light_color_t = dict_find(iterator, MESSAGE_KEY_BacklightColor);
  if (light_color_t) {
    settings.BacklightColor = GColorFromHEX(light_color_t->value->int32);
    light_set_color(settings.BacklightColor);
  }
  
  // Save and apply if any settings were changed
  if ( bg_color_t || hour_color_t || min_color_t || hour_mode_t || vibe_offset_t || numeral_type_t || hour_border_t ||\
      hour_size_t || hour_pulse_t || bt_pulse_t || tick_size_t || vibe_start_t || vibe_end_t || light_color_t) {
    prv_save_settings();
    apply_schedule_colors();
    
    curr_bg_color = settings.BackgroundColor;
    curr_hr_color = settings.HourColor;
    curr_min_color = settings.MinuteColor;
    curr_bl_color = settings.BacklightColor;
    
    window_set_background_color(s_main_window, curr_bg_color);
    
    layer_mark_dirty(s_window_layer);
  }
  
  #else
  
  // Save and apply if any settings were changed
  if ( bg_color_t || hour_color_t || min_color_t || hour_mode_t || vibe_offset_t || numeral_type_t || hour_border_t ||\
      hour_size_t || hour_pulse_t || bt_pulse_t || tick_size_t || vibe_start_t || vibe_end_t) {
    prv_save_settings();
    apply_schedule_colors();
    
    curr_bg_color = settings.BackgroundColor;
    curr_hr_color = settings.HourColor;
    curr_min_color = settings.MinuteColor;
    
    window_set_background_color(s_main_window, curr_bg_color);
    
    layer_mark_dirty(s_window_layer);
  }
  #endif
  
  bool sched_changed = false;
  
  for (int i = 0; i < SCHEDULE_ENTRIES; i++){
    Tuple *enable_t = dict_find(iterator, MESSAGE_KEY_EN+i);
    if (enable_t) {
      settings.Schedule[i].Enable = enable_t->value->int32 == 1;
      sched_changed = true;
    }
    
    Tuple *hour_t = dict_find(iterator, MESSAGE_KEY_HR+i);
    if (hour_t) {
      char* hour_s = hour_t->value->cstring;
      settings.Schedule[i].Hour = 10*(hour_s[0] - '0') + (hour_s[1] - '0');
      sched_changed = true;
    }
    
    Tuple *min_t = dict_find(iterator, MESSAGE_KEY_MIN+i);
    if (min_t) {
      char* min_s = min_t->value->cstring;
      settings.Schedule[i].Minute = 10*(min_s[0] - '0') + (min_s[1] - '0');
      sched_changed = true;
    }
    
    Tuple *bg_t = dict_find(iterator, MESSAGE_KEY_BG+i);
    if (bg_t) {
      settings.Schedule[i].BackgroundColor = GColorFromHEX(bg_t->value->int32);
      sched_changed = true;
    }
    
    Tuple *num_t = dict_find(iterator, MESSAGE_KEY_NUM+i);
    if (num_t) {
      settings.Schedule[i].HourColor = GColorFromHEX(num_t->value->int32);
      sched_changed = true;
    }
    
    Tuple *umb_t = dict_find(iterator, MESSAGE_KEY_UMB+i);
    if (umb_t) {
      settings.Schedule[i].MinuteColor = GColorFromHEX(umb_t->value->int32);
      sched_changed = true;
    }
    
    Tuple *bl_t = dict_find(iterator, MESSAGE_KEY_BL+i);
    if (bl_t) {
      settings.Schedule[i].BacklightColor = GColorFromHEX(bl_t->value->int32);
      sched_changed = true;
    }
  }
  
  if (sched_changed){
    prv_save_settings();
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
  
  bootstrap_schedule_colors();

  s_main_window = window_create();
  window_set_background_color(s_main_window, curr_bg_color);
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  window_stack_push(s_main_window, true);

  layer_mark_dirty(s_window_layer);

  #ifdef DEV
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  #else
  #ifdef SCREENSHOT
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  #else
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  #endif
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
  const int inbox_size = app_message_inbox_size_maximum();
  const int outbox_size = 256;
  app_message_open(inbox_size, outbox_size);
}

static void deinit() {
  window_destroy(s_main_window);
}

int main(void) {
  #ifdef FORCE_BACKLIGHT
  light_enable(true);
  #endif
  
  init();
  app_event_loop();
  deinit();
  
  #ifdef FORCE_BACKLIGHT
  light_enable(false);
  #endif
}
