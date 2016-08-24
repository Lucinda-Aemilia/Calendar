#include "eventcalendar.h"

#include "eventcalendar.h"
#include <QtWidgets>
#include <QInputDialog>
#include <QTextCharFormat>
#include <QDebug>
#include <QtGlobal>
// #include "draglabel.h"


EventCalendar::EventCalendar(QWidget* parent) : QCalendarWidget(parent)
{
    qDebug() << "come in EventCalendar constructor";
    m_outlinePen.setColor(Qt::red);
    m_transparentBrush.setColor(Qt::yellow);
    setAcceptDrops(true);

    connect(this, SIGNAL(activated(const QDate &)), this, SLOT(addNote(const QDate &)));
}

EventCalendar::~EventCalendar()
{

}

void EventCalendar::setCacheEventModel(CacheEventModel* cacheEventModel)
{
    qDebug() << "come in setCacheEventModel";
    m_cacheEventModel = cacheEventModel;
    connect(m_cacheEventModel, SIGNAL(oneDayRefreshed(QDate)), this, SLOT(updateCell(QDate)));
    updateCell(QDate::currentDate());
}

void EventCalendar::addNote( const QDate &date )
{
    qDebug()<<"come in addNote"<<endl;
    QString memo = QInputDialog::getText(this,
            "Memo", "Description:" );

    if( !memo.isEmpty() ){
        QBrush brush;
        brush.setColor(Qt::yellow);

        QTextCharFormat cf = this->dateTextFormat(date);
        cf.setBackground(brush);
        this->setDateTextFormat(date, cf);

        Event* event = new Event(memo, QDateTime(date, QTime(0, 0)),
                                 QDateTime(date, QTime(23, 59, 59)));
        m_cacheEventModel->addEvent(event);

    }

}

void EventCalendar::setColor(QColor& color)
{
   m_outlinePen.setColor(color);
}

QColor EventCalendar::color() const
{
   return (m_outlinePen.color());
}

void EventCalendar::updateCell(const QDate &date)
{
    QCalendarWidget::updateCell(date);

    qDebug() << "EventCalendar::updateCell";
    QList<Event*> events = m_cacheEventModel->eventsForDate(date);
    qDebug() << events.size();
    if (events.size() > 0)
    {
        // 设置背景为黄色
        QBrush brush;
        brush.setColor(Qt::yellow);
        QTextCharFormat cf = this->dateTextFormat(date);
        cf.setBackground(brush);
        setDateTextFormat(date, cf);
    }
}

void EventCalendar::paintCell(QPainter *painter, const QRect &rect, const QDate &date) const
{
    // qDebug() << "come in paintCell" << endl;
    QCalendarWidget::paintCell(painter, rect, date);

    /*
    Q_ASSERT(dates.size()==todolist.size());
    for (int i = 0; i < dates.size(); i++)
    {
       if (date == dates.at(i))
       {
           painter->setPen(m_outlinePen);
           painter->setBrush(m_transparentBrush);
           painter->drawRect(rect.adjusted(0, 0, -1, -1));
           painter->drawText(rect,todolist.at(i));
       }
    }

    */

    QList<Event*> events = m_cacheEventModel->eventsForDate(date);
    if (events.size() > 0)
    {
        painter->setPen(m_outlinePen);
        painter->setBrush(QBrush(QColor(255, 255, 0, 140)));
        // 画黄色背景
        painter->drawRect(rect.adjusted(0, 0, -1, -1));
        // qDebug() << "EventCalendar::paintCell  " << date << events.size();
        painter->drawText(rect, Qt::AlignHCenter, events.at(0)->name());
        // painter->drawText(rect, "dead");
        QTableView *tableView = QCalendarWidget::findChild<QTableView*>("qt_calendar_calendarview");
    }

}
