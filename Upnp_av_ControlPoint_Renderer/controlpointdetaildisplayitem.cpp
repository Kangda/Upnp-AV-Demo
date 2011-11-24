#include <HUpnpAv/HContainer>
#include <HUpnpAv/HItem>
#include <HUpnpAv/HCdsDataSource>
#include <HUpnpAv/HMediaBrowser>
#include <HUpnpAv/HConnectionInfo>
#include <HUpnpAv/HContentDirectoryAdapter>

#include <HUpnpCore/HClientService>
#include <HUpnpCore/HDeviceInfo>

#include <QModelIndex>
#include <QIcon>

#include "controlpointdetaildisplayitem.h"
#include "controlpointnavigatoritem.h"

using namespace Herqq::Upnp;
using namespace Herqq::Upnp::Av;

ControlPointDetailDisplayItem::ControlPointDetailDisplayItem(DetailDisplayItemType type,
                                                             ControlPointNavigatorItem *navItem,
                                                             HItem *hItem,
                                                             ControlPointNavigatorItem* parent):
    m_pParent(parent),
    QStandardItem()
{
    m_type = type;
    switch (m_type)
    {
    case ContentDirectory:
        m_pNavItem = navItem;
        this->setIcon(QIcon(":/icon/mediaserver.svg"));
        this->setText(dynamic_cast<ControlPointContentDirectoryItem*>(m_pNavItem)->browser()->
                      contentDirectory()->service()->parentDevice()->info().friendlyName());
        break;
    case CdsContainer:
        m_pNavItem = navItem;
        this->setIcon(QIcon(":/icon/folder.svg"));
        this->setText(dynamic_cast<ControlPointCdsContainerItem*>(m_pNavItem)->container()->title());
        break;
    case Item:
        m_pHItem = hItem;
        this->setIcon(getItemIcon(m_pHItem));
        this->setText(m_pHItem->title());
        break;
    }
}

ControlPointDetailDisplayItem::~ControlPointDetailDisplayItem()
{
}

void* ControlPointDetailDisplayItem::itemPointer()
{
    switch (m_type)
    {
    case ContentDirectory:
    case CdsContainer:
        return m_pNavItem;
        break;
    case Item:
        return m_pHItem;
        break;
    default:
        return 0;
        break;
    }
}

QIcon ControlPointDetailDisplayItem::getItemIcon(Herqq::Upnp::Av::HItem *item) const
{
    switch (item->type())
    {
    case HItem::ImageItem:
    case HItem::Photo:
        return QIcon(":/icon/image.svg");
        break;
    case HItem::AudioItem:
        return QIcon(":/icon/audio.svg");
        break;
    case HItem::VideoItem:
    case HItem::Movie:
    case HItem::MusicVideoClip:
        return QIcon(":/icon/video.svg");
        break;
    case HItem::TextItem:
        return QIcon(":/icon/text.svg");
        break;
    default:
        return QIcon(":/icon/empty.svg");
        break;
    }
}

int ControlPointDetailDisplayItem::childCount() const
{
    switch (m_type)
    {
    case ContentDirectory:
    {
        ControlPointContentDirectoryItem* item =
                dynamic_cast<ControlPointContentDirectoryItem*>(m_pNavItem);
        return item->browser()->dataSource()->count();
        break;
    }
    case CdsContainer:
    {
        ControlPointCdsContainerItem* item =
                dynamic_cast<ControlPointCdsContainerItem*>(m_pNavItem);
        return item->container()->childIds().count();
        break;
    }
    case Item:
        return 0;
        break;
    default:
        return 0;
        break;
    }
}

QString ControlPointDetailDisplayItem::toolTip() const
{
    switch (m_type)
    {
    case ContentDirectory:
    case CdsContainer:
        return QString("\"%1\" selected (containing %2 items)").
                arg(this->text(), QString::number(this->childCount()));
        break;
    case Item:
        return QString("\"%1\" selected").arg(this->text());
        break;
    default:
        return QString("");
        break;
    }
}
