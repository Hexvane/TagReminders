#include <pebble.h>
#include "main.h"

#define SOURCE_FOREGROUND 0
#define SOURCE_BACKGROUND 1

Window *test_window;

TextLayer *test_textLayer;

DictationSession *shake_session;

static char shake_text[512];

bool ifWorkerCalled = false;

void test_window_load(Window *test_window)
{
	test_textLayer = text_layer_create(GRect(0,0,144,180));
	text_layer_set_text(test_textLayer, "hello world");
	text_layer_set_text_color(test_textLayer, GColorBlack);
	layer_add_child(window_get_root_layer(test_window), text_layer_get_layer(test_textLayer));
}

void test_window_unload(Window *test_window)
{
	text_layer_destroy(test_textLayer);
}

void shake_session_callback(DictationSession *session, DictationSessionStatus status, char *transcription, void *context)
{
	if(status == DictationSessionStatusSuccess)
	{
		snprintf(shake_text, sizeof(shake_text), "%s", transcription);
	}
	if(ifWorkerCalled)
	{
		window_stack_pop_all(true);
	}
}

static void accel_tap_handler(AccelAxisType axis, int32_t direction)
{
	ifWorkerCalled = false;
	shake_session = dictation_session_create(sizeof(shake_text), shake_session_callback, NULL);
	dictation_session_start(shake_session);
	text_layer_set_text(test_textLayer, shake_text);
}

static void worker_message_handler(uint16_t type, AppWorkerMessage *message)
{
	if(type == SOURCE_BACKGROUND)
	{
		ifWorkerCalled = true;
		shake_session = dictation_session_create(sizeof(shake_text), shake_session_callback, NULL);
		dictation_session_start(shake_session);
		text_layer_set_text(test_textLayer, shake_text);
	}
}

void app_init()
{
	accel_tap_service_subscribe(accel_tap_handler);
	app_worker_message_subscribe(worker_message_handler);
	app_worker_launch();
}

int main()
{
	test_window = window_create();
	
	app_init();
	
	window_set_window_handlers(test_window, (WindowHandlers){.load = test_window_load, .unload = test_window_unload});
	
	window_stack_push(test_window, true);
	
	app_event_loop();
}