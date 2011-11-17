#include "controlpointwindow.h"
#include "ui_controlpointwindow.h"

#include <QStandardItem>

#include <HUpnpCore/HDeviceInfo>

#include <HUpnpAv/HContainer>
#include <HUpnpAv/HMediaBrowser>
#include <HUpnpAv/HAvControlPoint>
#include <HUpnpAv/HMediaServerAdapter>
#include <HUpnpAv/HAvControlPointConfiguration>

using namespace Herqq::Upnp;
using namespace Herqq::Upnp::Av;

ControlPointWindow::ControlPointWindow(QWidget *parent) :
    QMainWindow(parent),
    m_pUi(new Ui::ControlPointWindow)
{
    m_pUi->setupUi(this);

    HAvControlPointConfiguration config;

    m_controlPoint = new HAvControlPoint(config, this);

    Q_ASSERT(
            connect(m_controlPoint,
                    SIGNAL(mediaServerOnline(Herqq::Upnp::Av::HMediaServerAdapter*)),
                    this,
                    SLOT(mediaServerOnline(Herqq::Upnp::Av::HMediaServerAdapter*))
                    )
            );
    Q_ASSERT(
            connect(m_controlPoint,
                    SIGNAL(mediaRendererOffline(Herqq::Upnp::Av::HMediaRendererAdapter*)),
                    this,
                    SLOT(mediaServerOffline(Herqq::Upnp::Av::HMediaServerAdapter*))
                    )
            );



}

ControlPointWindow::~ControlPointWindow()
{
    delete m_pUi;
}

void ControlPointWindow::mediaServerOnline(Herqq::Upnp::Av::HMediaServerAdapter *adapter)
{
}

void ControlPointWindow::mediaServerOffline(Herqq::Upnp::Av::HMediaServerAdapter *adapter)
{
}
