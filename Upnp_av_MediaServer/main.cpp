#include <QtGui/QApplication>
#include "mediaserverwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MediaServerWindow w;
    w.show();

    return a.exec();
}
