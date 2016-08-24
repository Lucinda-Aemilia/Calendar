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
    void init(Event* event = NULL);

private slots:
    void on_startDateTimeEdit_dateTimeChanged(const QDateTime &dateTime);

    void on_repeatCheckBox_stateChanged(int arg1);

    void on_endDateTimeEdit_dateChanged(const QDate &date);

    void on_allDayCheckBox_stateChanged(int arg1);

private:
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
