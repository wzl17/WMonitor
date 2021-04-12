#ifndef BUTTONS_H
#define BUTTONS_H

#include "globals.h"
#include <QPushButton>

QT_BEGIN_NAMESPACE
class QGroupBox;
QT_END_NAMESPACE

class Buttons : public QGroupBox
{
    Q_OBJECT

public:
    Buttons(const QString &title, QWidget *parent = nullptr);
    virtual ~Buttons();

    QPushButton *startButton;
    QPushButton *stopButton;
private:
    /// @brief Initiation of control buttons
    void initButtons();
};

#endif // BUTTONS_H
