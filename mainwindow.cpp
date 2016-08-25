#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "windows.h"

#include <QDebug>
#include <QHeaderView>
#include <QComboBox>
#include <QPushButton>
#include <QCalendarWidget>
#include <QTableView>
#include <QModelIndex>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    cacheEventModel(new CacheEventModel)
{
    ui->setupUi(this);

    // layout()->setSizeConstraint(QLayout::SetFixedSize);

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
    // 设置日历widget
    on_month_calendar_currentPageChanged(0, 0);


    // 测试QCalendarWidget的tableView
    QTableView *tableView = ui->quickCalendar->findChild<QTableView*>("qt_calendar_calendarview");
    qDebug() << "number of columns: " << tableView->model()->columnCount();
    qDebug() << "number of rows: " << tableView->model()->rowCount();

    // 设置固定到桌面和透明度
    // windowFlags = windowFlags();
    // attachToDesktop();
    // setAttribute(Qt::WA_TranslucentBackground);
    // setStyleSheet("background:transparent;");
    // no window decorations
    // setWindowFlags(Qt::WindowStaysOnTopHint);
    // setWindowFlags(Qt::WindowStaysOnBottomHint);
    // setAttribute(Qt::WA_OpaquePaintEvent,true);
    // setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnBottomHint);
    // setAttribute(Qt::WA_TranslucentBackground, true);
    // setAttribute(Qt::WA_TransparentForMouseEvents, true);

    /*
    QList<QWidget*> lstChildren = findChildren<QWidget*>();
    foreach (QWidget* pWidget, lstChildren)
    {
        if (pWidget != ui->freezeCheckBox && pWidget != ui->settingsGroupBox)
        {
            pWidget->setAttribute(Qt::WA_TranslucentBackground, true);
            pWidget->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        }
    }
    */

    // setWindowFlags(Qt::Tool);
    // setWindowFlags(Qt::FramelessWindowHint);
    /*
    etWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_TransparentForMouseEvents, true);
     * */

    // 设置是否冻结
    mFrozen = false;
    QList<QWidget*> lstChildren = findChildren<QWidget*>();
    foreach (QWidget* pWidget, lstChildren)
    {
        if (pWidget != ui->freezeCheckBox)
            pWidget->installEventFilter(this);
    }

    // 记下default背景颜色
    windowDefaultPalette = palette();
}

bool MainWindow::eventFilter(QObject* obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress ||
            event->type() == QEvent::MouseButtonDblClick ||
            event->type() == QEvent::MouseMove ||
            event->type() == QEvent::MouseTrackingChange ||
            event->type() == QEvent::MouseButtonRelease ||
            event->type() == QEvent::Enter ||
            event->type() == QEvent::Leave ||
            event->type() == QEvent::HoverEnter ||
            event->type() == QEvent::HoverLeave ||
            event->type() == QEvent::HoverMove ||
            event->type() == QEvent::FocusIn ||
            event->type() == QEvent::FocusOut)
    {
        // QMouseEvent* mouseEvent = qobject_cast<QMouseEvent*>(event);
        // qDebug() << "MainWindow::eventFilter" << event->type() << mFrozen;
        if (mFrozen)
            return true;
    }
    return QMainWindow::eventFilter(obj, event);

}

void MainWindow::freeze(bool frozen)
{
    if (frozen && !mFrozen)
    {
        windowMinSize = this->minimumSize();
        windowMaxSize = this->maximumSize();
        QSize curSize(this->size());
        this->setMinimumSize(curSize);
        this->setMaximumSize(curSize);
        // layout()->setSizeConstraint(QLayout::SetFixedSize);

        /*
        QList<QWidget*> lstChildren = findChildren<QWidget*>();
        foreach (QWidget* pWidget, lstChildren)
        {
            if (pWidget != ui->freezeCheckBox)
            {
                pWidget->setAttribute(Qt::WA_TransparentForMouseEvents, true);
            }
        }
        */

        setWindowFlags(Qt::FramelessWindowHint);
        // windowFlags();
        // setAttribute(Qt::WA_TranslucentBackground, true);
        // setAttribute(Qt::WA_TransparentForMouseEvents, true);
        show();
    }
    else if (!frozen && mFrozen)
    {
        // layout()->setSizeConstraint(QLayout::SetNoConstraint);
        this->setMinimumSize(windowMinSize);
        this->setMaximumSize(windowMaxSize);

        /*
        QList<QWidget*> lstChildren = findChildren<QWidget*>();
        foreach (QWidget* pWidget, lstChildren)
        {
            if (pWidget != ui->freezeCheckBox)
            {
                pWidget->setAttribute(Qt::WA_TransparentForMouseEvents, false);
            }
        }
        */

        setWindowFlags(Qt::Window);
        // setAttribute(Qt::WA_TranslucentBackground, false);
        // setAttribute(Qt::WA_TransparentForMouseEvents, false);
        show();
    }
    mFrozen = frozen;
}

void MainWindow::attachToDesktop()
{
    if (!attachedToDesktop)
    {
        ui->opacityHorizontalSlider->setValue(100);
        desktopHwnd = findDesktopIconWnd();
        if(desktopHwnd)
            SetParent((HWND)winId(), desktopHwnd);
        attachedToDesktop = true;
    }
    setWindowOpacity();
}

