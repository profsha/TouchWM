#include "wmapp.h"

extern Atom WMApp::wmatom[WMLast];
extern Atom WMApp::netatom[NetLast];


WMApp::WMApp(int &argc, char **argv) :
    QApplication(argc,argv)
{

    if(!setlocale(LC_CTYPE, "") || !XSupportsLocale())
        qDebug("warning: no locale support\n");
    if(!(dpy = QX11Info::display())) {
        qDebug("dwm: cannot open display\n");
        this->quit();
    }
    XSetWindowAttributes wa;

    /* init screen */
    screen = QX11Info::appScreen();
    root = QX11Info::appRootWindow();
    desktop = QApplication::desktop();
    desk = new Desktop(desktop);
    desk->show();

    connect(this,SIGNAL(newClient(QString,int)), desk, SIGNAL(addClient(QString,int)));
    connect(this, SIGNAL(closeClient(int)), desk, SIGNAL(closeClient(int)));
    connect(desk, SIGNAL(turnClient(int)), this, SLOT(turnClient(int)));
    connect(desk, SIGNAL(chooseClient(int)), this, SLOT(raiseClient(int)));

//    desk->repaint();
    screenGeometry = desk->getWorkflow();
    wd = screenGeometry.width();
    ht = screenGeometry.height();

    /* init atoms */
    wmatom[WMProtocols] = XInternAtom(dpy, "WM_PROTOCOLS", False);
    wmatom[WMDelete] = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
    wmatom[WMState] = XInternAtom(dpy, "WM_STATE", False);
    wmatom[WMTakeFocus] = XInternAtom(dpy, "WM_TAKE_FOCUS", False);
    netatom[NetActiveWindow] = XInternAtom(dpy, "_NET_ACTIVE_WINDOW", False);
    netatom[NetSupported] = XInternAtom(dpy, "_NET_SUPPORTED", False);
    netatom[NetWMName] = XInternAtom(dpy, "_NET_WM_NAME", False);
    netatom[NetWMState] = XInternAtom(dpy, "_NET_WM_STATE", False);
    netatom[NetWMFullscreen] = XInternAtom(dpy, "_NET_WM_STATE_FULLSCREEN", False);
    netatom[NetWMWindowType] = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE", False);
    netatom[NetWMWindowTypeDialog] = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_DIALOG", False);
    netatom[NetWMIconName] = XInternAtom(dpy, "_NET_WM_ICON_NAME", FALSE);

    XSelectInput(dpy, root,
        SubstructureNotifyMask|SubstructureRedirectMask|
        StructureNotifyMask|
        ButtonPressMask|
        PropertyChangeMask|
        KeyPressMask|
        ColormapChangeMask|
        EnterWindowMask);


    XSync(dpy, FALSE);
}

QWindow* WMApp::getWindow(Window id)
{

    if(id != desk->winId() || id != desk->panel->winId()) {
    QWindow* w = clients[id];
    if(w) return w;
    w = new QWindow(id, clients.count());
    clients[w->id] = w;
    return w;
    }
    else {
        return 0;
    }
}

