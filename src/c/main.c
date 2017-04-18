#include <pebble.h>

// pointer to main window
static Window *s_main_window;

// pointer to main window layer
static Layer *s_main_window_layer;

// pointer to canvas layer
static Layer *s_canvas_layer;

// variables to help with screen positioning
static uint8_t shortest_side;
static GPoint center;

// integers to hold the radial segment sizes
static uint8_t outer_segment_size;
static uint8_t inner_segment_size;

// GRects to hold the Radial bounds
static GRect second_bounds;
static GRect minute_bounds;
static GRect hour_bounds;

// GRects to hold the Text bounds
static GRect second_text_bounds;
static GRect minute_text_bounds;
static GRect hour_text_bounds;
static GRect date_text_bounds;

// integers to help with text positioning
static uint8_t text_offset = 6;
static uint8_t text_height = 20;
static uint8_t date_text_height = 14;

// GColor to hold the radial and text colors
static GColor second_color;
static GColor minute_color;
static GColor hour_color;
static GColor second_text_color;
static GColor date_text_color;


// Function to update the bound sizes when the unobstructed area changes
void prv_unobstructed_change(AnimationProgress progress, void *context) {
  
  // Get the full screen bounds
  GRect bounds = layer_get_frame(s_main_window_layer);
  
  // Get the unobstructed screen bounds
  GRect unobstructed_bounds = layer_get_unobstructed_bounds(s_main_window_layer);
  
  // Calculate the offset based on half of the difference of the bounds
  uint8_t offset = (bounds.size.h - unobstructed_bounds.size.h)/2;
  
  // Adjust the y position of the bounds
  bounds.origin.y -= offset;
    
  // Calculate the shortest side
  shortest_side = bounds.size.w < bounds.size.h ? bounds.size.w : bounds.size.h;
  
  // Calculate the center point
  center = GPoint(bounds.origin.x + (bounds.size.w/2), bounds.origin.y + (bounds.size.h/2));

  // Calculate the outer segment size relative to the sortest side
  outer_segment_size = shortest_side / 14;
  
  // Make the inner segment 2/3 of the outer segment size
  inner_segment_size = outer_segment_size * 2 / 3;

  // Calculate the diameter of the inner dials
  uint8_t dial_diameter = ((shortest_side/2)-outer_segment_size-2);
  
  // Ensure the diameter is an even number
  if (dial_diameter%2){ dial_diameter -= 1;}
  
  // Set the bounds for the second radials
  second_bounds = GRect(center.x-(shortest_side/2), center.y-(shortest_side/2), shortest_side, shortest_side);

  // Set the bounds for the minute radials
  minute_bounds = GRect(center.x+1, center.y-(dial_diameter/2), dial_diameter, dial_diameter);
  
  // Set the bounds for the hour radials
  hour_bounds = GRect(center.x-1-dial_diameter, center.y-(dial_diameter/2), dial_diameter, dial_diameter);
  
  // Set the bounds for the second text
  second_text_bounds = GRect(bounds.origin.x, center.y-((shortest_side/2)-(dial_diameter/2))-text_offset-(text_height/2), bounds.size.w, text_height);
  
  // Set the bounds for the minute text
  minute_text_bounds = GRect(minute_bounds.origin.x, center.y-text_offset-(text_height/2), minute_bounds.size.w, text_height);
  
  // Set the bounds for the hour text
  hour_text_bounds = GRect(hour_bounds.origin.x, center.y-text_offset-(text_height/2), hour_bounds.size.w, text_height);
  
  // Set the bounds for the date text
  date_text_bounds = GRect(bounds.origin.x, center.y-text_offset-(date_text_height/2)+((shortest_side/2)-(dial_diameter/2)), bounds.size.w, text_height);
  
}


