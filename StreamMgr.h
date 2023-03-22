#ifndef STREAMMGR_H
#define STREAMMGR_H

#include <QObject>
#include <QTimer>

#include "MsgBusCheckThread.h"
#include "Common.h"

class StreamMgr : public QObject
{
    Q_OBJECT
public:
    explicit StreamMgr(QObject *parent = nullptr);
    virtual ~StreamMgr();

    void SetPlayBinState(GstState State);
    void Pause();
    void Play();
    void Stop();

    void SetWindowHandle(qint64 nID);
private:
    void Init();

    MsgBusCheckThread* pThread;

    CustomData* data;
    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn ret;

signals:
    void emit_changeDuration(int);
    void emit_currentTime(int);

private slots:
    void slot_getDuration(int nDuration);
    void slot_getTime(int nTime);
};

#endif // STREAMMGR_H
