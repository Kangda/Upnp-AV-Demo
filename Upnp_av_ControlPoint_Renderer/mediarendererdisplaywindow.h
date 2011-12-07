#ifndef RENDERERWINDOW_H
#define RENDERERWINDOW_H

#include <HUpnpAv/HUpnpAv>

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
            const QString&, QNetworkAccessManager*,
            QWidget *parent = 0);
    ~MediaRendererDisplayWindow();

    Herqq::Upnp::Av::HRendererConnection* connection();

protected:
    void closeEvent(QCloseEvent *);
    void resizeEvent(QResizeEvent *);

private:
    Ui::MediaRendererDisplayWindow *m_pUi;
    bool isPlaying;

    MediaRendererConnection* m_pConnection;

private slots:
    void on_stopButton_clicked();
    void on_playButton_clicked();

private Q_SLOTS:
//    void disposed(Herqq::Upnp::Av::HRendererConnection*);
};

#endif // RENDERERWINDOW_H
