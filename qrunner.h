#ifndef QRUNNER_H
#define QRUNNER_H

#include <QLineEdit>
#include <QStringList>
#include <QTextEdit>
#include <QTextStream>
#include <QProcess>

class QRunner : public QLineEdit
{
    Q_OBJECT
public:
    explicit QRunner(QWidget *parent = 0);
    QTextEdit output;
    QProcess* proc;
    
signals:
    
public slots:
    void runProcess();
    void closeProcess();
    
};

#endif // QRUNNER_H
