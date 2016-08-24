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

void EditEventDialog::init(Event *event)
{
    if (event != NULL)
        ui->dialogButtonBox->addButton(QDialogButtonBox::Save);
}

// 结束的时间日期必须比开始的时间日期大
void EditEventDialog::on_startDateTimeEdit_dateTimeChanged(const QDateTime &dateTime)
{
    ui->endDateTimeEdit->setMinimumDateTime(dateTime);
}

// 是否显示关于重复的groupbox
void EditEventDialog::on_repeatCheckBox_stateChanged(int arg1)
{
    // qDebug() << "EditEventDialog::on_repeatCheckBox_stateChanged" << arg1;

    if (arg1 == Qt::Unchecked)
        ui->repetitionGroupBox->hide();
    else if (arg1 == Qt::Checked)
        ui->repetitionGroupBox->show();
    layout()->setSizeConstraint(QLayout::SetFixedSize);
}

// 重复的结束日期必须大于结束的日期
void EditEventDialog::on_endDateTimeEdit_dateChanged(const QDate &date)
{
    ui->repeatEndDateEdit->setMinimumDate(date);
}

// 如果为全天，则使两个datetimeedit disable。
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
    }
}
