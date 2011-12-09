#include "mediarendererdisplaywindow.h"
#include "ui_mediarendererdisplaywindow.h"
#include "mediarendererconnection.h"

#include <HUpnpCore/HUdn>
#include <HUpnpCore/HDeviceInfo>
#include <HUpnpCore/HClientDevice>

#include <HUpnpAv/HRendererConnection>
#include <HUpnpAv/HItem>
#include <HUpnpAv/HDuration>
#include <HUpnpAv/HResource>
#include <HUpnpAv/HContainer>
#include <HUpnpAv/HConnection>
#include <HUpnpAv/HCdsDataSource>
#include <HUpnpAv/HAvControlPoint>
#include <HUpnpAv/HAvtLastChangeInfo>
#include <HUpnpAv/HRcsLastChangeInfo>
#include <HUpnpAv/HConnectionManagerId>
#include <HUpnpAv/HAvTransportAdapter>
#include <HUpnpAv/HMediaRendererAdapter>
#include <HUpnpAv/HConnectionManagerAdapter>


#include <QMessageBox>
#include <QUrl>

using namespace Herqq::Upnp;
using namespace Herqq::Upnp::Av;

namespace
{
    bool isText(const QString& cf)
    {
        return cf.startsWith("text", Qt::CaseInsensitive);
    }

    bool isImage(const QString& cf)
    {
        return cf.startsWith("image", Qt::CaseInsensitive);
    }

    bool isAudio(const QString& cf)
    {
        return cf.startsWith("audio", Qt::CaseInsensitive);
    }

    bool isVideo(const QString& cf)
    {
        return cf.startsWith("video", Qt::CaseInsensitive);
    }
}


MediaRendererDisplayWindow::MediaRendererDisplayWindow(
        Herqq::Upnp::Av::HAvControlPoint *cp,
        Herqq::Upnp::Av::HMediaRendererAdapter* renderer,
        Herqq::Upnp::Av::HItem *item,
        QWidget *parent) :
    QMainWindow(parent),
    m_pUi(new Ui::MediaRendererDisplayWindow),
    m_pControlPoint(cp),
    m_pRenderer(renderer),
    m_pConnection(0),
    m_pCurItem(item),
    m_connectionRetrieveOp(),
    m_state(Stopped),
    m_isPlaying(false),
    m_pRenderingConnection(0)
{
    m_pUi->setupUi(this);
}

MediaRendererDisplayWindow::~MediaRendererDisplayWindow()
{
    if (m_pRenderingConnection)
        m_pRenderingConnection->deleteLater();
    delete m_pUi;
}

void MediaRendererDisplayWindow::initializeRenderingControl(
        const QString &cf, QNetworkAccessManager *netMgr)
{
    Q_UNUSED(cf);
    Q_UNUSED(netMgr);

    if (!m_pUi)
    {
        QMessageBox::critical(0, tr("initializeRenderingControl()"),
                              tr("Unknown Error with m_pUi, Please retry."));
        return;
    }

    if (isText(cf))
    {
        m_pRenderingConnection = new MediaRendererConnectionForText(
                netMgr, m_pUi->scrollAreaWidgetContents);
    }
    else if (isImage(cf))
    {
        m_pRenderingConnection = new MediaRendererConnectionForImage(
                netMgr, m_pUi->scrollAreaWidgetContents);
    }
    else if (isAudio(cf))
    {
        m_pRenderingConnection = new MediaRendererConnectionForAudio(
                m_pUi->scrollAreaWidgetContents);
    }
    else if (isVideo(cf))
    {
        m_pRenderingConnection = new MediaRendererConnectionForVideo(
                m_pUi->scrollAreaWidgetContents);
    }
    else if (cf == "*" || cf.isEmpty() || cf == "application/octet-stream")
    {
        m_pRenderingConnection = 0;
        QMessageBox::warning(this, tr("Invalid Content Format."),
                             tr("The Content Format is not supported yet."));
    }

    Q_ASSERT(m_pRenderingConnection);

    if (m_pRenderingConnection)
    {
        bool ok = connect(
                m_pRenderingConnection,
                SIGNAL(disposed(Herqq::Upnp::Av::HRendererConnection*)),
                this,
                SLOT(disposed(Herqq::Upnp::Av::HRendererConnection*)));
        Q_ASSERT(ok);
        Q_UNUSED(ok);
    }

//    m_pUi->timeLabel->setText(QString("%1:%2:%3").
//                              arg(QString::number(m_pRenderingConnection->info()->mediaInfo().
//                                                  mediaDuration().hours()),
//                                  QString::number(m_pRenderingConnection->info()->mediaInfo().
//                                                  mediaDuration().minutes()),
//                                  QString::number(m_pRenderingConnection->info()->mediaInfo().
//                                                  mediaDuration().seconds())));
}

