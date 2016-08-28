#include "calendartableeventbutton.h"
#include "ui_calendartableeventbutton.h"

CalendarTableEventButton::CalendarTableEventButton(QSharedPointer<Event> event, QWidget *parent) :
    QPushButton(parent),
    mEvent(event),
    ui(new Ui::CalendarTableEventButton)
{
    ui->setupUi(this);
    QString text(QString("%1\n%2-%3\n%4%5")
                 .arg(mEvent->name())
                 .arg(mEvent->startDate().time().toString("hh:ss"))
                 .arg(mEvent->endDate().time().toString("hh:ss"))
                 .arg(tr("Location: "))
                 .arg(mEvent->location()));
    setText(text);

    setAutoFillBackground(true);
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, mEvent->color());
    setPalette(palette);
    setStyleSheet(QString("background-color: %1;").arg(mEvent->color().lighter(180).name()));
}

CalendarTableEventButton::~CalendarTableEventButton()
{
    delete ui;
}
