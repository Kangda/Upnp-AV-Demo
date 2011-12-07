#include "mediarendererdisplaywindow.h"
#include "ui_mediarendererdisplaywindow.h"
#include "mediarendererconnection.h"

#include <HUpnpAv/HRendererConnection>

using namespace Herqq::Upnp;
using namespace Herqq::Upnp::Av;

MediaRendererDisplayWindow::MediaRendererDisplayWindow(
        const QString &cf, QNetworkAccessManager* netMgr, QWidget *parent) :
    QMainWindow(parent),
    m_pUi(new Ui::MediaRendererDisplayWindow)
{
    m_pUi->setupUi(this);
    isPlaying = false;
}

MediaRendererDisplayWindow::~MediaRendererDisplayWindow()
{
    delete m_pUi;
}

HRendererConnection* MediaRendererDisplayWindow::connection()
{
    return dynamic_cast<HRendererConnection*>(m_pConnection);
}

void MediaRendererDisplayWindow::closeEvent(QCloseEvent *e)
{
 //   this->deleteLater();
    QMainWindow::closeEvent(e);
}

void MediaRendererDisplayWindow::resizeEvent(QResizeEvent *e)
{
    //m_pConnection->resizeEventOccurred(*e);
}

void MediaRendererDisplayWindow::on_playButton_clicked()
{
    if (isPlaying)
    {
        isPlaying = false;
        m_pUi->playButton->setIcon(QIcon(":/icon/start.svg"));
    }
    else
    {
        isPlaying = true;
        m_pUi->playButton->setIcon(QIcon(":/icon/pause.svg"));
    }
}

void MediaRendererDisplayWindow::on_stopButton_clicked()
{
    isPlaying = false;
    m_pUi->playButton->setIcon(QIcon(":/icon/start.svg"));
}
