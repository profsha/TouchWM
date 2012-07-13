#ifndef PANEL_H
#define PANEL_H

#include <QWidget>
#include <QTabBar>

class Panel : public QTabBar
{
    Q_OBJECT
public:
    explicit Panel(QWidget *parent = 0);
    Panel(Shape shape, QWidget *parent = 0);
    QWidget w;
signals:
    
public slots:
    
};

#endif // PANEL_H
