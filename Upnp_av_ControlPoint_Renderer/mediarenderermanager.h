#ifndef RENDERERMANAGER_H
#define RENDERERMANAGER_H

#include <HUpnpCore/HDeviceHost>
#include <HUpnpAv/HUpnpAv>

#include <QObject>
#include <QSet>

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
    void newDisplayWindow(Herqq::Upnp::Av::HAvControlPoint*,
                          Herqq::Upnp::Av::HMediaRendererAdapter*,
                          Herqq::Upnp::Av::HItem*,
                          QWidget *parent = 0);

private:
    Herqq::Upnp::HDeviceHost* m_pDeviceHost;
    Herqq::Upnp::Av::HAvControlPoint* m_pControlPoint;
    Herqq::Upnp::Av::HMediaRendererAdapter* m_pMediaRenderer;

    MediaRendererConnectionManager* m_pConnectionMgr;
    QSet<MediaRendererDisplayWindow*> m_displayWindows;

    QNetworkAccessManager* m_pNetworkMgr;

private slots:
    void mediaRendererOnline(Herqq::Upnp::Av::HMediaRendererAdapter*);
    void closeDisplayWindow(QObject*);

};

#endif // RENDERERMANAGER_H
