#include "cacheeventmodel.h"
#include "event.h"

#include <QDebug>

CacheEventModel::CacheEventModel() : SqlEventModel()
{
    qDebug() << "CacheEventModel constructor";

    // 在启动时读入3年的数据
    int curYear = QDate::currentDate().year();

    for (int i = curYear-1; i <= curYear+1; i++)
    {
        readOneYear(i);
    }

    // 当某个event被改变时，刷新当日
    connect(this, SIGNAL(oneDayChanged(QDate)), this, SLOT(refreshOneDay(QDate)));
}

void CacheEventModel::addEvent(Event *event)
{
    qDebug() << "CacheEventModel::addEvent  " << event->name();
    SqlEventModel::addEventToDb(event);
    refreshEventInDays(event);

    // 如果event有重复
    QStringList repeatList(event->repeat().split(','));
    int times = repeatList.at(3).toInt();
    int frequency = repeatList.at(2).toInt();
    QString time(repeatList.at(1));
    int id = event->id();

    QString repeat = QString("%1,%2,%3,%4").arg(id).arg(time).arg(frequency).arg(times);
    event->setRepeat(repeat);
    QDateTime startDate(event->startDate());
    QDateTime endDate(event->endDate());

    if (time == "day")
    {
        for (int i = 1; i < times; i++)
        {
            Event* newEvent = new Event(event->name(), startDate.addDays(frequency * i),
                            endDate.addDays(frequency * i),
                            event->description(), event->location(), event->color(),
                            repeat);
            SqlEventModel::addEventToDb(newEvent);
            refreshEventInDays(newEvent);
        }
    }
    else if (time == "week")
    {
        for (int i = 1; i < times; i++)
        {
            Event* newEvent = new Event(event->name(), startDate.addDays(frequency * i * 7),
                            endDate.addDays(frequency * i * 7),
                            event->description(), event->location(), event->color(),
                            repeat);
            SqlEventModel::addEventToDb(newEvent);
            refreshEventInDays(newEvent);
        }
    }
    else if (time == "month")
    {
        for (int i = 1; i < times; i++)
        {
            Event* newEvent = new Event(event->name(), startDate.addMonths(frequency * i),
                            endDate.addDays(frequency * i),
                            event->description(), event->location(), event->color(),
                            repeat);
            SqlEventModel::addEventToDb(newEvent);
            refreshEventInDays(newEvent);
        }
    }
    else if (time == "year")
    {
        for (int i = 1; i < times; i++)
        {
            Event* newEvent = new Event(event->name(), startDate.addYears(frequency * i),
                            endDate.addYears(frequency * i),
                            event->description(), event->location(), event->color(),
                            repeat);
            SqlEventModel::addEventToDb(newEvent);
            refreshEventInDays(newEvent);
        }
    }
}

void CacheEventModel::deleteEvent(Event *event)
{
    SqlEventModel::deleteEventFromDb(event);
    for (QDate i(event->startDate().date()); i <= event->endDate().date(); i = i.addDays(1))
    {
        refreshOneDay(i);
    }
}

void CacheEventModel::refreshEventInDays(Event* event)
{
    for (QDate i (event->startDate().date()); i <= event->endDate().date(); i = i.addDays(1))
        refreshOneDay(i);
}

void CacheEventModel::refreshOneDay(const QDate &date)
{
    QMap<QDate, QList<Event*> >::iterator iter;
    iter = mEvents.find(date);
    if (iter == mEvents.end())
        readOneYear(date.year());

    iter = mEvents.find(date);
    // 删除这些events

    QList<Event*> events = iter.value();
    for (int i = 0; i != events.size(); i++)
    {
        delete events.at(i);
    }

    iter.value() = SqlEventModel::eventsForDate(date);

    emit oneDayRefreshed(date);
}

void CacheEventModel::readOneYear(int year)
{
    qDebug() << "CacheEventModel::readOneYear";
    QMap<QDate, QList<Event*> >::iterator iter;
    for (QDate i(year, 1, 1); i.year() == year; i = i.addDays(1))
    {
        iter = mEvents.find(i);
        if (iter == mEvents.end())
        {
            QList<Event*> events = SqlEventModel::eventsForDate(i);
            mEvents.insert(i, events);
            emit(oneDayRefreshed(i));
        }
    }
}

QList<Event*> CacheEventModel::eventsForDate(const QDate &date)
{
    QMap<QDate, QList<Event*> >::iterator iter;
    qDebug() << date;

    QDate newDate(date);

    iter = mEvents.find(newDate);
    bool flag = false;
    if (iter == mEvents.end())
    {
        readOneYear(date.year());
        flag = true;
    }
    iter = mEvents.find(date);
    return iter.value();

    // 也许这样写不好
    if (flag)
        emit oneDayRefreshed(date);
}
