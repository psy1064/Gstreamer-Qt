#include <QDebug>
#include "StreamMgr.h"
#include "gst/video/videooverlay.h"

StreamMgr::StreamMgr(QObject *parent)
    : QObject{parent}
    , pThread(NULL)
{
    Init();

    pThread = new MsgBusCheckThread();

    if ( pThread == NULL || bus == NULL || data == NULL ) { return; }

    connect(pThread, SIGNAL(emit_changeDuration(int)), this, SLOT(slot_getDuration(int)));
    connect(pThread, SIGNAL(emit_currentTime(int)),    this, SLOT(slot_getTime(int)));

    pThread->SetBus(bus);
    pThread->setData(data);
    pThread->start();
}

StreamMgr::~StreamMgr()
{
    if ( pThread->isRunning() ) {
        pThread->quit();
        pThread->wait();
    }

    delete data;
}

void StreamMgr::Init()
{
    /* Initialize GStreamer */
    gst_init (NULL, NULL);

    data = new CustomData;
    if ( data == NULL ) { return; }

    /* Create the elements */
    data->playbin = gst_element_factory_make ("playbin", "playbin");

    if (!data->playbin) {
        g_printerr ("Not all elements could be created.\n");
        return;
    }

    /* Set the URI to play */
    g_object_set (data->playbin, "uri", "https://www.freedesktop.org/software/gstreamer-sdk/data/media/sintel_trailer-480p.webm", NULL);

    /* Listen to the bus */
    bus = gst_element_get_bus (data->playbin);
}

void StreamMgr::slot_getDuration(int nDuration)
{
    emit emit_changeDuration(nDuration);
}

void StreamMgr::slot_getTime(int nTime)
{
    emit emit_currentTime(nTime);
}

void StreamMgr::SetPlayBinState(GstState State)
{
    ret = gst_element_set_state (data->playbin, State);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        g_printerr ("Unable to set the pipeline to the playing state.\n");
        gst_object_unref (data->playbin);
        return;
    }
}

void StreamMgr::Pause()
{
    SetPlayBinState(GST_STATE_PAUSED);
}

void StreamMgr::Play()
{
    SetPlayBinState(GST_STATE_PLAYING);
}

void StreamMgr::Stop()
{
    SetPlayBinState(GST_STATE_READY);
}

void StreamMgr::SetWindowHandle(qint64 nID)
{
    gst_video_overlay_set_window_handle(GST_VIDEO_OVERLAY (data->playbin), static_cast<guintptr>(nID));
}
