#ifndef COMMON_H
#define COMMON_H

#include "gst/gst.h"

typedef struct _CustomData {
    GstElement *playbin;  /* Our one and only element */
    gboolean playing;      /* Are we in the PLAYING state? */
    gboolean terminate;    /* Should we terminate execution? */
    gboolean seek_enabled; /* Is seeking enabled for this media? */
    gboolean seek_done;    /* Have we performed the seek already? */
    gint64 duration;       /* How long does this media last, in nanoseconds */

    _CustomData() {
        playbin = NULL;
        playing = FALSE;
        terminate = FALSE;
        seek_enabled = FALSE;
        seek_done = FALSE;
        duration = GST_CLOCK_TIME_NONE;
    }
} CustomData;

#endif // COMMON_H
