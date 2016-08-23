#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFont font;
    font.setFamily("Microsoft Yahei UI");
    font.setPointSize(12);
    font.setBold(false);
    a.setFont(font);

    MainWindow w;
    w.show();

    return a.exec();
}
