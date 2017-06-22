#include <pebble.h>

static Window *mainWindow;

static Layer *layerTime;
static Layer *layerBatt;

static TextLayer *tLayerDate1;
static TextLayer *tLayerDate2;

GFont fontSquareM; 
GFont fontSquareS; 

static GColor colorHour;
static GColor colorTens;
static GColor colorOnes;
static GColor colorBatt;
static GColor colorStep1;
static GColor colorStep2;

static void time_layer_update(Layer *layer, GContext *context) {
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  int hour = tick_time->tm_hour;
  int tens = tick_time->tm_min / 10;
  int ones = tick_time->tm_min % 10;

  static char dayOfMo[] = "00";
  static char dayOfYr[] = "000";
  
  graphics_context_set_fill_color(context, colorHour); 
  if (hour < 13) {
    for (int i = 0; i < hour; i++) {
      graphics_fill_radial(context, GRect(14, 14, 130 , 130), GOvalScaleModeFitCircle, 12,
                                  DEG_TO_TRIGANGLE(1 + 30 * i), DEG_TO_TRIGANGLE(30 + 30 * i));
    }
    graphics_fill_radial(context, GRect(19, 19, 120 , 120), GOvalScaleModeFitCircle, 2,
                                DEG_TO_TRIGANGLE(30 * hour), DEG_TO_TRIGANGLE(360));
  }
  else {
    hour = hour - 12;
    graphics_fill_radial(context, GRect(19, 19, 120 , 120), GOvalScaleModeFitCircle, 2,
                                DEG_TO_TRIGANGLE(0), DEG_TO_TRIGANGLE(30 * hour));
    for (int i = hour; i < 12; i++) {
      graphics_fill_radial(context, GRect(14, 14, 130 , 130), GOvalScaleModeFitCircle, 12,
                                  DEG_TO_TRIGANGLE(1 + 30 * i), DEG_TO_TRIGANGLE(30 + 30 * i));
    }
  }
  
  graphics_context_set_fill_color(context, colorTens); 
  if (tens < 4) {
    for (int i = 0; i < tens; i++) {
      graphics_fill_radial(context, GRect(28, 28, 102, 102), GOvalScaleModeFitCircle, 12,
                                  DEG_TO_TRIGANGLE(1 + 120 * i), DEG_TO_TRIGANGLE(120 + 120 * i));
    }
    graphics_fill_radial(context, GRect(33, 33, 92, 92), GOvalScaleModeFitCircle, 2,
                                DEG_TO_TRIGANGLE(120 * tens), DEG_TO_TRIGANGLE(360));
  }
  else {
    tens = tens - 3;
    graphics_fill_radial(context, GRect(33, 33, 92, 92), GOvalScaleModeFitCircle, 2,
                                DEG_TO_TRIGANGLE(0), DEG_TO_TRIGANGLE(120 * tens));
    for (int i = tens; i < 3; i++) {
      graphics_fill_radial(context, GRect(28, 28, 102, 102), GOvalScaleModeFitCircle, 12,
                                  DEG_TO_TRIGANGLE(1 + 120 * i), DEG_TO_TRIGANGLE(120 + 120 * i));
    }
  }
  
  graphics_context_set_fill_color(context, colorOnes); 
  if (ones < 6) {
    for (int i = 0; i < ones; i++) {
      graphics_fill_radial(context, GRect(42, 42, 74, 74), GOvalScaleModeFitCircle, 12,
                                  DEG_TO_TRIGANGLE(1 + 72 * i), DEG_TO_TRIGANGLE(71 + 72 * i));
    }
    graphics_fill_radial(context, GRect(47, 47, 64, 64), GOvalScaleModeFitCircle, 2,
                                DEG_TO_TRIGANGLE(72 * ones), DEG_TO_TRIGANGLE(360));
  }
  else {
    ones = ones - 5;
    graphics_fill_radial(context, GRect(47, 47, 64, 64), GOvalScaleModeFitCircle, 2,
                                DEG_TO_TRIGANGLE(0), DEG_TO_TRIGANGLE(72 * ones));
    for (int i = ones; i < 5; i++) {
      graphics_fill_radial(context, GRect(42, 42, 74, 74), GOvalScaleModeFitCircle, 12,
                                  DEG_TO_TRIGANGLE(1 + 72 * i), DEG_TO_TRIGANGLE(71 + 72 * i));
    }
  }
  
  bool connected = bluetooth_connection_service_peek();
  BatteryChargeState charge = battery_state_service_peek();
  int battLvl = 10 - charge.charge_percent / 10;
  
  graphics_context_set_fill_color(context, colorBatt);
  graphics_fill_radial(context, GRect(61, 61, 36, 36), GOvalScaleModeFitCircle, 2,
                       DEG_TO_TRIGANGLE(0), DEG_TO_TRIGANGLE(36 * battLvl));
  graphics_fill_radial(context, GRect(56, 56, 46, 46), GOvalScaleModeFitCircle, 12,
                       DEG_TO_TRIGANGLE(36 * battLvl), DEG_TO_TRIGANGLE(360));    
  
  graphics_context_set_fill_color(context, GColorWhite);
  int i = connected ? 9 : 2;
  graphics_fill_radial(context, GRect(70, 70, 18, 18), GOvalScaleModeFitCircle, i,
                       DEG_TO_TRIGANGLE(0), DEG_TO_TRIGANGLE(360));    

  int hVal = (int)health_service_sum_today(HealthMetricStepCount);
  int meter1 = (hVal / 1000) % 10;
  int meter2 = 10 - (hVal / 100) % 10;
  int meter3 = hVal / 10000;
  
  graphics_context_set_fill_color(context, GColorWhite); 
  for (int i = 0; i < 10; i++) {
    graphics_context_set_fill_color(context, colorStep1); 
    if (i < meter2) {
      graphics_fill_radial(context, GRect(5, 5, 148, 148), GOvalScaleModeFitCircle, 2,
                           DEG_TO_TRIGANGLE(180 + 6 * i), DEG_TO_TRIGANGLE(186 + 6 * i));      
    }
    else {
      graphics_fill_radial(context, GRect(0, 0, 158, 158), GOvalScaleModeFitCircle, 12,
                           DEG_TO_TRIGANGLE(180 + 6 * i), DEG_TO_TRIGANGLE(185 + 6 * i));      
    }
    graphics_context_set_fill_color(context, colorStep2); 
    if (i < meter1) {
      graphics_fill_radial(context, GRect(0, 0, 158, 158), GOvalScaleModeFitCircle, 12,
                           DEG_TO_TRIGANGLE(240 + 12 * i), DEG_TO_TRIGANGLE(251 + 12 * i));      
    }
    else {
      graphics_fill_radial(context, GRect(5, 5, 148, 148), GOvalScaleModeFitCircle, 2,
                           DEG_TO_TRIGANGLE(240 + 12 * i), DEG_TO_TRIGANGLE(252 + 12 * i));      
    }
  }
  
  strftime(dayOfMo, sizeof(".."), "%d", tick_time);
  strftime(dayOfYr, sizeof("..."), "%j", tick_time);  
  text_layer_set_text(tLayerDate1, dayOfMo);
  text_layer_set_text(tLayerDate2, dayOfYr);
}

