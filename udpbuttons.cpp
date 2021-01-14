#include "udpbuttons.h"

#include <QGridLayout>

UdpButtons::UdpButtons(const QString &title, QWidget *parent)
    : QGroupBox(title,parent)
{
    initButtons();
}

UdpButtons::~UdpButtons()
{
    delete startButton;
    delete stopButton;
}

void UdpButtons::initButtons()
{
    startButton = new QPushButton(tr("Start"));
    stopButton = new QPushButton(tr("Stop"));
    QGridLayout *buttonGrid = new QGridLayout;
    buttonGrid->addWidget(startButton, 0, 0);
    buttonGrid->addWidget(stopButton, 0, 1);
    this->setLayout(buttonGrid);
}
