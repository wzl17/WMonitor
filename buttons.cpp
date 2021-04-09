#include "buttons.h"

#include <QGridLayout>

Buttons::Buttons(const QString &title, QWidget *parent)
    : QGroupBox(title,parent)
{
    initButtons();
}

Buttons::~Buttons()
{
    delete startButton;
    delete stopButton;
}

void Buttons::initButtons()
{
    startButton = new QPushButton(tr("Start"));
    stopButton = new QPushButton(tr("Stop"));
    QGridLayout *buttonGrid = new QGridLayout;
    buttonGrid->addWidget(startButton, 0, 0);
    buttonGrid->addWidget(stopButton, 0, 1);
    this->setLayout(buttonGrid);
}
