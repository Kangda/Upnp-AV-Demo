#ifndef CONTROLPOINTDETAILDISPLAYITEM_H
#define CONTROLPOINTDETAILDISPLAYITEM_H

#include <QStandardItem>

class ControlPointNavigatorItem;
class ControlPointContentDirectoryItem;
class ControlPointCdsContainerItem;
class HItem;
class QIcon;

enum DetailDisplayItemType
{
    ContentDirectory,
    CdsContainer,
    Item
};

/************************************
*ControlPointDetailDisplayItem
************************************/

class ControlPointDetailDisplayItem : public QStandardItem
{
public:
    explicit ControlPointDetailDisplayItem(DetailDisplayItemType,
                                           ControlPointNavigatorItem*,
                                           HItem*,
                                           ControlPointNavigatorItem* parent);
    ~ControlPointDetailDisplayItem();

    inline DetailDisplayItemType type()
    {
        return m_type;
    }
    inline parent() const
    {
        return m_pParent;
    }

    union item
    {
        ControlPointContentDirectoryItem* contentDirectoryItem;
        ControlPointCdsContainerItem* cdsContainerItem;
        HItem* normalItem;
    };
private:
    DetailDisplayItemType m_type;
    ControlPointNavigatorItem* m_pParent;

    QIcon getItemIcon(HItem*) const;
};


#endif // CONTROLPOINTDETAILDISPLAYITEM_H
