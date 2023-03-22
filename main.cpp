#include "GstreamerPlayer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GstreamerPlayer w;
    w.show();
    return a.exec();
}
