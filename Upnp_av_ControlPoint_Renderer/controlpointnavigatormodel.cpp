#include "controlpointnavigatormodel.h"
#include "controlpointnavigatoritem.h"

#include <QSet>
#include <QVariant>
#include <QEventLoop>

#include <HUpnpCore/HClientService>

#include <HUpnpAv/HContainer>
#include <HUpnpAv/HProtocolInfo>
#include <HUpnpAv/HMediaBrowser>
#include <HUpnpAv/HCdsDataSource>
#include <HUpnpAv/HMediaServerAdapter>
#include <HUpnpAv/HContentDirectoryAdapter>

using namespace Herqq::Upnp;
using namespace Herqq::Upnp::Av;

ControlPointNavigatorModel::ControlPointNavigatorModel(QObject *parent) :
    QAbstractItemModel(parent),
    m_pRootItem(new ControlPointRootItem()),
    m_pServersItem()
{
    m_pServersItem = new ControlPointContainerItem(
            tr("Media Server"), /*dynamic_cast<ControlPointContainerItem*>*/(m_pRootItem));
    m_pRootItem->appendChild(m_pServersItem);
}

ControlPointNavigatorModel::~ControlPointNavigatorModel()
{
    /*
     You ONLY need to delete the root item, because it will delete the child item while calling
     the deconstruct function of root item in which children are stored in the variable
     m_childItems(QList).
    */
    delete m_pRootItem;
}

HMediaBrowser* ControlPointNavigatorModel::mediaServerOnline(
        Herqq::Upnp::Av::HMediaServerAdapter *deviceAdapter)
{
    HMediaBrowser* browser = new HMediaBrowser(this);

    if (!browser->reset(deviceAdapter->contentDirectory(), false))
    {
        return 0;
    }

    bool ok;

    ok = connect(
                 browser,
                 SIGNAL(objectsBrowsed(Herqq::Upnp::Av::HMediaBrowser*,QSet<QString>)),
                 this,
                 SLOT(objectBrowsered(Herqq::Upnp::Av::HMediaBrowser*,QSet<QString>))
                 );
    Q_ASSERT(ok);

    ok = connect(
                 browser,
                 SIGNAL(browseFailed(Herqq::Upnp::Av::HMediaBrowser*)),
                 this,
                 SLOT(browserFailed(Herqq::Upnp::Av::HMediaBrowser*))
                 );
    Q_ASSERT(ok);

    if (!browser->browseRoot())
    {
        delete browser;
        browser = 0;
    }
    else
    {
        ControlPointContentDirectoryItem* cdsItem =
                new ControlPointContentDirectoryItem(browser, m_pServersItem);

        beginInsertRows(QModelIndex(), m_pRootItem->childCount(), m_pRootItem->childCount());
        m_pServersItem->appendChild(cdsItem);
        endInsertRows();
    }

    return browser;
}

ControlPointCdsContainerItem* ControlPointNavigatorModel::findParentContainer(
        ControlPointCdsContainerItem *rootItem, const QString &id)
{
    for (int i = 0; i < rootItem->childCount(); ++i)
    {
        ControlPointCdsContainerItem *cdsContainerItem =
                static_cast<ControlPointCdsContainerItem*>(rootItem->child(i));
        if (cdsContainerItem->container()->id() == id)
        {
            return cdsContainerItem;
        }
        else
        {
            cdsContainerItem = findParentContainer(
                    dynamic_cast<ControlPointCdsContainerItem*>(rootItem->child(i)),
                    id);
            if (cdsContainerItem)
            {
                return cdsContainerItem;
            }
        }
    }
    return 0;
}


namespace
{
    bool hasContainerItem(const ControlPointCdsContainerItem* cdsContainerItem,
                          const HContainer* container)
    {
        for (int i = 0; i < cdsContainerItem->childCount(); ++i)
        {
            const ControlPointCdsContainerItem* childContainerItem =
                    dynamic_cast<const ControlPointCdsContainerItem*>
                    (cdsContainerItem->child(i));
            if (childContainerItem && childContainerItem->container() == container)
            {
                return true;
            }
            else if (hasContainerItem(childContainerItem, container))
            {
                return true;
            }
        }
        return false;
    }

}