static void time_handler(struct tm *tick_time, TimeUnits units_changed) {
  layer_mark_dirty(layerTime);
}

static void bluetooth_handler(bool btConn) {
  if (btConn) vibes_short_pulse();
  else vibes_long_pulse();
  layer_mark_dirty(layerTime);
}

static void battery_handler(BatteryChargeState charge) {
  layer_mark_dirty(layerTime);
}

static void main_window_load_handler(Window *window) {
  window_set_background_color(mainWindow, GColorBlack);
  
  fontSquareM = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SQUARE_22));
  fontSquareS = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SQUARE_26));
  
  layerTime = layer_create(GRect(0, 5, 144, 158));
  layer_set_update_proc(layerTime, time_layer_update);
  
  tLayerDate1 = text_layer_create(GRect(48, -8, 96, 28));
  text_layer_set_text_color(tLayerDate1, GColorWhite);
  text_layer_set_background_color(tLayerDate1, GColorClear);
  text_layer_set_text_alignment(tLayerDate1, GTextAlignmentRight);
  text_layer_set_font(tLayerDate1, fontSquareS);
  text_layer_set_text(tLayerDate1, "00.00");
  
  tLayerDate2 = text_layer_create(GRect(48, 142, 94, 24));
  text_layer_set_text_color(tLayerDate2, GColorWhite);
  text_layer_set_background_color(tLayerDate2, GColorClear);
  text_layer_set_text_alignment(tLayerDate2, GTextAlignmentRight);
  text_layer_set_font(tLayerDate2, fontSquareM);
  text_layer_set_text(tLayerDate2, "000");
  
  layer_add_child(window_get_root_layer(mainWindow), layerTime);
  layer_add_child(window_get_root_layer(mainWindow), text_layer_get_layer(tLayerDate1));
  layer_add_child(window_get_root_layer(mainWindow), text_layer_get_layer(tLayerDate2));
}

