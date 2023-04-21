#ifndef GSTREAMERPLAYER_H
#define GSTREAMERPLAYER_H

#include <QMainWindow>
#include "StreamMgr.h"

QT_BEGIN_NAMESPACE
namespace Ui { class GstreamerPlayer; }
QT_END_NAMESPACE

class GstreamerPlayer : public QMainWindow
{
    Q_OBJECT

public:
    explicit GstreamerPlayer(QWidget *parent = nullptr);
    ~GstreamerPlayer();

private:
    Ui::GstreamerPlayer *ui;
    StreamMgr* pMgr;

private slots:
    void Slot_Play();
    void Slot_Pause();
    void Slot_Stop();
    void Slot_StreamDuration(int nDuration);
    void Slot_CurrentTime(int nTime);
};
#endif // GSTREAMERPLAYER_H
