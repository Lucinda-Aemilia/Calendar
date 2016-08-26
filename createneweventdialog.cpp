#include "createneweventdialog.h"
#include "ui_createneweventdialog.h"

#include <QDebug>
#include <QRadioButton>
#include <QPushButton>

CreateNewEventDialog::CreateNewEventDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateNewEventDialog)
{
    ui->setupUi(this);

    ui->repeatCheckBox->setCheckState(Qt::Unchecked);
    ui->repetitionGroupBox->hide();
    layout()->setSizeConstraint(QLayout::SetFixedSize);
}

CreateNewEventDialog::~CreateNewEventDialog()
{
    delete ui;
}

void CreateNewEventDialog::fillWithEvent(Event *event)
{
    ui->eventNameLineEdit->setText(event->name());
    ui->startDateTimeEdit->setDateTime(event->startDate());
    ui->endDateTimeEdit->setDateTime(event->endDate());

    QStringList repeatList(event->repeat().split(','));
    if (repeatList.at(0).toInt() != -1)
        ui->repeatCheckBox->setChecked(true);
    else
        ui->repeatCheckBox->setChecked(false);

    ui->locationLineEdit->setText(event->location());
    ui->descriptionTextEdit->setPlainText(event->description());

    if (event->color() == Qt::white)
        ui->colorComboBox->setCurrentIndex(0);
    else if (event->color() == Qt::red)
        ui->colorComboBox->setCurrentIndex(1);
    else if (event->color() == Qt::green)
        ui->colorComboBox->setCurrentIndex(2);
    else if (event->color() == Qt::blue)
        ui->colorComboBox->setCurrentIndex(3);
    else if (event->color() == Qt::cyan)
        ui->colorComboBox->setCurrentIndex(4);
    else if (event->color() == Qt::magenta)
        ui->colorComboBox->setCurrentIndex(5);
    else if (event->color() == Qt::yellow)
        ui->colorComboBox->setCurrentIndex(6);
    else if (event->color() == Qt::gray)
        ui->colorComboBox->setCurrentIndex(7);
    else if (event->color() == Qt::lightGray)
        ui->colorComboBox->setCurrentIndex(8);
}

void CreateNewEventDialog::disableAllEdits(bool disable)
{
    ui->eventNameLineEdit->setDisabled(disable);
    ui->startDateTimeEdit->setDisabled(disable);
    ui->endDateTimeEdit->setDisabled(disable);
    ui->allDayCheckBox->setDisabled(disable);
    ui->repeatCheckBox->setDisabled(disable);
    ui->locationLineEdit->setDisabled(disable);
    ui->descriptionTextEdit->setDisabled(disable);
    ui->colorComboBox->setDisabled(disable);
    ui->eventDetailsGroupBox->setDisabled(disable);
    ui->repetitionGroupBox->setDisabled(disable);
}

void CreateNewEventDialog::setButtonsToViewSet()
{
    ui->dialogButtonBox->clear();
    ui->dialogButtonBox->addButton(tr("Ok"), QDialogButtonBox::AcceptRole);
    ui->dialogButtonBox->addButton(tr("Edit"), QDialogButtonBox::ActionRole);
    ui->dialogButtonBox->addButton(tr("Delete"), QDialogButtonBox::DestructiveRole);
    ui->dialogButtonBox->addButton(tr("Close"), QDialogButtonBox::RejectRole);

    QList<QAbstractButton*> list = ui->dialogButtonBox->buttons();
    qDebug() << list.at(0);
}

void CreateNewEventDialog::setButtonsToEditSet()
{

}

Event* CreateNewEventDialog::getEvent()
{
    qDebug() << "Event* CreateNewEventDialog::getEvent()";
    // 先写添加事件
    // repeat自动处理了
    Event* event = new Event(mName, mStartDate, mEndDate, mDescription,
                             mLocation, mColor, mRepeat);
    mCacheEventModel->addEvent(event);
}

