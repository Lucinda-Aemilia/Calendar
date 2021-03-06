#ifndef SQLEVENTMODEL_H
#define SQLEVENTMODEL_H

#include <QList>
#include <QObject>
#include <QSharedPointer>

#include "event.h"
#include "eventdbcontract.h"

class SqlEventModel : public QObject
{
    Q_OBJECT

public:
    SqlEventModel();

    QList<QSharedPointer<Event> > eventsForDate(const QDate &date);
    QList<QSharedPointer<Event> > allEvents();

public slots:
    // 可以用 qobject_cast<chatDialog*>(QObject::sender()) 获得sender
    /*
    void onEventNameChanged(const QString &name);
    void onStartDateChanged(const QDateTime &startDate);
    void onEndDateChanged(const QDateTime &endDate);
    void onDescriptionChanged(const QString &description);
    void onLocationChanged(const QString &location);
    void onColorChanged(const QColor &color);
    */
    void onRepeatChanged(QSharedPointer<Event> event, const QString& repeat);

    void deleteEventFromDb(QSharedPointer<Event> event, int deleteRepeatDirect); // 默认只删除自己
    void deleteEventFromDb(QSharedPointer<Event> event);
    void deleteEventFromDb(int id);
    void addEventToDb(QSharedPointer<Event> event);

signals:
    void oneDayChanged(const QDate& date);
    void oneDayRefreshed(const QDate& date);

private:
    static void createConnection();
};

#endif
