#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "cacheeventmodel.h"
#include "windows.h"

#include <QMainWindow>
#include <QPushButton>
#include <QTableWidget>
#include <QSharedPointer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    void initCalendarTable(int dayNumber, QTableWidget* tableWidget);

    ~MainWindow();

    bool eventFilter(QObject* obj, QEvent* event);

public slots:
    // 用于关联按钮和stackedwidget之间状态的槽函数
    void activateDay(bool toggled);
    void activateWeek(bool toggled);
    void activateMonth(bool toggled);
    void activateFourDays(bool toggled);
    void activateSchedule(bool toggled);
    void changeCurrentButtonToggleState(int index);

    void attachToDesktop();
    void detachFromDesktop();

    void setWindowOpacity();

    void freeze(bool frozen);

    void setChildWidgetsPalette(const QPalette& palette);

private slots:
    void on_opacityHorizontalSlider_valueChanged(int value);

    void on_attachCheckBox_stateChanged(int arg1);

    void on_freezeCheckBox_stateChanged(int arg1);

    void on_leftSwitchButton_clicked();

    void on_rightSwitchButton_clicked();

    void on_todayButton_clicked();

    void on_quickCalendar_selectionChanged();

    void on_month_calendar_selectionChanged();

    void on_month_calendar_currentPageChanged(int year, int month);

private:
    bool enumUserWindowsCB(HWND hwnd, LPARAM lParam);
    HWND findDesktopIconWnd();
    void changeDisplayDate(int increment);


    Ui::MainWindow *ui;
    QPushButton* switchButtons[5];
    CacheEventModel* cacheEventModel;
    HWND desktopHwnd;
    bool attachedToDesktop;
    bool mFrozen;

    QSize windowMinSize, windowMaxSize;
    QPalette windowDefaultPalette;
    int windowFlags;
};

#endif // MAINWINDOW_H
