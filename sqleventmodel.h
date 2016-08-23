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

    QList<QObject*> eventsForDate(const QDate &date);

private:
    static void createConnection();
};

#endif
