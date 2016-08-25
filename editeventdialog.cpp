#include "editeventdialog.h"
#include "ui_editeventdialog.h"

#include <QDebug>

EditEventDialog::EditEventDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditEventDialog)
{
    ui->setupUi(this);

    ui->repeatCheckBox->setCheckState(Qt::Checked);
    if (ui->repeatCheckBox->checkState() == Qt::Unchecked)
        ui->repetitionGroupBox->hide();
    else
        ui->repetitionGroupBox->show();
   layout()->setSizeConstraint(QLayout::SetFixedSize);
}

EditEventDialog::~EditEventDialog()
{
    delete ui;
}

Event* EditEventDialog::getEvent()
{
    qDebug() << "EditEventDialog::~EditEventDialog()";
    // 先写添加事件
    // 如果有repeat，我需要在这里手动添加
    Event* event = new Event(mName, mStartDate, mEndDate, mDescription,
                             mLocation, mColor, mRepeat);
    mCacheEventModel->addEvent(event);

    // 接下来，如果是编辑，而且有改变，那么就删除原来的事件，直接添加一些。
}

void EditEventDialog::init(CacheEventModel* cacheEventModel, Event *event, const QDateTime& startDate,
                           const QDateTime& endDate)
{
    mCacheEventModel = cacheEventModel;

    // 设置初值（如果是edit）或者默认值
    if (event != NULL)
    {
        ui->dialogButtonBox->addButton(QDialogButtonBox::Save);

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

    else
    {
        ui->eventNameLineEdit->setText(tr("Untitled Event"));
        ui->startDateTimeEdit->setDateTime(startDate);
        ui->endDateTimeEdit->setDateTime(endDate);
        ui->repeatCheckBox->setChecked(false);
        ui->locationLineEdit->setText("");
        ui->descriptionTextEdit->setPlainText("");
        ui->colorComboBox->setCurrentIndex(3);

    }
}

// 结束的时间日期必须比开始的时间日期大
void EditEventDialog::on_startDateTimeEdit_dateTimeChanged(const QDateTime &dateTime)
{
    ui->endDateTimeEdit->setMinimumDateTime(dateTime);
    mStartDate = dateTime;
}

// 是否显示关于重复的groupbox
void EditEventDialog::on_repeatCheckBox_stateChanged(int arg1)
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
}

// 重复的结束日期必须大于结束的日期
void EditEventDialog::on_endDateTimeEdit_dateChanged(const QDate &date)
{
    ui->repeatEndDateEdit->setMinimumDate(date);
}

// 如果为全天，则使两个datetimeedit disable。
// 设置开始和结束时间
void EditEventDialog::on_allDayCheckBox_stateChanged(int arg1)
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
void EditEventDialog::on_repeatEndDateEdit_dateChanged(const QDate &date)
{
    ui->repeatEndDateLabel->setText(getWeekDayName(date));
}

void EditEventDialog::on_eventNameLineEdit_textChanged(const QString &arg1)
{
    mName = arg1;
}

// 改结束时间
void EditEventDialog::on_endDateTimeEdit_dateTimeChanged(const QDateTime &dateTime)
{
    mEndDate = dateTime;
}

void EditEventDialog::on_locationLineEdit_textChanged(const QString &arg1)
{
    mLocation = arg1;
}

void EditEventDialog::on_descriptionTextEdit_textChanged()
{
    mDescription = ui->descriptionTextEdit->toPlainText();
}

void EditEventDialog::on_clorComboBox_currentIndexChanged(const QString &arg1)
{
    mColor = QColor(arg1);
}



void EditEventDialog::on_startDateTimeEdit_dateChanged(const QDate &date)
{
    ui->repeatTimeLabel->setText(getWeekDayName(date));
}

QString EditEventDialog::getWeekDayName(const QDate &date)
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

void EditEventDialog::on_repeatComboBox_currentIndexChanged(int index)
{
    if (index == 1)
        ui->repeatTimeLabel->setText(getWeekDayName(ui->startDateTimeEdit->date()));
    else
        ui->repeatTimeLabel->setText("");
}

void EditEventDialog::on_colorComboBox_currentTextChanged(const QString &arg1)
{
    mColor = QColor(arg1);
}
