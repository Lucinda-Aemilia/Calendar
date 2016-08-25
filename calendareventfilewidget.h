#ifndef CALENDAREVENTFILEWIDGET_H
#define CALENDAREVENTFILEWIDGET_H

#include "cacheeventmodel.h"

#include <QWidget>
#include <QDate>

namespace Ui {
class CalendarEventFileWidget;
}

class CalendarEventFileWidget : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QDate curDate READ curDate WRITE setCurDate NOTIFY curDateChanged)

public:
    explicit CalendarEventFileWidget(CacheEventModel* cacheEventModel, QDate curDate = QDate::currentDate(), QWidget *parent = 0);
    ~CalendarEventFileWidget();
    QDate curDate() const;
    void setCurDate(const QDate& curDate);

signals:
    void curDateChanged(const QDate& curDate);

private slots:
    void onCurDateChanged(const QDate& curDate);
    void refreshEvents(const QDate& date);
    void refreshFiles();

    void on_addEventPushButton_clicked();

private:
    Ui::CalendarEventFileWidget *ui;
    QDate mCurDate;
    CacheEventModel* mCacheEventModel;
    QList<Event*> mEvents;
};

#endif // CALENDAREVENTFILEWIDGET_H
