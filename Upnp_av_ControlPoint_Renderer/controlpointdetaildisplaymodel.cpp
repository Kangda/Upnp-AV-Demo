#include "controlpointdetaildisplaymodel.h"
#include "controlpointdetaildisplayitem.h"

#include <HUpnpCore/HDeviceInfo>
#include <HUpnpCore/HServiceInfo>
#include <HUpnpCore/HResourceType>
#include <HUpnpCore/HClientDevice>
#include <HUpnpCore/HClientService>

#include <HUpnpAv/HItem>
#include <HUpnpAv/HResource>
#include <HUpnpAv/HContainer>
#include <HUpnpAv/HProtocolInfo>
#include <HUpnpAv/HMediaBrowser>
#include <HUpnpAv/HCdsDataSource>
#include <HUpnpAv/HConnectionInfo>
#include <HUpnpAv/HMediaRendererAdapter>
#include <HUpnpAv/HContentDirectoryAdapter>

//#include <QSet>
//#include <QUrl>

using namespace Herqq::Upnp;
using namespace Herqq::Upnp::Av;



/********************************************
  NavigatorItemVisitor
********************************************/

ControlPointDetailDisplayModel::NavigatorItemVisitor::NavigatorItemVisitor(
        ControlPointDetailDisplayModel *model) :
        m_pOwner(model)
{
    Q_ASSERT(m_pOwner);
}

ControlPointDetailDisplayModel::NavigatorItemVisitor::~NavigatorItemVisitor()
{
}

HUdn ControlPointDetailDisplayModel::NavigatorItemVisitor::findCurrentDeviceUdn(
        ControlPointCdsContainerItem *cdsContainerItem)
{
    ControlPointCdsContainerItem* curItem = cdsContainerItem;

    while (curItem->container()->id() != "0")
    {
        curItem = dynamic_cast<ControlPointCdsContainerItem*>(curItem->parent());
    }

    Q_ASSERT(curItem->container()->id() == "0");

    ControlPointContentDirectoryItem* cdItem =
            dynamic_cast<ControlPointContentDirectoryItem*>(curItem->parent());

    Q_ASSERT(cdItem);

    return cdItem->browser()->contentDirectory()->
            service()->parentDevice()->info().udn();

}

void ControlPointDetailDisplayModel::NavigatorItemVisitor::visit(
        ControlPointContainerItem *containerItem)
{
    Q_ASSERT(containerItem);

    m_pOwner->m_pCurItem = containerItem;

    m_pOwner->beginResetModel();

    m_pOwner->clearModel();

    for(int i = 0; i < containerItem->childCount(); ++i)
    {
        ControlPointDetailDisplayItem* newitem =
                new ControlPointDetailDisplayItem(ControlPointDetailDisplayItem::ContentDirectory,
                                                  containerItem->child(i),
                                                  0,
                                                  containerItem);
        m_pOwner->m_modelData.append(newitem);
    }

    m_pOwner->endResetModel();
}

void ControlPointDetailDisplayModel::NavigatorItemVisitor::visit(
        ControlPointContentDirectoryItem *cdItem)
{
    Q_ASSERT(cdItem);

    m_pOwner->m_pCurItem = cdItem;

    m_pOwner->m_rootDeviceUdn =
            cdItem->browser()->contentDirectory()->service()->parentDevice()->info().udn();

    m_pOwner->beginResetModel();

    m_pOwner->clearModel();

    for (int i = 0; i < cdItem->childCount(); ++i)
    {
        ControlPointDetailDisplayItem* newitem =
                new ControlPointDetailDisplayItem(ControlPointDetailDisplayItem::CdsContainer,
                                                  cdItem->child(i),
                                                  0,
                                                  cdItem);
        m_pOwner->m_modelData.append(newitem);
    }

    m_pOwner->endResetModel();
}

