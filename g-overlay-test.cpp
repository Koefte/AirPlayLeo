#include <gst/gst.h>
#include <string>
#include "cfgparse.h"
#include <stdio.h>
#include <stdlib.h>
#include <fmt/format.h>
#include <fmt/core.h>

const char* img_src;
const char* img_videosink;
const char* display_room;
const char* uxplay_params;

void cfginit() {
	ConfigItem configArray[CFG_MAX_CONFIG_ITEMS];

	int configCount = cfg_parsefile("config.conf", configArray, CFG_MAX_CONFIG_ITEMS);

	if(configCount < 0) {
		fprintf(stderr, "Failed to parse configuration file\n");
		return;
	}

	img_src = cfg_get("src", "Background", configArray, configCount);
	display_room = cfg_get("room", "Display", configArray, configCount);
	img_videosink = cfg_get("videosink","Background",configArray,configCount);
	uxplay_params = cfg_get("params","UxPlay",configArray,configCount);


	cfg_free(configArray, configCount);
}

int main(int argc, char *argv[]){
//	system("killall uxplay");
//	system("uxplay -n kys -nh &");

	cfginit();

	GstElement *pipeline;
	GstBus *bus;
	GstMessage *msg;

	gst_init(&argc, &argv);
	const char* command = fmt::format("filesrc location={} ! jpegdec ! videoconvert ! videoscale ! imagefreeze ! fbdevsink",img_src).c_str();
	printf("%s",command);
	//pipeline = gst_parse_launch("playbin uri=\"https://gstreamer.freedesktop.org/data/media/sintel_trailer-480p.webm\" ! textoverlay text=\"KYSKYSKYS\", NULL);
/*	pipeline = gst_parse_launch(
		,
		 NULL);
	gst_element_set_state (pipeline, GST_STATE_PLAYING);
	bus = gst_element_get_bus(pipeline);
	msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE,(GstMessageType) ( GST_MESSAGE_ERROR | GST_MESSAGE_EOS ));

	if(GST_MESSAGE_TYPE (msg) == GST_MESSAGE_ERROR) {
		g_printerr("You should kys Re-run with GST_DEBUG=*:WARN env var set");
	}

	gst_message_unref(msg);
	gst_object_unref(bus);
	gst_element_set_state(pipeline, GST_STATE_NULL);
	gst_object_unref(pipeline);
*/
	return 0;
}
