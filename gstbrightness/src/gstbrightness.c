#include "gstbrightness.h"
#include<stdio.h>

GST_DEBUG_CATEGORY_STATIC(gst_brightness_debug);
#define GST_CAT_DEFAULT gst_brightness_debug

enum { PROP_0, PROP_BRIGHTNESS };

#define DEFAULT_BRIGHTNESS 1 //0.0

/* Pad templates */
static GstStaticPadTemplate sink_template = 
    GST_STATIC_PAD_TEMPLATE("sink",
                            GST_PAD_SINK,
                            GST_PAD_ALWAYS,
                            GST_STATIC_CAPS("video/x-raw, format=RGB"));

static GstStaticPadTemplate src_template = 
    GST_STATIC_PAD_TEMPLATE("src",
                            GST_PAD_SRC,
                            GST_PAD_ALWAYS,
                            GST_STATIC_CAPS("video/x-raw, format=RGB"));

/* Define the type */
#define gst_brightness_parent_class parent_class
G_DEFINE_TYPE(GstBrightness, gst_brightness, GST_TYPE_VIDEO_FILTER);

static void gst_brightness_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec);
static void gst_brightness_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec);
static GstFlowReturn gst_brightness_transform_frame_ip(GstVideoFilter *filter, GstVideoFrame *frame);


// /* get notified of caps and plug in the correct process function */
// static gboolean gst_brightness_set_info (GstVideoFilter * vfilter, GstCaps * incaps, GstVideoInfo * in_info, GstCaps * outcaps, GstVideoInfo * out_info)
// {
//   GstBrightness *videobalance = GST_BRIGHTNESS (vfilter);

//   GST_DEBUG_OBJECT (videobalance,
//       "in %" GST_PTR_FORMAT " out %" GST_PTR_FORMAT, incaps, outcaps);

//   switch (GST_VIDEO_INFO_FORMAT (in_info)) {
//     case GST_VIDEO_FORMAT_ARGB:
//     case GST_VIDEO_FORMAT_ABGR:
//     case GST_VIDEO_FORMAT_RGBA:
//     case GST_VIDEO_FORMAT_BGRA:
//     case GST_VIDEO_FORMAT_xRGB:
//     case GST_VIDEO_FORMAT_xBGR:
//     case GST_VIDEO_FORMAT_RGBx:
//     case GST_VIDEO_FORMAT_BGRx:
//     case GST_VIDEO_FORMAT_RGB:
//     case GST_VIDEO_FORMAT_BGR:
//       break;
//     default:
//       if (!gst_video_balance_is_passthrough (videobalance))
//         goto unknown_format;
//       break;
//   }

//   return TRUE;

//   /* ERRORS */
// unknown_format:
//   {
//     GST_ERROR_OBJECT (videobalance, "unknown format %" GST_PTR_FORMAT, incaps);
//     return FALSE;
//   }
// }



// static void gst_brightness_before_transform (GstBaseTransform * base, GstBuffer * buf)
// {
//   GstBrightness *filter = GST_BRIGHTNESS (base);
//   GstClockTime timestamp, stream_time;

//   timestamp = GST_BUFFER_TIMESTAMP (buf);
//   stream_time =
//       gst_segment_to_stream_time (&base->segment, GST_FORMAT_TIME, timestamp);

//   GST_DEBUG_OBJECT (filter, "sync to %" GST_TIME_FORMAT, GST_TIME_ARGS (timestamp));

//   if (GST_CLOCK_TIME_IS_VALID (stream_time))
//     gst_object_sync_values (GST_OBJECT (filter), stream_time);
// }


/* Class initialization */
static void gst_brightness_class_init(GstBrightnessClass *klass) {
    GObjectClass *gobject_class = (GObjectClass *) klass;
    GstBaseTransformClass *trans_class = (GstBaseTransformClass *) klass;
    GstVideoFilterClass *video_filter_class = (GstVideoFilterClass *)klass;

    gobject_class->set_property = gst_brightness_set_property;
    gobject_class->get_property = gst_brightness_get_property;

    g_object_class_install_property(gobject_class, PROP_BRIGHTNESS,
        g_param_spec_double("brightness", "Brightness", "Adjust brightness level",
                            -1.0, 1.0, DEFAULT_BRIGHTNESS, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  /* Set some basic metadata about your new element */
  gst_element_class_set_details_simple (video_filter_class,
    "Video Filter Template", /* FIXME: short name */
    "Filter/Effect/Video",
    "Filters video", /* FIXME: short description*/
    "Name <amiyamaity23@gmail.com>"); /* FIXME: author */

    // trans_class->before_transform = GST_DEBUG_FUNCPTR (gst_brightness_before_transform);
    trans_class->transform_ip_on_passthrough = FALSE;

    // video_filter_class->set_info = GST_DEBUG_FUNCPTR (gst_brightness_set_info);
    video_filter_class->transform_frame_ip = GST_DEBUG_FUNCPTR(gst_brightness_transform_frame_ip);

    // Add pad templates to the element class
    gst_element_class_add_pad_template(GST_ELEMENT_CLASS(klass),
        gst_static_pad_template_get(&sink_template));
    gst_element_class_add_pad_template(GST_ELEMENT_CLASS(klass),
        gst_static_pad_template_get(&src_template));

    // GST_DEBUG_CATEGORY_INIT (gst_brightness_debug, "brightness", 0, "Template brightness");

}

/* Instance initialization */
static void gst_brightness_init(GstBrightness *filter) {
    filter->brightness = DEFAULT_BRIGHTNESS;
}

/* Property functions */
static void gst_brightness_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    GstBrightness *filter = GST_BRIGHTNESS(object);
    switch (prop_id) {
        case PROP_BRIGHTNESS:
            filter->brightness = g_value_get_double(value);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
            break;
    }
}

static void gst_brightness_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    GstBrightness *filter = GST_BRIGHTNESS(object);
    switch (prop_id) {
        case PROP_BRIGHTNESS:
            g_value_set_double(value, filter->brightness);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
            break;
    }
}

/* Frame transformation function */
static GstFlowReturn gst_brightness_transform_frame_ip(GstVideoFilter *filter, GstVideoFrame *frame) {
    GstBrightness *brightness = GST_BRIGHTNESS(filter);
    guint8 *data = GST_VIDEO_FRAME_PLANE_DATA(frame, 0);
    gsize size = GST_VIDEO_FRAME_COMP_PSTRIDE(frame, 0) * GST_VIDEO_FRAME_HEIGHT(frame);

    printf("frame received:%f\n", brightness->brightness);
    for (gsize i = 0; i < size; i++) {
        gint pixel = data[i] + (gint)(brightness->brightness * 255.0);
        data[i] = 0; //CLAMP(pixel, 0, 255);
    }

    return GST_FLOW_OK;
}


/* entry point to initialize the plug-in
 * initialize the plug-in itself
 * register the element factories and other features
 */
static gboolean
brightness_init (GstPlugin *brightness)
{
  return gst_element_register (brightness, "brightness", GST_RANK_NONE, GST_TYPE_BRIGHTNESS);
}


/* Plugin definition */
GST_PLUGIN_DEFINE(
    GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    brightness,
    "Brightness adjustment filter",
    brightness_init,
    "1.0",
    "LGPL",
    "GStreamer",
    "https://gstreamer.freedesktop.org/"
)
