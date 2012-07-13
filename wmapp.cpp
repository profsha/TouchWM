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
    screenGeometry = desktop->screenGeometry(screen);
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

    //create commandLine
    runner.move(wd/100*2.5, ht-ht/100*2.5);
    runner.resize(wd-wd/100*2.5*2,ht/100*2.5);
    runner.show();
    //create right tabs panel
    westPanel = new Panel(QTabBar::RoundedEast);
    connect(westPanel, SIGNAL(tabCloseRequested(int)), this, SLOT(closeClient(int)));
    connect(westPanel, SIGNAL(currentChanged(int)), this, SLOT(chooseClient(int)));
    westPanel->move(0, ht/100*2.5);
    westPanel->resize(wd/100*2.5,ht-ht/100*2.5*2);
    westPanel->show();
    //create left tabs panel
    eastPanel = new Panel(QTabBar::RoundedWest);
    connect(eastPanel, SIGNAL(tabCloseRequested(int)), this, SLOT(closeClient(int)));
    connect(eastPanel, SIGNAL(currentChanged(int)), this, SLOT(chooseClient(int)));
    eastPanel->move(wd-wd/100*2.5, ht/100*2.5);
    eastPanel->resize(wd/100*2.5,ht-ht/100*2.5*2);
    eastPanel->show();
    XSync(dpy, FALSE);
}

QWindow* WMApp::getWindow(Window id)
{

    if(id != runner.winId()
            && id != westPanel->winId()
            && id != eastPanel->winId()) {
    QWindow* w = clients[id];
    if(w) return w;
    w = new QWindow(id);
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
//    case EnterNotify: qDebug()<<"EnterNotify"; return false;
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
            if( !w->dialog)
                if (w->tabIndex < 100) {
                    eastPanel->removeTab(w->tabIndex);
                }
                else {
                    westPanel->removeTab(w->tabIndex-100);
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
                if (clients.size()%2) {
                    w->tabIndex = eastPanel->count();
                    w->currentPanel = eastPanel;
                    eastPanel->addTab(w->wmname);
                }
                else {
                    w->tabIndex = westPanel->count()+100;
                    w->currentPanel = westPanel;
                    westPanel->addTab(w->wmname);
                }
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
                wc.x = wd/100*2.5;
                wc.y = ht/100*2.5;
                wc.height = ht - ht/100*2.5*2;
                wc.width = wd - wd/100*2.5*2;
                XConfigureWindow(dpy,w->id,15,&wc);
            }
            else
            {
                w->dialog = true;
                XWindowAttributes wa;
                XGetWindowAttributes(dpy,wt,&wa);
                XWindowChanges wc;
                wc.x = wd/10;
                wc.y = ht/10;
                wc.height = ht - ht/10*2;
                wc.width = wd - wd/10*2;
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

void WMApp::closeClient(int index)
{
    QTabBar* bar = (QTabBar*)sender();
    QHashIterator<Window, QWindow*> i(clients);
     while (i.hasNext()) {
         i.next();
         if(bar == eastPanel) {
                 if( i.value()->tabIndex == index) {
                     XDestroyWindow(dpy,i.key());
             }
         }
         else {
             if( i.value()->tabIndex == index+100) {
                 XDestroyWindow(dpy,i.key());
             }
         }
     }

}

void WMApp::chooseClient(int index)
{
    QTabBar* bar = (QTabBar*)sender();
    QHashIterator<Window, QWindow*> i(clients);
     while (i.hasNext()) {
         i.next();
         if(bar == eastPanel) {
                 if( i.value()->tabIndex == index) {
                     XRaiseWindow(dpy,i.key());
             }
         }
         else {
             if( i.value()->tabIndex == index+100) {
                 XRaiseWindow(dpy,i.key());
             }
         }
     }
}
