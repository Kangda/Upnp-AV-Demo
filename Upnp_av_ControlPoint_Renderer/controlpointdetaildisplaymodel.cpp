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

QString ControlPointDetailDisplayModel::NavigatorItemVisitor::findCurrentDeviceUdn(
        ControlPointCdsContainerItem *cdsContainerItem)
{
    ControlPointCdsContainerItem* curItem = cdsContainerItem;

    while (curItem->container()->id() != "0")
    {
        curItem = curItem->parent();
    }

    Q_ASSERT(curItem->container()->id() == "0");

    ControlPointContentDirectoryItem* cdItem =
            dynamic_cast<ControlPointContentDirectoryItem*>(curItem->parent());

    Q_ASSERT(cdItem);

    return cdItem->browser()->contentDirectory()->service()->parentDevice()->info().udn();

}

void ControlPointDetailDisplayModel::NavigatorItemVisitor::visit(
        ControlPointContainerItem *containerItem)
{
    Q_ASSERT(containerItem);

    m_pOwner->beginResetModel();

    m_pOwner->clearModel();

    for(int i = 0; i < item->childCount(); ++i)
    {
        ControlPointDetailDisplayItem* item =
                new ControlPointDetailDisplayItem(ContentDirectory,
                                                  containerItem->child(i),
                                                  0,
                                                  containerItem);
        m_pOwner->m_modelData.append(item);
    }

    m_pOwner->endResetModel();
}

void ControlPointDetailDisplayModel::NavigatorItemVisitor::visit(
        ControlPointContentDirectoryItem *cdItem)
{
    Q_ASSERT(cdItem);

    m_pOwner->beginResetModel();

    m_pOwner->m_rootDeviceUdn =
            cdItem->browser()->contentDirectory()->service()->parentDevice()->info().udn();

    m_pOwner->endResetModel();
}

void ControlPointDetailDisplayModel::NavigatorItemVisitor::visit(
        ControlPointCdsContainerItem *item)
{

}


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

}

QModelIndex ControlPointDetailDisplayModel::parent(const QModelIndex &child) const
{

}

int ControlPointDetailDisplayModel::rowCount(const QModelIndex &parent) const
{

}

int ControlPointDetailDisplayModel::columnCount(const QModelIndex &parent) const
{

}

QVariant ControlPointDetailDisplayModel::data(const QModelIndex &index, int role) const
{

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







