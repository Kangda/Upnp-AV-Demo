#include "mediarendererconnection.h"

#include <HUpnpAv/HSeekInfo>
#include <HUpnpAv/HMediaInfo>
#include <HUpnpAv/HTransportState>

#include <phonon/AudioOutput>
#include <phonon/MediaSource>

#include <QUrl>
#include <QBuffer>
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
        HRendererConnection(parent)
{
}

MediaRendererConnection::~MediaRendererConnection()
{
}

//void MediaRendererConnection::resizeEventOccurred(const QResizeEvent &e)
//{
//    Q_UNUSED(e);
//}

///*************************************************
//  MediaRendererConnectionForText
//**************************************************/
//MediaRendererConnectionForText::MediaRendererConnectionForText(
//        QNetworkAccessManager *netMgr, QWidget *parent) :
//        MediaRendererConnection(parent),
//        m_pNetworkMgr(netMgr),
//        m_pCurrentResource(0),
//        m_currentData()
//{
//}

//MediaRendererConnectionForText::~MediaRendererConnectionForText()
//{
//    delete m_pCurrentResource;
//    delete m_pTextEdit;
//}

//void MediaRendererConnectionForText::resizeEventOccurred(const QResizeEvent &e)
//{

//}

//void MediaRendererConnectionForText::setup()
//{

//}

//void MediaRendererConnectionForText::finished()
//{

//}

//qint32 MediaRendererConnectionForText::doPlay(const QString &arg)
//{

//}

//qint32 MediaRendererConnectionForText::doStop()
//{

//}

//qint32 MediaRendererConnectionForText::doSeek(const HSeekInfo& seekInfo)
//{
//}

//qint32 MediaRendererConnectionForText::doNext()
//{
//}

//qint32 MediaRendererConnectionForText::doPrevious()
//{
//}

//qint32 MediaRendererConnectionForText::doSetResource(
//        const QUrl &resourceUri, Herqq::Upnp::Av::HObject *cdsObjectData)
//{
//}

//qint32 MediaRendererConnectionForText::doSelectPreset(const QString &presetName)
//{
//}

///*************************************************
//  MediaRendererConnectionForImage
//**************************************************/
//MediaRendererConnectionForImage::MediaRendererConnectionForImage(
//        QNetworkAccessManager *netMgr, QWidget *parent) :
//        MediaRendererConnection(parent),
//        m_pNetworkMgr(netMgr),
//        m_pCurrentResource(0),
//        m_currentData()
//{
//}

//MediaRendererConnectionForImage::~MediaRendererConnectionForImage()
//{
//    delete m_pCurrentResource;
//    delete m_pImage;
//    delete m_pImageContainer;
//}

//void MediaRendererConnectionForImage::resizeEventOccurred(const QResizeEvent &e)
//{

//}

//void MediaRendererConnectionForImage::setup()
//{

//}

//void MediaRendererConnectionForImage::finished()
//{

//}

//qint32 MediaRendererConnectionForImage::doPlay(const QString &arg)
//{

//}

//qint32 MediaRendererConnectionForImage::doStop()
//{

//}

//qint32 MediaRendererConnectionForImage::doSeek(const HSeekInfo& seekInfo)
//{
//}

//qint32 MediaRendererConnectionForImage::doNext()
//{
//}

//qint32 MediaRendererConnectionForImage::doPrevious()
//{
//}

//qint32 MediaRendererConnectionForImage::doSetResource(
//        const QUrl &resourceUri, Herqq::Upnp::Av::HObject *cdsObjectData)
//{
//}

//qint32 MediaRendererConnectionForImage::doSelectPreset(const QString &presetName)
//{
//}
///*************************************************
//  MediaRendererConnectionForAudio
//**************************************************/
//MediaRendererConnectionForAudio::MediaRendererConnectionForAudio(QWidget *parent) :
//        MediaRendererConnection(parent),
//        m_mediaObject(parent),
//        m_pMediaSource(0)
//{
//}

//MediaRendererConnectionForAudio::~MediaRendererConnectionForAudio()
//{
//    delete m_pMediaSource;
//}

//void MediaRendererConnectionForAudio::setup()
//{
//}

//void MediaRendererConnectionForAudio::hasVideoChanged(bool b)
//{
//}

//void MediaRendererConnectionForAudio::stateChanged(
//        Phonon::State newstate, Phonon::State oldstate)
//{
//}

//qint32 MediaRendererConnectionForAudio::doPlay(const QString &arg)
//{

//}

//qint32 MediaRendererConnectionForAudio::doStop()
//{

//}

//qint32 MediaRendererConnectionForAudio::doSeek(const HSeekInfo& seekInfo)
//{
//}

//qint32 MediaRendererConnectionForAudio::doNext()
//{
//}

//qint32 MediaRendererConnectionForAudio::doPrevious()
//{
//}

//qint32 MediaRendererConnectionForAudio::doSetResource(
//        const QUrl &resourceUri, Herqq::Upnp::Av::HObject *cdsObjectData)
//{
//}

//qint32 MediaRendererConnectionForAudio::doSelectPreset(const QString &presetName)
//{
//}
///*************************************************
//  MediaRendererConnectionForVedio
//**************************************************/
//MediaRendererConnectionForVideo::MediaRendererConnectionForVideo(QWidget *parent) :
//        MediaRendererConnection(parent),
//        m_mediaObject(parent),
//        m_pMediaSource(0),
//        m_pVideoWidget(0)
//{
//}

//MediaRendererConnectionForVideo::~MediaRendererConnectionForVideo()
//{
//    delete m_pVideoWidget;
//    delete m_pMediaSource;
//}

//void MediaRendererConnectionForVideo::setup()
//{
//}

//void MediaRendererConnectionForVideo::hasVideoChanged(bool b)
//{
//}

//void MediaRendererConnectionForVideo::stateChanged(
//        Phonon::State newstate, Phonon::State oldstate)
//{
//}

//qint32 MediaRendererConnectionForVideo::doPlay(const QString &arg)
//{

//}

//qint32 MediaRendererConnectionForVideo::doStop()
//{

//}

//qint32 MediaRendererConnectionForVideo::doSeek(const HSeekInfo& seekInfo)
//{
//}

//qint32 MediaRendererConnectionForVideo::doNext()
//{
//}

//qint32 MediaRendererConnectionForVideo::doPrevious()
//{
//}

//qint32 MediaRendererConnectionForVideo::doSetResource(
//        const QUrl &resourceUri, Herqq::Upnp::Av::HObject *cdsObjectData)
//{
//}

//qint32 MediaRendererConnectionForVideo::doSelectPreset(const QString &presetName)
//{
//}
