#ifndef SQLEVENTMODEL_H
#define SQLEVENTMODEL_H

#include <QList>
#include <QObject>

#include "event.h"
#include "eventdbcontract.h"

class SqlEventModel : public QObject
{
    Q_OBJECT

public:
    SqlEventModel();

    QList<Event*> eventsForDate(const QDate &date);

public slots:
    // 可以用 qobject_cast<chatDialog*>(QObject::sender()) 获得sender
    void onEventNameChanged(const QString &name);
    void onStartDateChanged(const QDateTime &startDate);
    void onEndDateChanged(const QDateTime &endDate);
    void onDescriptionChanged(const QString &description);
    void onLocationChanged(const QString &location);
    void onColorChanged(const QColor &color);

    void deleteEventFromDb(Event* event);
    void addEventToDb(Event* event);

private:
    static void createConnection();
};

#endif