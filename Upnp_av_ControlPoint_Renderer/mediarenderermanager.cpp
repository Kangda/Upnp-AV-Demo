#include "mediarenderermanager.h"
#include "mediarendererconnectionmanager.h"

#include <HUpnpAv/HMediaRendererDeviceConfiguration>
#include <HUpnpAv/HAvDeviceModelCreator>
#include <HUpnpAv/HAbstractMediaRendererDevice>
#include <HUpnpAv/HAbstractConnectionManagerService>

#include <HUpnpCore/HDeviceConfiguration>
#include <HUpnpCore/HDeviceHostConfiguration>
#include <HUpnpCore/HServerStateVariable>

#include <QNetworkAccessManager>
#include <QDebug>

using namespace Herqq::Upnp;
using namespace Herqq::Upnp::Av;

/**************************************
 MediaRendererManager
***************************************/

MediaRendererManager::MediaRendererManager() :
        m_pConnectionMgr(new MediaRendererConnectionManager(this)),
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

    m_pMediaRenderer =
            qobject_cast<HAbstractMediaRendererDevice*>(m_pDeviceHost->rootDevices().at(0));

//    HServerStateVariable* currentConnectionIDs =
//            m_pMediaRenderer->connectionManager()->
//            stateVariables().value("CurrentConnectionIDs");

}

MediaRendererManager::~MediaRendererManager()
{
    delete m_pConnectionMgr;
    delete m_pDeviceHost;
    delete m_pMediaRenderer;
    delete m_pNetworkMgr;
}

HAbstractMediaRendererDevice* MediaRendererManager::mediaRenderer() const
{
    return m_pMediaRenderer;
}
