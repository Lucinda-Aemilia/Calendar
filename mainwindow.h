#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "cacheeventmodel.h"
#include "windows.h"
#include "eventcalendar.h"

#include <QMainWindow>
#include <QPushButton>
#include <QTableWidget>
#include <QSharedPointer>
#include <QTranslator>
#include <QListWidget>
#include <QLabel>

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

signals:
    void actionEnableDragDropToggled(bool toggled);

protected:
    void showEvent(QShowEvent *);
    void moveEvent(QMoveEvent *);

public slots:
    // 用于关联按钮和stackedwidget之间状态的槽函数
    void activateDay(bool toggled);
    void activateWeek(bool toggled);
    void activateMonth(bool toggled);
    void activateFourDays(bool toggled);
    void activateSchedule(bool toggled);
    void activateYear(bool toggled);
    void changeCurrentButtonToggleState(int index);

    void attachToDesktop(bool attach);
    // void detachFromDesktop();

    void setWindowOpacity();

    void freeze(bool frozen);

    void setChildWidgetsPalette(const QPalette& palette);

    void setMonthCalendarChildWidgetsDragDrop(bool toggled);

    void refreshCalendarTable(int dayNumber, QTableWidget* tableWidget);
    void refreshCalendarTable();
    // 日历上的按钮被按下之后出现编辑页面
    void onCalendarTableEventButtonClicked();
    // 刷新schedule页面
    void refreshSchedule();

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

    void on_quickCalendar_activated(const QDate &date);

    void initYearCalendars();

    // 设置拖拽的开启
    void onActionEnableDragDropToggled(bool toggled);

    // 导出所有配置
    void onActionExportTriggered();
    // 导入所有配置
    void onActionImportTriggered();
    // 切换语言
    void onLanguageActionTriggered(bool toggled);
    void initInterface();

    void on_freezeCheckBox_toggled(bool checked);

    void on_createButton_clicked();

private:
    bool enumUserWindowsCB(HWND hwnd, LPARAM lParam);
    HWND findDesktopIconWnd();
    void changeDisplayDate(int increment);


    Ui::MainWindow *ui;
    QPushButton* switchButtons[6];
    EventCalendar* yearCalendar[12];
    QListWidget* scheduleEventList[7];
    QListWidget* scheduleFileList[7];
    QLabel* scheduleDateLabel[7];

    CacheEventModel* cacheEventModel;
    HWND desktopHwnd;
    bool attachedToDesktop;
    bool mFrozen;

    QSize windowMinSize, windowMaxSize, windowCurSize;
    QPalette windowDefaultPalette;
    QPoint windowPos;
    int windowFlags;

    QTranslator chineseTranslator, chineseQtTranslator;
    QLocale curLocale;
};

#endif // MAINWINDOW_H
