#include <pebble.h>

//Set pointer for main window
static Window *s_main_window;

//Time Text Layer
static TextLayer *s_time_layer;


static void update_time(){
	//Set time structure
	time_t temp = time(NULL);
	struct tm *tick_time = localtime(&temp);

	//Create a time buffer
	static char buffer() = "00:00";
	
	//Write the current hours and minutes!
	if (clock_is_24h_style() == true){
		//Use 24 hours format
		strftime(buffer, sizeof("00:00"), "H:%M", tick_time);
	} else{
		strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
	}

	//Display this on the textlayer
	text_layer_set_text(s_time_layer, buffer);
}

//Load and unload the windows
static void main_window_load(Window *window){

	//Create Time Layer
	s_time_layer = text_layer_create(GRect(0, 55, 144, 50));
	text_layer_set_background_color(s_time_layer, GColorClear);
	text_layer_set_text_color(s_time_layer, GColorBlack);
	//Set default value for text layer
	text_layer_set_text(s_time_layer, "00:00");

	//Make things like a watch
	text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
	text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

	//Add layer
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));

}

static void main_window_unload(Window *window){

	// Destroy the text main text window
	text_layer_destroy(s_time_layer);

}

//tick timer
static void tick_handler(struct tm * tick_time, TimeUnits units_changed){

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
