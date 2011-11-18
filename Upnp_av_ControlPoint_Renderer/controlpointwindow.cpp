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
    m_pUi(new Ui::ControlPointWindow),
    m_pControlPoint(),
    m_pModel()
{
    m_pUi->setupUi(this);

    HAvControlPointConfiguration config;

    m_pControlPoint = new HAvControlPoint(config, this);

    Q_ASSERT(
            connect(m_pControlPoint,
                    SIGNAL(mediaServerOnline(Herqq::Upnp::Av::HMediaServerAdapter*)),
                    this,
                    SLOT(mediaServerOnline(Herqq::Upnp::Av::HMediaServerAdapter*))
                    )
            );
    Q_ASSERT(
            connect(m_pControlPoint,
                    SIGNAL(mediaRendererOffline(Herqq::Upnp::Av::HMediaRendererAdapter*)),
                    this,
                    SLOT(mediaServerOffline(Herqq::Upnp::Av::HMediaServerAdapter*))
                    )
            );

    m_pModel = new ControlPointNavigatorModel(this);
    m_pUi->navigatorTreeView->setModel(m_pModel);

    Q_ASSERT(
            m_pControlPoint->init()
            );

}

ControlPointWindow::~ControlPointWindow()
{
    delete m_pUi;
    delete m_pControlPoint;
    delete m_pModel;
}

void ControlPointWindow::mediaServerOnline(Herqq::Upnp::Av::HMediaServerAdapter *deviceAdapter)
{
    HMediaBrowser* browser = m_controlpointNavigator->mediaServerOnline(deviceAdapter);
    bool ok = connect(
            browser,
            SIGNAL(objectsBrowsed(Herqq::Upnp::Av::HMediaBrowser*,QSet<QString>)),
            this,
            SLOT(objectsBrowsed(Herqq::Upnp::Av::HMediaBrowser*,QSet<QString>)));
    Q_ASSERT(ok); Q_UNUSED(ok)

}

void ControlPointWindow::mediaServerOffline(Herqq::Upnp::Av::HMediaServerAdapter *deviceAdapter)
{
    m_pControlPoint->mediaServerOffline(deviceAdapter);
}

