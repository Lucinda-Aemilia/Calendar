#include "sqleventmodel.h"

#include <QDebug>
#include <QFileInfo>
#include <QSqlError>
#include <QSqlQuery>

SqlEventModel::SqlEventModel()
{
    createConnection();
}

QList<QObject*> SqlEventModel::eventsForDate(const QDate &date)
{
    const QString queryStr = QString::fromLatin1("SELECT * FROM Event WHERE '%1' >= startDate \
                                                 AND '%1' <= endDate").arg(date.toString("yyyy-MM-dd"));
    QSqlQuery query(queryStr);
    if (!query.exec())
        qFatal("Query failed");

    QList<QObject*> events;
    while (query.next())
    {
        Event *event = new Event(this);
        event->setName(query.value(EventDbContract::HEADING).toString());

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
    // We store the time as seconds because it's easier to query.
    query.exec(QString("create table %1 (%2 TEXT, %3 DATE, \
                        %4 INT, %5 DATE, %6 INT, %7 TEXT, %8 TEXT)")
                      .arg(EventDbContract::TABLE_NAME)
                      .arg(EventDbContract::HEADING)
                      .arg(EventDbContract::START_DATE)
                      .arg(EventDbContract::START_TIME)
                      .arg(EventDbContract::END_DATE)
                      .arg(EventDbContract::END_TIME)
                      .arg(EventDbContract::LOCATION)
                      .arg(EventDbContract::COLOR));
    query.prepare(QString("INSERT INTO calendar VALUES (:name, :startDate, :startTime)"));
    query.exec("insert into calendar values('Grocery shopping', '2016-08-25', 36000 \
               , '2016-08-25', 39600, 'Xicheng District', 'red')");
    query.exec("insert into calendar values('Ice skating', '2016-08-25', 57600 \
               , '2016-08-25', 61200, 'Haidian District', 'blue')");
    query.exec("insert into calendar values('Doctor''s appointment', '2016-08-25', 57600 \
               , '2016-08-25', 63000, 'Chaoyang District', 'green')");
    query.exec("insert into calendar values('Conference', '2016-08-25', 32400 \
               , '2016-08-27', 61200, 'Xuanwu District', 'darkGreen')");

    return;
}
