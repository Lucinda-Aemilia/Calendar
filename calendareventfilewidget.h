#ifndef CALENDAREVENTFILEWIDGET_H
#define CALENDAREVENTFILEWIDGET_H

#include "cacheeventmodel.h"

#include <QWidget>
#include <QDate>
#include <QSharedPointer>

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
    void dragEnterEvent(QDragEnterEvent* event);
    void dragMoveEvent(QDragMoveEvent* event);
    void dragLeaveEvent(QDragLeaveEvent* event);
    void dropEvent(QDropEvent* event);
    // 翻译
    void changeEvent(QEvent* event);
    void retranslateUi();

signals:
    void curDateChanged(const QDate& curDate);
    // void forceChangeCalendarPage(int year, int month);
    void forceChangeCalendarPagePrevious();
    void forceChangeCalendarPageNext();

public slots:
    void onSelectionChanged(const QDate& curDisplayDate);
    void setFileBoxDrags(bool toggled);

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
    QList<QSharedPointer<Event>> mEvents;
};

#endif // CALENDAREVENTFILEWIDGET_H
