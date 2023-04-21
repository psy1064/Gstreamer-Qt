#include "MsgBusCheckThread.h"
#include <QDebug>
#include <QTime>

MsgBusCheckThread::MsgBusCheckThread()
    : QThread()
    , bus(NULL)
    , msg(NULL)
    , data(NULL)
{
}

void MsgBusCheckThread::run()
{
    if ( bus == NULL ) { return; }

    while (true) {
        msg = gst_bus_timed_pop_filtered (bus, 100 * GST_MSECOND, (GstMessageType)(GST_MESSAGE_STATE_CHANGED | GST_MESSAGE_ERROR | GST_MESSAGE_EOS | GST_MESSAGE_DURATION));

        /* Parse message */
        if (msg != NULL) {
            HandleMessage (data, msg);
        } else {
            /* We got no message, this means the timeout expired */
            gint64 current = -1;

            /* Query the current position of the stream */
            if (gst_element_query_position (data->playbin, GST_FORMAT_TIME, &current)) {
                int nCurrent = gstTimeToSecond(current);
                emit emit_currentTime(nCurrent);
            }

            /* If we didn't know it yet, query the stream duration */
            if (!GST_CLOCK_TIME_IS_VALID (data->duration)) {
                if (gst_element_query_duration (data->playbin, GST_FORMAT_TIME, &data->duration)) {
                    int nDuration = gstTimeToSecond(data->duration);
                    emit emit_changeDuration(nDuration);
                }
            }
        }
    }
}

void MsgBusCheckThread::HandleMessage(CustomData *data, GstMessage *msg)
{
    GError *err;
    gchar *debug_info;

    switch (GST_MESSAGE_TYPE (msg)) {
    case GST_MESSAGE_ERROR:
        gst_message_parse_error (msg, &err, &debug_info);
        g_printerr ("Error received from element %s: %s\n", GST_OBJECT_NAME (msg->src), err->message);
        g_printerr ("Debugging information: %s\n", debug_info ? debug_info : "none");
        g_clear_error (&err);
        g_free (debug_info);
        data->terminate = TRUE;
        break;
    case GST_MESSAGE_EOS:
        g_print ("\nEnd-Of-Stream reached.\n");
        data->terminate = TRUE;
        break;
    case GST_MESSAGE_DURATION:
        /* The duration has changed, mark the current one as invalid */
        data->duration = GST_CLOCK_TIME_NONE;
        break;
    case GST_MESSAGE_STATE_CHANGED: {
        GstState old_state, new_state, pending_state;
        gst_message_parse_state_changed (msg, &old_state, &new_state, &pending_state);
        if (GST_MESSAGE_SRC (msg) == GST_OBJECT (data->playbin)) {
            g_print ("Pipeline state changed from %s to %s:\n",
                     gst_element_state_get_name (old_state), gst_element_state_get_name (new_state));

            /* Remember whether we are in the PLAYING state or not */
            data->playing = (new_state == GST_STATE_PLAYING);

            if (data->playing) {
                /* We just moved to PLAYING. Check if seeking is possible */
                GstQuery *query;
                gint64 start, end;
                query = gst_query_new_seeking (GST_FORMAT_TIME);
                if (gst_element_query (data->playbin, query)) {
                    gst_query_parse_seeking (query, NULL, &data->seek_enabled, &start, &end);
                    if (data->seek_enabled) {
                        g_print ("Seeking is ENABLED from %" GST_TIME_FORMAT " to %" GST_TIME_FORMAT "\n",
                                 GST_TIME_ARGS (start), GST_TIME_ARGS (end));
                    } else {
                        g_print ("Seeking is DISABLED for this stream.\n");
                    }
                }
                else {
                    g_printerr ("Seeking query failed.");
                }
                gst_query_unref (query);
            }
        }
    } break;
    default:
        /* We should not reach here */
        g_printerr ("Unexpected message received.\n");
        break;
    }
    gst_message_unref (msg);
}

int MsgBusCheckThread::gstTimeToSecond(gint64 nTime)
{
    QString sTime = QString::asprintf("%" GST_TIME_FORMAT, GST_TIME_ARGS (nTime));
    sTime = sTime.remove(sTime.indexOf("."),sTime.length()-sTime.indexOf("."));
    QTime tTime = QTime::fromString(sTime,"h:mm:ss");
    int nSecond = QTime(0,0,0).secsTo(tTime);
    return nSecond;
}

void MsgBusCheckThread::SetBus(GstBus *pBus)
{
    bus = pBus;
}

void MsgBusCheckThread::setData(CustomData *pData)
{
    data = pData;
}