static void main_window_unload_handler(Window *window) {
  fonts_unload_custom_font(fontSquareM);
  fonts_unload_custom_font(fontSquareS);
  layer_destroy(layerTime);
  text_layer_destroy(tLayerDate1);
  text_layer_destroy(tLayerDate2);
}

static void inbox_received_handler(DictionaryIterator *iter, void *context) {
  Tuple* hrColor = dict_find(iter, MESSAGE_KEY_HOUR);
  if (hrColor) {
    colorHour = GColorFromHEX(hrColor->value->int32);
    persist_write_int(0, hrColor->value->int32);
  }
  Tuple* tnColor = dict_find(iter, MESSAGE_KEY_TENS);
  if (tnColor) {
    colorTens = GColorFromHEX(tnColor->value->int32);
    persist_write_int(1, tnColor->value->int32);
  }
  Tuple* onColor = dict_find(iter, MESSAGE_KEY_ONES);
  if (onColor) {
    colorOnes = GColorFromHEX(onColor->value->int32);
    persist_write_int(2, onColor->value->int32);
  }
  Tuple* baColor = dict_find(iter, MESSAGE_KEY_BATT);
  if (baColor) {
    colorBatt = GColorFromHEX(baColor->value->int32);
    persist_write_int(4, baColor->value->int32);
  }
  Tuple* s1Color = dict_find(iter, MESSAGE_KEY_STEP1);
  if (s1Color) {
    colorStep1 = GColorFromHEX(s1Color->value->int32);
    persist_write_int(5, s1Color->value->int32);
  }
  Tuple* s2Color = dict_find(iter, MESSAGE_KEY_STEP2);
  if (s2Color) {
    colorStep2 = GColorFromHEX(s2Color->value->int32);
    persist_write_int(6, s2Color->value->int32);
  }
  
  layer_mark_dirty(layerTime);
}

void init(void) {
  colorHour = persist_exists(0) ? GColorFromHEX(persist_read_int(0)) : GColorRed;
  colorTens = persist_exists(1) ? GColorFromHEX(persist_read_int(1)) : GColorGreen;
  colorOnes = persist_exists(2) ? GColorFromHEX(persist_read_int(2)) : GColorBlue;
  colorBatt = persist_exists(4) ? GColorFromHEX(persist_read_int(4)) : GColorLightGray;
  colorStep1 = persist_exists(5) ? GColorFromHEX(persist_read_int(5)) : GColorOrange;
  colorStep2 = persist_exists(6) ? GColorFromHEX(persist_read_int(6)) : GColorYellow;
  
  mainWindow = window_create();

  window_set_window_handlers(mainWindow, (WindowHandlers) {
    .load = main_window_load_handler,
    .unload = main_window_unload_handler
  });
  
  app_message_register_inbox_received(inbox_received_handler);
  app_message_open(128, 128);
  
  tick_timer_service_subscribe(MINUTE_UNIT, time_handler);
  bluetooth_connection_service_subscribe(bluetooth_handler);
  battery_state_service_subscribe(battery_handler);

  window_stack_push(mainWindow, true);
}

void deinit(void) {
  window_destroy(mainWindow);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
