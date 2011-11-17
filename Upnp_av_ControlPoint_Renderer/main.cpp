#include <QtGui/QApplication>
#include "controlpointwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ControlPointWindow w;
    w.show();

    return a.exec();
}
