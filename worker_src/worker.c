#include <pebble_worker.h>
#include "worker.h"

static void accel_tap_handler(AccelAxisType axis, int32_t direction)
{
	AppWorkerMessage message = {.data0 = 3, .data1 = 4};
	worker_launch_app();
	app_worker_send_message(1, &message);
}

int main()
{	
	accel_tap_service_subscribe(accel_tap_handler);
	
	worker_event_loop();
}