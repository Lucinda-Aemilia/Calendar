#ifndef CACHEEVENTMODEL_H
#define CACHEEVENTMODEL_H

#include "sqleventmodel.h"

#include <QSharedPointer>

#include <QObject>
#include <QVector>
#include <QPair>
#include <map>
#include <QDate>
/*
 * 启动时自动读取今年和前后两年内数据。
 * 在切换到新一年时，再多读一年数据。
 * 读入的数据都保存下来。
*/
class CacheEventModel : public SqlEventModel
{
    Q_OBJECT

public:
    CacheEventModel();
    void addEvent(QSharedPointer<Event> event);
    void deleteEvent(QSharedPointer<Event> event, int direc);
};

#endif // CACHEEVENTMODEL_H