void MediaRendererDisplayWindow::mediaRendererOffline(
        Herqq::Upnp::Av::HMediaRendererAdapter *deviceAdapter)
{
    if (deviceAdapter->device()->parentDevice()->info().udn() ==
        m_pRenderer->device()->parentDevice()->info().udn())
    {
        QMessageBox::warning(this, tr("mediaRendererOffline()"),
                             tr("Local Renderer offlined suddenly."));
    }
}

void MediaRendererDisplayWindow::connectionReady(
        Herqq::Upnp::Av::HMediaRendererAdapter *rendererAdapter, qint32 cid)
{
    if (m_state != Connecting ||
        m_connectionRetrieveOp.returnValue() != UpnpSuccess ||
        m_connectionRetrieveOp.value() != cid)
    {
        //The Signal is not for our connection.
        return;
    }

    Q_ASSERT(!m_pConnection);

    if (!m_pConnection)
        delete m_pConnection;
    m_pConnection = rendererAdapter->takeConnection(cid);
    m_pConnection->setAutoCloseConnection(true);

    Q_ASSERT(m_pConnection);

    HClientAdapterOpNull op =
            m_pConnection->transport()->setAVTransportURI(
                    m_pCurItem->resources().at(0).location(), "");
    Q_ASSERT(!op.isNull());
    Q_UNUSED(op);

    bool ok = connect(
            m_pConnection->transport(),
            SIGNAL(setAVTransportURICompleted(Herqq::Upnp::Av::HAvTransportAdapter*,
                                              Herqq::Upnp::HClientAdapterOpNull)),
            this,
            SLOT(setAVTransportURICompleted(Herqq::Upnp::Av::HAvTransportAdapter*,
                                            Herqq::Upnp::HClientAdapterOpNull)));
    Q_ASSERT(ok);

    ok = connect(
            m_pConnection->transport(),
            SIGNAL(playCompleted(Herqq::Upnp::Av::HAvTransportAdapter*,
                                 Herqq::Upnp::HClientAdapterOpNull)),
            this,
            SLOT(playCompleted(Herqq::Upnp::Av::HAvTransportAdapter*,
                               Herqq::Upnp::HClientAdapterOpNull)));
    Q_ASSERT(ok);

    ok = connect(m_pConnection->transport(),
                 SIGNAL(stopCompleted(Herqq::Upnp::Av::HAvTransportAdapter*,
                                      Herqq::Upnp::HClientAdapterOpNull)),
                 this,
                 SLOT(stopCompleted(Herqq::Upnp::Av::HAvTransportAdapter*,
                                    Herqq::Upnp::HClientAdapterOpNull)));
    Q_ASSERT(ok);

    ok = connect(m_pConnection->transport(),
                 SIGNAL(pauseCompleted(Herqq::Upnp::Av::HAvTransportAdapter*,
                                       Herqq::Upnp::HClientAdapterOpNull)),
                 this,
                 SLOT(pauseCompleted(Herqq::Upnp::Av::HAvTransportAdapter*,
                                     Herqq::Upnp::HClientAdapterOpNull)));
    Q_ASSERT(ok);

    ok = connect(
            m_pConnection,
            SIGNAL(invalidated(Herqq::Upnp::Av::HConnection*)),
            this,
            SLOT(invalidated(Herqq::Upnp::Av::HConnection*)));
    Q_ASSERT(ok);

    ok = connect(
            m_pConnection,
            SIGNAL(avTransportStateChanged(Herqq::Upnp::Av::HConnection*,
                                           Herqq::Upnp::Av::HAvtLastChangeInfos)),
            this,
            SLOT(avTransportStateChanged(Herqq::Upnp::Av::HConnection*,
                                         Herqq::Upnp::Av::HAvtLastChangeInfos)));
    Q_ASSERT(ok);

    ok = connect(
            m_pConnection,
            SIGNAL(renderingControlStateChanged(Herqq::Upnp::Av::HConnection*,
                                                Herqq::Upnp::Av::HRcsLastChangeInfos)),
            this,
            SLOT(renderingControlStateChanged(Herqq::Upnp::Av::HConnection*,
                                              Herqq::Upnp::Av::HRcsLastChangeInfos)));
    Q_ASSERT(ok);

    m_state = SettingUp;
}

