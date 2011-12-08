#include "mediarenderermanager.h"
#include "mediarendererconnectionmanager.h"

#include <HUpnpAv/HMediaRendererDeviceConfiguration>
#include <HUpnpAv/HAvDeviceModelCreator>
#include <HUpnpAv/HAvControlPoint>
#include <HUpnpAv/HMediaRendererAdapter>
#include <HUpnpAv/HAbstractMediaRendererDevice>
#include <HUpnpAv/HAbstractConnectionManagerService>

#include <HUpnpCore/HDeviceConfiguration>
#include <HUpnpCore/HDeviceHostConfiguration>
#include <HUpnpCore/HServerStateVariable>
#include <HUpnpCore/HDeviceInfo>

#include <QNetworkAccessManager>
#include <QDebug>

using namespace Herqq::Upnp;
using namespace Herqq::Upnp::Av;

/**************************************
 MediaRendererManager
***************************************/

MediaRendererManager::MediaRendererManager(Herqq::Upnp::Av::HAvControlPoint *cp) :
        QObject(),
        m_pDeviceHost(0),
        m_pControlPoint(cp),
        m_pMediaRenderer(0),
        m_pConnectionMgr(new MediaRendererConnectionManager(this)),
        m_pCurWindow(0),
        m_pNetworkMgr(new QNetworkAccessManager(this))
{
    HMediaRendererDeviceConfiguration mediaRendererConfig;
    mediaRendererConfig.setRendererConnectionManager(m_pConnectionMgr, false);

    HAvDeviceModelCreator creator;
    creator.setMediaRendererConfiguration(mediaRendererConfig);

    HDeviceConfiguration config;
    config.setPathToDeviceDescription("./descriptions/herqq_mediarenderer_description.xml");

    HDeviceHostConfiguration hostConfig;
    hostConfig.setDeviceModelCreator(creator);
    hostConfig.add(config);

    m_pDeviceHost = new HDeviceHost(this);
    if (!m_pDeviceHost->init(hostConfig))
    {
        qWarning() << m_pDeviceHost->errorDescription();
        Q_ASSERT(false);
    }

//    bool ok = connect(
//            m_pControlPoint,
//            SIGNAL(mediaRendererOnline(Herqq::Upnp::Av::HMediaRendererAdapter*)),
//            this,
//            SLOT(mediaRendererOnline(Herqq::Upnp::Av::HMediaRendererAdapter*)));
//    Q_ASSERT(ok);
//    Q_UNUSED(ok);

//    HServerStateVariable* currentConnectionIDs =
//            m_pMediaRenderer->connectionManager()->
//            stateVariables().value("CurrentConnectionIDs");

}

MediaRendererManager::~MediaRendererManager()
{
    delete m_pConnectionMgr;
    delete m_pDeviceHost;
    delete m_pNetworkMgr;
}

HMediaRendererAdapter* MediaRendererManager::mediaRendererAdapter() const
{
    return m_pMediaRenderer;
}

void MediaRendererManager::setMediaRendererAdapter(
        Herqq::Upnp::Av::HMediaRendererAdapter *adapter)
{
    m_pMediaRenderer = adapter;
}

void MediaRendererManager::setDisplayWindow(MediaRendererDisplayWindow *win)
{
    m_pCurWindow = win;
}

void MediaRendererManager::mediaRendererOnline(
        Herqq::Upnp::Av::HMediaRendererAdapter *deviceAdapter)
{
    if (deviceAdapter->device()->parentDevice()->
        info().modelDescription().
        compare("Media Renderer Within Control Point", Qt::CaseInsensitive) == 0)
    {
        m_pMediaRenderer = deviceAdapter;
    }
}
