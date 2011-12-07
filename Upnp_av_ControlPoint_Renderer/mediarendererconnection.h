#ifndef RENDERERCONNECTION_H
#define RENDERERCONNECTION_H

#include <HUpnpAv/HRendererConnection>

#include <phonon/MediaObject>
#include <phonon/VideoWidget>

#include <QWidget>

class QNetworkAccessManager;
class QNetworkReply;
class QTextEdit;
class QLabel;
class QPixmap;

/*************************************************
  MediaRendererConnection
**************************************************/
class MediaRendererConnection :
        public Herqq::Upnp::Av::HRendererConnection
{
public:
    MediaRendererConnection(QObject* parent);
     ~MediaRendererConnection();

    //virtual void resizeEventOccurred(const QResizeEvent&);
};

///*************************************************
//  MediaRendererConnectionForText
//**************************************************/
//class MediaRendererConnectionForText : public MediaRendererConnection
//{
//    Q_OBJECT
//    Q_DISABLE_COPY(MediaRendererConnectionForText)
//public:
//    MediaRendererConnectionForText(QNetworkAccessManager*, QWidget* parent = 0);
//    virtual ~MediaRendererConnectionForText();

//    virtual void resizeEventOccurred(const QResizeEvent&);
//protected:

//    virtual qint32 doPlay(const QString& arg);
//    virtual qint32 doStop();

//    virtual qint32 doSeek(const Herqq::Upnp::Av::HSeekInfo& seekInfo);
//    virtual qint32 doNext();
//    virtual qint32 doPrevious();

//    virtual qint32 doSetResource(
//        const QUrl& resourceUri, Herqq::Upnp::Av::HObject* cdsObjectData = 0);

//    virtual qint32 doSelectPreset(const QString& presetName);

//private:
//    QNetworkAccessManager* m_pNetworkMgr;
//    QNetworkReply* m_pCurrentResource;
//    QByteArray m_currentData;
//    QTextEdit* m_pTextEdit;

//    void setup();

//private Q_SLOTS:
//    void finished();

//};

///*************************************************
//  MediaRendererConnectionForImage
//**************************************************/
//class MediaRendererConnectionForImage : public MediaRendererConnection
//{
//    Q_OBJECT
//    Q_DISABLE_COPY(MediaRendererConnectionForImage)
//public:
//    MediaRendererConnectionForImage(QNetworkAccessManager*, QWidget* parent = 0);
//    virtual ~MediaRendererConnectionForImage();

//    virtual void resizeEventOccurred(const QResizeEvent&);
//protected:

//    virtual qint32 doPlay(const QString& arg);
//    virtual qint32 doStop();

//    virtual qint32 doSeek(const Herqq::Upnp::Av::HSeekInfo& seekInfo);
//    virtual qint32 doNext();
//    virtual qint32 doPrevious();

//    virtual qint32 doSetResource(
//        const QUrl& resourceUri, Herqq::Upnp::Av::HObject* cdsObjectData = 0);

//    virtual qint32 doSelectPreset(const QString& presetName);

//private:
//    QNetworkAccessManager* m_pNetworkMgr;
//    QNetworkReply* m_pCurrentResource;
//    QByteArray m_currentData;
//    QLabel* m_pImageContainer;
//    QPixmap* m_pImage;

//    void setup();

//private Q_SLOTS:
//    void finished();
//};

///*************************************************
//  MediaRendererConnectionForAudio
//**************************************************/
//class MediaRendererConnectionForAudio : public MediaRendererConnection
//{
//    Q_OBJECT
//    Q_DISABLE_COPY(MediaRendererConnectionForAudio)
//public:
//    MediaRendererConnectionForAudio(QWidget* parent = 0);
//    virtual ~MediaRendererConnectionForAudio();

//protected:

//    virtual qint32 doPlay(const QString& arg);
//    virtual qint32 doStop();

//    virtual qint32 doSeek(const Herqq::Upnp::Av::HSeekInfo& seekInfo);
//    virtual qint32 doNext();
//    virtual qint32 doPrevious();

//    virtual qint32 doSetResource(
//        const QUrl& resourceUri, Herqq::Upnp::Av::HObject* cdsObjectData = 0);

//    virtual qint32 doSelectPreset(const QString& presetName);

//private:
//    Phonon::MediaObject m_mediaObject;
//    Phonon::MediaSource* m_pMediaSource;

//    void setup();

//private Q_SLOTS:

//    void hasVideoChanged(bool);
//    void stateChanged(Phonon::State newstate, Phonon::State oldstate);

//};

///*************************************************
//  MediaRendererConnectionForVideo
//**************************************************/
//class MediaRendererConnectionForVideo : public MediaRendererConnection
//{
//    Q_OBJECT
//    Q_DISABLE_COPY(MediaRendererConnectionForVideo)
//public:
//    MediaRendererConnectionForVideo(QWidget* parent = 0);
//    virtual ~MediaRendererConnectionForVideo();

//protected:

//    virtual qint32 doPlay(const QString& arg);
//    virtual qint32 doStop();

//    virtual qint32 doSeek(const Herqq::Upnp::Av::HSeekInfo& seekInfo);
//    virtual qint32 doNext();
//    virtual qint32 doPrevious();

//    virtual qint32 doSetResource(
//        const QUrl& resourceUri, Herqq::Upnp::Av::HObject* cdsObjectData = 0);

//    virtual qint32 doSelectPreset(const QString& presetName);

//private:
//    Phonon::MediaObject m_mediaObject;
//    Phonon::MediaSource* m_pMediaSource;
//    Phonon::VideoWidget* m_pVideoWidget;

//    void setup();

//private Q_SLOTS:

//    void hasVideoChanged(bool);
//    void stateChanged(Phonon::State newstate, Phonon::State oldstate);
//};

#endif // RENDERERCONNECTION_H
