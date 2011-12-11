#include "controlpointwindow.h"
#include "ui_controlpointwindow.h"
#include "controlpointnavigatoritem.h"
#include "controlpointdetaildisplayitem.h"
#include "controlpointpropertydialog.h"
#include "mediarenderermanager.h"
#include "mediarendererdisplaywindow.h"

#include <QMenu>
#include <QStandardItem>

#include <HUpnpCore/HDeviceInfo>

#include <HUpnpAv/HContainer>
#include <HUpnpAv/HMediaBrowser>
#include <HUpnpAv/HAvControlPoint>
#include <HUpnpAv/HMediaServerAdapter>
#include <HUpnpAv/HMediaRendererAdapter>
#include <HUpnpAv/HAvControlPointConfiguration>

#include <QMessageBox>

using namespace Herqq::Upnp;
using namespace Herqq::Upnp::Av;

ControlPointWindow::ControlPointWindow(QWidget *parent) :
    QMainWindow(parent),
    m_pUi(new Ui::ControlPointWindow),
    m_pControlPoint(0),
    m_pNavModel(0)
{
    m_pUi->setupUi(this);

    isFocusOnTreeView = true;

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

    ok = connect(
            m_pControlPoint,
            SIGNAL(mediaRendererOnline(Herqq::Upnp::Av::HMediaRendererAdapter*)),
            this,
            SLOT(mediaRendererOnline(Herqq::Upnp::Av::HMediaRendererAdapter*)));
    Q_ASSERT(ok);

    ok = connect(&m_timer,
                 SIGNAL(timeout()),
                 this,
                 SLOT(preLoadSubContainer())
                 );
    Q_ASSERT(ok);

    ok = m_pControlPoint->init();
    Q_ASSERT(ok);

    //Initialize Renderer.
    m_pRendererMgr = new MediaRendererManager(m_pControlPoint);

    //Set Model.
    m_pNavModel = new ControlPointNavigatorModel(this);
    m_pUi->navigatorTreeView->setModel(m_pNavModel);

    m_pDetailModel = new ControlPointDetailDisplayModel(this);
    m_pUi->detaiDisplaylView->setModel(m_pDetailModel);

    //Adjust Ui.
    uiInitialization();
}

ControlPointWindow::~ControlPointWindow()
{

    delete m_pUi;
    delete m_pNavModel;
    delete m_pDetailModel;
    delete m_pPopUpMenu;
    delete m_pControlPoint;

}

void ControlPointWindow::uiInitialization()
{
    //For View
    m_pUi->detaiDisplaylView->setGridSize(QSize(120, 100));
    m_pUi->detaiDisplaylView->setSpacing(20);
    m_pUi->detaiDisplaylView->setIconSize(QSize(48, 48));

    //For spiltter
    m_pUi->splitter->setStretchFactor(0, 1);
    m_pUi->splitter->setStretchFactor(1, 2);

    //For Popup menu
    m_pPopUpMenu = new QMenu(this);
    m_pPopUpMenu->addAction(m_pUi->actionProperties);
    m_pUi->actionProperties->setEnabled(false);
}

void ControlPointWindow::mediaServerOnline(
        Herqq::Upnp::Av::HMediaServerAdapter *deviceAdapter)
{
    //QMessageBox::information(this, tr("test"), tr("test"));
    HMediaBrowser* browser = m_pNavModel->mediaServerOnline(deviceAdapter);

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
    m_pNavModel->mediaServerOffline(deviceAdapter);

    if (deviceAdapter->device()->info().udn() == m_pDetailModel->deviceUdn())
    {
        m_pDetailModel->deviceRemoved(deviceAdapter->device()->info().udn());
        updateDetailDisplay(m_pNavModel->serverContainerItem());
    }
}

void ControlPointWindow::mediaRendererOnline(
        Herqq::Upnp::Av::HMediaRendererAdapter *deviceAdapter)
{
    Q_UNUSED(deviceAdapter);
    if (deviceAdapter->device()->info().modelDescription().
        compare("Media Renderer Within Control Point", Qt::CaseInsensitive) == 0)
    {
        m_pRendererMgr->setMediaRendererAdapter(deviceAdapter);
    }
//    QMessageBox::warning(this, tr("mediaRendererOnline()"),
//                         tr("%1").arg(deviceAdapter->device()->parentDevice()->
//                                      info().modelDescription()));
}

void ControlPointWindow::mediaRendererOffline(
        Herqq::Upnp::Av::HMediaRendererAdapter *)
{
}

void ControlPointWindow::closeEvent(QCloseEvent *)
{
    emit closed();
}


void ControlPointWindow::objectsBrowserd(Herqq::Upnp::Av::HMediaBrowser *browser,
                                        const QSet<QString> &ids)
{
    Q_UNUSED(browser);
    Q_UNUSED(ids);

    if (isFocusOnTreeView)
    {
        QModelIndex curIndex = m_pUi->navigatorTreeView->currentIndex();

        if (!curIndex.isValid())
            return;

        ControlPointNavigatorItem* navItem =
                static_cast<ControlPointNavigatorItem*>(curIndex.internalPointer());

        if (navItem)
        {
            updateDetailDisplay(navItem);
        }
    }
}