void MainWindow::detachFromDesktop()
{
    if (attachedToDesktop)
    {
        SetParent((HWND)winId(), (HWND)0);
        attachedToDesktop = false;
        // 将palette设置回去
        setPalette(windowDefaultPalette);
    }
    setWindowOpacity();
}

bool MainWindow::enumUserWindowsCB(HWND hwnd, LPARAM lParam)
{
    long wflags = GetWindowLong(hwnd, GWL_STYLE);
    if(!(wflags & WS_VISIBLE)) return TRUE;

    HWND sndWnd;
    if( !(sndWnd=FindWindowEx(hwnd, NULL, L"SHELLDLL_DefView", NULL)) ) return TRUE;

    HWND targetWnd;
    if( !(targetWnd=FindWindowEx(sndWnd, NULL, L"SysListView32", L"FolderView")) ) return TRUE;

    HWND* resultHwnd = (HWND*)lParam;
    *resultHwnd = targetWnd;

    return FALSE;
}

HWND MainWindow::findDesktopIconWnd()
{
    HWND resultHwnd = NULL;
    EnumWindows((WNDENUMPROC)&enumUserWindowsCB, (LPARAM)&resultHwnd);
    return resultHwnd;
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

void MainWindow::on_opacityHorizontalSlider_valueChanged(int value)
{
    setWindowOpacity();
}

void MainWindow::setWindowOpacity()
{
    int value = ui->opacityHorizontalSlider->value();
    if (!attachedToDesktop)
    {
        QMainWindow::setWindowOpacity(value / 100.0);

        // 把背景palette改回去
        if (palette() != windowDefaultPalette)
        {
            setChildWidgetsPalette(windowDefaultPalette);
            setPalette(windowDefaultPalette);
            setAutoFillBackground(true);
        }
    }
    else
    {
        // setStyleSheet("background-color:white;");

        // 存下目前的默认palette
        QPalette pal = palette();
        QColor oriColor(windowDefaultPalette.color(QPalette::Background));
        pal.setColor(QPalette::Background,
                     QColor(oriColor.red(), oriColor.green(), oriColor.blue(),
                            (int)(value / 100.0 * 255)));


        // 改背景palette

        if (palette() != pal)
        {
            setChildWidgetsPalette(pal);
            setPalette(pal);
            setAutoFillBackground(true);
        }
    }
    ui->windowOpacityLabel->setText(QString::number(value) + "%");
}

void MainWindow::setChildWidgetsPalette(const QPalette &palette)
{
    // 目前发现好像没什么用
    /*
    QList<QWidget*> lstChildren = findChildren<QWidget*>();
    foreach (QWidget* pWidget, lstChildren)
    {
        pWidget->setPalette(palette);
        setAutoFillBackground(true);
    }
    */
}

void MainWindow::on_attachCheckBox_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked)
        attachToDesktop();
    else
        detachFromDesktop();
}

void MainWindow::on_freezeCheckBox_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked)
        freeze(true);
    else
        freeze(false);
}

// 将日历转到上个月的本日
void MainWindow::on_leftSwitchButton_clicked()
{
    QDate date = ui->month_calendar->selectedDate();
    date = date.addMonths(-1);
    ui->quickCalendar->setSelectedDate(date);
}

void MainWindow::on_rightSwitchButton_clicked()
{
    QDate date = ui->month_calendar->selectedDate();
    date = date.addMonths(1);
    ui->quickCalendar->setSelectedDate(date);
}

void MainWindow::on_todayButton_clicked()
{
    ui->quickCalendar->setSelectedDate(QDate::currentDate());
}

void MainWindow::on_quickCalendar_selectionChanged()
{
    ui->month_calendar->setSelectedDate(ui->quickCalendar->selectedDate());
}

void MainWindow::on_month_calendar_selectionChanged()
{
    ui->quickCalendar->setSelectedDate(ui->month_calendar->selectedDate());
}

void MainWindow::on_month_calendar_currentPageChanged(int year, int month)
{
    /*
    QTableView *tableView = ui->month_calendar->findChild<QTableView*>("qt_calendar_calendarview");
    int height = tableView->horizontalHeader()->count();
    int width = tableView->verticalHeader()->count();

    if (tableView->indexWidget(tableView->model()->index(1, 1)) == NULL)
    {
        for (int i = 1; i < height; i++)
        {
            for (int j = 1; j < width; j++)
            {
                // QComboBox *comBox = new QComboBox();
                // comBox->addItem("Y");
                // comBox->addItem("N");
                int date = tableView->model()->index(j, i).data().toInt();
                qDebug() << "tableview" << date;
                QTableWidget* tableWidget = new QTableWidget(3, 2, ui->month_calendar);
                tableWidget->setSpan(0, 0, 1, 2);
                tableWidget->horizontalHeader()->setVisible(false);
                tableWidget->verticalHeader()->setVisible(false);
                tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); // 不能编辑
                tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
                tableWidget->horizontalHeader()->setStretchLastSection(true); // 设置占满并均分
                tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
                tableWidget->verticalHeader()->setStretchLastSection(true); // 设置占满并均分
                tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

                // QSignalMapper *m = new QSignalMapper(this);
                // connect(tableWidget, SIGNAL(cellDoubleClicked(int,int)), m, SLOT(map(int, int)));

                tableWidget->setItem(0, 0, new QTableWidgetItem(QString::number(date)));
                tableView->setIndexWidget(tableView->model()->index(j, i), tableWidget);
            }
        }
    }
    */
}
