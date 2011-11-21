#include "controlpointwindow.h"
#include "ui_controlpointwindow.h"
#include "controlpointnavigatoritem.h"

#include <QStandardItem>

#include <HUpnpCore/HDeviceInfo>

#include <HUpnpAv/HContainer>
#include <HUpnpAv/HMediaBrowser>
#include <HUpnpAv/HAvControlPoint>
#include <HUpnpAv/HMediaServerAdapter>
#include <HUpnpAv/HAvControlPointConfiguration>

#include <QMessageBox>

using namespace Herqq::Upnp;
using namespace Herqq::Upnp::Av;

ControlPointWindow::ControlPointWindow(QWidget *parent) :
    QMainWindow(parent),
    m_pUi(new Ui::ControlPointWindow),
    m_pControlPoint(0),
    m_pModel(0)
{
    m_pUi->setupUi(this);

    HAvControlPointConfiguration config;

    m_pControlPoint = new HAvControlPoint(config);

    bool ok;

    ok = connect(m_pControlPoint,
                 SIGNAL(mediaServerOnline(Herqq::Upnp::Av::HMediaServerAdapter*)),
                 this,
                 SLOT(mediaServerOnline(Herqq::Upnp::Av::HMediaServerAdapter*))
                 );
    Q_ASSERT(ok);

    ok = connect(m_pControlPoint,
                 SIGNAL(mediaServerOffline(Herqq::Upnp::Av::HMediaServerAdapter*)),
                 this,
                 SLOT(mediaServerOffline(Herqq::Upnp::Av::HMediaServerAdapter*))
                 );
    Q_ASSERT(ok);

    m_pModel = new ControlPointNavigatorModel(this);
    m_pUi->navigatorTreeView->setModel(m_pModel);

    ok = m_pControlPoint->init();
    Q_ASSERT(ok);

}

ControlPointWindow::~ControlPointWindow()
{

    delete m_pUi;
    delete m_pModel;
    delete m_pControlPoint;

}

void ControlPointWindow::mediaServerOnline(
        Herqq::Upnp::Av::HMediaServerAdapter *deviceAdapter)
{
    //QMessageBox::information(this, tr("test"), tr("test"));
    HMediaBrowser* browser = m_pModel->mediaServerOnline(deviceAdapter);
//    bool ok = connect(
//            browser,
//            SIGNAL(objectsBrowsed(Herqq::Upnp::Av::HMediaBrowser*,QSet<QString>)),
//            this,
//            SLOT(objectsBrowsed(Herqq::Upnp::Av::HMediaBrowser*,QSet<QString>))
//            );
    bool ok = connect(
            browser,
            SIGNAL(objectsBrowsed(Herqq::Upnp::Av::HMediaBrowser*,QSet<QString>)),
            this,
            SLOT(objectsBrowserd(Herqq::Upnp::Av::HMediaBrowser*,QSet<QString>))
            );
    Q_ASSERT(ok);
    Q_UNUSED(ok)

}

void ControlPointWindow::mediaServerOffline(
        Herqq::Upnp::Av::HMediaServerAdapter *deviceAdapter)
{
    //m_pControlPoint->removeMediaServer(deviceAdapter);
    m_pModel->mediaServerOffline(deviceAdapter);
}

void ControlPointWindow::closeEvent(QCloseEvent *)
{
    emit closed();
}


void ControlPointWindow::on_navigatorTreeView_clicked(QModelIndex index)
{

    ControlPointNavigatorItem* navItem
            = static_cast<ControlPointNavigatorItem*>(index.internalPointer());

    //QMessageBox::information(this, tr("test"), tr("Type: %1").arg(navItem->type()));

    if (navItem)
    {
        ControlPointCdsContainerItem* cdsContainerItem =
                dynamic_cast<ControlPointCdsContainerItem*>(navItem);

        if (cdsContainerItem && cdsContainerItem->container()->childIds().isEmpty())
        {
            ControlPointNavigatorItem* curItem = cdsContainerItem->parent();
            ControlPointContentDirectoryItem* parItem = 0;

            do
            {
                parItem = dynamic_cast<ControlPointContentDirectoryItem*>(curItem);
                curItem = curItem->parent();
            }while (!parItem);

            parItem->browser()->browse(HBrowseParams(cdsContainerItem->container()->id(),
                                                     HBrowseParams::ObjectAndDirectChildren));
        }
    }
}

void ControlPointWindow::objectsBrowserd(Herqq::Upnp::Av::HMediaBrowser *browser,
                                        const QSet<QString> &ids)
{

}
