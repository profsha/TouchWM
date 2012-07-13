#include "qrunner.h"

QRunner::QRunner(QWidget *parent) :
    QLineEdit(parent)
{
    connect(this,SIGNAL(returnPressed()),this,SLOT(runProcess()));
}

void QRunner::runProcess()
{
    QStringList command = this->text().split(" ");
    proc = new QProcess();
//    proc->start(command.takeFirst(),command, QIODevice::ReadOnly);
//    connect(proc,SIGNAL(finished(int)), this, SLOT(closeProcess()));
    QProcess::startDetached(command.takeFirst(),command);
    this->clear();
}

void QRunner::closeProcess()
{

}
