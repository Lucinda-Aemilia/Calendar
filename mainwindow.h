#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "cacheeventmodel.h"
#include "windows.h"

#include <QMainWindow>
#include <QPushButton>
#include <QTableWidget>

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

private slots:
    void on_opacityHorizontalSlider_valueChanged(int value);

    void on_checkBox_stateChanged(int arg1);

private:
    bool enumUserWindowsCB(HWND hwnd, LPARAM lParam);
    HWND findDesktopIconWnd();


    Ui::MainWindow *ui;
    QPushButton* switchButtons[5];
    CacheEventModel* cacheEventModel;
    HWND desktopHwnd;
    bool attachedToDesktop;
};

#endif // MAINWINDOW_H
