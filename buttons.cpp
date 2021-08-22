#include "buttons.h"

OnOffButtons::OnOffButtons(const QString &title, const QString &on_name, const QString &off_name, QWidget *parent)
    : QGroupBox(title,parent)
    , m_on(on_name)
    , m_off(off_name)
{
    initButtons();
}

OnOffButtons::~OnOffButtons()
{
    delete onButton;
    delete offButton;
}

void OnOffButtons::initButtons()
{
    onButton = new QPushButton(m_on);
    offButton = new QPushButton(m_off);
    QGridLayout *buttonGrid = new QGridLayout;
    buttonGrid->addWidget(onButton, 0, 0);
    buttonGrid->addWidget(offButton, 0, 1);
    this->setLayout(buttonGrid);
}

StartStopButtons::StartStopButtons(const QString &title, QWidget *parent)
    : QGroupBox(title,parent)
{
    initButtons();
}

StartStopButtons::~StartStopButtons()
{
    delete startButton;
    delete stopButton;
}

void StartStopButtons::initButtons()
{
    startButton = new QPushButton(tr("Start"));
    stopButton = new QPushButton(tr("Stop"));
    QGridLayout *buttonGrid = new QGridLayout;
    buttonGrid->addWidget(startButton, 0, 0);
    buttonGrid->addWidget(stopButton, 0, 1);
    this->setLayout(buttonGrid);
}
