#include "mediarenderermanager.h"
#include "mediarendererconnectionmanager.h"
#include "mediarendererdisplaywindow.h"

#include <HUpnpAv/HMediaRendererDeviceConfiguration>
#include <HUpnpAv/HAvDeviceModelCreator>
#include <HUpnpAv/HAvControlPoint>
#include <HUpnpAv/HConnection>
#include <HUpnpAv/HConnectionInfo>
#include <HUpnpAv/HMediaRendererAdapter>
#include <HUpnpAv/HAbstractMediaRendererDevice>
#include <HUpnpAv/HAbstractConnectionManagerService>

#include <HUpnpCore/HDeviceConfiguration>
#include <HUpnpCore/HDeviceHostConfiguration>
#include <HUpnpCore/HServerStateVariable>
#include <HUpnpCore/HDeviceInfo>

#include <QNetworkAccessManager>
#include <QDebug>
#include <QMessageBox>

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
        m_displayWindows(),
//        m_pCurWindow(0),
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

    m_pDeviceHost = new HDeviceHost();
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

    foreach(MediaRendererDisplayWindow* win, m_displayWindows)
    {
        if (win)
            delete win;
    }
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
    Q_UNUSED(win);
//    m_pCurWindow = win;
}

void MediaRendererManager::newDisplayWindow(Herqq::Upnp::Av::HAvControlPoint *cp,
                                            Herqq::Upnp::Av::HMediaRendererAdapter *ad,
                                            Herqq::Upnp::Av::HItem *item,
                                            Herqq::Upnp::HUdn udn,
                                            QWidget *parent)
{
    MediaRendererDisplayWindow* itemDisplay =
            new MediaRendererDisplayWindow(cp, ad, item, udn,parent);

    m_displayWindows.insert(itemDisplay);

    bool ok = connect(
            itemDisplay,
            SIGNAL(close(MediaRendererDisplayWindow*)),
            this,
            SLOT(closeDisplayWindow(MediaRendererDisplayWindow*)));
    Q_ASSERT(ok);

    ok = connect(
            m_pControlPoint,
            SIGNAL(mediaServerOffline(Herqq::Upnp::Av::HMediaServerAdapter*)),
            itemDisplay,
            SLOT(mediaServerOffline(Herqq::Upnp::Av::HMediaServerAdapter*)));
    Q_ASSERT(ok);

    itemDisplay->show();
}

void MediaRendererManager::closeDisplayWindow(MediaRendererDisplayWindow* win)
{
//    MediaRendererDisplayWindow* win =
//            dynamic_cast<MediaRendererDisplayWindow*>(obj);

//    foreach(MediaRendererDisplayWindow* w, m_displayWindows)
//    {
//        if (w->connection() &&
//            w->connection()->info().connectionId()
//            == win->connection()->info().connectionId())
//        {
//            if (m_displayWindows.remove(w))
//                QMessageBox::information(0, tr(""), tr("remove a window."));
//            break;
//        }
//    }
    if (!m_displayWindows.remove(win))
        QMessageBox::warning(0, tr("closeDisplayWindow()"),
                            tr("The windows closed matches none in the set. Advise RESTART."));

}

void MediaRendererManager::mediaRendererOnline(
        Herqq::Upnp::Av::HMediaRendererAdapter */*deviceAdapter*/)
{
//    if (deviceAdapter->device()->
//        info().modelDescription().
//        compare("Media Renderer Within Control Point", Qt::CaseInsensitive) == 0)
//    {
//        m_pMediaRenderer = deviceAdapter;
//    }
}
