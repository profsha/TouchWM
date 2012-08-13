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
    scanningApps();
}

void TopPanel::focusOutEvent(QFocusEvent *event)
{
    emit changedFocus();
}
struct Shortcut {
    QString name;
    QString exec;
    QString icon;
    QString categorie;
    bool terminal;
    bool nodisplay;
};

void TopPanel::scanningApps()
{
    QDir dir("/usr/share/applications");
    if (!dir.exists()) {
        qDebug()<<"dir not exist";
    }
    QStringList list = dir.entryList(QDir::Files);
    QRegExp rxExec("(Exec=)([^\\n]*)");
    QRegExp rxCategories("(Categories=)([^\\n]*)");
    QRegExp rxIcon("(Icon=)([^\\n]*)");
    QRegExp rxTerminal("(Terminal=)([^\\n]*)");
    QRegExp rxName("(Name=)([^\\n]*)");
    QRegExp rxNoDisplay("(NoDisplay=)([^\\n]*)");

    QHash<QString, int> categories;
    QVector<Shortcut> shortcuts;
    while (!list.isEmpty()) {
        QFile file("/usr/share/applications/"+list.takeLast());
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug()<<file.fileName()<<"can't open";
            continue;
        }
        QTextStream in(&file);
        QString str = in.readAll();
        Shortcut sh;
        if(rxExec.indexIn(str) != -1) {
            sh.exec = rxExec.cap(2);
//            qDebug()<<"Exec="<<rxExec.cap(2);
        };
        if (rxName.indexIn(str) != -1) {
            sh.name = rxName.cap(2);
        }
        if (rxCategories.indexIn(str) != -1) {
            QRegExp rxCat("(.*)(Development|AudioVideo|System|Settings|Utility|Graphics|Network|Game|Office|Education)([^\\n]*)");
            QString cat = rxCategories.cap(2);
            if(rxCat.indexIn(cat) != 1) {
                if(rxCat.cap(2) == "") {
                    sh.categorie = "Other";
                    categories["Other"]=1;
                }
                else {
                    categories[rxCat.cap(2)]=1;
                    sh.categorie = rxCat.cap(2);
//                    qDebug()<<"Categories="<<rxCat.cap(2);
                }
            }
//
        }
        if(rxTerminal.indexIn(str) != -1) {
            if(rxTerminal.cap(2)=="true") {
                sh.terminal = true;
            }
            else {
                sh.terminal = false;
            }
//            qDebug()<<"Terminal="<<rxTerminal.cap(2);
        }
        if(rxIcon.indexIn(str) != -1) {
            if (rxIcon.cap(2).contains(QRegExp(".(xpm|png|svg)$"))) {
                sh.icon = rxIcon.cap(2);
            }
            else {
                QDirIterator iterator("/usr/share/pixmaps/", QDir::Files);
                while (iterator.hasNext()) {
                    iterator.next();
                    if(iterator.fileName() == rxIcon.cap(2)+".png" | iterator.fileName() == rxIcon.cap(2)+".xpm"| iterator.fileName() == rxIcon.cap(2)+".svg") {
                        sh.icon = iterator.fileInfo().absoluteFilePath();
                    }
                }
                if(sh.icon == "") {
                    QDirIterator iterator("/usr/share/icons/", QDir::Files , QDirIterator::Subdirectories);
                    while (iterator.hasNext()) {
                        iterator.next();
                        if(iterator.fileName() == rxIcon.cap(2)+".png" | iterator.fileName() == rxIcon.cap(2)+".xpm" | iterator.fileName() == rxIcon.cap(2)+".svg") {
                            sh.icon = iterator.fileInfo().absoluteFilePath();
                        }
                    }
                }
            }
        }
        sh.nodisplay = false;
        if(rxNoDisplay.indexIn(str) != -1) {
            if(rxNoDisplay.cap(2)=="true") {
                sh.nodisplay = true;
            }
            else {
                sh.nodisplay = false;
            }
        }
        shortcuts.append(sh);
    }
    int countCat = 0;
    emit clearCategories();
    if(categories["Office"]) {

        categories["Office"]=countCat++;
        emit countCategories(categories["Office"],"Office", "images/icons/application_x_executable.png");
    }
    if(categories["Graphics"]) {
        categories["Graphics"]=countCat++;
        emit countCategories(categories["Graphics"],"Graphics", "images/icons/application_x_executable.png");
    }
    if(categories["AudioVideo"]) {
        categories["AudioVideo"]=countCat++;
        emit countCategories(categories["AudioVideo"],"AudioVideo", "images/icons/application_x_executable.png");
    }
    if(categories["Game"]) {
        categories["Game"]=countCat++;
        emit countCategories(categories["Game"],"Game", "images/icons/application_x_executable.png");
    }
    if(categories["Education"]) {
        categories["Education"]=countCat++;
        emit countCategories(categories["Education"],"Education", "images/icons/application_x_executable.png");
    }
    if(categories["Network"]) {
        categories["Network"]=countCat++;
        emit countCategories(categories["Network"],"Network", "images/icons/application_x_executable.png");
    }
    if(categories["Development"]) {
        categories["Development"]=countCat++;
        emit countCategories(categories["Development"],"Development", "images/icons/application_x_executable.png");
    }
    if(categories["System"]) {
        categories["System"]=countCat++;
        emit countCategories(categories["System"],"System", "images/icons/application_x_executable.png");
    }
    if(categories["Utility"]) {
        categories["Utility"]=countCat++;
        emit countCategories(categories["Utility"],"Utility", "images/icons/application_x_executable.png");
    }
    if(categories["Settings"]) {
        categories["Settings"]=countCat++;
        emit countCategories(categories["Settings"],"Settings", "images/icons/application_x_executable.png");
    }
    if(categories["Other"]) {
        categories["Other"]=countCat++;
        emit countCategories(categories["Other"],"Other", "images/icons/application_x_executable.png");
    }
    for (int i=0; i<shortcuts.size(); i++) {
        if (!shortcuts.at(i).nodisplay) {
            if(shortcuts.at(i).icon == "") {
                emit addApp(categories[shortcuts.at(i).categorie], shortcuts.at(i).exec,
                            "images/icons/application_x_executable.png", shortcuts.at(i).name, shortcuts.at(i).terminal);
            }
            else {
                emit addApp(categories[shortcuts.at(i).categorie], shortcuts.at(i).exec,
                            shortcuts.at(i).icon, shortcuts.at(i).name, shortcuts.at(i).terminal);
            }
        }
    }
}
