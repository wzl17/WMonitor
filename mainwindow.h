#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "jsondata.h"
#include "controlbuttons.h"
#include "chartview.h"
#include "udpsocket.h"

#include <QtWidgets/QMainWindow>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QLineEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();

private:
    /// @brief Initiation of UI
    void initUI();

    Ui::MainWindow *ui;
    QWidget *mainWidget;
    QHBoxLayout *mainLayout;
    QVBoxLayout *sideLayout;
    UdpSocket *udpSocket;
    ChartView *chartView;
    JsonData *json;
};
#endif // MAINWINDOW_H
