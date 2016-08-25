#ifndef EDITEVENTDIALOG_H
#define EDITEVENTDIALOG_H

#include "event.h"
#include "cacheeventmodel.h"

#include <QDialog>

namespace Ui {
class CreateNewEventDialog;
}

class CreateNewEventDialog : public QDialog
{
    Q_OBJECT

public:

    explicit CreateNewEventDialog(QWidget *parent = 0);
    ~CreateNewEventDialog();
    void init(CacheEventModel* cacheEventModel, Event* event = NULL, const QDateTime &startDate = QDateTime::currentDateTime(),
              const QDateTime &endDate = QDateTime::currentDateTime().addSecs(3600));
    Event* getEvent();
    void generateRepeat();

private slots:
    void on_startDateTimeEdit_dateTimeChanged(const QDateTime &dateTime);

    void on_repeatCheckBox_stateChanged(int arg1);

    void on_endDateTimeEdit_dateChanged(const QDate &date);

    void on_allDayCheckBox_stateChanged(int arg1);

    void on_repeatEndDateEdit_dateChanged(const QDate &date);

    void on_eventNameLineEdit_textChanged(const QString &arg1);

    void on_endDateTimeEdit_dateTimeChanged(const QDateTime &dateTime);

    void on_locationLineEdit_textChanged(const QString &arg1);

    void on_descriptionTextEdit_textChanged();

    void on_clorComboBox_currentIndexChanged(const QString &arg1);

    void on_startDateTimeEdit_dateChanged(const QDate &date);

    void on_repeatComboBox_currentIndexChanged(int index);

    void on_colorComboBox_currentTextChanged(const QString &arg1);

    void on_frequencyComboBox_currentIndexChanged(int index);

    void on_repeatTimesRadioButton_toggled(bool checked);

    void on_repeatTimesSpinBox_valueChanged(int arg1);

    void on_repeatEndTimeRadioButton_toggled(bool checked);

private:

    QString getWeekDayName(const QDate& date);

    Ui::CreateNewEventDialog *ui;
    QString mName;
    QDateTime mStartDate, mEndDate;
    QString mDescription;
    QString mLocation;
    QColor mColor;
    QString mRepeat;

    CacheEventModel* mCacheEventModel;
};

#endif // EDITEVENTDIALOG_H
