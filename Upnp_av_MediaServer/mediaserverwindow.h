#ifndef MEDIASERVERWINDOW_H
#define MEDIASERVERWINDOW_H

#include <QDialog>
#include <QAction>
#include <QStandardItemModel>

#include <HUpnpAv/HUpnpAv>
#include <HUpnpCore/HDeviceHost>

namespace Ui {
    class MediaServerWindow;
}

class MediaServerWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MediaServerWindow(QWidget *parent = 0);
    ~MediaServerWindow();

protected:
    virtual void closeEvent(QCloseEvent *);

private:
    Ui::MediaServerWindow * m_pUi;

    QStandardItemModel* m_pModel;

    Herqq::Upnp::HDeviceHost* m_pDeviceHost;
    Herqq::Upnp::Av::HFileSystemDataSource* m_pDataSource;

    int lastClickedRow;

    void windowInitialization();
    void mediaServerInitilization();
    void setRootDirectoryTableModel();

    QMenu* rightMouseButtonPopUpMenu;
    QAction* deleteDirectoryAction;


private slots:
    void on_rootDirectoriesTable_pressed(QModelIndex index);
    void on_addDirectoryPushButton_clicked();
    void removeDirectory();

Q_SIGNALS:
    void close();
};

#endif // MEDIASERVERWINDOW_H
