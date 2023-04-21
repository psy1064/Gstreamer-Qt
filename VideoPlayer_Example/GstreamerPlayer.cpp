#include "GstreamerPlayer.h"
#include "ui_GstreamerPlayer.h"
#include <QDebug>
#include <QTime>

GstreamerPlayer::GstreamerPlayer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GstreamerPlayer)
    , pMgr(NULL)
{
    ui->setupUi(this);

    pMgr = new StreamMgr(this);

    connect(ui->btnPlay,    SIGNAL(clicked(bool)),            this, SLOT(Slot_Play()));
    connect(ui->btnPause,   SIGNAL(clicked(bool)),            this, SLOT(Slot_Pause()));
    connect(ui->btnStop,    SIGNAL(clicked(bool)),            this, SLOT(Slot_Stop()));
    connect(pMgr,           SIGNAL(emit_changeDuration(int)), this, SLOT(Slot_StreamDuration(int)));
    connect(pMgr,           SIGNAL(emit_currentTime(int)),    this, SLOT(Slot_CurrentTime(int)));
}

GstreamerPlayer::~GstreamerPlayer()
{
    delete ui;
    delete pMgr;
}

void GstreamerPlayer::Slot_Play()
{
    if ( pMgr == NULL ) { return; }
    pMgr->Play();
    pMgr->SetWindowHandle(ui->frame->winId());
}

void GstreamerPlayer::Slot_Pause()
{
    if ( pMgr == NULL ) { return; }
    pMgr->Pause();
}

void GstreamerPlayer::Slot_Stop()
{
    if ( pMgr == NULL ) { return; }
    pMgr->Stop();
}

void GstreamerPlayer::Slot_StreamDuration(int nDuration)
{
    ui->bar->setRange(0, nDuration);
    QTime time(0,0,0);
    time = time.addSecs(nDuration);
    ui->lbDuration->setText(time.toString());
}

void GstreamerPlayer::Slot_CurrentTime(int nTime)
{
    ui->bar->setValue(nTime);
    QTime time(0,0,0);
    time = time.addSecs(nTime);
    ui->lbCurrent->setText(time.toString());
}