void CreateNewEventDialog::init(CacheEventModel* cacheEventModel, Event *event, const QDateTime& startDate,
                           const QDateTime& endDate)
{
    mCacheEventModel = cacheEventModel;

    ui->eventNameLineEdit->setText(tr("Untitled Event"));
    ui->startDateTimeEdit->setDateTime(startDate);
    ui->endDateTimeEdit->setDateTime(endDate);
    ui->repeatCheckBox->setChecked(false);
    ui->locationLineEdit->setText("");
    ui->descriptionTextEdit->setPlainText("");
    ui->colorComboBox->setCurrentIndex(3);

    ui->repeatCheckBox->setChecked(false);

    generateRepeat();
}

// 结束的时间日期必须比开始的时间日期大
void CreateNewEventDialog::on_startDateTimeEdit_dateTimeChanged(const QDateTime &dateTime)
{
    ui->endDateTimeEdit->setMinimumDateTime(dateTime);
    mStartDate = dateTime;
}

// 是否显示关于重复的groupbox
void CreateNewEventDialog::on_repeatCheckBox_stateChanged(int arg1)
{
    // qDebug() << "EditEventDialog::on_repeatCheckBox_stateChanged" << arg1;

    if (arg1 == Qt::Unchecked)
    {
        ui->repetitionGroupBox->hide();
        mRepeat = -1;
    }
    else if (arg1 == Qt::Checked)
    {
        ui->repetitionGroupBox->show();
        mRepeat = 1;
    }
    layout()->setSizeConstraint(QLayout::SetFixedSize);
    generateRepeat();
}

// 重复的结束日期必须大于结束的日期
void CreateNewEventDialog::on_endDateTimeEdit_dateChanged(const QDate &date)
{
    ui->repeatEndDateEdit->setMinimumDate(date);
}

// 如果为全天，则使两个datetimeedit disable。
// 设置开始和结束时间
void CreateNewEventDialog::on_allDayCheckBox_stateChanged(int arg1)
{
    if (arg1 == Qt::Unchecked)
    {
        ui->startDateTimeEdit->setDisabled(false);
        ui->endDateTimeEdit->setDisabled(false);
    }
    else
    {
        ui->startDateTimeEdit->setDisabled(true);
        ui->endDateTimeEdit->setDisabled(true);
        ui->startDateTimeEdit->setDateTime(QDateTime(ui->startDateTimeEdit->date(), QTime(0, 0)));
        ui->endDateTimeEdit->setDateTime(QDateTime(ui->startDateTimeEdit->date(), QTime(23, 59)));
    }
}

// 改变后面显示的星期数
void CreateNewEventDialog::on_repeatEndDateEdit_dateChanged(const QDate &date)
{
    ui->repeatEndDateLabel->setText(getWeekDayName(date));
    generateRepeat();
}

void CreateNewEventDialog::on_eventNameLineEdit_textChanged(const QString &arg1)
{
    mName = arg1;
    setWindowTitle(arg1);
    generateRepeat();
}

// 改结束时间
void CreateNewEventDialog::on_endDateTimeEdit_dateTimeChanged(const QDateTime &dateTime)
{
    mEndDate = dateTime;
}

void CreateNewEventDialog::on_locationLineEdit_textChanged(const QString &arg1)
{
    mLocation = arg1;
}

void CreateNewEventDialog::on_descriptionTextEdit_textChanged()
{
    mDescription = ui->descriptionTextEdit->toPlainText();
}

void CreateNewEventDialog::on_colorComboBox_currentIndexChanged(const QString &arg1)
{
    mColor = QColor(arg1);
}



void CreateNewEventDialog::on_startDateTimeEdit_dateChanged(const QDate &date)
{
    ui->repeatTimeLabel->setText(getWeekDayName(date));
}

