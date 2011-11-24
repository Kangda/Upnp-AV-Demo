#ifndef CONTROLPOINTPROPERTYDIALOG_H
#define CONTROLPOINTPROPERTYDIALOG_H

#include <QDialog>

class ControlPointDetailDisplayItem;

namespace Ui {
    class ControlPointPropertyDialog;
}

class ControlPointPropertyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ControlPointPropertyDialog(ControlPointDetailDisplayItem* , QWidget *parent = 0);
    ~ControlPointPropertyDialog();

private:
    Ui::ControlPointPropertyDialog *m_pUi;
    ControlPointDetailDisplayItem* m_pItem;

    void setTableHeader();
    void setTableContent();
};

#endif // CONTROLPOINTPROPERTYDIALOG_H
