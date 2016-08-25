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
    // 排序
    const QString queryStr = QString("SELECT * FROM %1 WHERE '%2' >= %3 AND '%2' <= %4 \
ORDER BY %3, %5, %4, %6")
                                     .arg(EventDbContract::TABLE_NAME)
                                     .arg(date.toString("yyyy-MM-dd"))
                                     .arg(EventDbContract::START_DATE)
                                     .arg(EventDbContract::END_DATE)
                                     .arg(EventDbContract::START_TIME)
                                     .arg(EventDbContract::END_TIME);
    QSqlQuery query(queryStr);
    qDebug() << queryStr;
    if (!query.exec())
        qFatal("Query failed");

    QList<Event*> events;
    while (query.next())
    {
        QString name(query.value(EventDbContract::NAME).toString());

        QDateTime startDate;
        startDate.setDate(query.value(EventDbContract::START_DATE).toDate());
        startDate.setTime(QTime(0, 0).addSecs(query.value(EventDbContract::START_TIME).toInt()));

        QDateTime endDate;
        endDate.setDate(query.value(EventDbContract::END_DATE).toDate());
        endDate.setTime(QTime(0, 0).addSecs(query.value(EventDbContract::END_TIME).toInt()));

        QString description(query.value(EventDbContract::DESCRIPTION).toString());

        QString location(query.value(EventDbContract::LOCATION).toString());

        QColor color(query.value(EventDbContract::COLOR).toString());

        int id = query.value(EventDbContract::ID).toInt();

        QString repeat = query.value(EventDbContract::REPEAT).toString();

        Event *event = new Event(name, startDate, endDate, description, location,
                                 color, repeat, id);
        events.append(event);
    }

    /*
    qDebug() << "打印sql语句一共找到了多少events";
    for (int i = 0; i < events.size(); i++)
        qDebug() << events.at(i)->name();
    */

    return events;
}

// slot functions
void SqlEventModel::deleteEventFromDb(Event *event, int deleteRepeatDirect)
{
    // 删除重复事件
    if (deleteRepeatDirect == 0)
    {
        deleteEventFromDb(event);
        return;
    }
    else if (deleteRepeatDirect == -1 || deleteRepeatDirect == 1) // 全部删除
    {
        QSqlQuery query;
        QString queryString;
        queryString = QString("SELECT * FROM %1 ROM %1 WHERE %2='%3' AND  \
%4='%5' AND %6='%7' AND %8='%9' AND")
                .arg(EventDbContract::TABLE_NAME)
                .arg(EventDbContract::NAME)
                .arg(event->name())
                .arg(EventDbContract::DESCRIPTION)
                .arg(event->description())
                .arg(EventDbContract::START_TIME)
                .arg(QTime(0, 0).secsTo(event->startDate().time()))
                .arg(EventDbContract::END_TIME)
                .arg(QTime(0, 0).secsTo(event->endDate().time()));

        queryString += QString("%1='%2' AND %3='%4' AND %5='%6' AND %7='%8'")
                .arg(EventDbContract::LOCATION)
                .arg(event->location())
                .arg(EventDbContract::COLOR)
                .arg(event->color().name());

        if (deleteRepeatDirect > 0)
        {
            queryString += QString("AND %1>='%2'").arg(EventDbContract::START_DATE)
                    .arg(event->startDate().date().toString("yyyy-MM-dd"));
        }
        queryString += ')';

        query.exec(queryString);
        if (query.isActive())
        {
            while (query.next())
            {
                QString repeat = query.value(EventDbContract::REPEAT).toString();
                int repeatFatherId = repeat.split(',').at(0).toInt();
                if (repeatFatherId == event->repeat().split(',').at(0).toInt())
                {
                    int id = query.value(EventDbContract::REPEAT).toInt();
                    deleteEventFromDb(id);
                }
            }
        }
    }
}

void SqlEventModel::deleteEventFromDb(int id)
{
    QSqlQuery query;
    QString queryString;
    queryString = QString("DELETE FROM %1 WHERE %2='%3'")
            .arg(EventDbContract::TABLE_NAME)
            .arg(EventDbContract::ID)
            .arg(id);
    query.prepare(queryString);
    query.exec();
}

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
    queryString += QString("%1='%2' AND %3='%4' AND %5='%6' AND %7='%8')")
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
            .arg(EventDbContract::ID)
            .arg(event->id());
    query.prepare(queryString);
    query.exec();
}

// 添加时需要考虑重复的问题
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
            .arg(QTime(0, 0).secsTo(event->startDate().time()))
            .arg(event->endDate().toString("yyyy-MM-dd"))
            .arg(QTime(0, 0).secsTo(event->endDate().time()))
            .arg(event->location())
            .arg(event->color().name())
            .arg(event->repeat());
    qDebug() << queryString;
    query.prepare(queryString);
    query.exec();

    // 改变event的主键
    query.exec(QString("SELECT * FROM %1 ORDER BY %2 DESC LIMIT 1")
               .arg(EventDbContract::TABLE_NAME)
               .arg(EventDbContract::ID));
    query.first();
    event->setId(query.value(EventDbContract::ID).toInt());

    // 连接槽函数
    connect(event, SIGNAL(nameChanged(QString)), this, SLOT(onEventNameChanged(QString)));
    connect(event, SIGNAL(startDateChanged(QDateTime)), this, SLOT(onStartDateChanged(QDateTime)));
    connect(event, SIGNAL(endDateChanged(QDateTime)), this, SLOT(onEndDateChanged(QDateTime)));
    // to do
    connect(event, SIGNAL(repeatChanged(QString)), this, SLOT(onRepeatChanged(QString)));
}

