#include "mainwindow.h"
#include "windows.h"
#include <QApplication>
#include <QSharedPointer>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFont font;
    font.setFamily("Microsoft Yahei UI");
    font.setPointSize(9);
    font.setBold(false);
    a.setFont(font);

    QString str("10");
    QSharedPointer<QString> pointer(&str);
    qDebug() << pointer->append("10");

    MainWindow w;
    w.show();

    return a.exec();
}