void ControlPointDetailDisplayModel::NavigatorItemVisitor::visit(
        ControlPointCdsContainerItem *cdsContainerItem)
{
    Q_ASSERT(cdsContainerItem);

    m_pOwner->m_pCurItem = cdsContainerItem;

    m_pOwner->m_rootDeviceUdn = findCurrentDeviceUdn(cdsContainerItem);

    m_pOwner->beginResetModel();

    m_pOwner->clearModel();

    for (int i = 0; i < cdsContainerItem->childCount(); ++i)
    {
        ControlPointDetailDisplayItem* newitem =
                new ControlPointDetailDisplayItem(ControlPointDetailDisplayItem::CdsContainer,
                                                  cdsContainerItem->child(i),
                                                  0,
                                                  cdsContainerItem);
        m_pOwner->m_modelData.append(newitem);
    }

    HItems items = cdsContainerItem->dataSource()->
                   findItems(cdsContainerItem->container()->childIds());
    foreach(HItem *item, items)
    {
        ControlPointDetailDisplayItem* newitem =
                new ControlPointDetailDisplayItem(ControlPointDetailDisplayItem::Item,
                                                  0,
                                                  item,
                                                  cdsContainerItem);
        m_pOwner->m_modelData.append(newitem);
    }

    m_pOwner->endResetModel();

}

//HUdn ControlPointDetailDisplayModel::NavigatorItemVisitor::findCurrentDeviceUdn(
//        ControlPointCdsContainerItem *item)
//{
//    while (item->container()->id() != "0")
//    {
//        item = dynamic_cast<ControlPointCdsContainerItem*>(item->parent());
//    }

//    ControlPointContentDirectoryItem* cdItem =
//            dynamic_cast<ControlPointContentDirectoryItem*>(item->parent());
//    return cdItem->browser()->contentDirectory()->
//            service()->parentDevice()->info().udn();
//}

/********************************************
  ControlPointDetailDisplayModel
********************************************/
ControlPointDetailDisplayModel::ControlPointDetailDisplayModel(QObject *parent) :
    QAbstractItemModel(parent),
    m_modelData(),
    m_rootDeviceUdn()
{
}

ControlPointDetailDisplayModel::~ControlPointDetailDisplayModel()
{
}

QModelIndex ControlPointDetailDisplayModel::index(int row,
                                                  int column,
                                                  const QModelIndex &parent) const
{
    return hasIndex(row, column, parent) ?
            createIndex(row, column, m_modelData.at(row)) :
            QModelIndex();
}

QModelIndex ControlPointDetailDisplayModel::parent(const QModelIndex &/*child*/) const
{
    return QModelIndex();
}

int ControlPointDetailDisplayModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : m_modelData.count();
}

int ControlPointDetailDisplayModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : 1;
}

QVariant ControlPointDetailDisplayModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_modelData.count())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole || Qt::DecorationRole)
        return m_modelData.at(index.row())->data(role);

    return QVariant();
}

void ControlPointDetailDisplayModel::clearModel()
{
    qDeleteAll(m_modelData);
    m_modelData.clear();
}

void ControlPointDetailDisplayModel::deviceRemoved(const Herqq::Upnp::HUdn &udn)
{
    if (m_rootDeviceUdn == udn)
    {
        beginResetModel();
        clearModel();
        endResetModel();
    }
}

void ControlPointDetailDisplayModel::init(ControlPointNavigatorItem *navItem)
{
    NavigatorItemVisitor visitor(this);
    navItem->getDetail(&visitor);
}

ControlPointNavigatorItem* ControlPointDetailDisplayModel::currentItem() const
{
    return m_pCurItem;
}

HUdn ControlPointDetailDisplayModel::deviceUdn() const
{
    return m_rootDeviceUdn;
}

QString ControlPointDetailDisplayModel::path(ControlPointNavigatorItem* item)
{

    switch (item->type())
    {
    case ControlPointNavigatorItem::Root:
         return QString("/");
         break;
    case ControlPointNavigatorItem::Container:
         return QString("/");
         break;
    case ControlPointNavigatorItem:: ContentDirectory:
         return QString("/%1").arg(item->data(0).toString());
         break;
    case ControlPointNavigatorItem::CdsContainer:
         return QString("%1/%2").arg(path(item->parent()), item->data(0).toString());
         break;
    default:
         return QString("");
         break;
    }

}





