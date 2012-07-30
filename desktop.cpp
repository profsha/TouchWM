#include "desktop.h"
#include <QDebug>

Desktop::Desktop(QDesktopWidget* desktop,QWidget *parent) :
    QDeclarativeView(parent)
{
    setFocusPolicy(Qt::StrongFocus);
      // Изменять размеры QML объекта под размеры окна
    setResizeMode(QDeclarativeView::SizeRootObjectToView);

    QRect geom = desktop->screenGeometry();
rootContext()->setContextProperty("window", this);
    setWindowFlags(Qt::X11BypassWindowManagerHint | Qt::FramelessWindowHint);
    setSource(QUrl::fromLocalFile("desktop.qml"));




    root = rootObject();
    settings = root->findChild<QGraphicsObject*>("settings");
setGeometry(geom);





//    for (int i=0; i<20;i++)
//    emit addClient("testClient",i);

}

QRect Desktop::getWorkflow()
{
    panel = new TopPanel;
    panel->show();
    QDeclarativeProperty::write((QObject*)settings,"rootHeight", height());
    QDeclarativeProperty::write((QObject*)settings,"rootWidth", width());
    QDeclarativeProperty::write((QObject*)settings,"panelMaximalHeight", height());
    QObject* work = panel->rootObject();
    QDeclarativeProperty::write((QObject*)work, "width",width());
    QObject* workflow = root->findChild<QObject*>("workflow");
    workflowRect.setX(QDeclarativeProperty::read((QObject*)workflow, "x").toInt());
    workflowRect.setY(QDeclarativeProperty::read((QObject*)workflow, "y").toInt());
    workflowRect.setHeight(QDeclarativeProperty::read((QObject*)workflow, "height").toInt());
    workflowRect.setWidth(QDeclarativeProperty::read((QObject*)workflow, "width").toInt());
    return workflowRect;
}

//void Desktop::chooseClient(int index)
//{
//}

void Desktop::runProcess(QString str)
{
    QStringList command = str.split(" ");
//    proc = new QProcess();
//    proc->start(command.takeFirst(),command, QIODevice::ReadOnly);
//    connect(proc,SIGNAL(finished(int)), this, SLOT(closeProcess()));
    QProcess::startDetached(command.takeFirst(),command);
}