// function to redraw the watch
static void canvas_update_proc(Layer *this_layer, GContext *ctx) {
  
  // Get the current time
	time_t now = time(NULL);
	struct tm *tick_time = localtime(&now);
  
  // Setup character buffers with the correct size for the text
  static char seconds[] = "00";
  static char minutes[] = "00";
  static char hours[] = "00";
  static char date[] = "DD/MM";
  
  // Convert each time element to text and store in the character buffer
  strftime(seconds, sizeof(seconds), "%S", tick_time);
  strftime(minutes, sizeof(minutes), "%M", tick_time);
  strftime(hours, sizeof(hours), "%H", tick_time);
  strftime(date, sizeof(date), "%d/%m", tick_time);

  // Integer for the radial loops
  uint8_t i;
  
  // GColor to hold the color to draw the radial
  GColor tick_color;
  
  // Seconds
  for (i = 0; i<59; i++){
        
    // Set the color to draw the radial depending if the second has passed
    tick_color = i < tick_time->tm_sec ? second_color : GColorDarkGray;
    graphics_context_set_fill_color(ctx, tick_color);
    
    // Draw the second radial
    graphics_fill_radial(ctx, second_bounds, GOvalScaleModeFitCircle, outer_segment_size, DEG_TO_TRIGANGLE(6.1 * i), DEG_TO_TRIGANGLE((6.1 * i)+4.4));
    
  }

  // Set the second text color
  graphics_context_set_text_color(ctx, second_text_color);
  
  // Draw the text to represent the seconds
  graphics_draw_text(ctx, seconds, fonts_get_system_font(FONT_KEY_LECO_26_BOLD_NUMBERS_AM_PM), second_text_bounds,
                     GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);

  // Minutes
  for (i = 0; i<59; i++){

    // Set the color to draw the radial depending if the minute has passed
    tick_color = i < tick_time->tm_min ? minute_color : GColorDarkGray;
    graphics_context_set_fill_color(ctx, tick_color);
    
    // Draw the minute radial
    graphics_fill_radial(ctx, minute_bounds, GOvalScaleModeFitCircle, inner_segment_size, DEG_TO_TRIGANGLE(6.1 * i), DEG_TO_TRIGANGLE((6.1 * i)+4));
  
  }
  
  // Set the minute text color
  graphics_context_set_text_color(ctx, PBL_IF_COLOR_ELSE(GColorYellow, GColorWhite));
  
  // Draw the text to represent the minutes
  graphics_draw_text(ctx, minutes, fonts_get_system_font(FONT_KEY_LECO_26_BOLD_NUMBERS_AM_PM), minute_text_bounds,
                     GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);

  // Hours
  for (i = 0; i<24; i++){
     
    // Set the color to draw the radial depending if the hour has passed
    tick_color = i < tick_time->tm_hour ? hour_color : GColorDarkGray;
    graphics_context_set_fill_color(ctx, tick_color);
    
    // Draw the hour radial
    graphics_fill_radial(ctx, hour_bounds, GOvalScaleModeFitCircle, inner_segment_size, DEG_TO_TRIGANGLE(i*360/24), DEG_TO_TRIGANGLE((i*360/24)+11));
    
  }

  // Set the hour text color
  graphics_context_set_text_color(ctx, PBL_IF_COLOR_ELSE(GColorRed, GColorWhite));
  
  // Draw the text to represent the hours
  graphics_draw_text(ctx, hours, fonts_get_system_font(FONT_KEY_LECO_26_BOLD_NUMBERS_AM_PM), hour_text_bounds,
                     GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);

  // Date
  // Set the date text color
  graphics_context_set_text_color(ctx, date_text_color);
  
  // Draw the text to represent the date
  graphics_draw_text(ctx, date, fonts_get_system_font(FONT_KEY_LECO_20_BOLD_NUMBERS), date_text_bounds,
                     GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
  
}


// Tick handler to force update of watchface
static void tick_handler(struct tm *tick_time, TimeUnits units_changed){

	// Force canvas layer to redraw
	layer_mark_dirty(s_canvas_layer);

}




static void main_window_load(Window *window) {

  second_color = PBL_IF_COLOR_ELSE(GColorPictonBlue, GColorWhite);
  minute_color = PBL_IF_COLOR_ELSE(GColorYellow, GColorWhite);
  hour_color = PBL_IF_COLOR_ELSE(GColorRed, GColorWhite);
  second_text_color = PBL_IF_COLOR_ELSE(GColorDarkGray, GColorWhite);
  date_text_color = GColorWhite;

	// get the main window layer
	s_main_window_layer = window_get_root_layer(s_main_window);

	// set the main window background color
	window_set_background_color(s_main_window, GColorBlack);

	// Create the layer we will draw on
	s_canvas_layer = layer_create(layer_get_bounds(s_main_window_layer));

	// Set the update procedure for our layer
	layer_set_update_proc(s_canvas_layer, canvas_update_proc);

	// Add the layer to our main window layer
	layer_add_child(s_main_window_layer, s_canvas_layer);  

  // Call the unobstructed bounds handler to set the initial positions
  prv_unobstructed_change(100, NULL);
  
  // Quick View
  #if PBL_API_EXISTS(unobstructed_area_service_subscribe)
      
    // Subscribe to the unobstructed area events
    UnobstructedAreaHandlers handlers = {
      .change = prv_unobstructed_change
    };
    
    unobstructed_area_service_subscribe(handlers, NULL);

  #endif

  layer_mark_dirty(s_canvas_layer);
  
	// Subscribe to event services
	tick_timer_service_subscribe(SECOND_UNIT, tick_handler);

}


static void main_window_unload(Window *window) {

  // Unsubscribe from event services
	tick_timer_service_unsubscribe();
	
}


static void init(void) {

	// Create the main window
	s_main_window = window_create();

	// set the window load and unload handlers
	window_set_window_handlers(s_main_window, (WindowHandlers) {
		.load = main_window_load,
		.unload = main_window_unload,
	});

	// show the window on screen
	window_stack_push(s_main_window, true);

}


static void deinit(void) {

	// Destroy the main window
	window_destroy(s_main_window);

}


int main(void) {

	init();
	app_event_loop();
	deinit();

}