QString CreateNewEventDialog::getWeekDayName(const QDate &date)
{
    QString dayName;
    switch (date.dayOfWeek())
    {
    case 1:
        dayName = QObject::tr("Monday");
        break;
    case 2:
        dayName = QObject::tr("Tuesday");
        break;
    case 3:
        dayName = QObject::tr("Wednesday");
        break;
    case 4:
        dayName = QObject::tr("Thursday");
        break;
    case 5:
        dayName = QObject::tr("Friday");
        break;
    case 6:
        dayName = QObject::tr("Saturday");
        break;
    case 7:
        dayName = QObject::tr("Sunday");
        break;
    }

    return dayName;
}

void CreateNewEventDialog::on_repeatComboBox_currentIndexChanged(int index)
{
    if (index == 1)
        ui->repeatTimeLabel->setText(getWeekDayName(ui->startDateTimeEdit->date()));
    else
        ui->repeatTimeLabel->setText("");
    generateRepeat();
}

void CreateNewEventDialog::on_colorComboBox_currentTextChanged(const QString &arg1)
{
    mColor = QColor(arg1);
}

void CreateNewEventDialog::generateRepeat()
{
    QString repeat;
    if (ui->repeatCheckBox->checkState() == Qt::Unchecked)
    {
        repeat = "-1,,,";
    }
    else
    {
        repeat += "1,";
        switch (ui->repeatComboBox->currentIndex())
        {
        case 0:
            repeat += "day,";
            break;
        case 1:
            repeat += "week,";
            break;
        case 2:
            repeat += "month,";
            break;
        case 3:
            repeat += "year,";
            break;
        }
        repeat += QString::number(ui->frequencyComboBox->currentIndex() + 1) + ",";

        // 从结束日期计算次数
        int times = 0;
        int frequency = ui->frequencyComboBox->currentIndex() + 1;
        if (ui->repeatTimesRadioButton->isChecked())
            times = ui->repeatTimesSpinBox->value();
        else
        {
            QDate endDate(ui->repeatEndDateEdit->date());
            switch (ui->repeatComboBox->currentIndex())
            {
            case 0: // day
                for (QDate i(mStartDate.date()); i <= endDate; i = i.addDays(frequency))
                    times++;
                break;
            case 1: // week
                for (QDate i(mStartDate.date()); i <= endDate; i = i.addDays(7 * frequency))
                    times++;
                break;
            case 2: // month
                for (int i = 0; mStartDate.date().addMonths(i*frequency) <= endDate; i++)
                {
                    times++;
                }
                break;
            case 3: // year
                for (int i = 0; mStartDate.date().addYears(i*frequency) <= endDate; i++)
                {
                    times++;
                }
                break;
            }
        }
        repeat += QString::number(times);
    }

    mRepeat = repeat;
    qDebug() << "Repeat style:" << mRepeat;

    // 计算summary
    QString summary;
    summary = ui->repeatComboBox->currentText();
    if (ui->repeatComboBox->currentIndex() == 1) // week
        summary += tr(", ") + getWeekDayName(ui->startDateTimeEdit->date());
    else if (ui->repeatComboBox->currentIndex() == 2) // month
    {
        int day = ui->startDateTimeEdit->date().day();
        summary += tr(", ") + QString::number(day);
        if (day == 1)
            summary += tr(" st");
        else if (day == 2)
            summary += tr(" nd");
        else if (day == 3)
            summary += tr(" rd");
        else
            summary += tr(" th");
    }
    else if (ui->repeatComboBox->currentIndex() == 3)
    {
        summary += tr(", ") + ui->startDateTimeEdit->date().toString("M.d");
    }
    ui->repeatSummaryLabel->setText(summary);
}

void CreateNewEventDialog::on_frequencyComboBox_currentIndexChanged(int index)
{
    generateRepeat();
}

void CreateNewEventDialog::on_repeatTimesRadioButton_toggled(bool checked)
{
    generateRepeat();
}

void CreateNewEventDialog::on_repeatTimesSpinBox_valueChanged(int arg1)
{
    generateRepeat();
}

void CreateNewEventDialog::on_repeatEndTimeRadioButton_toggled(bool checked)
{
    generateRepeat();
}

void CreateNewEventDialog::on_colorComboBox_currentIndexChanged(int index)
{

}
