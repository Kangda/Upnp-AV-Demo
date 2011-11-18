#include "controlpointnavigatoritem.h"

#include <HUpnpCore/HDeviceInfo>
#include <HUpnpCore/HClientService>
#include <HUpnpCore/HStateVariableInfo>

#include <HUpnpAv/HContainer>
#include <HUpnpAv/HMediaBrowser>
#include <HUpnpAv/HConnectionInfo>
#include <HUpnpAv/HMediaRendererAdapter>
#include <HUpnpAv/HContentDirectoryAdapter>

#include <QVariant>

using namespace Herqq::Upnp;
using namespace Herqq::Upnp::Av;

/************************************
*ControlPointNavigatorItem
************************************/

ControlPointNavigatorItem::ControlPointNavigatorItem(ControlPointNavigatorItem* parent):
        m_childItems(),
        m_pParentItem(parent)
{
}

ControlPointNavigatorItem::~ControlPointNavigatorItem()
{
    qDeleteAll(m_childItems);
    delete m_pParentItem;
}

void ControlPointNavigatorItem::appendChild(ControlPointNavigatorItem *child)
{
    Q_ASSERT(child);
    m_childItems.append(child);
}

void ControlPointNavigatorItem::removeChild(qint32 row)
{
    Q_ASSERT(row < m_childItems.size());

    delete m_childItems.at(row);
    m_childItems.removeAt(row);
}

ControlPointNavigatorItem* ControlPointNavigatorItem::child(int row) const
{
    return m_childItems.value(row);
}

int ControlPointNavigatorItem::childCount()
{
    return m_childItems.count();
}

int ControlPointNavigatorItem::columnCount()
{
    return 1;
}

ControlPointNavigatorItem* ControlPointNavigatorItem::parent()
{
    return m_pParentItem;
}

int ControlPointNavigatorItem::row()
{
    if (m_pParentItem)
    {
        for (int i = 0; i < m_pParentItem->m_childItems.count(); ++i)
        {
            if (m_pParentItem->m_childItems.at(i) == this)
            {
                return i;
            }
        }
    }
    return 0;
}


int ControlPointNavigatorItem::rowCount()
{
    qint32 rowCount = childCount();
    foreach(ControlPointNavigatorItem* child, m_childItems)
    {
        rowCount += child->rowCount();
    }
    return rowCount;
}

/************************************
*ControlPointRootItem
************************************/

ControlPointRootItem::ControlPointRootItem(ControlPointNavigatorItem *parent):
        ControlPointNavigatorItem(parent)
{
}

ControlPointRootItem::~ControlPointRootItem()
{
}

QVariant ControlPointRootItem::data(int) const
{
    return "Root";
}

/************************************
*ControlPointContainerItem
************************************/

ControlPointContainerItem::ControlPointContainerItem(const QString &name,
                                                     ControlPointContainerItem *parent):
    m_name(name),
    ControlPointNavigatorItem(parent)
{
}

ControlPointContainerItem::~ControlPointContainerItem()
{
}

QVariant ControlPointContainerItem::data(int) const
{
    return m_name;
}

/************************************
*ControlPointContentDirectoryItem
************************************/

ControlPointContentDirectoryItem::ControlPointContentDirectoryItem(
        Herqq::Upnp::Av::HMediaBrowser *browser,
        ControlPointNavigatorItem *parent):
        m_pBrowser(browser),
        ControlPointNavigatorItem(parent)
{
    Q_ASSERT(m_pBrowser);
}

ControlPointContentDirectoryItem::~ControlPointContentDirectoryItem()
{
    delete m_pBrowser;
}

QVariant ControlPointContentDirectoryItem::data(int) const
{
    return m_pBrowser->contentDirectory()->service()->parentDevice()->info().friendlyName();
}


/************************************
*ControlPointCdsContainerItem
************************************/

ControlPointCdsContainerItem::ControlPointCdsContainerItem(
        Herqq::Upnp::Av::HContainer *container,
        Herqq::Upnp::Av::HCdsDataSource *datasource,
        ControlPointNavigatorItem *parent):
        m_pContainer(container),
        m_pDataSource(datasource),
        ControlPointNavigatorItem(parent)
{
    Q_ASSERT(m_pDataSource);
    Q_ASSERT(m_pContainer);
}

ControlPointCdsContainerItem::~ControlPointCdsContainerItem()
{
    delete m_pDataSource;
    delete m_pContainer;
}

QVariant ControlPointCdsContainerItem::data(int) const
{
    return m_pContainer->title();
}