void ControlPointNavigatorModel::objectBrowsered(Herqq::Upnp::Av::HMediaBrowser *browser,
                                                 const QSet<QString> &ids)
{
    HCdsDataSource* cdsDataSource = browser->dataSource();

    ControlPointContentDirectoryItem* cdsItem = 0;
    for (int i = 0; i < m_pServersItem->childCount(); ++i)
    {
        cdsItem = static_cast<ControlPointContentDirectoryItem*>
                  (m_pServersItem->child(i));
        if (cdsItem->browser() == browser)
        {
            break;
        }
    }

    Q_ASSERT(cdsItem);

    foreach(const QString id, ids)
    {
        HContainer* container = cdsDataSource->findContainer(id);
        //if the item is not container,it will display in the detailListView
        if (!container)
        {
            continue;
        }

        /*
         MAYBE the id that is equal to 0 is the first Object that been sended to the Contrl
         Point,so all the objects will be located under the cdsItem->child(0),and it is the
         root Dir for the device.
        */
        if (id != "0")
        {
            ControlPointCdsContainerItem* rootItem =
                    static_cast<ControlPointCdsContainerItem*>(cdsItem->child(0));

            if (hasContainerItem(rootItem, container))
            {
                continue;
            }

            if ("0" == container->parentId())
            {
                ControlPointCdsContainerItem* newCdsContainerItem =
                        new ControlPointCdsContainerItem(container, cdsDataSource, rootItem);
                rootItem->appendChild(newCdsContainerItem);
            }
            else
            {
                ControlPointCdsContainerItem* parentCdsContainerItem =
                        findParentContainer(rootItem, container->parentId());
                if (parentCdsContainerItem)
                {
                    ControlPointCdsContainerItem* newCdsContainerItem =
                            new ControlPointCdsContainerItem(container, cdsDataSource, parentCdsContainerItem);
                    parentCdsContainerItem->appendChild(newCdsContainerItem);
                }
            }
        }
        else
        {
            /*
             The if statment avoid "contents" container appearing too many time after clicking on the existed
             "content" container.
            */
            if (cdsItem->child(0))
            {
                continue;
            }
            ControlPointCdsContainerItem* newCdsContainerItem =
                    new ControlPointCdsContainerItem(container, cdsDataSource, cdsItem);
            cdsItem->appendChild(newCdsContainerItem);
        }
    }
}

void ControlPointNavigatorModel::browserFailed(Herqq::Upnp::Av::HMediaBrowser *browser)
{
}

void ControlPointNavigatorModel::mediaServerOffline(
        Herqq::Upnp::Av::HMediaServerAdapter *deviceAdapter)
{
    for (qint32 i = 0; i < m_pServersItem->childCount(); ++i)
    {
        ControlPointContentDirectoryItem* cdsContentDirectoryItem =
                static_cast<ControlPointContentDirectoryItem*>(m_pServersItem->child(i));

        if (cdsContentDirectoryItem->browser()->contentDirectory()->service()->parentDevice()
            == deviceAdapter->device())
        {
            beginInsertRows(QModelIndex(), i, i);
            m_pServersItem->removeChild(i);
            endInsertRows();
            break;  /*One Device, One ContentDirectoryItem*/
        }
    }
}

int ControlPointNavigatorModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        return static_cast<ControlPointNavigatorItem*>(parent.internalPointer())
                ->columnCount();
    }
    else
    {
        return m_pRootItem->columnCount();
    }
}

QVariant ControlPointNavigatorModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
    {
        return QVariant();
    }

    ControlPointNavigatorItem* item =
            static_cast<ControlPointNavigatorItem*>(index.internalPointer());

    Q_ASSERT(item);

    return item->data(index.column());
}

Qt::ItemFlags ControlPointNavigatorModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return 0;
    }

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QModelIndex ControlPointNavigatorModel::index(int row, int column,
                                              const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    ControlPointNavigatorItem* parentItem = 0;
    if (!parent.isValid())
    {
        parentItem = m_pRootItem;
    }
    else
    {
        parentItem = static_cast<ControlPointNavigatorItem*>(parent.internalPointer());
    }

    Q_ASSERT(parentItem);

    ControlPointNavigatorItem* childItem =parentItem->child(row);

    if (childItem)
    {
        return createIndex(row, column, childItem);
    }
    else
    {
        return QModelIndex();
    }
}

QVariant ControlPointNavigatorModel::headerData(int section,
                                                Qt::Orientation orientation,
                                                int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        return m_pRootItem->data(section);
    }

    return QVariant();
}

QModelIndex ControlPointNavigatorModel::parent(const QModelIndex &child) const
{
    if (!child.isValid() || !child.internalPointer())
    {
        return QModelIndex();
    }

    ControlPointNavigatorItem* childItem =
            static_cast<ControlPointNavigatorItem*>(child.internalPointer());
    ControlPointNavigatorItem* parentItem = childItem->parent();

    Q_ASSERT(parentItem);

    if (parentItem == m_pRootItem)
    {
        return QModelIndex();
    }

    return createIndex(0, 0, parentItem);

}

int ControlPointNavigatorModel::rowCount(const QModelIndex &parent) const
{
    ControlPointNavigatorItem* parentItem = 0;

    if (parent.column() > 0)
    {
        return 0;
    }

    if (!parent.isValid())
    {
        parentItem = m_pRootItem;
    }
    else
    {
        parentItem =
                static_cast<ControlPointNavigatorItem*>(parent.internalPointer());
    }

    Q_ASSERT(parentItem);

    return parentItem->childCount();

}
