void ControlPointWindow::updateDetailDisplay(ControlPointNavigatorItem* item)
{
    m_pUi->actionProperties->setEnabled(false);

    m_pDetailModel->init(item);
    m_pUi->pathLine->setText(m_pDetailModel->path(item));

    //prebrowser folders in current dir
//    for (int i = 0; i < item->childCount(); ++i)
//    {
//        getCdsContainerDetail(item->child(i));
//    }
    m_curChild = 0;
    m_timer.start(200);
}

bool ControlPointWindow::getCdsContainerDetail(ControlPointNavigatorItem* navItem)
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
        return false;
    }
    return true;
}

void ControlPointWindow::preLoadSubContainer()
{
    for (;m_curChild < m_pDetailModel->currentItem()->childCount()
        && m_pDetailModel->currentItem()->child(m_curChild)->childCount() != 0;
    m_curChild++)
    {
    }

//    QMessageBox::information(this , tr("ttt"), tr("%1:%2").arg(
//            QString::number(m_curChild),
//            QString::number(m_curChild < m_pDetailModel->currentItem()->childCount() - 1)));

    if (m_curChild >= m_pDetailModel->currentItem()->childCount())
    {
        m_timer.stop();
        return;
    }

    getCdsContainerDetail(m_pDetailModel->currentItem()->child(m_curChild));

    if (m_curChild >= m_pDetailModel->currentItem()->childCount() - 1)
        m_timer.stop();

    ++m_curChild;
}

void ControlPointWindow::on_navigatorTreeView_clicked(QModelIndex index)
{
    isFocusOnTreeView = true;

    ControlPointNavigatorItem* navItem
            = static_cast<ControlPointNavigatorItem*>(index.internalPointer());

    //QMessageBox::information(this, tr("test"), tr("Type: %1").arg(navItem->type()));

    if (navItem->type() == ControlPointNavigatorItem::CdsContainer)
    {
        if (getCdsContainerDetail(navItem))
            updateDetailDisplay(navItem);
    }
    else
        updateDetailDisplay(navItem);
}


void ControlPointWindow::on_upDirButton_clicked()
{
    isFocusOnTreeView = false;
    updateDetailDisplay(m_pDetailModel->currentItem()->parent());
}

void ControlPointWindow::on_detaiDisplaylView_activated(QModelIndex index)
{
    isFocusOnTreeView = false;

    if (index.isValid())
    {
        ControlPointDetailDisplayItem* item =
                static_cast<ControlPointDetailDisplayItem*>(index.internalPointer());

        switch (item->type())
        {
        case ControlPointDetailDisplayItem::ContentDirectory:
        case ControlPointDetailDisplayItem::CdsContainer:
            {
                if (getCdsContainerDetail(static_cast<ControlPointNavigatorItem*>
                                          (item->itemPointer())))
                    updateDetailDisplay(
                            static_cast<ControlPointNavigatorItem*>(item->itemPointer()));
            }
            break;
        case ControlPointDetailDisplayItem::Item:
            //For Player!
            {
                if (!m_pRendererMgr->mediaRendererAdapter())
                {
                    QMessageBox::warning(this,
                                         tr("on_detaiDisplaylView_activated()"),
                                         tr("No Renderering Connection."));
                    break;
                }

                m_pRendererMgr->newDisplayWindow(
                        m_pControlPoint,
                        m_pRendererMgr->mediaRendererAdapter(),
                        static_cast<HItem*>(item->itemPointer()),
                        m_pDetailModel->deviceUdn(),
                        this);
            }
            break;
        default:
            break;
        }
    }

}

void ControlPointWindow::on_detaiDisplaylView_clicked(QModelIndex index)
{
    if (index.isValid())
    {
        m_pUi->actionProperties->setEnabled(true);
        m_pLastSelectedItem =
                static_cast<ControlPointDetailDisplayItem*>(index.internalPointer());

        ControlPointDetailDisplayItem* item =
                static_cast<ControlPointDetailDisplayItem*>(index.internalPointer());

        m_pUi->statusBar->showMessage(item->toolTip());
    }
}

void ControlPointWindow::on_detaiDisplaylView_pressed(QModelIndex index)
{
    if (QApplication::mouseButtons() == Qt::RightButton)
    {
        m_pUi->actionProperties->setEnabled(true);
        m_pLastSelectedItem =
                static_cast<ControlPointDetailDisplayItem*>(index.internalPointer());

        m_pPopUpMenu->exec(QCursor::pos());
    }
}

void ControlPointWindow::on_actionProperties_triggered()
{
    ControlPointPropertyDialog dlg(m_pLastSelectedItem, this);
    dlg.exec();
}

void ControlPointWindow::on_actionRefresh_triggered()
{
    updateDetailDisplay(m_pDetailModel->currentItem());
}