void MediaRendererDisplayWindow::invalidated(Herqq::Upnp::Av::HConnection *source)
{
    Q_ASSERT(source == m_pConnection);
    Q_UNUSED(source);

    m_pConnection->setAutoCloseConnection(false);

    // When the connection is invalidated at server-side there's no "need" to
    // explicitly close it from client-side. It would actually cause an
    // action invocation error.

    if (m_pConnection)
        delete m_pConnection;

    m_state = Stopped;
    enableControl(true);
}

void MediaRendererDisplayWindow::avTransportStateChanged(
        Herqq::Upnp::Av::HConnection *source,
        const Herqq::Upnp::Av::HAvtLastChangeInfos &infos)
{
    Q_ASSERT(source == m_pConnection);

    if(source != m_pConnection)
        return;

    foreach(const HAvtLastChangeInfo& info, infos)
    {
        if (info.propertyName().compare("TransportState", Qt::CaseInsensitive) == 0)
        {
            HTransportState state(info.value().toString());
            if(state.isValid())
            {
                switch(state.type())
                {
                case HTransportState::Playing:
                    {
                        m_state = Playing;
                        enableControl(true);
                    }
                    break;
                case HTransportState::Stopped:
                    {
                        m_state = Stopped;
                        enableControl(true);
                    }
                    break;
                default:
                    break;
                }
            }
        }
    }
}

void MediaRendererDisplayWindow::renderingControlStateChanged(
        Herqq::Upnp::Av::HConnection *source,
        const Herqq::Upnp::Av::HRcsLastChangeInfos &info)
{
    Q_UNUSED(source);
    Q_UNUSED(info);
}

void MediaRendererDisplayWindow::playCompleted(
        Herqq::Upnp::Av::HAvTransportAdapter *source,
        const Herqq::Upnp::HClientAdapterOpNull &op)
{
    Q_UNUSED(source);
    Q_UNUSED(op);

    enableControl(true);
}

void MediaRendererDisplayWindow::stopCompleted(
        Herqq::Upnp::Av::HAvTransportAdapter *source,
        const Herqq::Upnp::HClientAdapterOpNull &op)
{
    Q_UNUSED(source);
    Q_UNUSED(op);

    m_state = Stopped;
    enableControl(true);
}

void MediaRendererDisplayWindow::pauseCompleted(
        Herqq::Upnp::Av::HAvTransportAdapter *source,
        const Herqq::Upnp::HClientAdapterOpNull &op)
{
    Q_UNUSED(source);
    Q_UNUSED(op);

    m_state = Paused;
    enableControl(true);
}

void MediaRendererDisplayWindow::setAVTransportURICompleted(
        Herqq::Upnp::Av::HAvTransportAdapter *source,
        const Herqq::Upnp::HClientAdapterOpNull &op)
{
    Q_UNUSED(source);
    Q_UNUSED(op);

    m_pConnection->transport()->play("1");
    m_state = Playing;
    enableControl(true);
}

