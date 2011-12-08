#include "mediarendererconnection.h"

#include <HUpnpAv/HSeekInfo>
#include <HUpnpAv/HMediaInfo>
#include <HUpnpAv/HTransportState>

#include <phonon/AudioOutput>
#include <phonon/MediaSource>

#include <QUrl>
#include <QLayout>
#include <QLabel>
#include <QPixmap>
#include <QTextEdit>
#include <QNetworkAccessManager>
#include <QNetworkReply>

using namespace Herqq::Upnp;
using namespace Herqq::Upnp::Av;

using namespace Phonon;

/*************************************************
  MediaRendererConnection
**************************************************/
MediaRendererConnection::MediaRendererConnection(QObject *parent) :
        HRendererConnection(parent),
        m_isPlaying(false)
{
}

MediaRendererConnection::~MediaRendererConnection()
{
}

void MediaRendererConnection::resizeEventOccurred(const QResizeEvent &e)
{
    Q_UNUSED(e);
}

/*************************************************
  MediaRendererConnectionForText
**************************************************/
MediaRendererConnectionForText::MediaRendererConnectionForText(
        QNetworkAccessManager *netMgr, QWidget *parent) :
        MediaRendererConnection(parent),
        m_pNetworkMgr(netMgr),
        m_pCurrentResource(0),
        m_currentData()
{
    m_pTextEdit = new QTextEdit(parent);
    m_pTextEdit->setReadOnly(true);
    m_pTextEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pTextEdit->setSizePolicy(
            QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    parent->layout()->addWidget(m_pTextEdit);

    setup();
}

MediaRendererConnectionForText::~MediaRendererConnectionForText()
{
    delete m_pCurrentResource;
    delete m_pTextEdit;
}

void MediaRendererConnectionForText::resizeEventOccurred(const QResizeEvent &e)
{
    Q_UNUSED(e);
    setup();
}

void MediaRendererConnectionForText::setup()
{
    if (m_isPlaying)
    {
        m_pTextEdit->setText(QString::fromUtf8(m_currentData));
        m_pTextEdit->setEnabled(true);
    }
    else
    {
        m_pTextEdit->clear();
        m_pTextEdit->setEnabled(false);
    }
}

void MediaRendererConnectionForText::finished()
{
    m_currentData = m_pCurrentResource->readAll();
    m_pCurrentResource->deleteLater();
    m_pCurrentResource = 0;
}

qint32 MediaRendererConnectionForText::doPlay(const QString &arg)
{
    Q_UNUSED(arg);

    m_isPlaying = true;

    if (!m_pCurrentResource)
    {
        setup();
    }

    return UpnpSuccess;
}

qint32 MediaRendererConnectionForText::doStop()
{
    m_isPlaying = false;

    if (m_pCurrentResource)
    {
        m_pCurrentResource->deleteLater();
    }
    setup();

    return UpnpSuccess;
}

qint32 MediaRendererConnectionForText::doSeek(const HSeekInfo& seekInfo)
{
    Q_UNUSED(seekInfo);
    return UpnpSuccess;
}

qint32 MediaRendererConnectionForText::doNext()
{
    return UpnpSuccess;
}

qint32 MediaRendererConnectionForText::doPrevious()
{
    return UpnpSuccess;
}

qint32 MediaRendererConnectionForText::doSetResource(
        const QUrl &resourceUri, Herqq::Upnp::Av::HObject *cdsObjectData)
{
    Q_UNUSED(resourceUri);
    Q_UNUSED(cdsObjectData);

    QNetworkRequest req(resourceUri);
    m_pCurrentResource = m_pNetworkMgr->get(req);
    bool ok = connect(m_pCurrentResource, SIGNAL(finished()), this, SLOT(finished()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);

    return UpnpSuccess;
}

qint32 MediaRendererConnectionForText::doSelectPreset(const QString &presetName)
{
    Q_UNUSED(presetName);
    return UpnpSuccess;
}

/*************************************************
  MediaRendererConnectionForImage
**************************************************/
MediaRendererConnectionForImage::MediaRendererConnectionForImage(
        QNetworkAccessManager *netMgr, QWidget *parent) :
        MediaRendererConnection(parent),
        m_pNetworkMgr(netMgr),
        m_pCurrentResource(0),
        m_currentData(),
        m_readyToShow(false)
{
    m_pImageContainer = new QLabel(parent);
    m_pImageContainer->setScaledContents(true);
    m_pImageContainer->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    m_pImage = new QPixmap(200, 200);
    parent->layout()->addWidget(m_pImageContainer);

    setup();
}

MediaRendererConnectionForImage::~MediaRendererConnectionForImage()
{
    delete m_pCurrentResource;
    delete m_pImage;
    delete m_pImageContainer;
}

void MediaRendererConnectionForImage::resizeEventOccurred(const QResizeEvent &e)
{
    Q_UNUSED(e);
    setup();
}

void MediaRendererConnectionForImage::setup()
{
    if (m_isPlaying)
    {
        m_pImageContainer->setPixmap(
                m_pImage->scaled(m_pImageContainer->size(), Qt::KeepAspectRatio));
    }
    else //if stopped, black instead
    {
        m_pImage->fill(QColor(Qt::black));
        m_pImageContainer->setPixmap(m_pImage->scaled(m_pImageContainer->size()));
    }
}

void MediaRendererConnectionForImage::finished()
{
    m_currentData = m_pCurrentResource->readAll();
    if (m_readyToShow)
    {
        m_pImage->loadFromData(m_currentData);
        setup();
    }
    m_pCurrentResource->deleteLater();
    m_pCurrentResource = 0;
}

qint32 MediaRendererConnectionForImage::doPlay(const QString &arg)
{
    Q_UNUSED(arg);
    m_isPlaying = true;
    if (m_pCurrentResource)
    {
        m_readyToShow = true;
    }
    else
    {
        m_pImage->loadFromData(m_currentData);
        m_readyToShow = false;
        setup();
    }
    return UpnpSuccess;
}

qint32 MediaRendererConnectionForImage::doStop()
{
    m_isPlaying = false;

    if(m_pCurrentResource)
    {
        m_pCurrentResource->deleteLater();
    }
    setup();

    return UpnpSuccess;
}

qint32 MediaRendererConnectionForImage::doSeek(const HSeekInfo& seekInfo)
{
    Q_UNUSED(seekInfo);
    return UpnpSuccess;
}

qint32 MediaRendererConnectionForImage::doNext()
{
    return UpnpSuccess;
}

qint32 MediaRendererConnectionForImage::doPrevious()
{
    return UpnpSuccess;
}

qint32 MediaRendererConnectionForImage::doSetResource(
        const QUrl &resourceUri, Herqq::Upnp::Av::HObject *cdsObjectData)
{
    Q_UNUSED(resourceUri);
    Q_UNUSED(cdsObjectData);

    QNetworkRequest req(resourceUri);
    m_pCurrentResource = m_pNetworkMgr->get(req);
    bool ok = connect(m_pCurrentResource, SIGNAL(finished()), this, SLOT(finished()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);

    return UpnpSuccess;
}

qint32 MediaRendererConnectionForImage::doSelectPreset(const QString &presetName)
{
    Q_UNUSED(presetName);
    return UpnpSuccess;
}
/*************************************************
  MediaRendererConnectionForAudio
**************************************************/
MediaRendererConnectionForAudio::MediaRendererConnectionForAudio(QWidget *parent) :
        MediaRendererConnection(parent),
        m_mediaObject(parent),
        m_pMediaSource(0)
{
    bool ok = connect(
            &m_mediaObject,
            SIGNAL(stateChanged(Phonon::State,Phonon::State)),
            this,
            SLOT(stateChanged(Phonon::State,Phonon::State)));
    Q_ASSERT(ok);
    Q_UNUSED(ok);

    AudioOutput* audioOuput = new AudioOutput(Phonon::MusicCategory, parent);
    createPath(&m_mediaObject, audioOuput);
}

MediaRendererConnectionForAudio::~MediaRendererConnectionForAudio()
{
}

void MediaRendererConnectionForAudio::setup()
{
    //For audio, there is nothing to do.
}

void MediaRendererConnectionForAudio::hasVideoChanged(bool b)
{
    Q_UNUSED(b);
    //For audio, there is nothing to do.
}

void MediaRendererConnectionForAudio::stateChanged(
        Phonon::State newstate, Phonon::State oldstate)
{
    Q_UNUSED(newstate);
    Q_UNUSED(oldstate);
    switch (newstate)
    {
    case Phonon::ErrorState:
        {
            qDebug() << m_mediaObject.errorString();
        }
        break;
    case Phonon::PlayingState:
        writableInfo()->setTransportState(HTransportState::Playing);
        break;
    case Phonon::StoppedState:
        writableInfo()->setTransportState(HTransportState::Stopped);
        break;
    case Phonon::PausedState:
        writableInfo()->setTransportState(HTransportState::PausedPlayback);
        break;
    case Phonon::LoadingState:
        break;
    case Phonon::BufferingState:
        break;
    }
}

qint32 MediaRendererConnectionForAudio::doPlay(const QString &arg)
{
    Q_UNUSED(arg);

    m_mediaObject.play();

    return UpnpSuccess;
}

qint32 MediaRendererConnectionForAudio::doStop()
{
    m_mediaObject.stop();

    return UpnpSuccess;
}

qint32 MediaRendererConnectionForAudio::doSeek(const HSeekInfo& seekInfo)
{
    Q_UNUSED(seekInfo);

    return UpnpSuccess;
}

qint32 MediaRendererConnectionForAudio::doNext()
{
    return UpnpSuccess;
}

qint32 MediaRendererConnectionForAudio::doPrevious()
{
    return UpnpSuccess;
}

qint32 MediaRendererConnectionForAudio::doSetResource(
        const QUrl &resourceUri, Herqq::Upnp::Av::HObject *cdsObjectData)
{
    Q_UNUSED(resourceUri);
    Q_UNUSED(cdsObjectData);

    if (m_pMediaSource)
    {
        m_mediaObject.clear();
        delete m_pMediaSource;
    }

    m_pMediaSource = new MediaSource(resourceUri);
    m_mediaObject.setCurrentSource(*m_pMediaSource);

    return UpnpSuccess;
}

qint32 MediaRendererConnectionForAudio::doSelectPreset(const QString &presetName)
{
    Q_UNUSED(presetName);
    return UpnpSuccess;
}
/*************************************************
  MediaRendererConnectionForVedio
**************************************************/
MediaRendererConnectionForVideo::MediaRendererConnectionForVideo(QWidget *parent) :
        MediaRendererConnection(parent),
        m_mediaObject(parent),
        m_pMediaSource(0),
        m_pVideoWidget(0)
{
    bool ok = connect(
            &m_mediaObject,
            SIGNAL(stateChanged(Phonon::State,Phonon::State)),
            this,
            SLOT(stateChanged(Phonon::State,Phonon::State)));
    Q_ASSERT(ok);
    Q_UNUSED(ok);

    setup();

    AudioOutput* audioOuput = new AudioOutput(Phonon::MusicCategory, parent);
    createPath(&m_mediaObject, audioOuput);
}

MediaRendererConnectionForVideo::~MediaRendererConnectionForVideo()
{
}

void MediaRendererConnectionForVideo::setup()
{
    QWidget* parentWidget = static_cast<QWidget*>(parent());

    m_pVideoWidget = new VideoWidget(parentWidget);
    m_pVideoWidget->setMinimumSize(200,200);
    m_pVideoWidget->setSizePolicy(
            QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    createPath(&m_mediaObject, m_pVideoWidget);

    parentWidget->layout()->addWidget(m_pVideoWidget);
}

void MediaRendererConnectionForVideo::hasVideoChanged(bool b)
{
    Q_UNUSED(b);
    if (b && !m_pVideoWidget && m_mediaObject.hasVideo())
    {
        setup();
        m_pVideoWidget->show();
    }
}

void MediaRendererConnectionForVideo::stateChanged(
        Phonon::State newstate, Phonon::State oldstate)
{
    Q_UNUSED(newstate);
    Q_UNUSED(oldstate);
    switch (newstate)
    {
    case Phonon::ErrorState:
        {
            qDebug() << m_mediaObject.errorString();
        }
        break;
    case Phonon::PlayingState:
        writableInfo()->setTransportState(HTransportState::Playing);
        break;
    case Phonon::StoppedState:
        writableInfo()->setTransportState(HTransportState::Stopped);
        break;
    case Phonon::PausedState:
        writableInfo()->setTransportState(HTransportState::PausedPlayback);
        break;
    case Phonon::LoadingState:
        break;
    case Phonon::BufferingState:
        break;
    }
}

qint32 MediaRendererConnectionForVideo::doPlay(const QString &arg)
{
    Q_UNUSED(arg);

    m_mediaObject.play();

    return UpnpSuccess;
}

qint32 MediaRendererConnectionForVideo::doStop()
{
    m_mediaObject.stop();

    return UpnpSuccess;
}

qint32 MediaRendererConnectionForVideo::doSeek(const HSeekInfo& seekInfo)
{
    Q_UNUSED(seekInfo);
    return UpnpSuccess;
}

qint32 MediaRendererConnectionForVideo::doNext()
{
    return UpnpSuccess;
}

qint32 MediaRendererConnectionForVideo::doPrevious()
{
    return UpnpSuccess;
}

qint32 MediaRendererConnectionForVideo::doSetResource(
        const QUrl &resourceUri, Herqq::Upnp::Av::HObject *cdsObjectData)
{
    Q_UNUSED(resourceUri);
    Q_UNUSED(cdsObjectData);

    if (m_pMediaSource)
    {
        m_mediaObject.clear();
        delete m_pMediaSource;
    }

    m_pMediaSource = new MediaSource(resourceUri);
    m_mediaObject.setCurrentSource(*m_pMediaSource);

    if (!m_pVideoWidget)
    {
        if (m_mediaObject.hasVideo())
        {
            setup();
            m_pVideoWidget->show();
        }
        else
        {
            bool ok = connect(
                    &m_mediaObject,
                    SIGNAL(hasVideoChanged(bool)),
                    this,
                    SLOT(hasVideoChanged(bool)));
            Q_ASSERT(ok);
            Q_UNUSED(ok);
        }
    }

    return UpnpSuccess;
}

qint32 MediaRendererConnectionForVideo::doSelectPreset(const QString &presetName)
{
    Q_UNUSED(presetName);
    return UpnpSuccess;
}
