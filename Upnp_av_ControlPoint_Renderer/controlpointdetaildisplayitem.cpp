#include "controlpointdetaildisplayitem.h"
#include "controlpointnavigatoritem.h"

#include <HUpnpAv/HContainer>
#include <HUpnpAv/HItem>
#include <HUpnpAv/HMediaBrowser>
#include <HUpnpAv/HConnectionInfo>

#include <QModelIndex>
#include <QIcon>

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
        item.contentDirectoryItem = dynamic_cast<ControlPointContentDirectoryItem>(navItem);
        this->setIcon(QIcon(":/icon/mediaserver.svg"));
        this->setText(item.contentDirectoryItem->browser()->
                      contentDirectory()->service()->parentDevice()->info().friendlyName());
        break;
    case CdsContainer:
        item.cdsContainerItem = dynamic_cast<ControlPointCdsContainerItem>(navItem);
        this->setIcon(QIcon(":/icon/folder.svg"));
        this->setText(item.cdsContainerItem->container()->title());
        break;
    case Item:
        item.normalItem = hItem;
        this->setIcon(getItemIcon(item.normalItem));
        this->setText(item.normalItem->title());
        break;
    }
}

ControlPointDetailDisplayItem::~ControlPointDetailDisplayItem()
{
}

QIcon ControlPointDetailDisplayItem::getItemIcon(HItem *item) const
{
    switch (item->type())
    {
    case ImageItem:
        return QIcon(":/icon/image.svg");
        break;
    case AudioItem:
        return QIcon(":/icon/audio.svg");
        break;
    case VideoItem:
        return QIcon(":/icon/video.svg");
        break;
    case TextItem:
        return QIcon(":/icon/text.svg");
        break;
    }
}
