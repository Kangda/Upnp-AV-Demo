#include "mediaserverwindow.h"
#include "ui_mediaserverwindow.h"

#include <HUpnpCore/HUpnp>
#include <HUpnpCore/HDeviceInfo>
#include <HUpnpCore/HDeviceHostConfiguration>

#include <HUpnpAv/HRootDir>
#include <HUpnpAv/HAvDeviceModelCreator>
#include <HUpnpAv/HMediaServerDeviceConfiguration>
#include <HUpnpAv/HFileSystemDataSource>
#include <HUpnpAv/HFileSystemDataSourceConfiguration>
#include <HUpnpAv/HContentDirectoryServiceConfiguration>
#include <HUpnpAv/HObject>

#include <QDir>
#include <QMenu>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardItem>
#include <QDebug>
#include <QContextMenuEvent>

using namespace Herqq::Upnp;
using namespace Herqq::Upnp::Av;

MediaServerWindow::MediaServerWindow(QWidget *parent) :
    QDialog(parent),
    m_pUi(new Ui::MediaServerWindow),
    m_pModel(0),
    m_pDeviceHost(0),
    m_pDataSource(0)
{
    m_pUi->setupUi(this);

    this->windowInitialization();

    this->mediaServerInitilization();   //Initilize the Media Server

    this->setRootDirectoryTableModel(); //Set the Model

}

MediaServerWindow::~MediaServerWindow()
{
    m_pDeviceHost->quit();

    delete m_pUi;
    delete m_pModel;
    delete m_pDataSource;
    delete m_pDeviceHost;
    //QMessageBox::information(this, tr("test"), tr("test!"));
}

void MediaServerWindow::closeEvent(QCloseEvent *)
{
    emit close();
}

void MediaServerWindow::windowInitialization()
{
    this->lastClickedRow = 0;

    this->setWindowFlags(Qt::WindowMinimizeButtonHint //Show minimize button
                         | Qt::WindowCloseButtonHint //Show close button
                         );
    this->setFixedSize(this->size()); //Fix the size of the main Dialog

    rightMouseButtonPopUpMenu = new QMenu(this);
    deleteDirectoryAction = new QAction(tr("Delete Directory"), this);

    //m_pUi->rootDirectoriesTable->addAction(deleteDirectoryAction);
    //m_pUi->rootDirectoriesTable->setContextMenuPolicy(Qt::ActionsContextMenu);

    rightMouseButtonPopUpMenu->addAction(deleteDirectoryAction);
    connect(deleteDirectoryAction, SIGNAL(triggered()), this, SLOT(removeDirectory()));
}

void MediaServerWindow::mediaServerInitilization()
{
    //DataSource Setting
    HFileSystemDataSourceConfiguration datasourceConfig;

    m_pDataSource = new HFileSystemDataSource(datasourceConfig);

    //ContentDirectory Service Setting
    HContentDirectoryServiceConfiguration cdsConfig;
    cdsConfig.setDataSource(m_pDataSource, false);

    //MediaServer Setting
    HMediaServerDeviceConfiguration mediaServerConfig;
    mediaServerConfig.setContentDirectoryConfiguration(cdsConfig);

    HAvDeviceModelCreator creator;
    creator.setMediaServerConfiguration(mediaServerConfig);

    //Common Device Setting
    HDeviceConfiguration deviceConfig;
    deviceConfig.setPathToDeviceDescription("./descriptions/herqq_mediaserver_description.xml");
    deviceConfig.setCacheControlMaxAge(180);

    //Device Host Setting
    HDeviceHostConfiguration deviceHostConfig;
    deviceHostConfig.setDeviceModelCreator(creator);
    deviceHostConfig.add(deviceConfig);

    //Initialization of the Device Host
    m_pDeviceHost = new HDeviceHost();
    if (!m_pDeviceHost->init(deviceHostConfig))
    {
        Q_ASSERT_X(false, "DeviceHost", m_pDeviceHost->errorDescription().toLocal8Bit());
    }
}

void MediaServerWindow::setRootDirectoryTableModel()
{
    //Model Setting
    m_pModel = new QStandardItemModel(this);
    m_pModel->setHorizontalHeaderLabels(QStringList()
                                        << tr("Recursively")
                                        << tr("Directory Path"));
    m_pUi->rootDirectoriesTable->setModel(m_pModel);
    m_pUi->rootDirectoriesTable->horizontalHeader()->setMinimumSectionSize(80);
    //m_pUi->rootDirectoriesTable->horizontalHeader()->setResizeMode(0, QHeaderView::Interactive);
    m_pUi->rootDirectoriesTable->horizontalHeader()->setResizeMode(1, QHeaderView::Interactive);
    m_pUi->rootDirectoriesTable->horizontalHeader()->setStretchLastSection(true);
    m_pUi->rootDirectoriesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void MediaServerWindow::on_addDirectoryPushButton_clicked()
{
    QString dirName = QFileDialog::getExistingDirectory(this, tr("Choose an Directory"));

    if (!dirName.isEmpty() )
    {
        HRootDir::ScanMode smode =
                m_pUi->scanRecursivelyCheckBox->isChecked()?
                HRootDir::RecursiveScan:
                HRootDir::SingleDirectoryScan;
        HRootDir rootDir(QDir(dirName), smode);
        if (m_pDataSource->add(rootDir))
        {
            int rowCount = m_pModel->rowCount();
            m_pModel->insertRow(rowCount);

            QStandardItem* newItemScanType =
                    new QStandardItem(smode == HRootDir::RecursiveScan ? tr("Yes") : tr("No"));
            newItemScanType->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

            m_pModel->setItem( rowCount, 0, newItemScanType);

            QStandardItem* newItemDirectoryPath =
                    new QStandardItem(rootDir.dir().absolutePath());
            newItemDirectoryPath->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

            m_pModel->setItem( rowCount, 1, newItemDirectoryPath);

        }
        else
        {
            qDebug() << "ERROR: DataSource add dir unsuccessfully.";
        }
    }
    else
    {
        qDebug() << "ERROR: Obtain the Directory Path unsuccessfully.";
    }
}

void MediaServerWindow::removeDirectory()
{
    const HFileSystemDataSourceConfiguration* config = m_pDataSource->configuration();
    HRootDirs rootDirs = config->rootDirs();

    for (int i = 0; i < rootDirs.count(); ++i)
        if (rootDirs.at(i).dir().absolutePath()
            == m_pModel->item(lastClickedRow, 1)->text())
        {
            if (!const_cast<HFileSystemDataSourceConfiguration*>(config)
                    ->removeRootDir(rootDirs.at(i)))
            {
                QMessageBox::information(this, tr("test"), tr("FAILED."));
            }
            m_pModel->removeRow(lastClickedRow);
            break;
        }
}

void MediaServerWindow::on_rootDirectoriesTable_pressed(QModelIndex index)
{
    if (QApplication::mouseButtons() == Qt::RightButton)
    {
        lastClickedRow = index.row();
        rightMouseButtonPopUpMenu->exec(QCursor::pos());
    }
}