void MediaRendererDisplayWindow::getConnection()
{
    Q_ASSERT(!m_pConnection && m_pRenderer);
    Q_ASSERT(m_state == Stopped);

    if (m_pRenderer->connectionManager()->implementsAction("PrepareForConnection"))
    {
        Q_ASSERT(m_pCurItem);
        if (m_pCurItem->resources().size() > 0)
        {
            m_connectionRetrieveOp =
                    m_pRenderer->prepareNewConnection(
                            m_pCurItem->resources().at(0).protocolInfo(),
                            HConnectionManagerId());
            if (m_connectionRetrieveOp.isNull())
            {
                QMessageBox::warning(this, tr("GetConnection()"),
                                     tr("No connection retrieved."));
                return;
            }
        }
        else
        {
            //No resource exist in current item.
            QMessageBox::warning(this, tr("GetConnection()"),
                                 tr("Current Item has no resources."));
            return;
        }
    }
    else
    {
        m_connectionRetrieveOp = m_pRenderer->getConnection(0);
        //Connection 0 is the default connection.
    }

    m_state = Connecting;
}

void MediaRendererDisplayWindow::enableControl(bool enable)
{
    if (enable)
    {
        m_pUi->playButton->setEnabled(true);
        if (m_state == Stopped)
        {
//            m_pUi->forwardButton->setEnabled(false);
//            m_pUi->backwardButton->setEnabled(false);
            m_pUi->stopButton->setEnabled(false);
        }
        else
        {
//            m_pUi->forwardButton->setEnabled(true);
//            m_pUi->backwardButton->setEnabled(true);
            m_pUi->stopButton->setEnabled(true);
        }
    }
    else
    {
//        m_pUi->forwardButton->setEnabled(false);
//        m_pUi->backwardButton->setEnabled(false);
        m_pUi->stopButton->setEnabled(false);
        m_pUi->playButton->setEnabled(false);
    }
}

HRendererConnection* MediaRendererDisplayWindow::renderingControlConnection() const
{
    return dynamic_cast<HRendererConnection*>(m_pRenderingConnection);
}

HConnection* MediaRendererDisplayWindow::connection() const
{
    return m_pConnection;
}

void MediaRendererDisplayWindow::closeEvent(QCloseEvent *e)
{
    this->deleteLater();
    QMainWindow::closeEvent(e);
}

void MediaRendererDisplayWindow::resizeEvent(QResizeEvent *e)
{
    Q_UNUSED(e);

    QMainWindow::resizeEvent(e);
    if (m_pRenderingConnection)
        m_pRenderingConnection->resizeEventOccurred(*e);
}

void MediaRendererDisplayWindow::disposed(Herqq::Upnp::Av::HRendererConnection *)
{
    deleteLater();
}

void MediaRendererDisplayWindow::on_playButton_clicked()
{
    if (m_state == Playing/*m_isPlaying*/)
    {
//        m_isPlaying = false;
        m_state = Paused;
        m_pConnection->transport()->pause();
        m_pUi->playButton->setIcon(QIcon(":/icon/start.svg"));
        return;
    }
    else
    {
        m_isPlaying = true;
        m_pUi->playButton->setIcon(QIcon(":/icon/pause.svg"));
    }

    enableControl(false);

    Q_ASSERT(m_pRenderer);
    if (!m_pRenderer)
    {
        QMessageBox::warning(this, tr("on_playButton_clicked()"),
                             tr("No renderer has setted. Play Failed."));
        return;
    }

    if (m_pConnection)
    {
        m_pConnection->transport()->play("1");
        m_state = Playing;
    }
    else
    {
        bool ok = connect(
                m_pRenderer,
                SIGNAL(connectionReady(Herqq::Upnp::Av::HMediaRendererAdapter*,qint32)),
                this,
                SLOT(connectionReady(Herqq::Upnp::Av::HMediaRendererAdapter*,qint32)));
        Q_ASSERT(ok);
        Q_UNUSED(ok);

        getConnection();
    }

    enableControl(true);
}

void MediaRendererDisplayWindow::on_stopButton_clicked()
{
    m_isPlaying = false;
    m_pUi->playButton->setIcon(QIcon(":/icon/start.svg"));

    if (m_state == Playing)
    {
        Q_ASSERT(m_pConnection);

        m_pConnection->transport()->stop();
        m_state = Stopping;

        enableControl(false);
    }
}
