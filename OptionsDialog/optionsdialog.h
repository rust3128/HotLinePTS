#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include <QSqlError>


namespace Ui {
class OptionsDialog;
}

class OptionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionsDialog(QWidget *parent = nullptr);
    ~OptionsDialog();

protected:
    void changeEvent(QEvent *e);

private slots:
    void on_buttonBox_rejected();

    void on_checkBoxTray_clicked();

    void on_buttonBox_accepted();

private:
    void createUI();
private:
    Ui::OptionsDialog *ui;
    QHash<uint, QVariant> opt;
    bool optChanged;
};

#endif // OPTIONSDIALOG_H
