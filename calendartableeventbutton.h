#ifndef CALENDARTABLEEVENTBUTTON_H
#define CALENDARTABLEEVENTBUTTON_H

#include "event.h"

#include <QWidget>
#include <QPushButton>
#include <QSharedPointer>

namespace Ui {
class CalendarTableEventButton;
}

class CalendarTableEventButton : public QPushButton
{
    Q_OBJECT

public:
    explicit CalendarTableEventButton(QSharedPointer<Event> event, QWidget *parent = 0);
    ~CalendarTableEventButton();
    QSharedPointer<Event> event()
    {
        return mEvent;
    }

private:
    Ui::CalendarTableEventButton *ui;
    QSharedPointer<Event> mEvent;
};

#endif // CALENDARTABLEEVENTBUTTON_H
