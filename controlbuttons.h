#ifndef CONTROLBUTTONS_H
#define CONTROLBUTTONS_H

#include <QGroupBox>
#include <QPushButton>

QT_BEGIN_NAMESPACE
class QGroupBox;
QT_END_NAMESPACE

class ControlButtons : public QGroupBox
{
    Q_OBJECT

public:
    ControlButtons(const QString &title, QWidget *parent = nullptr);
    virtual ~ControlButtons();

    QPushButton *startButton;
    QPushButton *stopButton;
private:
    /// @brief Initiation of control buttons
    void initButtons();
};

#endif // CONTROLBUTTONS_H
