#ifndef BUTTONS_H
#define BUTTONS_H

#include "globals.h"
#include <QPushButton>

QT_BEGIN_NAMESPACE
class QGroupBox;
QT_END_NAMESPACE

class OnOffButtons : public QGroupBox
{
    Q_OBJECT

public:
    OnOffButtons(const QString &title, const QString &on_name, const QString &off_name, QWidget *parent = nullptr);
    virtual ~OnOffButtons();

    QPushButton *onButton;
    QPushButton *offButton;
private:
    /// @brief Initiation of control buttons
    void initButtons();

    QString m_on;
    QString m_off;
};

class StartStopButtons : public QGroupBox
{
    Q_OBJECT

public:
    StartStopButtons(const QString &title, QWidget *parent = nullptr);
    virtual ~StartStopButtons();

    QPushButton *startButton;
    QPushButton *stopButton;
private:
    /// @brief Initiation of control buttons
    void initButtons();
};

#endif // BUTTONS_H
