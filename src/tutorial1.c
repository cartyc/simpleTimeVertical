#include <pebble.h>

//Set pointer for main window
static Window *s_main_window;

//Time Text Layer
static TextLayer *s_time_layer;


//Load and unload the windows
static void main_window_load(Window *window){

	//Create Time Layer
	s_time_layer = text_layer_create(GRect(0, 55, 144, 50));
	text_layer_set_background_color(s_time_layer, GColorClear);
	text_layer_set_text_color(s_time_layer, GColorBlack);
	//Set default value for text layer
	text_layer_set_text(s_time_layer, "00:00");

}

static void main_window_unload(Window *window){

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
