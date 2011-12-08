#ifndef RENDERERMANAGER_H
#define RENDERERMANAGER_H

#include <HUpnpCore/HDeviceHost>
#include <HUpnpAv/HUpnpAv>

#include <QObject>

class QNetworkAccessManager;
class MediaRendererConnectionManager;
class MediaRendererDisplayWindow;

class MediaRendererManager : public QObject
{
Q_OBJECT

friend class MediaRendererConnectionManager;

public:

    MediaRendererManager(Herqq::Upnp::Av::HAvControlPoint*);
    ~MediaRendererManager();

    void setMediaRendererAdapter(Herqq::Upnp::Av::HMediaRendererAdapter*);
    Herqq::Upnp::Av::HMediaRendererAdapter* mediaRendererAdapter() const;
    void setDisplayWindow(MediaRendererDisplayWindow*);

private:
    Herqq::Upnp::HDeviceHost* m_pDeviceHost;
    Herqq::Upnp::Av::HAvControlPoint* m_pControlPoint;
    Herqq::Upnp::Av::HMediaRendererAdapter* m_pMediaRenderer;

    MediaRendererConnectionManager* m_pConnectionMgr;
    MediaRendererDisplayWindow* m_pCurWindow;

    QNetworkAccessManager* m_pNetworkMgr;

private slots:
    void mediaRendererOnline(Herqq::Upnp::Av::HMediaRendererAdapter*);

};

#endif // RENDERERMANAGER_H
