#include "controlpointpropertydialog.h"
#include "ui_controlpointpropertydialog.h"
#include "controlpointdetaildisplayitem.h"
#include "controlpointnavigatoritem.h"

#include <QTableWidgetItem>
#include <QList>
#include <QUrl>

#include <HUpnpAv/HItem>
#include <HUpnpAv/HContainer>
#include <HUpnpAv/HResource>
#include <HUpnpAv/HMediaBrowser>
#include <HUpnpAv/HContentDirectoryAdapter>

#include <HUpnpCore/HUdn>
#include <HUpnpCore/HDeviceInfo>
#include <HUpnpCore/HClientDevice>
#include <HUpnpCore/HClientService>

using namespace Herqq::Upnp;
using namespace Herqq::Upnp::Av;

ControlPointPropertyDialog::ControlPointPropertyDialog(ControlPointDetailDisplayItem* item,
                                                       QWidget *parent) :
    QDialog(parent),
    m_pItem(item),
    m_pUi(new Ui::ControlPointPropertyDialog)
{
    m_pUi->setupUi(this);

    m_pUi->propertyTable->setColumnCount(1);

    this->setTableHeader();
    this->setTableContent();

    m_pUi->propertyTable->resizeColumnToContents(0);

}

ControlPointPropertyDialog::~ControlPointPropertyDialog()
{
    delete m_pUi;
}

void ControlPointPropertyDialog::setTableHeader()
{
    switch (m_pItem->type())
    {
    case ControlPointDetailDisplayItem::ContentDirectory:
        {
            m_pUi->propertyTable->setRowCount(6);
            m_pUi->propertyTable->setVerticalHeaderItem(
                    0, new QTableWidgetItem(tr("Friendly Name")));
            m_pUi->propertyTable->setVerticalHeaderItem(
                    1, new QTableWidgetItem(tr("Device Type")));
            m_pUi->propertyTable->setVerticalHeaderItem(
                    2, new QTableWidgetItem(tr("Model Name")));
            m_pUi->propertyTable->setVerticalHeaderItem(
                    3, new QTableWidgetItem(tr("Manufacturer")));
            m_pUi->propertyTable->setVerticalHeaderItem(
                    4, new QTableWidgetItem(tr("UDN")));
            m_pUi->propertyTable->setVerticalHeaderItem(
                    5, new QTableWidgetItem(tr("Device Discription URL")));
            break;
        }
    case ControlPointDetailDisplayItem::CdsContainer:
        {
            m_pUi->propertyTable->setRowCount(4);
            m_pUi->propertyTable->setVerticalHeaderItem(
                    0, new QTableWidgetItem(tr("Name")));
            m_pUi->propertyTable->setVerticalHeaderItem(
                    1, new QTableWidgetItem(tr("Id")));
            m_pUi->propertyTable->setVerticalHeaderItem(
                    2, new QTableWidgetItem(tr("Type")));
            m_pUi->propertyTable->setVerticalHeaderItem(
                    3, new QTableWidgetItem(tr("Contents")));
            break;
        }
    case ControlPointDetailDisplayItem::Item:
        {
            m_pUi->propertyTable->setRowCount(4);
            m_pUi->propertyTable->setVerticalHeaderItem(
                    0, new QTableWidgetItem(tr("Name")));
            m_pUi->propertyTable->setVerticalHeaderItem(
                    1, new QTableWidgetItem(tr("Id")));
            m_pUi->propertyTable->setVerticalHeaderItem(
                    2, new QTableWidgetItem(tr("Type")));
            m_pUi->propertyTable->setVerticalHeaderItem(
                    3, new QTableWidgetItem(tr("Location")));
            break;
        }
    default:
        break;

    }
}

void ControlPointPropertyDialog::setTableContent()
{
    switch (m_pItem->type())
    {
    case ControlPointDetailDisplayItem::ContentDirectory:
        {
            HClientDevice* device = static_cast<ControlPointContentDirectoryItem*>
                                    (m_pItem->itemPointer())->browser()->
                                    contentDirectory()->service()->parentDevice();
            HDeviceInfo info = device->info();
            m_pUi->propertyTable->setItem(0, 0, new QTableWidgetItem(info.friendlyName()));
            m_pUi->propertyTable->setItem(1, 0, new QTableWidgetItem(info.deviceType().toString()));
            m_pUi->propertyTable->setItem(2, 0, new QTableWidgetItem(info.modelName()));
            m_pUi->propertyTable->setItem(3, 0, new QTableWidgetItem(info.manufacturer()));
            m_pUi->propertyTable->setItem(4, 0, new QTableWidgetItem(info.udn().toString()));
            m_pUi->propertyTable->setItem(5, 0, new QTableWidgetItem(device->locations().
                                                                     at(0).toString()));
            break;
        }
    case ControlPointDetailDisplayItem::CdsContainer:
        {
            HContainer* container = static_cast<ControlPointCdsContainerItem*>
                                    (m_pItem->itemPointer())->container();
            m_pUi->propertyTable->setItem(0, 0, new QTableWidgetItem(container->title()));
            m_pUi->propertyTable->setItem(1, 0, new QTableWidgetItem(container->id()));
            m_pUi->propertyTable->setItem(2, 0, new QTableWidgetItem(tr("Folder")));
            m_pUi->propertyTable->setItem(3, 0, new QTableWidgetItem(tr("%1 items").
                                                                     arg(m_pItem->childCount())));
            break;
        }
    case ControlPointDetailDisplayItem::Item:
        {
            HItem* hItem = static_cast<HItem*>(m_pItem->itemPointer());
            QString resStr;
            for (int i = 0; i < hItem->resources().count(); ++i)
            {
                resStr.append(hItem->resources().at(i).location().toString().append(";"));
            }
            m_pUi->propertyTable->setItem(0, 0, new QTableWidgetItem(hItem->title()));
            m_pUi->propertyTable->setItem(1, 0, new QTableWidgetItem(hItem->id()));
            m_pUi->propertyTable->setItem(2, 0, new QTableWidgetItem(hItem->clazz()));
            m_pUi->propertyTable->setItem(3, 0, new QTableWidgetItem(resStr));
            break;
        }
    default:
        break;

    }
}
