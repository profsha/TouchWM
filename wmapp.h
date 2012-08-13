#ifndef WMAPP_H
#define WMAPP_H


#include "desktop.h"
#include "toppanel.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QX11Info>
#include <QRect>
#include <QHash>
#include <locale.h>
#include <QDebug>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xproto.h>
#include <X11/Xutil.h>

#include "client.h"

enum { NetSupported, NetWMName, NetWMState,
       NetWMFullscreen, NetActiveWindow, NetWMWindowType,
       NetWMWindowTypeDialog,NetWMIconName, NetLast };     /* EWMH atoms */
enum { WMProtocols, WMDelete, WMState, WMTakeFocus, WMLast }; /* default atoms */

class WMApp : public QApplication
{
    Q_OBJECT
public:
    explicit WMApp(int & argc, char ** argv);
    Display *dpy;
    int countTabs;
    int screen;
    Window root;
    QRect screenGeometry;
    int wd, ht;
    QDesktopWidget* desktop;
    Desktop* desk;
static    Atom wmatom[WMLast], netatom[NetLast];

    QHash<Window,Client*> clients;
    TopPanel* panel;
    Client *currentClient;

    Client* getClient(Window id);

protected:
    virtual bool x11EventFilter(XEvent *event);
    
signals:
    void newClient(QString, int);
    void closeClient(int);
    
public slots:
    void mapClient(int index);
    void unmapClient(int index);
    void chooseClient(int index);

    
};

#endif // WMAPP_H
