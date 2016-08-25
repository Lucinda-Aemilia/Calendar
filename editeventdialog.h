#ifndef EDITEVENTDIALOG_H
#define EDITEVENTDIALOG_H

#include "event.h"

#include <QDialog>

namespace Ui {
class EditEventDialog;
}

class EditEventDialog : public QDialog
{
    Q_OBJECT

public:
    enum EditFlag { create, edit };
    explicit EditEventDialog(QWidget *parent = 0);
    ~EditEventDialog();
    void init(Event* event = NULL, const QDateTime &startDate = QDateTime::currentDateTime(),
              const QDateTime &endDate = QDateTime::currentDateTime().addSecs(3600));

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

private:
    QString getWeekDayName(const QDate& date);

    Ui::EditEventDialog *ui;
    EditFlag mEditFlag;
    QString mName;
    QDateTime mStartDate, mEndDate;
    QString mDescription;
    QString mLocation;
    QColor mColor;
    int mRepeat;
};

#endif // EDITEVENTDIALOG_H
