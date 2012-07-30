#ifndef DESKTOP_H
#define DESKTOP_H

#include "toppanel.h"
#include <QDesktopWidget>
#include <QGraphicsObject>
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QDeclarativeProperty>
#include <QRect>
#include <QUrl>
#include <QProcess>

class Desktop : public QDeclarativeView
{
    Q_OBJECT
public:
    Desktop(QDesktopWidget* desktop, QWidget *parent = 0);
    QGraphicsObject* root, *settings;
    TopPanel* panel;
    QRect workflowRect;

protected:

    
signals:
    void setCurrentTab(int index);
    void addClient(QString str, int index);
    void closeClient(int index);
    void chooseClient(int index);
    void turnClient(int index);

public slots:
    QRect getWorkflow();


    void runProcess(QString str);
    
};

#endif // DESKTOP_H
