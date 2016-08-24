#ifndef EVENTCALENDAR_H
#define EVENTCALENDAR_H

#include "cacheeventmodel.h"

#include <QCalendarWidget>
#include <QPainter>
#include <qcolor>
#include <qdate>
#include <qpen>
#include <qbrush>
#include <QVector>

QT_BEGIN_NAMESPACE
class QDragEnterEvent;
class QDropEvent;
QT_END_NAMESPACE

class EventCalendar: public QCalendarWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor)

public:
    EventCalendar(QWidget* parent = NULL);
    ~EventCalendar();

    void setColor(QColor& color);
    QColor color() const;

    void setCacheEventModel(CacheEventModel* cacheEventModel);

protected:
    virtual void paintCell(QPainter * painter, const QRect & rect, const QDate & date) const;
    void updateCell(const QDate &date);

private slots:
    void addNote(const QDate & );

private:
    QDate m_currentDate;
    QPen m_outlinePen;
    QBrush m_transparentBrush;

    CacheEventModel* m_cacheEventModel;

};

#endif // EVENTCALENDAR_H
