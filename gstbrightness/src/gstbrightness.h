#ifndef GST_BRIGHTNESS_H
#define GST_BRIGHTNESS_H

#include <gst/gst.h>
#include <gst/video/gstvideofilter.h>

G_BEGIN_DECLS

// Declare the type and instance cast macros for GstBrightness
#define GST_TYPE_BRIGHTNESS (gst_brightness_get_type())
#define GST_BRIGHTNESS(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), GST_TYPE_BRIGHTNESS, GstBrightness))
#define GST_BRIGHTNESS_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), GST_TYPE_BRIGHTNESS, GstBrightnessClass))
#define GST_IS_BRIGHTNESS(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), GST_TYPE_BRIGHTNESS))
#define GST_IS_BRIGHTNESS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), GST_TYPE_BRIGHTNESS))

#define PACKAGE "brightness"

typedef struct _GstBrightness GstBrightness;
typedef struct _GstBrightnessClass GstBrightnessClass;

struct _GstBrightness {
    GstVideoFilter parent;
    gdouble brightness;  // Brightness adjustment factor
};

struct _GstBrightnessClass {
    GstVideoFilterClass parent_class;
};

GType gst_brightness_get_type(void);

G_END_DECLS

#endif /* GST_BRIGHTNESS_H */
