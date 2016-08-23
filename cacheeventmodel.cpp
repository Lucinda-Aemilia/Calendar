#include "cacheeventmodel.h"

CacheEventModel::CacheEventModel(QWidget *parent) : SqlEventModel(parent)
{
    // 在启动时读入3年的数据
    int curYear = QDate::currentDate().year();
    for (int i = curYear-1; i <= curYear+1; i++)
    {
        QList<QList<Event*> > eventsInOneYear;
        for (QDate j(i, 1, 1); j.year() == i; j = j.addDays(1))
        {

        }
        mYearList.append(QPair(i, ));
    }
}
