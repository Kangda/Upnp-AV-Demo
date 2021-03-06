#ifndef CONTROLPOINTDETAILDISPLAYMODEL_H
#define CONTROLPOINTDETAILDISPLAYMODEL_H

#include "controlpointnavigatoritemvisitor.h"
#include "controlpointnavigatoritem.h"

#include <HUpnpCore/HUdn>

#include <HUpnpAv/HUpnpAv>

#include <QAbstractItemModel>
#include <QList>

class QModelIndex;
class QVariant;
class ControlPointDetailDisplayItem;

class ControlPointDetailDisplayModel : public QAbstractItemModel
{
    Q_OBJECT
    H_DISABLE_COPY(ControlPointDetailDisplayModel)

    friend class NavigatorItemVisitor;
private:

    class NavigatorItemVisitor : public ControlPointNavigatorItemVisitor
    {
        H_DISABLE_COPY(NavigatorItemVisitor)
    private:
        ControlPointDetailDisplayModel* m_pOwner;

        Herqq::Upnp::HUdn findCurrentDeviceUdn(ControlPointCdsContainerItem*);

    public:
        NavigatorItemVisitor(ControlPointDetailDisplayModel*);
        virtual ~NavigatorItemVisitor();

        virtual void visit(ControlPointContainerItem*);
        virtual void visit(ControlPointContentDirectoryItem*);
        virtual void visit(ControlPointCdsContainerItem*);
    };

    void clearModel();

    QList<ControlPointDetailDisplayItem*> m_modelData;
    ControlPointNavigatorItem* m_pCurItem;
    Herqq::Upnp::HUdn m_rootDeviceUdn;


public:
    explicit ControlPointDetailDisplayModel(QObject *parent = 0);
    ~ControlPointDetailDisplayModel();

    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;

    void deviceRemoved(const Herqq::Upnp::HUdn&);
    void init(ControlPointNavigatorItem*);

    ControlPointNavigatorItem* currentItem() const;
    Herqq::Upnp::HUdn deviceUdn() const;
    QString path(ControlPointNavigatorItem*);

signals:

public slots:

};

#endif // CONTROLPOINTDETAILDISPLAYMODEL_H
