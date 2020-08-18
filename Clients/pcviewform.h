#ifndef PCVIEWFORM_H
#define PCVIEWFORM_H

#include <QWidget>

namespace Ui {
class PCViewForm;
}

class PCViewForm : public QWidget
{
    Q_OBJECT

public:
    explicit PCViewForm(QWidget *parent = nullptr);
    ~PCViewForm();

public slots:
    void slotGetObjectID(int ID);
protected:
    void changeEvent(QEvent *e);

private:
    Ui::PCViewForm *ui;
    int objectID;
};

#endif // PCVIEWFORM_H
