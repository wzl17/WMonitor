#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "globals.h"
#include "jsondata.h"
#include "chartview.h"
#include "udpsocket.h"
#include "lasercontrol.h"
#include "shutters.h"

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
    /// @brief Read Config.ini for laser parameters setting
    void readLaserConfig();
    /// @brief Write current laser settings to config.ini
    void writeLaserConfig();
    /// @brief Read Config.ini for laser parameters setting
    void readShutterConfig();
    /// @brief Write current laser settings to config.ini
    void writeShutterConfig();
    /// @brief Initiation of UI
    void initUI();

    Ui::MainWindow *ui;
    QWidget *mainWidget;
    QVBoxLayout *mainLayout;
    QTabWidget *laserFreqTabs;
    QHBoxLayout *sideLayout;
    UdpSocket *udpSocket;
    PatternChart *pattern;
    JsonData *json;
    LaserControl *new_laserctrl;
    LaserFreqPlot *new_freqplot;
    ShutterControl *new_shutterctrl;

    /// @brief directory of config.ini
    QString config_dir;

signals:
    void freqReceived();

public slots:
    void statusShow(const QString &s);
};
#endif // MAINWINDOW_H
