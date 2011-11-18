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
    void mediaServerOnline(Herqq::Upnp::Av::HMediaServerAdapter*);
    void mediaServerOffline(Herqq::Upnp::Av::HMediaServerAdapter*);

};

#endif // CONTROLPOINTWINDOW_H
