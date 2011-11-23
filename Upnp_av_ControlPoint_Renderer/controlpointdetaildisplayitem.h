#ifndef CONTROLPOINTDETAILDISPLAYITEM_H
#define CONTROLPOINTDETAILDISPLAYITEM_H

#include <HUpnpAv/HItem>

#include <QStandardItem>

class ControlPointNavigatorItem;
class ControlPointContentDirectoryItem;
class ControlPointCdsContainerItem;

class QIcon;



/************************************
*ControlPointDetailDisplayItem
************************************/

class ControlPointDetailDisplayItem : public QStandardItem
{
public:

    enum DetailDisplayItemType
    {
        ContentDirectory,
        CdsContainer,
        Item
    };

    explicit ControlPointDetailDisplayItem(DetailDisplayItemType,
                                           ControlPointNavigatorItem*,
                                           Herqq::Upnp::Av::HItem*,
                                           ControlPointNavigatorItem*);
    ~ControlPointDetailDisplayItem();

    inline DetailDisplayItemType type()
    {
        return m_type;
    }
    inline ControlPointNavigatorItem* parent() const
    {
        return m_pParent;
    }

    void* itemPointer();
    int childCount() const;
    QString toolTip() const;

private:
    DetailDisplayItemType m_type;
    ControlPointNavigatorItem* m_pParent;
    ControlPointNavigatorItem* m_pNavItem;
    Herqq::Upnp::Av::HItem* m_pHItem;

    QIcon getItemIcon(Herqq::Upnp::Av::HItem*) const;
};


#endif // CONTROLPOINTDETAILDISPLAYITEM_H
