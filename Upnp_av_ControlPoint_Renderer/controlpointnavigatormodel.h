#ifndef CONTROLPOINTNAVIGATORMODEL_H
#define CONTROLPOINTNAVIGATORMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>

#include <HUpnpCore/HUpnp>
#include <HUpnpAv/HUpnpAv>

class QVariant;

class ControlPointNavigatorItem;
class ControlPointCdsContainerItem;


class ControlPointNavigatorModel : public QAbstractItemModel
{
    Q_OBJECT
    H_DISABLE_COPY(ControlPointNavigatorModel)

public:
    explicit ControlPointNavigatorModel(QObject *parent = 0);
    ~ControlPointNavigatorModel();

    Herqq::Upnp::Av::HMediaBrowser* mediaServerOnline(Herqq::Upnp::Av::HMediaServerAdapter*);
    void mediaServerOffline(Herqq::Upnp::Av::HMediaServerAdapter*);

    virtual QVariant data(const QModelIndex& index, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex& index) const;
    virtual QModelIndex index(
            int row, int column, const QModelIndex& parent = QModelIndex()) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

private:
    inline QModelIndex rootIndex()
    {
        return index(0, 0);
    }
    inline QModelIndex serverIndex()
    {
        return index(0, 0, rootIndex());
    }

    ControlPointCdsContainerItem* findParentContainer(ControlPointCdsContainerItem* rootItem,
                                                      const QString& id);

    ControlPointNavigatorItem* m_pRootItem;
    ControlPointNavigatorItem* m_pServersItem;


public Q_SLOTS:
    void objectBrowsered(Herqq::Upnp::Av::HMediaBrowser*, const QSet<QString>&);
    void browserFailed(Herqq::Upnp::Av::HMediaBrowser*);
    ControlPointNavigatorItem* serverContainerItem() const;

    //void error(Herqq::Upnp::Av::HMediaServerAdapter*,
    //           const Herqq::Upnp::HClientAdapterOp<qint32>&);
    //void connectionRead(Herqq::Upnp::Av::HMediaRendererAdapter*, qint32 id);

    //void connectionDestroyed(QObject*);
};

#endif // CONTROLPOINTNAVIGATORMODEL_H
