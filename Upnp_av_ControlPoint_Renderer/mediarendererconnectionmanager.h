#ifndef RENDERERCONNECTIONMANAGER_H
#define RENDERERCONNECTIONMANAGER_H

#include <HUpnpAv/HRendererConnectionManager>

class MediaRendererManager;
class ControlPointWindow;

class MediaRendererConnectionManager :
        public Herqq::Upnp::Av::HRendererConnectionManager
{
public:
    MediaRendererConnectionManager(MediaRendererManager*);
    ~MediaRendererConnectionManager();

protected:
    virtual Herqq::Upnp::Av::HRendererConnection* doCreate(
            const QString &contentFormat, qint32 connectionId);

private:
    MediaRendererManager* m_pOwner;

};

#endif // RENDERERCONNECTIONMANAGER_H
