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
        m_pOwner(owner),
        HRendererConnectionManager(owner)
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

    QString cf =
            contentFormat == "*" || contentFormat.isEmpty() ?
            "UNKNOWN" :
            contentFormat;



    return 0;
}