bool WMApp::x11EventFilter(XEvent *event)
{
    int e = event->type;



    XTextProperty tp;
    XClassHint ch;
//    case KeyPress: qDebug()<<"KeyPress"; return false;
//    case KeyRelease: qDebug()<<"KeyRelease"; return false;
//    case ButtonPress: qDebug()<<"ButtonPress"; return false;
//    case ButtonRelease: qDebug()<<"ButtonRelease"; return false;
//    case MotionNotify: qDebug()<<"MotionNotify"; return false;;
    if (e == EnterNotify) {
        if (desk && desk->panel && event->xany.window == desk->panel->winId()) {
            XRaiseWindow(dpy, event->xany.window);
        }
        qDebug()<<"EnterNotify"; return false;
    }
//    case LeaveNotify: qDebug()<<"LeaveNotify"; return false;
//    case FocusIn	: qDebug()<<"FocusIn"; return false;
//    case FocusOut: qDebug()<<"FocusOut"; return false;
//    case KeymapNotify: qDebug()<<"KeymapNotify"; return false;
//    case Expose: qDebug()<<"Expose"; return false;
//    case GraphicsExpose	: qDebug()<<"GraphicsExpose"; return false;
//    case NoExpose	: qDebug()<<"NoExpose"; return false;
//    case VisibilityNotify: qDebug()<<"VisibilityNotify"; return false;
    if( e == CreateNotify) {
        qDebug()<<"CreateNotify";
        return false;
    }
    if ( e == DestroyNotify) {
        qDebug()<<"DestroyNotify";
        QWindow* w = getWindow(event->xdestroywindow.window);
        if (w) {
            if( !w->dialog) {
                emit closeClient(w->tabIndex);
            }
            clients.remove(w->id);
            return true;
        }
        if(event->xdestroywindow.event != event->xdestroywindow.window)
            return true;
        return false;
    }
    if ( e == UnmapNotify) {
        qDebug()<<"UnmapNotify";
        return false;
    }
    if ( e == MapNotify) {
        qDebug()<<"MapNotify";
        return false;
    }
    if ( e == MapRequest) {
        qDebug()<<"MapRequest";
        QWindow* w = getWindow(event->xmap.window);
        if (w) {
            if(!w->dialog) {
                w->getWMName();
                emit newClient(w->wmname,clients.count());
            }
            XMapWindow(dpy, w->id);
            currentClient = w;
            return true;
        }
        else {
            return false;
        }
    }
    if ( e == ReparentNotify) {
        qDebug()<<"ReparentNotify";
        return false;
    }
    if ( e == ConfigureNotify) {
        qDebug()<<"ConfigureNotify";
        return false;
    }
    if ( e == ConfigureRequest) {
        qDebug()<<"ConfigureRequest";
        QWindow* w = getWindow(event->xconfigurerequest.window);
        if(w) {
            Window wt;
            if(!XGetTransientForHint(dpy,w->id,&wt)) {
                XWindowChanges wc;
                wc.x = screenGeometry.x();
                wc.y = screenGeometry.y();
                wc.height = screenGeometry.height();
                wc.width = screenGeometry.width();
                XConfigureWindow(dpy,w->id,15,&wc);
            }
            else
            {
                w->dialog = true;
                XWindowAttributes wa;
                XGetWindowAttributes(dpy,wt,&wa);
                XWindowChanges wc;
                wc.x = screenGeometry.x();
                wc.y = screenGeometry.y();
                wc.height = screenGeometry.height();
                wc.width = screenGeometry.width();
                XConfigureWindow(dpy,w->id,15,&wc);
            }
            return true;
        }
        else {
            return false;
        }
    }
//    if ( e == GravityNotify) { qDebug()<<"GravityNotify"; return false;
    if ( e == ResizeRequest) {
        qDebug()<<"ResizeRequest";
        return false;
    }
//    if ( e == CirculateNotify) { qDebug()<<"CirculateNotify"; return false;
//    if ( e == CirculateRequest) { qDebug()<<"CirculateRequest"; return false;
    if ( e == PropertyNotify) {
        qDebug()<<"PropertyNotify";
        return false;
    }
//    if ( e == SelectionClear) { qDebug()<<" SelectionClear"; return false;
//    if ( e == SelectionRequest) { qDebug()<<"SelectionRequest"; return false;
//    if ( e == SelectionNotify) { qDebug()<<"SelectionNotify"; return false;
//    if ( e == ColormapNotify) { qDebug()<<"ColormapNotify"; return false;
    if ( e == ClientMessage) {
        qDebug()<<"ClientMessage";
        return false;
    }
//    if ( e == MappingNotify) { qDebug()<<"MappingNotify"; return false;
//    if ( e == GenericEvent	) { qDebug()<<"GenericEvent"; return false;
//    if ( e == LASTEvent) { qDebug()<<"LASTEvent"; return false;
    return false;
}

void WMApp::raiseClient(int index)
{
    QHashIterator<Window,QWindow*> i(clients);
     while (i.hasNext()) {
         i.next();
         if (i.value()->tabIndex == index) {
            turnClient( currentClient->tabIndex);
             XMapWindow(dpy, i.key());
             currentClient = i.value();
         }
     }
}

void WMApp::turnClient(int index)
{
    QHashIterator<Window,QWindow*> i(clients);
     while (i.hasNext()) {
         i.next();
         if (i.value()->tabIndex == index) {
             XUnmapWindow(dpy, i.key());
         }
     }
}


