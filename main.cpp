#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.setWindowTitle("ROI Configure"); // Set the new title here

//    // Set the application icon
    QIcon icon(":/image/icon/PVSlogo-Bold-Cropped.png");
    w.setWindowIcon(icon);

    w.show();
    return a.exec();
}
