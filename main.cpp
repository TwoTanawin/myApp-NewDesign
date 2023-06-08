#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.setWindowTitle("My Application by Tanawin"); // Set the new title here

    w.show();
    return a.exec();
}
