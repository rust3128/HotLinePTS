#ifndef MIGRATEOPTIONSDIALOG_H
#define MIGRATEOPTIONSDIALOG_H

#include <QDialog>
#include <QSqlTableModel>
#include <QSortFilterProxyModel>

struct migOpt {
    int ID;
    QString type;
};



namespace Ui {
class MigrateOptionsDialog;
}

class MigrateOptionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MigrateOptionsDialog(QWidget *parent = nullptr);
    ~MigrateOptionsDialog();

private slots:
    void on_lineEdit_textChanged(const QString &strFind);
    void on_pushButtonAdd_clicked();
    void on_buttonBoxEdit_accepted();
    void on_radioButtonInt_clicked(bool checked);
    void on_radioButtonFloat_clicked(bool checked);
    void on_radioButtonBool_clicked(bool checked);
    void on_radioButtonString_clicked(bool checked);
    void on_radioButtonDate_clicked(bool checked);

    void on_tableView_doubleClicked(const QModelIndex &idx);

    void on_pushButtonClose_clicked();

    void on_buttonBoxEdit_rejected();

private:
    void createUI();
    void createModel();
    bool validateRecord();

private:
    Ui::MigrateOptionsDialog *ui;
    QSqlTableModel *modelMO;
    QSortFilterProxyModel *proxyModel;
    migOpt curMigOpt;
};

#endif // MIGRATEOPTIONSDIALOG_H
