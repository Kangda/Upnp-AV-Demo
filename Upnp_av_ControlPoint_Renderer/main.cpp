#include <QtGui/QApplication>
#include "controlpointwindow.h"
#include "mediarendererdisplaywindow.h"

#include <HUpnpCore/HUpnp>

int main(int argc, char *argv[])
{
    Herqq::Upnp::SetLoggingLevel(Herqq::Upnp::Debug);

    QApplication a(argc, argv);
    //ControlPointWindow *w = new ControlPointWindow;
    //w->show();
//    MediaRendererDisplayWindow *w = new MediaRendererDisplayWindow;
//    w->show();

    ControlPointWindow w;
    w.show();

    return a.exec();
}
