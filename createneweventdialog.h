#ifndef EDITEVENTDIALOG_H
#define EDITEVENTDIALOG_H

#include "event.h"
#include "cacheeventmodel.h"

#include <QDialog>
#include <QSharedPointer>

namespace Ui {
class CreateNewEventDialog;
}

class CreateNewEventDialog : public QDialog
{
    Q_OBJECT

public:

    explicit CreateNewEventDialog(QWidget *parent = 0);
    ~CreateNewEventDialog();
    void init(CacheEventModel* cacheEventModel, QSharedPointer<Event> event = QSharedPointer<Event>(NULL),
              const QDateTime &startDate = QDateTime::currentDateTime(),
              const QDateTime &endDate = QDateTime::currentDateTime().addSecs(3600));
    QSharedPointer<Event> getEvent();
    void generateRepeat();

public slots:
    void deleteCurEvent();

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

    void on_startDateTimeEdit_dateChanged(const QDate &date);

    void on_repeatComboBox_currentIndexChanged(int index);

    void on_colorComboBox_currentTextChanged(const QString &arg1);

    void on_frequencyComboBox_currentIndexChanged(int index);

    void on_repeatTimesRadioButton_toggled(bool checked);

    void on_repeatTimesSpinBox_valueChanged(int arg1);

    void on_repeatEndTimeRadioButton_toggled(bool checked);

    void on_colorComboBox_currentIndexChanged(int index);

    void on_colorComboBox_currentIndexChanged(const QString &arg1);

protected:

    // 为了继承类的方便……
    void fillWithEvent(QSharedPointer<Event> event);
    void disableAllEdits(bool disable);
    void setButtonsToViewSet();
    void setButtonsToEditSet();

    QString getWeekDayName(const QDate& date);
    Ui::CreateNewEventDialog *ui;
    QString mName;
    QDateTime mStartDate, mEndDate;
    QString mDescription;
    QString mLocation;
    QColor mColor;
    QString mRepeat;

    QSharedPointer<Event> mCurEvent;

    CacheEventModel* mCacheEventModel;
};

#endif // EDITEVENTDIALOG_H
