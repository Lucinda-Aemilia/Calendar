#include "vieweventdialog.h"

ViewEventDialog::ViewEventDialog(QWidget *parent) :
    CreateNewEventDialog(parent)
{

}

void init(CacheEventModel* cacheEventModel, Event* event)
{
    if (event == NULL)
    {
        return;
    }


    CreateNewEventDialog::ui->dialogButtonBox->addButton(QDialogButtonBox::Save);

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
