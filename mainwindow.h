#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "globals.h"
#include "jsondata.h"
#include "chartview.h"
#include "udpsocket.h"
#include "lasercontrol.h"

#include <QtWidgets/QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    /// @brief Read Config.ini for parameters setting
    void readConfig();
    /// @brief Write current settings to config.ini
    void writeConfig();
    /// @brief Initiation of UI
    void initUI();

    Ui::MainWindow *ui;
    QWidget *mainWidget;
    QHBoxLayout *mainLayout;
    QVBoxLayout *sideLayout;
    UdpSocket *udpSocket;
    ChartView *chartView;
    JsonData *json;
    LaserControl *laserControl;

    /// @brief directory of config.ini
    QString config_dir;
};
#endif // MAINWINDOW_H
