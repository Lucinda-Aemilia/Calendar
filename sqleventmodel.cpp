#include "sqleventmodel.h"

#include <QDebug>
#include <QFileInfo>
#include <QSqlError>
#include <QSqlQuery>
#include <QColor>

SqlEventModel::SqlEventModel()
{
    createConnection();
}

QList<Event*> SqlEventModel::eventsForDate(const QDate &date)
{
    const QString queryStr = QString::fromLatin1("SELECT * FROM Event WHERE '%1' >= startDate \
                                                 AND '%1' <= endDate").arg(date.toString("yyyy-MM-dd"));
    QSqlQuery query(queryStr);
    if (!query.exec())
        qFatal("Query failed");

    QList<Event*> events;
    while (query.next())
    {
        Event *event = new Event(this);
        event->setName(query.value(EventDbContract::NAME).toString());

        QDateTime startDate;
        startDate.setDate(query.value(EventDbContract::START_DATE).toDate());
        startDate.setTime(QTime(0, 0).addSecs(query.value(EventDbContract::START_TIME).toInt()));
        event->setStartDate(startDate);

        QDateTime endDate;
        endDate.setDate(query.value(EventDbContract::END_DATE).toDate());
        endDate.setTime(QTime(0, 0).addSecs(query.value(EventDbContract::END_TIME).toInt()));
        event->setEndDate(endDate);

        event->setDescription(query.value(EventDbContract::DESCRIPTION).toString());

        event->setLocation(query.value(EventDbContract::LOCATION).toString());

        QColor color(query.value(EventDbContract::COLOR).toString());
        event->setColor(color);

        events.append(event);
    }

    return events;
}

// slot functions
// 决定干脆先删掉再插入
void SqlEventModel::deleteEventFromDb(Event *event)
{
    QSqlQuery query;
    QString queryString;
    /*
    queryString = QString("DELETE FROM %1 WHERE %2='%3' AND %4='%5' AND %6='%7' AND %8='%9' AND")
                          .arg(EventDbContract::TABLE_NAME)
                          .arg(EventDbContract::NAME)
                          .arg(event->name())
                          .arg(EventDbContract::DESCRIPTION)
                          .arg(event->description())
                          .arg(EventDbContract::START_DATE)
                          .arg(event->startDate().toString("yyyy-MM-dd"))
                          .arg(EventDbContract::START_TIME)
                          .arg(QTime(0, 0).secsTo(event->startDate().time()));
    queryString += QString("%1='%2' AND %3='%4' AND %5='%6' AND %7='%8'")
            .arg(EventDbContract::END_DATE)
            .arg(event->endDate().toString("yyyy-MM-dd"))
            .arg(EventDbContract::END_TIME)
            .arg(QTime(0, 0).secsTo(event->endDate().time()))
            .arg(EventDbContract::LOCATION)
            .arg(event->location())
            .arg(EventDbContract::COLOR)
            .arg(event->color().name());
    */
    queryString = QString("DELETE FROM %1 WHERE %2='%3'")
            .arg(EventDbContract::TABLE_NAME)
            .arg(EventDbContract::id)
            .arg(event->id());
    query.prepare(queryString);
    query.exec();
}

void SqlEventModel::addEventToDb(Event *event)
{
    QSqlQuery query;
    QString queryString;
    queryString = QString("INSERT INTO %1 (%2, %3, %4, %5, %6, %7, %8, %9, ")
            .arg(EventDbContract::TABLE_NAME)
            .arg(EventDbContract::NAME)
            .arg(EventDbContract::DESCRIPTION)
            .arg(EventDbContract::START_DATE)
            .arg(EventDbContract::START_TIME)
            .arg(EventDbContract::END_DATE)
            .arg(EventDbContract::END_TIME)
            .arg(EventDbContract::LOCATION)
            .arg(EventDbContract::COLOR);
    queryString += QString("%1) ").arg(EventDbContract::REPEAT);

    queryString += QString("VALUES ('%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8', '%9')")
            .arg(event->name())
            .arg(event->description())
            .arg(event->startDate().toString("yyyy-MM-dd"))
            .arg(QTime(0, 0).secsTo(event->startDate().time()));
            .arg(event->endDate().toString("yyyy-MM-dd"))
            .arg(QTime(0, 0).secsTo(event->endDate().time()))
            .arg(event->location())
            .arg(event->color().name())
            .arg(event->repeat());

    query.prepare(queryString);
    query.exec();
}

void SqlEventModel::onEventNameChanged(const QString &name)
{
    Event* event = qobject_cast<Event*>(QObject::sender());

}

void SqlEventModel::onStartDateChanged(const QDateTime &startDate)
{

}

void SqlEventModel::onEndDateChanged(const QDateTime &endDate)
{

}

void SqlEventModel::onDescriptionChanged(const QString &description)
{

}

void SqlEventModel::onLocationChanged(const QString &location)
{
  }


void SqlEventModel::onColorChanged(const QColor &color)
{

}


/*
    Defines a helper function to open a connection to an
    in-memory SQLITE database and to create a test table.
*/
void SqlEventModel::createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(EventDbContract::DB_NAME);
    if (!db.open()) {
        qFatal("Cannot open database");
        return;
    }

    QSqlQuery query;
    QString queryString = QString("create table %1 (%2 TEXT, %3 DATE, \
                                  %4 INT, %5 DATE, %6 INT, %7 TEXT, %8 TEXT, \
                                  %9 INTEGER PRIMARY KEY AUTOINCREMENT, ")
            .arg(EventDbContract::TABLE_NAME)
            .arg(EventDbContract::NAME)
            .arg(EventDbContract::START_DATE)
            .arg(EventDbContract::START_TIME)
            .arg(EventDbContract::END_DATE)
            .arg(EventDbContract::END_TIME)
            .arg(EventDbContract::LOCATION)
            .arg(EventDbContract::COLOR)
            .arg(EventDbContract::ID);
    queryString += QString("%1 INT)").arg(EventDbContract::REPEAT);
    // We store the time as seconds because it's easier to query.
    query.exec(queryString);
    return;
}
