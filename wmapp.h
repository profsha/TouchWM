#ifndef WMAPP_H
#define WMAPP_H

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
#include "qwindow.h"
#include "qrunner.h"
#include "panel.h"

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
    int screen;
    Window root;
    QRect screenGeometry;
    int wd, ht;
    QDesktopWidget* desktop;
static    Atom wmatom[WMLast], netatom[NetLast];
    QWindow* getWindow(Window id);
    QHash<Window,QWindow*> clients;
    QRunner runner;
    Panel* westPanel, *eastPanel;
    QWindow *currentClient;



protected:
    virtual bool x11EventFilter(XEvent *event);
    
signals:
    
public slots:
    void closeClient(int index);
    void chooseClient(int index);
    
};

#endif // WMAPP_H
