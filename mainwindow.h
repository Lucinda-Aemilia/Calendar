#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

private:
    Ui::MainWindow *ui;
    QPushButton* switchButtons[5];
};

#endif // MAINWINDOW_H
