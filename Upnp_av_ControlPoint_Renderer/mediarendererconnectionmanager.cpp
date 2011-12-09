#include "mediarendererconnectionmanager.h"
#include "mediarendererdisplaywindow.h"
#include "mediarenderermanager.h"

#include <HUpnpAv/HRendererConnection>
#include <HUpnpAv/HConnection>
#include <HUpnpAv/HConnectionInfo>

using namespace Herqq::Upnp;
using namespace Herqq::Upnp::Av;
/*********************************************
  RendererConnectionManager
**********************************************/
MediaRendererConnectionManager::MediaRendererConnectionManager(MediaRendererManager *owner) :
        HRendererConnectionManager(owner),
        m_pOwner(owner)
{

}

MediaRendererConnectionManager::~MediaRendererConnectionManager()
{
}

HRendererConnection* MediaRendererConnectionManager::doCreate(
        const QString &contentFormat, qint32 connectionId)
{
//    MediaRendererDisplayWindow* displayWindow =
//            new MediaRendererDisplayWindow(contentFormat, *m_pOwner->m_pNetworkMgr, 0);
    Q_UNUSED(contentFormat);
    Q_UNUSED(connectionId);

    QString cf = contentFormat == "*" || contentFormat.isEmpty() ?
                 "UNKNOWN" :
                 contentFormat;

    MediaRendererDisplayWindow* curWin = 0;

    foreach(MediaRendererDisplayWindow* win, m_pOwner->m_displayWindows)
    {
        if (win->connection() == 0)
        //Now, the connection has not built yet. So no HConnecton Object exists.
        {
            curWin = win;
            break;
        }
    }

    if (!curWin)
    {
        return 0;
    }

    curWin->initializeRenderingControl(cf, m_pOwner->m_pNetworkMgr);

    if (!curWin->renderingControlConnection())
    {
        curWin->deleteLater();
        return 0;
    }

    bool ok = QObject::connect(
            m_pOwner,
            SIGNAL(destroyed()),
            curWin,
            SLOT(deleteLater()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);

    return curWin->renderingControlConnection();
}
