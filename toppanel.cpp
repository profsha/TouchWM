#include "toppanel.h"
#include <QDebug>

TopPanel::TopPanel(QWidget *parent) :
    QDeclarativeView(parent)
{
    setWindowFlags(Qt::X11BypassWindowManagerHint | Qt::FramelessWindowHint| Qt::WindowStaysOnTopHint);
    rootContext()->setContextProperty("toppanel", this);
    setSource(QUrl::fromLocalFile("Panel.qml"));

    setFocusPolicy(Qt::StrongFocus);
    setResizeMode(QDeclarativeView::SizeViewToRootObject);
}

void TopPanel::focusOutEvent(QFocusEvent *event)
{
    emit changedFocus();
}
