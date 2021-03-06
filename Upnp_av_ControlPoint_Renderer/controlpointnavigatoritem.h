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
class ControlPointNavigatorItemVisitor;

/************************************
*ControlPointNavigatorItem
************************************/

class ControlPointNavigatorItem
{
public:

    enum NavigatorItemType
    {
        Normal,
        Root,
        Container,
        ContentDirectory,
        CdsContainer
    };

    explicit ControlPointNavigatorItem(ControlPointNavigatorItem* parent = 0);
    virtual ~ControlPointNavigatorItem();

    void appendChild(ControlPointNavigatorItem* child);
    void removeChild(qint32 row);
    ControlPointNavigatorItem* child (int row) const;
    ControlPointNavigatorItem* parent() const;

    int childCount() const;
    int columnCount() const;

    int row() const;
    int rowCount() const;

    virtual QVariant data(int column) const = 0;
    virtual void getDetail(ControlPointNavigatorItemVisitor*) = 0;
    inline NavigatorItemType type() const
    {
        return m_type;
    }

protected:

    QList<ControlPointNavigatorItem*> m_childItems;
    ControlPointNavigatorItem* m_pParentItem;
    NavigatorItemType m_type;

};

/************************************
*ControlPointRootItem
************************************/
class ControlPointRootItem : public ControlPointNavigatorItem
{
public:
    explicit ControlPointRootItem(ControlPointNavigatorItem* parent = 0);
    virtual ~ControlPointRootItem();

    virtual QVariant data(int) const;

    virtual void getDetail(ControlPointNavigatorItemVisitor*);
};

/************************************
*ControlPointContainerItem
************************************/
class ControlPointContainerItem : public ControlPointNavigatorItem
{
public:
    explicit ControlPointContainerItem(const QString&,
                                       ControlPointNavigatorItem* parent = 0);
    virtual ~ControlPointContainerItem();

    virtual QVariant data(int column) const;
    virtual void getDetail(ControlPointNavigatorItemVisitor*);
private:
    QString m_name;
};


/************************************
*ControlPointCotentDirectoryItem
************************************/
class ControlPointContentDirectoryItem : public ControlPointNavigatorItem
{
public:
    explicit ControlPointContentDirectoryItem(Herqq::Upnp::Av::HMediaBrowser*,
                                              ControlPointNavigatorItem* parent = 0);
    virtual ~ControlPointContentDirectoryItem();

    virtual QVariant data(int) const;

    inline Herqq::Upnp::Av::HMediaBrowser* browser() const
    {
        return m_pBrowser;
    }
    virtual void getDetail(ControlPointNavigatorItemVisitor*);
private:
    Herqq::Upnp::Av::HMediaBrowser* m_pBrowser;

};

/************************************
*ControlPointCdsContainerItem
************************************/
class ControlPointCdsContainerItem : public ControlPointNavigatorItem
{
public:
    explicit ControlPointCdsContainerItem(
            Herqq::Upnp::Av::HContainer*,
            Herqq::Upnp::Av::HCdsDataSource*,
            ControlPointNavigatorItem* parent = 0);
    virtual ~ControlPointCdsContainerItem();

    virtual QVariant data(int) const;

    inline Herqq::Upnp::Av::HContainer* container() const
    {
        return m_pContainer;
    }
    inline Herqq::Upnp::Av::HCdsDataSource* dataSource() const
    {
        return m_pDataSource;
    }

    virtual void getDetail(ControlPointNavigatorItemVisitor*);
private:
    Herqq::Upnp::Av::HContainer* m_pContainer;
    Herqq::Upnp::Av::HCdsDataSource* m_pDataSource;
};























#endif // CONTROLPOINTNAVIGATORITEM_H
