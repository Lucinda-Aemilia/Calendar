#include "eventcalendar.h"
#include "createneweventdialog.h"

#include <QtWidgets>
#include <QInputDialog>
#include <QTextCharFormat>
#include <QDebug>
#include <QtGlobal>
// #include "draglabel.h"


EventCalendar::EventCalendar(QWidget* parent) : QCalendarWidget(parent)
{
    // qDebug() << "come in EventCalendar constructor";
    QCalendarWidget::setGridVisible(true);
    m_outlinePen.setColor(Qt::black);
    m_outlinePen.setStyle(Qt::DotLine);
    m_transparentBrush.setColor(Qt::transparent);
    m_unActivatedDatePen.setColor(QColor("gray"));
    m_activatedDatePen.setColor(QColor("black"));
    m_hasMemoBrush.setColor(QColor("yellow"));
    setAcceptDrops(true);

    connect(this, SIGNAL(activated(const QDate &)), this, SLOT(addNote(const QDate &)));

    // 设置列宽
    QTableView *tableView = QCalendarWidget::findChild<QTableView*>("qt_calendar_calendarview");
    tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    tableView->setColumnWidth(0, 50);
    tableView->verticalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    tableView->setRowHeight(0, 50);

    // widget测试
    /*
    QComboBox *comBox = new QComboBox();
    comBox->addItem("Y");
    comBox->addItem("N");
    tableView->setIndexWidget(tableView->model()->index(1, 1), comBox);
    */
    // 拖拽
    setAcceptDrops(true);
    QList<QWidget*> lstChildren = findChildren<QWidget*>();
    foreach (QWidget* pWidget, lstChildren)
    {
        pWidget->setAcceptDrops(true);
    }
}

EventCalendar::~EventCalendar()
{

}

void EventCalendar::setCacheEventModel(CacheEventModel* cacheEventModel)
{
    qDebug() << "come in setCacheEventModel";
    m_cacheEventModel = cacheEventModel;
}

void EventCalendar::addNote(const QDate &date)
{
    qDebug()<<"come in addNote"<<endl;
    /*
    QString memo = QInputDialog::getText(this,
            "Memo", "Description:" );
    */
    CreateNewEventDialog* editEventDialog = new CreateNewEventDialog(this);
    editEventDialog->init(m_cacheEventModel, QSharedPointer<Event>(NULL), QDateTime(date, QTime::currentTime()),
                          QDateTime(date, QTime::currentTime()));

    int result = editEventDialog->exec();

    // delete editEventDialog;

    // if( !memo.isEmpty() ){
    if (result == QDialog::Accepted)
    {
        QSharedPointer<Event> newEvent = editEventDialog->getEvent();
        /*
        QBrush brush;
        brush.setColor(Qt::yellow);

        QTextCharFormat cf = this->dateTextFormat(date);
        cf.setBackground(brush);
        this->setDateTextFormat(date, cf);
        */

        /*
        QSharedPointer<Event> event = new Event(memo, QDateTime(date, QTime(0, 0)),
                                 QDateTime(date, QTime(23, 59, 59)));
        m_cacheEventModel->addEvent(event);
        */
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

void EventCalendar::paintCell(QPainter *painter, const QRect &rect, const QDate &date) const
{
    // qDebug() << "come in paintCell" << endl;
    QCalendarWidget::paintCell(painter, rect, date);

    QList<QSharedPointer<Event>> events = m_cacheEventModel->eventsForDate(date);

    QFont font;
    font.setFamily("Microsoft Yahei UI");
    font.setPointSize(9);
    painter->setFont(font);

    // 先画是否被选中
    if (date == this->selectedDate())
    {
        painter->setPen(m_outlinePen);
        // 白色背景 or 斜线黄色背景，被选中
        painter->setBrush(QBrush(QColor(0, 162, 232)));
        painter->drawRect(rect.adjusted(0, 0, -1, -1));
        painter->setPen(QPen(Qt::transparent));
    }
    else
    {
        painter->setPen(Qt::transparent);
        painter->setBrush(Qt::white);
        painter->drawRect(rect.adjusted(0, 0, -1, -1));
        painter->setPen(QPen(Qt::transparent));
    }


    // 手画日期之类的
    if (events.size() > 0 && date != this->selectedDate())
    {
        // memo色背景
        QColor memoColor(events.at(0)->color());
        memoColor.setAlphaF(0.3);
        painter->setBrush(QBrush(memoColor));
        painter->drawRect(rect.adjusted(0, 0, -1, -1));

        // 日期和待办事项
        /*
        if (date.month() == this->monthShown())
            painter->setPen(m_activatedDatePen);
        else
            painter->setPen(m_unActivatedDatePen);
        QString wordsToShow;
        wordsToShow += date.toString("d") + '\n';
        for (int i = 0; i != events.size(); i++)
        {
            // painter->setPen(QPen(QColor(events.at(i)->color())));
            wordsToShow += events.at(i)->name() + '\n';
        }
        painter->drawText(rect, Qt::AlignHCenter, wordsToShow);
        */
    }
    else
    {
        // 写日期
        /*
        if (date.month() == this->monthShown())
            painter->setPen(m_activatedDatePen);
        else
            painter->setPen(m_unActivatedDatePen);
        painter->drawText(rect, Qt::AlignCenter, date.toString("d"));
        */
    }

}
