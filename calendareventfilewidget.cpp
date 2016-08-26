#include "calendareventfilewidget.h"
#include "ui_calendareventfilewidget.h"
#include "createneweventdialog.h"
#include "vieweventdialog.h"
#include "file.h"

#include <QDebug>
#include <QDragMoveEvent>
#include <QMimeData>

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

    // 刷新文件列表
    ui->fileComboBox->refreshFileList(mCurDate);

    // 拖拽
    setAcceptDrops(true);
}

//! [dragEnterEvent() function]
void CalendarEventFileWidget::dragEnterEvent(QDragEnterEvent *event)
{
    /*
    setText(tr("<drop content>"));
    setBackgroundRole(QPalette::Highlight);


    emit changed(event->mimeData());
    */
    qDebug() << "CalendarEventFileWidget::dragEnterEvent(QDragEnterEvent *event)";
    event->acceptProposedAction();
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
    const QMimeData *mimeData = event->mimeData();
    //! [dropEvent() function part1]

    //! [dropEvent() function part2]
    if (mimeData->hasUrls())
    {
        QList<QUrl> urlList = mimeData->urls();
        QString text;
        for (int i = 0; i < urlList.size(); ++i) {
            QString url = urlList.at(i).path();
            qDebug() << "CalendarEventFileWidget::dropEvent(QDropEvent *event)" << url;

            // 处理目录格式
            if (url.at(0) == '/')
                url.remove(0, 1);
            File::copyFileToPath(url, mCurDate, true);
            // text += url; // + QString('\n');
        }



        // 刷新文件列表
        ui->fileComboBox->refreshFileList(mCurDate);
    }
    //! [dropEvent() function part2]

    //! [dropEvent() function part3]
    event->acceptProposedAction();
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
    // qDebug() << "CalendarEventFileWidget::refreshEvents" << date;
    if (date != curDate())
        return;
    mEvents = mCacheEventModel->eventsForDate(mCurDate);
    ui->eventComboBox->clear();
    for (int i = 0; i < mEvents.size(); i++)
    {
        ui->eventComboBox->addItem(mEvents.at(i)->name());
    }
}

void CalendarEventFileWidget::refreshFiles(const QDate &date)
{
    ui->fileComboBox->refreshFileList(date);
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

void CalendarEventFileWidget::setCurDateInRange(bool inRange)
{
    // qDebug() << "CalendarEventFileWidget::setCurDateInRange" << inRange;
    if (inRange)
    {
        QPalette pe;
        pe.setColor(QPalette::WindowText, Qt::black);
        ui->curDateLabel->setPalette(pe);
        ui->curDateLabel->setStyleSheet("color:black;");
    }
    else
    {
        QPalette pe;
        pe.setColor(QPalette::WindowText, Qt::gray);
        ui->curDateLabel->setPalette(pe);
        ui->curDateLabel->setStyleSheet("color:gray;");
    }
}

void CalendarEventFileWidget::onCurDateChanged(const QDate& curDate)
{
    ui->curDateLabel->setText(curDate.toString("d"));

    // 刷新事件
    refreshEvents(curDate);
    // 刷新文件
    refreshFiles(curDate);
}

void CalendarEventFileWidget::on_addEventPushButton_clicked()
{
    CreateNewEventDialog* dialog = new CreateNewEventDialog(this);
    dialog->init(mCacheEventModel, NULL, QDateTime(mCurDate, QTime::currentTime()),
                          QDateTime(mCurDate, QTime::currentTime()));
    int result = dialog->exec();
    if (result == QDialog::Accepted)
    {
        Event* event = dialog->getEvent();
    }

    /*
    CreateNewEventDialog* editEventDialog = new CreateNewEventDialog(this);
    editEventDialog->init(m_cacheEventModel, NULL, QDateTime(date, QTime::currentTime()),
                          QDateTime(date, QTime::currentTime()));

    int result = editEventDialog->exec();

    // delete editEventDialog;

    // if( !memo.isEmpty() ){
    if (result == QDialog::Accepted)
    {
        Event* newEvent = editEventDialog->getEvent();
    */
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
    // CreateNewEventDialog* dialog = new CreateNewEventDialog;
    dialog->init(mCacheEventModel, mEvents.at(index));
    int result = dialog->exec();
    if (result = QDialog::Accepted)
    {
        // Event* event = dialog->getEvent();
    }
}

void CalendarEventFileWidget::onSelectionChanged(const QDate& curDisplayDate)
{
    if (curDisplayDate != mCurDate)
    {
        ui->eventLabel->hide();
        ui->eventComboBox->hide();
        ui->addEventPushButton->hide();
        ui->fileLabel->hide();
        ui->fileComboBox->hide();
        ui->addFilePushButton->hide();
    }
    else
    {
        ui->eventLabel->show();
        ui->eventComboBox->show();
        ui->addEventPushButton->show();
        ui->fileLabel->show();
        ui->fileComboBox->show();
        ui->addFilePushButton->show();
    }
}
