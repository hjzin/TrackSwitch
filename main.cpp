#include "trackswitch.h"
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TrackSwitch t;
    t.show();
    return a.exec();
}
