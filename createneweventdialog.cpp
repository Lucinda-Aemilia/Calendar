#include "createneweventdialog.h"
#include "ui_editeventdialog.h"

#include <QDebug>
#include <QRadioButton>

CreateNewEventDialog::CreateNewEventDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateNewEventDialog)
{
    ui->setupUi(this);

    ui->repeatCheckBox->setCheckState(Qt::Checked);
    if (ui->repeatCheckBox->checkState() == Qt::Unchecked)
        ui->repetitionGroupBox->hide();
    else
        ui->repetitionGroupBox->show();
   layout()->setSizeConstraint(QLayout::SetFixedSize);
}

CreateNewEventDialog::~CreateNewEventDialog()
{
    delete ui;
}

Event* CreateNewEventDialog::getEvent()
{
    // qDebug() << "EditEventDialog::~EditEventDialog()";
    // 先写添加事件
    // 如果有repeat，我需要在这里手动添加
    Event* event = new Event(mName, mStartDate, mEndDate, mDescription,
                             mLocation, mColor, mRepeat);
    mCacheEventModel->addEvent(event);

    // 接下来，如果是编辑，而且有改变，那么就删除原来的事件，直接添加一些。
}

void CreateNewEventDialog::init(CacheEventModel* cacheEventModel, Event *event, const QDateTime& startDate,
                           const QDateTime& endDate)
{
    mCacheEventModel = cacheEventModel;

    // 设置初值（如果是edit）或者默认值
    if (event != NULL)
        event = NULL;

    ui->eventNameLineEdit->setText(tr("Untitled Event"));
    ui->startDateTimeEdit->setDateTime(startDate);
    ui->endDateTimeEdit->setDateTime(endDate);
    ui->repeatCheckBox->setChecked(false);
    ui->locationLineEdit->setText("");
    ui->descriptionTextEdit->setPlainText("");
    ui->colorComboBox->setCurrentIndex(3);

    ui->repeatCheckBox->setChecked(false);

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

void CreateNewEventDialog::on_clorComboBox_currentIndexChanged(const QString &arg1)
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
