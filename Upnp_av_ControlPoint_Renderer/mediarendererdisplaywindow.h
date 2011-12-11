#ifndef RENDERERWINDOW_H
#define RENDERERWINDOW_H

#include <HUpnpAv/HUpnpAv>
#include <HUpnpCore/HUdn>
#include <HUpnpCore/HClientAdapterOp>

#include <QMainWindow>

namespace Ui {
    class MediaRendererDisplayWindow;
}

class QNetworkAccessManager;
class MediaRendererConnection;

class MediaRendererDisplayWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MediaRendererDisplayWindow(
            Herqq::Upnp::Av::HAvControlPoint*,
            Herqq::Upnp::Av::HMediaRendererAdapter*,
            Herqq::Upnp::Av::HItem*,
            Herqq::Upnp::HUdn,
            QWidget *parent = 0);
    ~MediaRendererDisplayWindow();

    Herqq::Upnp::Av::HRendererConnection* renderingControlConnection() const;
    Herqq::Upnp::Av::HConnection* connection() const;
    Herqq::Upnp::Av::HItem* item() const;
    void initializeRenderingControl(const QString&, QNetworkAccessManager*);

    enum State
    {
        Stopped,
        Playing,
        Paused,
        Forwarding,
        Backwarding,
        Connecting,
        Stopping,
        SettingUp
    };

protected:
    void closeEvent(QCloseEvent *);
    void resizeEvent(QResizeEvent *);

private:
    Ui::MediaRendererDisplayWindow *m_pUi;

    Herqq::Upnp::Av::HAvControlPoint* m_pControlPoint;
    Herqq::Upnp::Av::HMediaServerAdapter* m_pServer;
    Herqq::Upnp::Av::HMediaRendererAdapter* m_pRenderer;
    Herqq::Upnp::Av::HConnection* m_pConnection;
    Herqq::Upnp::Av::HItem* m_pCurItem;
    Herqq::Upnp::HUdn m_serverUdn;
    Herqq::Upnp::HClientAdapterOp<qint32> m_connectionRetrieveOp;

    State m_state;
    bool m_isPlaying;
    bool m_isFirst;
    QString m_objectId;

    MediaRendererConnection* m_pRenderingConnection;

    void enableControl(bool enable);
    void getConnection();

private slots:
    void mediaRendererOffline(Herqq::Upnp::Av::HMediaRendererAdapter*);
    void mediaServerOffline(Herqq::Upnp::Av::HMediaServerAdapter*);
    void connectionReady(Herqq::Upnp::Av::HMediaRendererAdapter*, qint32);
    void invalidated(Herqq::Upnp::Av::HConnection* source);
    void avTransportStateChanged(
        Herqq::Upnp::Av::HConnection* source,
        const Herqq::Upnp::Av::HAvtLastChangeInfos& info);
    void renderingControlStateChanged(
        Herqq::Upnp::Av::HConnection* source,
        const Herqq::Upnp::Av::HRcsLastChangeInfos& info);
    void playCompleted(
        Herqq::Upnp::Av::HAvTransportAdapter* source,
        const Herqq::Upnp::HClientAdapterOpNull& op);
    void stopCompleted(
        Herqq::Upnp::Av::HAvTransportAdapter* source,
        const Herqq::Upnp::HClientAdapterOpNull &op);
    void pauseCompleted(
        Herqq::Upnp::Av::HAvTransportAdapter *source,
        const Herqq::Upnp::HClientAdapterOpNull &op);
    void setAVTransportURICompleted(
        Herqq::Upnp::Av::HAvTransportAdapter* source,
        const Herqq::Upnp::HClientAdapterOpNull& op);

    void on_stopButton_clicked();
    void on_playButton_clicked();

private Q_SLOTS:
    void disposed(Herqq::Upnp::Av::HRendererConnection*);

Q_SIGNALS:
    void close(MediaRendererDisplayWindow*);

};

#endif // RENDERERWINDOW_H
