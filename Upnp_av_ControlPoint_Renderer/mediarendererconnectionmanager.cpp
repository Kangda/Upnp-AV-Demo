#include "mediarendererconnectionmanager.h"
#include "mediarendererdisplaywindow.h"
#include "mediarenderermanager.h"

#include <HUpnpAv/HRendererConnection>

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

    m_pOwner->m_pCurWindow->initializeRenderingControl(cf, m_pOwner->m_pNetworkMgr);

    if (!m_pOwner->m_pCurWindow->renderingControlConnection())
    {
        return 0;
    }

    bool ok = QObject::connect(
            m_pOwner,
            SIGNAL(destroyed()),
            m_pOwner->m_pCurWindow,
            SLOT(deleteLater()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);

    return m_pOwner->m_pCurWindow->renderingControlConnection();
}