void SqlEventModel::onEventNameChanged(const QString &name)
{
    Event* event = qobject_cast<Event*>(QObject::sender());
    QSqlQuery query(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'")
                    .arg(EventDbContract::TABLE_NAME)
                    .arg(EventDbContract::NAME)
                    .arg(name)
                    .arg(EventDbContract::ID)
                    .arg(event->id()));
    query.exec();
}

void SqlEventModel::onStartDateChanged(const QDateTime &startDate)
{
    // 改日期和时间
    Event* event = qobject_cast<Event*>(QObject::sender());
    QSqlQuery query(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'")
                    .arg(EventDbContract::TABLE_NAME)
                    .arg(EventDbContract::START_DATE)
                    .arg(startDate.toString("yyyy-MM-dd"))
                    .arg(EventDbContract::ID)
                    .arg(event->id()));
    query.exec();

    query.prepare(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'")
                        .arg(EventDbContract::TABLE_NAME)
                        .arg(EventDbContract::START_TIME)
                        .arg(QTime(0, 0).secsTo(event->startDate().time()))
                        .arg(EventDbContract::ID)
                        .arg(event->id()));
    query.exec();

    for (QDate i(event->startDate().date()); i <= event->endDate().date(); i = i.addDays(1))
        emit oneDayChanged(i);
}

void SqlEventModel::onEndDateChanged(const QDateTime &endDate)
{
    Event* event = qobject_cast<Event*>(QObject::sender());
    QSqlQuery query(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'")
                    .arg(EventDbContract::TABLE_NAME)
                    .arg(EventDbContract::END_DATE)
                    .arg(endDate.toString("yyyy-MM-dd"))
                    .arg(EventDbContract::ID)
                    .arg(event->id()));
    query.exec();

    query.prepare(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'")
                        .arg(EventDbContract::TABLE_NAME)
                        .arg(EventDbContract::END_TIME)
                        .arg(QTime(0, 0).secsTo(event->endDate().time()))
                        .arg(EventDbContract::ID)
                        .arg(event->id()));
    query.exec();

    for (QDate i(event->startDate().date()); i <= event->endDate().date(); i = i.addDays(1))
        emit oneDayChanged(i);
}

void SqlEventModel::onDescriptionChanged(const QString &description)
{
    Event* event = qobject_cast<Event*>(QObject::sender());
    QSqlQuery query(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'")
                    .arg(EventDbContract::TABLE_NAME)
                    .arg(EventDbContract::DESCRIPTION)
                    .arg(description)
                    .arg(EventDbContract::ID)
                    .arg(event->id()));
    query.exec();

    for (QDate i(event->startDate().date()); i <= event->endDate().date(); i = i.addDays(1))
        emit oneDayChanged(i);
}

void SqlEventModel::onLocationChanged(const QString &location)
{
    Event* event = qobject_cast<Event*>(QObject::sender());
    QSqlQuery query(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'")
                    .arg(EventDbContract::TABLE_NAME)
                    .arg(EventDbContract::LOCATION)
                    .arg(location)
                    .arg(EventDbContract::ID)
                    .arg(event->id()));
    query.exec();

    for (QDate i(event->startDate().date()); i <= event->endDate().date(); i = i.addDays(1))
        emit oneDayChanged(i);
}


void SqlEventModel::onColorChanged(const QColor &color)
{
    Event* event = qobject_cast<Event*>(QObject::sender());
    QSqlQuery query(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'")
                    .arg(EventDbContract::TABLE_NAME)
                    .arg(EventDbContract::COLOR)
                    .arg(color.name())
                    .arg(EventDbContract::ID)
                    .arg(event->id()));
    query.exec();

    for (QDate i(event->startDate().date()); i <= event->endDate().date(); i = i.addDays(1))
        emit oneDayChanged(i);
}

void SqlEventModel::onRepeatChanged(const QString& repeat)
{
    Event* event = qobject_cast<Event*>(QObject::sender());
    QSqlQuery query(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'")
                    .arg(EventDbContract::TABLE_NAME)
                    .arg(EventDbContract::REPEAT)
                    .arg(repeat)
                    .arg(EventDbContract::ID)
                    .arg(event->id()));
    query.exec();

    for (QDate i(event->startDate().date()); i <= event->endDate().date(); i = i.addDays(1))
        emit oneDayChanged(i);
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
    QString queryString = QString("create table %1 (%2 TEXT, %3 DATE, %4 INT, \
%5 DATE, %6 INT, %7 TEXT, %8 TEXT, \
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
    queryString += QString("%1 TEXT, %2 TEXT)")
            .arg(EventDbContract::REPEAT)
            .arg(EventDbContract::DESCRIPTION);
    // We store the time as seconds because it's easier to query.
    query.exec(queryString);
    return;
}
