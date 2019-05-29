#include "mainwidget.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>

QT_CHARTS_USE_NAMESPACE

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QMainWindow window;
    //MainWindow window;

    MainWidget widget;

    window.setCentralWidget(&widget);
    window.resize(1600, 900);
    window.show();
    return a.exec();
}
