#ifndef CONTROLPOINTWINDOW_H
#define CONTROLPOINTWINDOW_H

#include "controlpointnavigatormodel.h"
#include "controlpointdetaildisplaymodel.h"

#include <QMainWindow>
#include <QTimer>
#include <QStandardItemModel>

#include <HUpnpAv/HUpnpAv>
#include <HUpnpCore/HClientDevice>

class QMenu;
class MediaRendererManager;


namespace Ui {
    class ControlPointWindow;
}

class ControlPointWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ControlPointWindow(QWidget *parent = 0);
    ~ControlPointWindow();

protected:
    virtual void closeEvent(QCloseEvent *);
    void uiInitialization();
    void updateDetailDisplay(ControlPointNavigatorItem*);
    bool getCdsContainerDetail(ControlPointNavigatorItem*);

private:
    Ui::ControlPointWindow *m_pUi;

    bool isFocusOnTreeView;
    QMenu* m_pPopUpMenu;
    QTimer m_timer;
    int m_curChild;

    Herqq::Upnp::Av::HAvControlPoint* m_pControlPoint;
    MediaRendererManager* m_pRendererMgr;

    ControlPointNavigatorModel* m_pNavModel;
    ControlPointDetailDisplayModel* m_pDetailModel;
    ControlPointDetailDisplayItem* m_pLastSelectedItem;



private slots:
    void on_actionRefresh_triggered();
    void on_actionProperties_triggered();
    void on_detaiDisplaylView_pressed(QModelIndex index);
    void on_detaiDisplaylView_clicked(QModelIndex index);
    void on_detaiDisplaylView_activated(QModelIndex index);
    void on_upDirButton_clicked();
    void on_navigatorTreeView_clicked(QModelIndex index);
    void mediaServerOnline(Herqq::Upnp::Av::HMediaServerAdapter*);
    void mediaServerOffline(Herqq::Upnp::Av::HMediaServerAdapter*);
    void mediaRendererOnline(Herqq::Upnp::Av::HMediaRendererAdapter*);
    void mediaRendererOffline(Herqq::Upnp::Av::HMediaRendererAdapter*);
    void objectsBrowserd(Herqq::Upnp::Av::HMediaBrowser*, const QSet<QString>&);
    void preLoadSubContainer();

Q_SIGNALS:
    void startInit();
    void contentSourceRemoved(Herqq::Upnp::HClientDevice*);
    void closed();

};

#endif // CONTROLPOINTWINDOW_H
