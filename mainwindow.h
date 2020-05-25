#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void slotExit();
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void on_actionParametrs_triggered();

    void on_actionClientsList_triggered();

private:
    void createUI();
private:
    Ui::MainWindow *ui;
    QSystemTrayIcon  *trayIcon;
    bool toClose = false;

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);
};
#endif // MAINWINDOW_H
