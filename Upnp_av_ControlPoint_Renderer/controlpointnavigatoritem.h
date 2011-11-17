#ifndef CONTROLPOINTNAVIGATORITEM_H
#define CONTROLPOINTNAVIGATORITEM_H

#include <QString>
#include <QPointer>

#include <HUpnpCore/HClientDevice>
#include <HUpnpAv/HConnection>

template<typename T>
class QList;

class QVariant;
class ContentDirectoryItem;
class ContentDirectoryContainerItem;


class ControlPointNavigatorItem
{

protected:
    QList<ControlPointNavigatorItem*> m_childItems;
    ControlPointNavigatorItem* m_pParentItem;

public:
    explicit ControlPointNavigatorItem(ControlPointNavigatorItem* parent = 0);
    virtual ~ControlPointNavigatorItem();

    virtual QVariant data(int column) const;

    void appendChild(ControlPointNavigatorItem* child);
    void removeChild(qint32 row);
    ControlPointNavigatorItem* child (int row) const;
    ControlPointNavigatorItem* parent();

    int childCount () const;
    int columnCount() const;

    int row        () const;
    int rowCount   () const;

    //virtual void accept(ControlPointNavigatorItemVisitor*) = 0;

};


#endif // CONTROLPOINTNAVIGATORITEM_H
