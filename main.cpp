#include <QProcess>
#include "wmapp.h"


int main(int argc, char *argv[])
{
    QProcess::startDetached("killall -q Xephyr -s SIGKILL");
    sleep(1);
    QProcess::startDetached("Xephyr :1 -screen 1024x768");
    putenv((char*)"DISPLAY=:1");
    sleep(1);
    WMApp a(argc, argv);
    QApplication::setOrganizationName("Qt");
    QApplication::setApplicationName("QTouchWM");

    QStringList args = a.arguments(); args.removeFirst();
    foreach(QString arg, args ) QProcess::startDetached(arg);

    return a.exec();
}
