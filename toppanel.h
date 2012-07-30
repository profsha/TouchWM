#ifndef TOPPANEL_H
#define TOPPANEL_H

#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QDeclarativeProperty>
#include <QGraphicsObject>

class TopPanel : public QDeclarativeView
{
    Q_OBJECT
public:
    explicit TopPanel(QWidget *parent = 0);
    void focusOutEvent(QFocusEvent *event);
    
signals:
    void changedFocus();

    
public slots:
    
};

#endif // PANEL_H
