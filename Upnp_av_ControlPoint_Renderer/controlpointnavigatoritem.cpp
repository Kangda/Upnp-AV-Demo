#include "controlpointnavigatoritem.h"
#include "controlpointnavigatoritemvisitor.h"

#include <HUpnpCore/HDeviceInfo>
#include <HUpnpCore/HClientService>
#include <HUpnpCore/HStateVariableInfo>

#include <HUpnpAv/HContainer>
#include <HUpnpAv/HMediaBrowser>
#include <HUpnpAv/HConnectionInfo>
#include <HUpnpAv/HMediaRendererAdapter>
#include <HUpnpAv/HContentDirectoryAdapter>

#include <QVariant>

#include <QDebug>

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
    //qDebug()<< m_childItems.begin() << tr(" ") << m_childItems.end();
    if (m_childItems.size() > 0)
        qDeleteAll(m_childItems);
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

int ControlPointNavigatorItem::childCount() const
{
    return m_childItems.count();
}

int ControlPointNavigatorItem::columnCount() const
{
    return 1;
}

ControlPointNavigatorItem* ControlPointNavigatorItem::parent() const
{
    return m_pParentItem;
}

int ControlPointNavigatorItem::row() const
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


int ControlPointNavigatorItem::rowCount() const
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
    m_type = Root;
}

ControlPointRootItem::~ControlPointRootItem()
{
}

QVariant ControlPointRootItem::data(int) const
{
    return "Root";
}

void ControlPointRootItem::getDetail(ControlPointNavigatorItemVisitor */*Visitor*/)
{
    //Visitor->visit(this);
}

/************************************
*ControlPointContainerItem
************************************/

ControlPointContainerItem::ControlPointContainerItem(const QString &name,
                                                     ControlPointNavigatorItem *parent):
    ControlPointNavigatorItem(parent),
    m_name(name)
{
    m_type = Container;
}

ControlPointContainerItem::~ControlPointContainerItem()
{
}

QVariant ControlPointContainerItem::data(int) const
{
    return m_name;
}

void ControlPointContainerItem::getDetail(ControlPointNavigatorItemVisitor *Visitor)
{
    Visitor->visit(this);
}

/************************************
*ControlPointContentDirectoryItem
************************************/

ControlPointContentDirectoryItem::ControlPointContentDirectoryItem(
        Herqq::Upnp::Av::HMediaBrowser *browser,
        ControlPointNavigatorItem *parent):
        ControlPointNavigatorItem(parent),
        m_pBrowser(browser)
{
    Q_ASSERT(m_pBrowser);
    m_type = ContentDirectory;
}

ControlPointContentDirectoryItem::~ControlPointContentDirectoryItem()
{
}

QVariant ControlPointContentDirectoryItem::data(int) const
{
    return m_pBrowser->contentDirectory()->service()->parentDevice()->info().friendlyName();
}

void ControlPointContentDirectoryItem::getDetail(ControlPointNavigatorItemVisitor *Visitor)
{
    Visitor->visit(this);
}

/************************************
*ControlPointCdsContainerItem
************************************/

ControlPointCdsContainerItem::ControlPointCdsContainerItem(
        Herqq::Upnp::Av::HContainer *container,
        Herqq::Upnp::Av::HCdsDataSource *datasource,
        ControlPointNavigatorItem *parent):
        ControlPointNavigatorItem(parent),
        m_pContainer(container),
        m_pDataSource(datasource)
{
    Q_ASSERT(m_pDataSource);
    Q_ASSERT(m_pContainer);
    m_type = CdsContainer;
}

ControlPointCdsContainerItem::~ControlPointCdsContainerItem()
{
}

QVariant ControlPointCdsContainerItem::data(int) const
{
    return m_pContainer->title();
}

void ControlPointCdsContainerItem::getDetail(ControlPointNavigatorItemVisitor *Visitor)
{
    Visitor->visit(this);
}
