#ifndef CONTROLPOINTWINDOW_H
#define CONTROLPOINTWINDOW_H

#include "controlpointnavigatormodel.h"

#include <QMainWindow>
#include <QStandardItemModel>

#include <HUpnpAv/HUpnpAv>
#include <HUpnpCore/HClientDevice>

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

private:
    Ui::ControlPointWindow *m_pUi;

    Herqq::Upnp::Av::HAvControlPoint* m_pControlPoint;

    ControlPointNavigatorModel* m_pModel;


private slots:
    void on_navigatorTreeView_clicked(QModelIndex index);
    void mediaServerOnline(Herqq::Upnp::Av::HMediaServerAdapter*);
    void mediaServerOffline(Herqq::Upnp::Av::HMediaServerAdapter*);
    void objectsBrowserd(Herqq::Upnp::Av::HMediaBrowser*, const QSet<QString>&);

Q_SIGNALS:
    void startInit();
    void contentSourceRemoved(Herqq::Upnp::HClientDevice*);
    void closed();

};

#endif // CONTROLPOINTWINDOW_H
