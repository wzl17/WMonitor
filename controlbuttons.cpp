#include "controlbuttons.h"

#include <QGridLayout>

ControlButtons::ControlButtons(const QString &title, QWidget *parent)
    : QGroupBox(title,parent)
{
    initButtons();
}

ControlButtons::~ControlButtons()
{
    delete startButton;
    delete stopButton;
}

void ControlButtons::initButtons()
{
    startButton = new QPushButton(tr("Start"));
    stopButton = new QPushButton(tr("Stop"));
    QGridLayout *buttonGrid = new QGridLayout;
    buttonGrid->addWidget(startButton, 0, 0);
    buttonGrid->addWidget(stopButton, 0, 1);
    this->setLayout(buttonGrid);
}
