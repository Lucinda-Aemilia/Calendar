#include "calendareventfilewidget.h"
#include "ui_calendareventfilewidget.h"
#include "createneweventdialog.h"

#include <QDebug>

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
    // 增加event
}

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
