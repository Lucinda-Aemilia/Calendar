#include "calendareventfilewidget.h"
#include "ui_calendareventfilewidget.h"
#include "vieweventdialog.h"

#include <QDebug>
#include <QDragMoveEvent>

CalendarEventFileWidget::CalendarEventFileWidget(CacheEventModel* cacheEventModel, QDate curDate, QWidget *parent) :
    mCacheEventModel(cacheEventModel),
    mCurDate(curDate),
    QWidget(parent),
    ui(new Ui::CalendarEventFileWidget)
{
    ui->setupUi(this);
    onCurDateChanged(mCurDate);

    // 保持events的同步
    refreshEvents(mCurDate);
    // connect((qobject_cast<SqlEventModel*>mCacheEventModel), SIGNAL(OneDayRefreshed(QDate)), this, SLOT(refreshEvents(QDate)));
    connect(mCacheEventModel, SIGNAL(oneDayRefreshed(QDate)), this, SLOT(refreshEvents(QDate)));
    connect(this, SIGNAL(curDateChanged(QDate)), this, SLOT(onCurDateChanged(QDate)));
    // connect(mCacheEventModel, SIGNAL(OneDayChanged(QDate)), this, SLOT(refreshEvents(QDate)));

    // 拖拽
    setAcceptDrops(true);
}

//! [dragEnterEvent() function]
void CalendarEventFileWidget::dragEnterEvent(QDragEnterEvent *event)
{
    /*
    setText(tr("<drop content>"));
    setBackgroundRole(QPalette::Highlight);

    event->acceptProposedAction();
    emit changed(event->mimeData());
    */
}
//! [dragEnterEvent() function]

//! [dragMoveEvent() function]
void CalendarEventFileWidget::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}
//! [dragMoveEvent() function]

//! [dropEvent() function part1]
void CalendarEventFileWidget::dropEvent(QDropEvent *event)
{
    /*
    const QMimeData *mimeData = event->mimeData();
//! [dropEvent() function part1]

//! [dropEvent() function part2]
    if (mimeData->hasImage()) {
        setPixmap(qvariant_cast<QPixmap>(mimeData->imageData()));
    } else if (mimeData->hasHtml()) {
        setText(mimeData->html());
        setTextFormat(Qt::RichText);
    } else if (mimeData->hasText()) {
        setText(mimeData->text());
        setTextFormat(Qt::PlainText);
    } else if (mimeData->hasUrls()) {
        QList<QUrl> urlList = mimeData->urls();
        QString text;
        for (int i = 0; i < urlList.size() && i < 32; ++i) {
            QString url = urlList.at(i).path();
            text += url + QString("\n");
        }
        setText(text);
    } else {
        setText(tr("Cannot display data"));
    }
//! [dropEvent() function part2]

//! [dropEvent() function part3]
    setBackgroundRole(QPalette::Dark);
    event->acceptProposedAction();
    */
}
//! [dropEvent() function part3]

//! [dragLeaveEvent() function]
void CalendarEventFileWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    // clear();
    event->accept();
}
//! [dragLeaveEvent() function]

void CalendarEventFileWidget::refreshEvents(const QDate &date)
{
    qDebug() << "CalendarEventFileWidget::refreshEvents" << date;
    if (date != curDate())
        return;
    mEvents = mCacheEventModel->eventsForDate(mCurDate);
    ui->eventComboBox->clear();
    for (int i = 0; i < mEvents.size(); i++)
    {
        ui->eventComboBox->addItem(mEvents.at(i)->name());
    }
}

void CalendarEventFileWidget::refreshFiles()
{

}

CalendarEventFileWidget::~CalendarEventFileWidget()
{
    delete ui;
}

QDate CalendarEventFileWidget::curDate() const
{
    return mCurDate;
}

void CalendarEventFileWidget::setCurDate(const QDate& curDate)
{
    if (curDate != mCurDate)
    {
        mCurDate = curDate;
        emit curDateChanged(mCurDate);
    }

}

void CalendarEventFileWidget::onCurDateChanged(const QDate& curDate)
{
    ui->curDateLabel->setText(curDate.toString("d"));
    refreshEvents(curDate);
}

void CalendarEventFileWidget::on_addEventPushButton_clicked()
{
    CreateNewEventDialog* dialog = new CreateNewEventDialog;
    dialog->init(mCacheEventModel, NULL, QDateTime(mCurDate, QTime::currentTime()),
                          QDateTime(mCurDate, QTime::currentTime()));
    int result = dialog->exec();
    if (result = QDialog::Accepted)
    {
        Event* event = dialog->getEvent();
    }
}

// 实际上，需要进入浏览编辑界面
void CalendarEventFileWidget::on_eventComboBox_currentIndexChanged(int index)
{

}

void CalendarEventFileWidget::on_eventComboBox_activated(int index)
{
    if (index != ui->eventComboBox->currentIndex())
        return;

    ViewEventDialog* dialog = new ViewEventDialog;
    dialog->init(mCacheEventModel, mEvents.at(index));
    int result = dialog->exec();
    if (result = QDialog::Accepted)
    {
        // Event* event = dialog->getEvent();
    }
}
