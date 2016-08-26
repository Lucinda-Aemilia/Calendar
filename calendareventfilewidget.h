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
    void setCurDateInRange(bool inRange);

    /*
public slots:
    void change
    */

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);

signals:
    void curDateChanged(const QDate& curDate);

public slots:
    void onSelectionChanged(const QDate& curDisplayDate);

private slots:
    void onCurDateChanged(const QDate& curDate);
    void refreshEvents(const QDate& date);
    void refreshFiles(const QDate& date);

    void on_addEventPushButton_clicked();

    void on_eventComboBox_currentIndexChanged(int index);

    void on_eventComboBox_activated(int index);

private:
    Ui::CalendarEventFileWidget *ui;
    QDate mCurDate;
    CacheEventModel* mCacheEventModel;
    QList<Event*> mEvents;
};

#endif // CALENDAREVENTFILEWIDGET_H
