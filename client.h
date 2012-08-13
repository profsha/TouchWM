#ifndef CLIENT_H
#define CLIENT_H

#include <QString>
#include <QIcon>
#include <QX11Info>

#include <X11/Xlib.h>

class Client
{
public:
    Client();
    Client(Window id);
    Window id;
    QString title,wmname,icname;
    QIcon icon;
    QRect geometry;
//    Panel* currentPanel;
    int tabIndex;
    bool dialog;

    void getWMName();
};

#endif // CLIENT_H
