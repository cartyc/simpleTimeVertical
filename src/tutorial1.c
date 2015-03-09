#include <pebble.h>

//Set pointer for main window
static Window *s_main_window;

//Time Text Layer
static TextLayer *s_time_layer;

//Text Layer
static TextLayer *s_text_layer;

//Weather Layer
static TextLayer *s_weather;


//Hour Font
static GFont s_hour_font;

//Minute Font
static GFont s_min_font;

//Weather Font

static GFont s_weather_font;

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
	text_layer_set_text(s_time_layer, minBuffer);
	text_layer_set_text(s_text_layer, hourBuffer);
}

//Load and unload the windows
static void main_window_load(Window *window){

	//Load Font
	s_hour_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_LATO_BOLD_52));
	s_min_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_LATO_48));

	//Create Minute  Layer
	s_time_layer = text_layer_create(GRect(0, 75, 144, 50));
	text_layer_set_background_color(s_time_layer, GColorClear);
	text_layer_set_text_color(s_time_layer, GColorBlack);

	//Make things like a watch
	text_layer_set_font(s_time_layer, s_min_font);
	text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

	//Add Hour layer
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));

	//text layer
	s_text_layer = text_layer_create(GRect(0,10,144,70));
	text_layer_set_background_color(s_text_layer, GColorWhite);
	text_layer_set_text_color(s_text_layer, GColorBlack);
	
	//Set text layer parameters
	text_layer_set_font(s_text_layer, s_hour_font);
	text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);	


	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_text_layer));

	//Weather Layer

	s_weather = text_layer_create(GRect(0,140,144,25));
	text_layer_set_background_color(s_weather, GColorWhite);
	text_layer_set_text_color(s_weather, GColorBlack);
	text_layer_set_text_alignment(s_weather, GTextAlignmentCenter);

	text_layer_set_text(s_weather, "loading...");

	s_weather_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_LATO_20));
	text_layer_set_font(s_weather, s_weather_font);
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_weather));
}

static void main_window_unload(Window *window){

	// Destroy the text main text window
	text_layer_destroy(s_time_layer);
	text_layer_destroy(s_text_layer);
}


//tick timer
static void tick_handler(struct tm * tick_time, TimeUnits units_changed){
	update_time();
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

	//Register the TickTimer Service
	tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

	window_stack_push(s_main_window, true);

	//So that time displays when face is loaded
	update_time();

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
