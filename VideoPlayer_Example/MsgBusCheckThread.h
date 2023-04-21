#ifndef MSGBUSCHECKTHREAD_H
#define MSGBUSCHECKTHREAD_H

#include <QThread>
#include "Common.h"

class MsgBusCheckThread : public QThread
{
    Q_OBJECT
public:
    MsgBusCheckThread();
    virtual void run();
    void SetBus(GstBus* pBus);
    void setData(CustomData* pData);
    void HandleMessage(CustomData *data, GstMessage *msg);
    int gstTimeToSecond(gint64 nTime);

private:
    GstBus *bus;
    GstMessage *msg;
    GstElement* playBin;
    CustomData* data;

signals:
    void emit_currentTime(int);
    void emit_changeDuration(int);
};

#endif // MSGBUSCHECKTHREAD_H
