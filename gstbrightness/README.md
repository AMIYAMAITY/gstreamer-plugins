<h1> Custom gstreamer plugin </h1>


<h2> How to build </h2>

`make`

<h2> How to run </h2>

`export GST_PLUGIN_PATH=$PWD`

`gst-launch-1.0 filesrc location=sample-15s.mp4 ! decodebin ! videoconvert ! video/x-raw,format=RGB ! brightness brightness=1.0 ! videoconvert ! vp8enc ! webmmux ! filesink location=capture1.webm`


