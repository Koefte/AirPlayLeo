#include <gst/gst.h>

#include <stdlib.h>

int main(int argc, char *argv[]){
system("killall uxplay");	
system("uxplay -n kys -nh &");
	
	GstElement *pipeline;
	GstBus *bus;
	GstMessage *msg;
	
	gst_init(&argc, &argv);

	//pipeline = gst_parse_launch("playbin uri=\"https://gstreamer.freedesktop.org/data/media/sintel_trailer-480p.webm\" ! textoverlay text=\"KYSKYSKYS\", NULL);
	pipeline = gst_parse_launch("filesrc location=miami.jpg ! jpegdec ! videoconvert ! videoscale ! imagefreeze ! fbdevsink", NULL);
	gst_element_set_state (pipeline, GST_STATE_PLAYING);
	bus = gst_element_get_bus(pipeline);
	msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

	if(GST_MESSAGE_TYPE (msg) == GST_MESSAGE_ERROR) {
		g_printerr("You should kys Re-run with GST_DEBUG=*:WARN env var set");
	}

	gst_message_unref(msg);
	gst_object_unref(bus);
	gst_element_set_state(pipeline, GST_STATE_NULL);
	gst_object_unref(pipeline);

	return 0;
}
