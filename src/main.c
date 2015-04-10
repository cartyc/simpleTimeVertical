 #include <pebble.h>

//place holders for weather values
#define KEY_TEMPERATURE 0
#define KEY_CONDITIONS 1

/////////////////////////
//Set Variables for use//
/////////////////////////

//Set pointer for main window
static Window *s_main_window;
//Time Text Layer
static TextLayer *s_minute;
//Text Layer
static TextLayer *s_hour;
//Weather Layer
static TextLayer *s_weather;
//Hour Font
static GFont s_hour_font;
//Minute Font
static GFont s_min_font;
//Weather Font
static GFont s_weather_font;

////////////////////////
//Make the time update//
////////////////////////
static void update_time(){
	//Set time structure
	time_t temp = time(NULL);
	struct tm *tick_time = localtime(&temp);

	//Create a time buffer
	static char hourBuffer[] = "00";
	static char minBuffer[] = "00";
	
	//Write the current hours and minutes!
	if (clock_is_24h_style() == true){
		//Use 24 hours format
		strftime(hourBuffer, sizeof("00"), "%H", tick_time);
		strftime(minBuffer, sizeof("00"), "%M", tick_time);
	} else{
		strftime(hourBuffer, sizeof("00"), "%I", tick_time);
		strftime(minBuffer, sizeof("00"), "%M", tick_time);
	}

	//Display this on the textlayer
	text_layer_set_text(s_minute, minBuffer);
	text_layer_set_text(s_hour, hourBuffer);

}

/////////////////////////
//Set Text Layer styles//
/////////////////////////
static void setLayer( TextLayer *layer, const char * fontColor, const char * backgroundColor, GFont fontType) {
	const char * clear = "clear";
	const char * black = "black";

	//Set the background color
	if ( backgroundColor == clear){
		text_layer_set_background_color(layer, GColorClear);
	} else if ( backgroundColor == black){
		text_layer_set_background_color(layer, GColorBlack);
	};
	
	//Set The font color
	if ( fontColor == black){
		text_layer_set_text_color(layer, GColorBlack);
	} else if ( fontColor == clear){
		text_layer_set_text_color(layer, GColorClear);
	};

	text_layer_set_font(layer, fontType);

}

///////////////////////////////
//Load and unload the windows//
///////////////////////////////
static void main_window_load(Window *window){

	//////////////
	//Load Fonts//
	//////////////
	s_hour_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_LATO_BOLD_58));
	s_min_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_LATO_52));
	s_weather_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_LATO_20));


	////////////////////////
	//Create Minute  Layer//
	////////////////////////
	s_minute = text_layer_create(GRect(0, 75, 144, 60));
	//Set Layer design
	setLayer(s_minute, "black", "clear", s_min_font);
	//Make things like a watch
	text_layer_set_text_alignment(s_minute, GTextAlignmentCenter);
	//Add Minute layer
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_minute));


	/////////////////////
	//Create Hour layer//
	/////////////////////
	s_hour = text_layer_create(GRect(0,10,144,70));
	//Set the Layer parameters
	setLayer(s_hour, "black", "clear", s_hour_font);
	//Set minute layer parameters
	text_layer_set_text_alignment(s_hour, GTextAlignmentCenter);
	//add layer
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_hour));


	/////////////////
	//Weather Layer//
	/////////////////
	s_weather = text_layer_create(GRect(0,140,144,40));
	setLayer(s_weather, "black", "clear", s_weather_font);
	text_layer_set_text_alignment(s_weather, GTextAlignmentCenter);
	text_layer_set_text(s_weather, "");
	//Add Weather Layer
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_weather));


	/////////////////////
	//Run a time update//
	/////////////////////
	update_time();
}

static void main_window_unload(Window *window){

	// Destroy the text main text window
	text_layer_destroy(s_minute);
	text_layer_destroy(s_hour);
	text_layer_destroy(s_weather);

	fonts_unload_custom_font(s_weather_font);
	fonts_unload_custom_font(s_hour_font);
	fonts_unload_custom_font(s_min_font);
}


//tick timer
static void tick_handler(struct tm *tick_time, TimeUnits units_changed){
	update_time();

	//set Weather Every 15 mins
	if(tick_time->tm_min % 15==0){
		//Begin dict
		DictionaryIterator *iter;
		app_message_outbox_begin(&iter);

		//Add a key-value pair
		dict_write_uint8(iter, 0,0);

		//Send the message
		app_message_outbox_send();
	}
}

//App communication
static void inbox_received_callback(DictionaryIterator *iterator, void *context){
  
  //Store incoming data
  static char temperature_buffer[8];
  static char conditions_buffer[32];
  static char weather_layer_buffer[32];
  
  Tuple *t = dict_read_first(iterator);
  
  //For all items
  while(t != NULL){
    switch(t->key){
      case KEY_TEMPERATURE:
        snprintf(temperature_buffer, sizeof(temperature_buffer), "%d°C", (int)t->value->int32);
        APP_LOG(APP_LOG_LEVEL_ERROR, "Temp %d", (int)t->value->int32);
        break;
      case KEY_CONDITIONS:
        snprintf(conditions_buffer, sizeof(conditions_buffer), "%s", t->value->cstring);
        break;
      default:
        APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
          break;
    }

    APP_LOG(APP_LOG_LEVEL_ERROR, "Temp Key %d", KEY_TEMPERATURE);
	if ( temperature_buffer != 0){
		text_layer_set_background_color(s_weather,GColorBlack);
		text_layer_set_text_color(s_weather, GColorClear);
	} else {
		text_layer_set_background_color(s_weather, GColorClear);
		text_layer_set_background_color(s_weather, GColorBlack);
	}

    snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "%s %s", temperature_buffer, conditions_buffer);
    text_layer_set_text(s_weather, weather_layer_buffer);
    t = dict_read_next(iterator);
  }
}

static void inbox_dropped_callback( AppMessageResult reason, void *context){
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message Dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context){
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context){
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send Success!");
}

//Initialize the watch
static void init(){
	
	//Create main Window element and assign to pointer
	s_main_window = window_create();

	//Set handlers to manage the elements in the window
	window_set_window_handlers(s_main_window, (WindowHandlers){
		.load = main_window_load,
		.unload = main_window_unload
	});

	
	window_stack_push(s_main_window, true);

	//Register the TickTimer Service
	tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

	// //So that time displays when face is loaded
	// update_time();

	//Register the calbacks
	app_message_register_inbox_received(inbox_received_callback);
	app_message_register_inbox_dropped(inbox_dropped_callback);
	app_message_register_outbox_failed(outbox_failed_callback);
	app_message_register_outbox_sent(outbox_sent_callback);

	//open the appmessage to receive incoming
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

}

static void deinit(){

	// Destroy Window
	window_destroy(s_main_window);
}

//This is where the watchface actually runs
int main(void){

	init();
	app_event_loop();
	deinit();
}
