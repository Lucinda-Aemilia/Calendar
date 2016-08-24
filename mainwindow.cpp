#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QHeaderView>
#include <QComboBox>
#include <QPushButton>
#include <QCalendarWidget>
#include <QTableView>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    cacheEventModel(new CacheEventModel)
{
    ui->setupUi(this);

    // 初始化stackwidget相关
    ui->stackedWidget->setCurrentIndex(0);
    switchButtons[0] = ui->dayButton;
    switchButtons[1] = ui->weekButton;
    switchButtons[2] = ui->monthButton;
    switchButtons[3] = ui->fourDaysButton;
    switchButtons[4] = ui->scheduleButton;

    // 设置切换按钮和stackwidget的关联
    connect(ui->dayButton, SIGNAL(toggled(bool)), this, SLOT(activateDay(bool)));
    connect(ui->weekButton, SIGNAL(toggled(bool)), this, SLOT(activateWeek(bool)));
    connect(ui->monthButton, SIGNAL(toggled(bool)), this, SLOT(activateMonth(bool)));
    connect(ui->fourDaysButton, SIGNAL(toggled(bool)), this, SLOT(activateFourDays(bool)));
    connect(ui->scheduleButton, SIGNAL(toggled(bool)), this, SLOT(activateSchedule(bool)));
    connect(ui->stackedWidget, SIGNAL(currentChanged(int)),
            this, SLOT(changeCurrentButtonToggleState(int)));
    changeCurrentButtonToggleState(0);

    // 初始化day, week, 4 days日历
    initCalendarTable(1, ui->dayTableWidget);
    initCalendarTable(7, ui->weekTableWidget);
    initCalendarTable(4, ui->fourDaysTableWidget);

    // 测试sql连接
    /*
    Event event;
    event.setName("Go to School");
    cacheEventModel->addEventToDb(&event);
    qDebug() << "event id: " << event.id();
    */

    // 将sqlmodel连接到日历
    ui->month_calendar->setCacheEventModel(cacheEventModel);

    // 测试透明度
    // this->setWindowOpacity(0.5);

    // 测试QCalendarWidget的tableView
    QTableView *tableView = ui->quickCalendar->findChild<QTableView*>("qt_calendar_calendarview");
    qDebug() << "number of columns: " << tableView->model()->columnCount();
    qDebug() << "number of rows: " << tableView->model()->rowCount();

}

void MainWindow::initCalendarTable(int dayNumber, QTableWidget* tableWidget)
{
    tableWidget->setColumnCount(1 + dayNumber*4); // 一列显示日期，其余每天有4列
    tableWidget->setRowCount(288 + 2); // 一行日期 + 一行comboBox + 24小时*12（5min）
    tableWidget->horizontalHeader()->setVisible(false); // 不显示表头
    tableWidget->verticalHeader()->setVisible(false);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); // 不能编辑
    tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    tableWidget->horizontalHeader()->setStretchLastSection(true); // 设置占满并均分
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // 设置行高
    tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    tableWidget->setRowHeight(0, 50);
    tableWidget->setRowHeight(1, 30);
    for (int i = 2; i < 288 + 2; i++)
    {
        tableWidget->setRowHeight(i, 10);
    }

    // 设置最左边一列的时间
    tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    tableWidget->setColumnWidth(0, 115);
    QFont smallFont("Microsoft Yahei UI");
    smallFont.setPointSize(10);
    for (int i = 0; i < 288; i += 12)
    {
        tableWidget->setSpan(i+2, 0, 12, 1);
        QTime time(i/12, 0);
        QTableWidgetItem* item = new QTableWidgetItem(time.toString("a h:mm"));
        item->setFont(smallFont);
        item->setTextAlignment(Qt::AlignTop);
        tableWidget->setItem(i+2, 0, item);
    }

    // 设置最上面一行的日期
    tableWidget->setSpan(0, 0, 2, 1);
    for (int i = 1; i <= dayNumber*4; i++)
    {
        tableWidget->setSpan(0, i, 1, 4);
        tableWidget->setSpan(1, i, 1, 4);
    }

    /* 说明可以在表格里直接插入按钮
    tableWidget->setSpan(0, 0, 3, 1);
    QComboBox *comBox = new QComboBox();
    comBox->addItem("Y");
    comBox->addItem("N");
    tableWidget->setCellWidget(0,0,comBox);

    tableWidget->setSpan(0, 2, 3, 3);
    QPushButton *btn = new QPushButton();
    tableWidget->setCellWidget(0, 2, btn);
    */
}

void MainWindow::changeCurrentButtonToggleState(int index)
{
    for (int i = 0; i < 5; i++)
        switchButtons[i]->setChecked(false);
    switchButtons[index]->setChecked(true);
}

void MainWindow::activateDay(bool toggled)
{
    qDebug() << "active day" << toggled;
    if (toggled)
        ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::activateWeek(bool toggled)
{
    if (toggled)
        ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::activateMonth(bool toggled)
{
    if (toggled)
        ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::activateFourDays(bool toggled)
{
    if (toggled)
        ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::activateSchedule(bool toggled)
{
    if (toggled)
        ui->stackedWidget->setCurrentIndex(4);
}

MainWindow::~MainWindow()
{
    delete ui;
}
