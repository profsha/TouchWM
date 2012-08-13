#ifndef TOPPANEL_H
#define TOPPANEL_H

#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QDeclarativeProperty>
#include <QGraphicsObject>
#include <QDirIterator>
#include <QDir>
#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <QRegExp>

class TopPanel : public QDeclarativeView
{
    Q_OBJECT
public:
    explicit TopPanel(QWidget *parent = 0);
    void focusOutEvent(QFocusEvent *event);
    
signals:
    void changedFocus();
    void countCategories(int count, QString text, QString picture);
    void addApp(int categorie, QString exec, QString icon, QString name, bool terminal);
    void runProcess(QString path);
    void clearCategories();

    
public slots:
    void scanningApps();
    
};

#endif // PANEL_H
