#ifndef RENDERERMANAGER_H
#define RENDERERMANAGER_H

#include <HUpnpCore/HDeviceHost>
#include <HUpnpAv/HUpnpAv>

#include <QObject>

class QNetworkAccessManager;
class MediaRendererConnectionManager;

class MediaRendererManager : public QObject
{
Q_OBJECT

friend class MediaRendererConnectionManager;

public:

    MediaRendererManager();
    ~MediaRendererManager();

    Herqq::Upnp::Av::HAbstractMediaRendererDevice* mediaRenderer() const;

private:
    Herqq::Upnp::HDeviceHost* m_pDeviceHost;
    Herqq::Upnp::Av::HAbstractMediaRendererDevice* m_pMediaRenderer;

    MediaRendererConnectionManager* m_pConnectionMgr;

    QNetworkAccessManager* m_pNetworkMgr;

};

#endif // RENDERERMANAGER_H
