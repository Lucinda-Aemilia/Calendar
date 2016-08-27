#include "cacheeventmodel.h"
#include "event.h"

#include <QDebug>

CacheEventModel::CacheEventModel() : SqlEventModel()
{
    qDebug() << "CacheEventModel constructor";

    // connect(this, SIGNAL(oneDayChanged(QDate)), this, SLOT(refreshOneDay(QDate)));
}

void CacheEventModel::addEvent(QSharedPointer<Event> event)
{
    qDebug() << "CacheEventModel::addEvent  " << event->name();

    SqlEventModel::addEventToDb(event);
    QStringList repeatList(event->repeat().split(','));

    if (repeatList.at(0).toInt() == -1)
        return;

    // 如果event有重复
    int times = repeatList.at(3).toInt();
    int frequency = repeatList.at(2).toInt();
    QString time(repeatList.at(1));
    int id = event->id();

    QString repeat = QString("%1,%2,%3,%4").arg(id).arg(time).arg(frequency).arg(times);
    event->setRepeat(repeat);
    // 需要在这个时候更新repeat
    SqlEventModel::onRepeatChanged(event, repeat);

    QDateTime startDate(event->startDate());
    QDateTime endDate(event->endDate());

    if (time == "day")
    {
        for (int i = 1; i < times; i++)
        {
            QSharedPointer<Event> newEvent(new Event(event->name(), startDate.addDays(frequency * i),
                            endDate.addDays(frequency * i),
                            event->description(), event->location(), event->color(),
                            repeat));
            SqlEventModel::addEventToDb(newEvent);
         }
    }
    else if (time == "week")
    {
        for (int i = 1; i < times; i++)
        {
            QSharedPointer<Event> newEvent(new Event(event->name(), startDate.addDays(frequency * i * 7),
                            endDate.addDays(frequency * i * 7),
                            event->description(), event->location(), event->color(),
                            repeat));
            SqlEventModel::addEventToDb(newEvent);
        }
    }
    else if (time == "month")
    {
        for (int i = 1; i < times; i++)
        {
            QSharedPointer<Event> newEvent(new Event(event->name(), startDate.addMonths(frequency * i),
                            endDate.addDays(frequency * i),
                            event->description(), event->location(), event->color(),
                            repeat));
            SqlEventModel::addEventToDb(newEvent);
        }
    }
    else if (time == "year")
    {
        for (int i = 1; i < times; i++)
        {
            QSharedPointer<Event> newEvent(new Event(event->name(), startDate.addYears(frequency * i),
                            endDate.addYears(frequency * i),
                            event->description(), event->location(), event->color(),
                            repeat));
            SqlEventModel::addEventToDb(newEvent);
        }
    }
}

void CacheEventModel::deleteEvent(QSharedPointer<Event> event, int direc)
{
    SqlEventModel::deleteEventFromDb(event, direc);
}

