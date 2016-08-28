#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "windows.h"
#include "createneweventdialog.h"
#include "calendareventfilewidget.h"
#include "event.h"
#include "calendartableeventbutton.h"
#include "vieweventdialog.h"

#include <QDebug>
#include <QHeaderView>
#include <QComboBox>
#include <QPushButton>
#include <QCalendarWidget>
#include <QTableView>
#include <QModelIndex>
#include <QDragEnterEvent>
#include <QEvent>
#include <QDesktopWidget>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFileDialog>
#include <QDateTime>
#include <QProgressDialog>
#include <QMessageBox>
#include <QAction>
#include <QPushButton>
#include <QTranslator>

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

    // 测试QCalendarWidget的tableView
    QTableView *tableView = ui->quickCalendar->findChild<QTableView*>("qt_calendar_calendarview");
    qDebug() << "number of columns: " << tableView->model()->columnCount();
    qDebug() << "number of rows: " << tableView->model()->rowCount();

    // 设置固定到桌面和透明度
    ui->attachCheckBox->hide();

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

    // 设置拖拽相关的槽函数
    connect(ui->actionEnable_File_Drag_and_Drop, SIGNAL(toggled(bool)),
            this, SLOT(onActionEnableDragDropToggled(bool)));
    connect(ui->actionEnable_File_Drag_and_Drop, SIGNAL(toggled(bool)),
            this, SLOT(setMonthCalendarChildWidgetsDragDrop(bool)));

    // import 和 export 相关槽函数
    connect(ui->actionExport, SIGNAL(triggered(bool)),
            this, SLOT(onActionExportTriggered()));
    connect(ui->actionImport, SIGNAL(triggered(bool)),
            this, SLOT(onActionImportTriggered()));

    // 切换语言槽函数
    connect(ui->chineseLanguageAction, SIGNAL(toggled(bool)),
            this, SLOT(onLanguageActionTriggered(bool)));
    connect(ui->englishLanguageAction, SIGNAL(toggled(bool)),
            this, SLOT(onLanguageActionTriggered(bool)));

    // 加载翻译
    chineseTranslator.load(":/translation/cn.qm");
    chineseQtTranslator.load(":/translation/qt_zh_CN.qm");
    curLocale = QLocale(QLocale::English, QLocale::UnitedStates);

    initInterface();
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event)
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
        // QMouseQSharedPointer<Event> mouseEvent = qobject_cast<QMouseQSharedPointer<Event>>(event);
        // qDebug() << "MainWindow::eventFilter" << event->type() << mFrozen;
        /*
        if (mFrozen)
        {
            // qDebug() << "MainWindow::eventFilter" << event->type();
            // show();
            // QApplication::sendEvent(QApplication::desktop(), event);
            return true;
        }
        */
    }

    if (event->type() == QEvent::DragEnter || event->type() == QEvent::DragLeave
            || event->type() == QEvent::DragMove
            || event->type() == QEvent::DragResponse || event->type() == QEvent::Drop)
    {
        /*
        qDebug() << "MainWindow::eventFilter" << event->type();
        QDragEnterQSharedPointer<Event> myEvent = static_cast<QDragEnterQSharedPointer<Event>>(event);
        qDebug() << myEvent;
        */
    }

    return QMainWindow::eventFilter(obj, event);

}

void MainWindow::freeze(bool frozen)
{
    qDebug() << "MainWindow::freeze(bool frozen)" << frozen << mFrozen;
    if (frozen)
    {
        windowMinSize = this->minimumSize();
        windowMaxSize = this->maximumSize();
        windowPos = this->pos();
        // windowFlags = this->win;
        QSize curSize(this->size());
        // this->setMinimumSize(curSize);
        // this->setMaximumSize(curSize);
        windowCurSize = curSize;

        // setAttribute(Qt::WA_TransparentForMouseEvents);
        setWindowFlags(Qt::WindowTransparentForInput);
        // setWindowFlags(Qt::FramelessWindowHint);
        show();

        // QMessageBox::information(this, tr("Do not freeze"), tr("Do not freeze"));
        // QPushButton button("Cancel freeze");
        // button.show();

        QDialog dialog(this, Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
        dialog.setModal(false);
        dialog.setSizeGripEnabled(true);
        dialog.setFixedSize(500, 10);
        // dialog.setGeometry();
        dialog.setWindowTitle(tr("Close this window to unlock the calendar"));
        // dialog.
        dialog.exec();



        // setAttribute(Qt::WA_TransparentForMouseEvents, false);
        setWindowFlags(Qt::Window | Qt::Widget);
        show();
        mFrozen = false;
        ui->freezeCheckBox->setChecked(false);
        // this->setMinimumSize(windowMinSize);
        // this->setMaximumSize(windowMaxSize);

    }

    else if (!frozen && mFrozen)
    {
        // layout()->setSizeConstraint(QLayout::SetNoConstraint);
        this->setMinimumSize(windowMinSize);
        this->setMaximumSize(windowMaxSize);
        windowPos = this->pos();

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

        // setWindowFlags(Qt::Window);
        // setAttribute(Qt::WA_TranslucentBackground, false);
        setAttribute(Qt::WA_TransparentForMouseEvents, false);
        // show();

        mFrozen = false;
    }

}

void MainWindow::showEvent(QShowEvent *event)
{
    /*
    // qDebug() << event->type();
    if (mFrozen)
    {
        this->move(windowPos);
    }
    */
    QMainWindow::showEvent(event);
}

void MainWindow::moveEvent(QMoveEvent *event)
{
    /*
    qDebug() << event->type();
    if (mFrozen)
    {
        this->move(windowPos);
        // return;
    }
    */
    QMainWindow::moveEvent(event);
}

void MainWindow::attachToDesktop(bool attach)
{
    /*
    if (!attachedToDesktop)
    {
        ui->opacityHorizontalSlider->setValue(100);
        desktopHwnd = findDesktopIconWnd();
        if(desktopHwnd)
            SetParent((HWND)winId(), desktopHwnd);
        attachedToDesktop = true;
    }
    setWindowOpacity();
    */
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
    /*
    tableWidget->setColumnCount(1 + dayNumber); // 一列显示日期，其余每天有4列
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
    */

    // 设置最上面一行的日期
    /*
    tableWidget->setSpan(0, 0, 2, 1);
    for (int i = 1; i <= dayNumber*4; i++)
    {
        tableWidget->setSpan(0, i, 1, 4);
        tableWidget->setSpan(1, i, 1, 4);
    }*/
    // tableWidget->clearSpans();

    /* 说明可以在表格里直接插入按钮
    tableWidget->setSpan(0, 0, 3, 1);


    tableWidget->setSpan(0, 2, 3, 3);
    QPushButton *btn = new QPushButton();
    tableWidget->setCellWidget(0, 2, btn);
    */
    /*
    QComboBox *comBox = new QComboBox();
    comBox->addItem("Y");
    comBox->addItem("N");
    tableWidget->setCellWidget(0,0,comBox);
    */
    refreshCalendarTable(dayNumber, tableWidget);
}

void MainWindow::refreshCalendarTable()
{
    refreshCalendarTable(1, ui->dayTableWidget);
    refreshCalendarTable(7, ui->weekTableWidget);
    refreshCalendarTable(4, ui->fourDaysTableWidget);
}

void MainWindow::refreshCalendarTable(int dayNumber, QTableWidget* tableWidget)
{
    tableWidget->clear();
    tableWidget->clearSpans();

    tableWidget->setColumnCount(1 + dayNumber); // 一列显示日期，其余每天有4列
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
        tableWidget->setRowHeight(i, 7);
    }

    tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    tableWidget->setColumnWidth(0, 115);
    // QFont smallFont("Microsoft Yahei UI");
    // smallFont.setPointSize(10);

    for (int i = 0; i < 288; i += 12)
    {
        tableWidget->setSpan(i+2, 0, 12, 1);
        QTime time(i/12, 0);

        QString timeName;
        if (curLocale.language() == QLocale::Chinese)
            timeName = curLocale.toString(time, "a h:mm");
        else
            timeName = curLocale.toString(time, "h:mm a");

        QTableWidgetItem* item = new QTableWidgetItem(timeName);
        // item->setFont(smallFont);
        item->setTextAlignment(Qt::AlignTop);
        tableWidget->setItem(i+2, 0, item);
    }

    QDate date(ui->quickCalendar->selectedDate());
    qDebug() << "calendar table" << date;
    for (int i = 0; i < dayNumber; i++, date = date.addDays(1))
    {
        QTableWidgetItem* item = new QTableWidgetItem(curLocale.toString(date, "yyyy.M.d\ndddd"));
        // item->setFont(smallFont);
        item->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(0, i + 1, item);

        QList<QSharedPointer<Event>> events(cacheEventModel->eventsForDate(date));
        QTime latestClearTime(0, 0);
        for (int j = 0; j < events.size(); j++)
        {
            QTime t1(events.at(j)->startDate().time());
            QTime t2(events.at(j)->endDate().time());

            int startMinute = std::floor(t1.minute() / 5.0 + 0.5) * 5;
            int endMinute = std::floor(t2.minute() / 5.0 + 0.5) * 5;

            QTime startTime, endTime;

            startTime = QTime(t1.hour(), startMinute);
            endTime = QTime(t2.hour(), endMinute);

            qDebug() << date << t1 << t2 << startTime << endTime;

            if (endTime <= latestClearTime || startTime == endTime)
                continue;
            if (latestClearTime > startTime)
                startTime = latestClearTime;
            // 需要改
            if (!startTime.isValid() || !endTime.isValid())
                continue;

            int startRow = QTime(0, 0).secsTo(startTime) / 60 / 5;
            int endRow = QTime(0, 0).secsTo(endTime) / 60 / 5;
            tableWidget->setSpan(startRow + 2, i + 1, endRow - startRow, 1);
            /*
            QTableWidgetItem* item =
                    new QTableWidgetItem(events.at(j)->name());
            item->setTextAlignment(Qt::AlignCenter);
            tableWidget->setItem(startRow + 2, i + 1, item);
            */
            // QPushButton* pushButton = new QPushButton(events.at(j)->name());
            // pushButton->data();
            CalendarTableEventButton* pushButton =
                    new CalendarTableEventButton(events.at(j));
            connect(pushButton, SIGNAL(clicked(bool)),
                    this, SLOT(onCalendarTableEventButtonClicked()));
            tableWidget->setIndexWidget(
                        tableWidget->model()->index(startRow + 2, i + 1), pushButton);
            qDebug() << date << startRow << endRow;

            latestClearTime = endTime;
        }
    }
}

void MainWindow::onCalendarTableEventButtonClicked()
{
    ViewEventDialog* dialog = new ViewEventDialog;
    CalendarTableEventButton* pushButton = dynamic_cast<CalendarTableEventButton*>(sender());
    // CreateNewEventDialog* dialog = new CreateNewEventDialog;
    dialog->init(cacheEventModel, pushButton->event());
    int result = dialog->exec();
    if (result == QDialog::Accepted)
    {
        QSharedPointer<Event> event(dialog->getEvent());
        // refreshEvents(curDate());
    }
    refreshCalendarTable();
}

void MainWindow::changeCurrentButtonToggleState(int index)
{
    for (int i = 0; i < 5; i++)
        switchButtons[i]->setChecked(false);
    switchButtons[index]->setChecked(true);
}

void MainWindow::activateDay(bool toggled)
{
    // qDebug() << "active day" << toggled;
    if (toggled)
    {
        ui->stackedWidget->setCurrentIndex(0);
        refreshCalendarTable();
    }
}

void MainWindow::activateWeek(bool toggled)
{
    if (toggled)
    {
        ui->stackedWidget->setCurrentIndex(1);
        refreshCalendarTable();
    }
}

void MainWindow::activateMonth(bool toggled)
{
    if (toggled)
        ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::activateFourDays(bool toggled)
{
    if (toggled)
    {
        ui->stackedWidget->setCurrentIndex(3);
        refreshCalendarTable();
    }
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
    /*
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
    */
    QMainWindow::setWindowOpacity(value / 100.0);
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
        attachToDesktop(true);
    else
        attachToDesktop(false);
}

void MainWindow::on_freezeCheckBox_stateChanged(int arg1)
{
    /*
    if (arg1 == Qt::Checked)
        freeze(true);
    else
        freeze(false);
    */
}

// 各种情况
// 将日历转到上个月的本日
void MainWindow::on_leftSwitchButton_clicked()
{
    changeDisplayDate(-1);
}

void MainWindow::on_rightSwitchButton_clicked()
{
    changeDisplayDate(1);
}

void MainWindow::changeDisplayDate(int increment)
{
    QDate date = ui->quickCalendar->selectedDate();
    if (ui->stackedWidget->currentIndex() == 0) // day
        date = date.addDays(increment);
    else if (ui->stackedWidget->currentIndex() == 1) // week
        date = date.addDays(increment * 7);
    else if (ui->stackedWidget->currentIndex() == 2) // month
        date = date.addMonths(increment);
    else if (ui->stackedWidget->currentIndex() == 3) // 4 days
        date = date.addDays(increment * 4);
    else if (ui->stackedWidget->currentIndex() == 4) // schedule
        date = date.addDays(increment * 7);

    ui->quickCalendar->setSelectedDate(date);
}

void MainWindow::on_todayButton_clicked()
{
    ui->quickCalendar->setSelectedDate(QDate::currentDate());
}

void MainWindow::on_quickCalendar_selectionChanged()
{
    ui->month_calendar->setSelectedDate(ui->quickCalendar->selectedDate());
    QDate date(ui->quickCalendar->selectedDate());
    // QLocale locale  = QLocale(QLocale::Swedish, QLocale::Sweden); // set the locale you want here
    // QString swedishDate = locale.toString(date, "dddd, d MMMM yyyy");
    ui->dateDisplayLabel->setText(curLocale.toString(date, "yyyy.M.d  dddd"));
    refreshCalendarTable();
}

void MainWindow::on_month_calendar_selectionChanged()
{
    ui->quickCalendar->setSelectedDate(ui->month_calendar->selectedDate());
    // 更新控件的隐藏和显示情况
    on_month_calendar_currentPageChanged(ui->month_calendar->yearShown(),
                                         ui->month_calendar->monthShown());
}

void MainWindow::on_month_calendar_currentPageChanged(int year, int month)
{
    year = ui->month_calendar->yearShown();
    month = ui->month_calendar->monthShown();

    QTableView *tableView = ui->month_calendar->findChild<QTableView*>("qt_calendar_calendarview");
    int height = tableView->horizontalHeader()->count();
    int width = tableView->verticalHeader()->count();

    // 如果是null那就先都加上
    if (tableView->indexWidget(tableView->model()->index(1, 1)) == NULL)
    {
        int flag = -1;
        for (int i = 1; i < width; i++)
        {
            for (int j = 1; j < height; j++)
            {
                int curDate = tableView->model()->index(i, j).data().toInt();
                int curYear = year, curMonth = month;
                // qDebug() << "add" << year << month << date;

                if (flag == -1 && curDate != 1)
                {
                    curMonth--;
                    //continue;
                }
                else if (flag == -1 && curDate == 1)
                    flag = 0;
                else if (flag == 1)
                {
                    curMonth++;
                    //continue;
                }
                else if (flag == 0 && curDate == 1)
                {
                    curMonth++;
                    flag = 1;
                    //continue;
                }

                CalendarEventFileWidget* widget = new CalendarEventFileWidget(
                            cacheEventModel, QDate(curYear, curMonth, curDate));

                // 更新控件的隐藏显示情况
                connect(ui->month_calendar, SIGNAL(clicked(QDate)),
                        widget, SLOT(onSelectionChanged(QDate)));
                // 强制刷新
                /*
                connect(widget, SIGNAL(forceChangeCalendarPage(int,int)),
                        ui->month_calendar, SLOT(setCurrentPage(int,int)));
                        */
                connect(widget, SIGNAL(forceChangeCalendarPagePrevious()),
                        ui->month_calendar, SLOT(showPreviousMonth()));
                connect(widget, SIGNAL(forceChangeCalendarPageNext()),
                        ui->month_calendar, SLOT(showNextMonth()));
                widget->onSelectionChanged(ui->month_calendar->selectedDate());

                // 更新控件字体颜色
                // qDebug() << "widget->setCurDateInRange" << curMonth << month;
                widget->setCurDateInRange(curMonth == month);

                tableView->setIndexWidget(tableView->model()->index(i, j), widget);
            }
        }
    }
    else
    {
        int flag = -1;
        for (int i = 1; i < width; i++)
        {
            for (int j = 1; j < height; j++)
            {
                int curDate = tableView->model()->index(i, j).data().toInt();
                int curYear = year, curMonth = month;
                // qDebug() << "add" << year << month << date;

                if (flag == -1 && curDate != 1)
                {
                    curMonth--;
                    //continue;
                }
                else if (flag == -1 && curDate == 1)
                    flag = 0;
                else if (flag == 1)
                {
                    curMonth++;
                    //continue;
                }
                else if (flag == 0 && curDate == 1)
                {
                    curMonth++;
                    flag = 1;
                   // continue;
                }

                // qDebug() << "modify" << curYear << curMonth << curDate;
                CalendarEventFileWidget* widget = qobject_cast<CalendarEventFileWidget*>
                        (tableView->indexWidget(tableView->model()->index(i, j)));
                widget->setCurDate(QDate(curYear, curMonth, curDate));

                // 更新控件的隐藏显示
                widget->onSelectionChanged(ui->month_calendar->selectedDate());

                // 更新控件字体颜色
                // qDebug() << "widget->setCurDateInRange" << curMonth << month;
                widget->setCurDateInRange(curMonth == month);

            }
        }
    }

    /*
    for (int i = 1; i < height; i++)
    {
        for (int j = 1; j < width; j++)
        {
            // QComboBox *comBox = new QComboBox();
            // comBox->addItem("Y");
            // comBox->addItem("N");
            if (tableView->indexWidget(tableView->model()->index(j, i)) != NULL)
            {
                // delete tableView->indexWidget(tableView->model()->index(j, i));
                tableView->setIndexWidget(tableView->model()->index(j, i), NULL);
            }

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
    */
}

void MainWindow::on_quickCalendar_activated(const QDate &date)
{

}

void MainWindow::onActionEnableDragDropToggled(bool toggled)
{
    qDebug() << "MainWindow::onActionEnableDragDropToggled(bool toggled)" << toggled;
    // 设置calendar本身的drag & drop
    ui->month_calendar->setAcceptDrops(toggled);
    emit actionEnableDragDropToggled(toggled);
}

// 改变drag & drop
void MainWindow::setMonthCalendarChildWidgetsDragDrop(bool toggled)
{
    qDebug() << "MainWindow::setMonthCalendarChildWidgetsDragDrop(bool toggled)" << toggled;
    QTableView *tableView = ui->month_calendar->findChild<QTableView*>("qt_calendar_calendarview");
    int height = tableView->horizontalHeader()->count();
    int width = tableView->verticalHeader()->count();

    // 如果不是null，则改变其drag & drop能力
    if (tableView->indexWidget(tableView->model()->index(1, 1)) != NULL)
    {
        int flag = -1;
        for (int i = 1; i < width; i++)
        {
            for (int j = 1; j < height; j++)
            {
                // qDebug() << "modify" << curYear << curMonth << curDate;
                CalendarEventFileWidget* widget = qobject_cast<CalendarEventFileWidget*>
                        (tableView->indexWidget(tableView->model()->index(i, j)));

                // 更新其drag&drop能力
                widget->setAcceptDrops(toggled);
                widget->setFileBoxDrags(toggled);
            }
        }
    }
}

// 导出所有配置
void MainWindow::onActionExportTriggered()
{
    QString filename = QFileDialog::getSaveFileName(this,
            tr("Export all events to XML"), "./calendar_events.xml",
            tr("XML files (*.xml)"));

    if (filename.isEmpty())
        return;

    QList<QSharedPointer<Event>> events(cacheEventModel->allEvents());

    QProgressDialog progressDialog(tr("Exporting..."), "", 0, events.size(), this);
    progressDialog.setCancelButton(NULL);
    progressDialog.setWindowModality(Qt::WindowModal);
    progressDialog.setValue(0);
    progressDialog.resize(100, 100);
    // progressDialog.setWindowFlags(Qt::FramelessWindowHint);
    progressDialog.show();

    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement(EventDbContract::DB_NAME);


    /*
    xmlWriter.writeStartElement("Parameters");
    xmlWriter.writeTextElement("para1",QString::number(1));
    xmlWriter.writeTextElement("para2",QString::number(2));
    xmlWriter.writeTextElement("para3",QString::number(3));
    */

    for (int i = 0; i < events.size(); i++)
    {
        QSharedPointer<Event> event(events.at(i));
        xmlWriter.writeStartElement(EventDbContract::TABLE_NAME);
        xmlWriter.writeTextElement(EventDbContract::NAME, event->name());
        xmlWriter.writeTextElement(EventDbContract::DESCRIPTION, event->description());
        // 只写了datetime，不做区分了，因此少了几个
        xmlWriter.writeTextElement(EventDbContract::START_DATE, event->startDate()
                                   .toString("yyyy-MM-dd hh::mm::ss::zzz"));
        xmlWriter.writeTextElement(EventDbContract::END_DATE, event->endDate()
                                   .toString("yyyy-MM-dd hh::mm::ss::zzz"));
        xmlWriter.writeTextElement(EventDbContract::LOCATION, event->location());
        xmlWriter.writeTextElement(EventDbContract::COLOR, event->color().name());
        xmlWriter.writeTextElement(EventDbContract::ID, QString::number(event->id()));
        xmlWriter.writeTextElement(EventDbContract::REPEAT, event->repeat());
        xmlWriter.writeEndElement();

        progressDialog.setValue(i + 1);
    }

    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();
    file.close();

    QMessageBox::information(this, tr("Finished") ,tr("Export finished."));
}

void MainWindow::onActionImportTriggered()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("Import XML"), ".",
                                                    tr("XML files (*.xml)"));
    if (filename.isEmpty())
        return;

    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error: Cannot read file " << qPrintable(filename)
                 << ": " << qPrintable(file.errorString());
    }

    QProgressDialog progressDialog(tr("Importing..."), "", 0, 2, this);
    progressDialog.setCancelButton(NULL);
    progressDialog.setWindowModality(Qt::WindowModal);
    progressDialog.setValue(0);
    progressDialog.resize(100, 100);
    // progressDialog.setWindowFlags(Qt::FramelessWindowHint);
    progressDialog.show();
    // progressDialog.reset();

    QXmlStreamReader xmlReader(&file);
    xmlReader.readNext();

    QString name, description, location, repeat;
    QColor color;
    QDateTime startDate, endDate;
    int id;

    while(!xmlReader.atEnd())
    {
        qApp->processEvents();
        if (xmlReader.isStartElement())
        {
            if (xmlReader.name() == EventDbContract::DB_NAME)
            {
                xmlReader.readNext();
            }
            else if (xmlReader.name() == EventDbContract::TABLE_NAME)
            {
                xmlReader.readNext();
            }
            else if (xmlReader.name() == EventDbContract::NAME)
            {
                name = xmlReader.readElementText();
                xmlReader.readNext();
            }
            else if (xmlReader.name() == EventDbContract::DESCRIPTION)
            {
                description = xmlReader.readElementText();
                xmlReader.readNext();
            }
            else if (xmlReader.name() == EventDbContract::START_DATE)
            {
                startDate = QDateTime::fromString(xmlReader.readElementText(),
                                                  "yyyy-MM-dd hh::mm::ss::zzz");
                xmlReader.readNext();
            }
            else if (xmlReader.name() == EventDbContract::END_DATE)
            {
                endDate = QDateTime::fromString(xmlReader.readElementText(),
                                                  "yyyy-MM-dd hh::mm::ss::zzz");
                xmlReader.readNext();
            }
            else if (xmlReader.name() == EventDbContract::LOCATION)
            {
                location = xmlReader.readElementText();
                xmlReader.readNext();
            }
            else if (xmlReader.name() == EventDbContract::COLOR)
            {
                color = QColor(xmlReader.readElementText());
                xmlReader.readNext();
            }
            else if (xmlReader.name() == EventDbContract::ID)
            {
                id = xmlReader.readElementText().toInt();
                xmlReader.readNext();
            }
            else if (xmlReader.name() == EventDbContract::REPEAT)
            {
                repeat = xmlReader.readElementText();
                xmlReader.readNext();
            }
            else
            {
                xmlReader.raiseError(QObject::tr("Not a options file"));
            }
        }
        else if (xmlReader.isEndElement() && xmlReader.name() == EventDbContract::TABLE_NAME)
        {
            qDebug() << "Parsed event from XML: " << name << description << id << repeat;

            // 新建一个event，插入database中，且如果是重复事件，只插第一个
            QStringList list(repeat.split(','));
            int fatherId = list.at(0).toInt();
            if (fatherId != -1 && fatherId != id) // 是重复事件，但不是第一个，不插了
            {
                xmlReader.readNext(); // 忘了之后死循环+10086
                continue;
            }

            QSharedPointer<Event> event(new Event(name, startDate, endDate
                                                  , description, location, color, repeat));
            cacheEventModel->addEvent(event);
            qDebug() << "After adding the parsed event to db" << event->id() << event->repeat();

            xmlReader.readNext(); // 忘了之后死循环
        }
        else
        {
            xmlReader.readNext();
        }
    }
    file.close();

    // 需要刷新界面
    ui->month_calendar->showPreviousMonth();
    ui->month_calendar->showNextMonth();

    if (xmlReader.hasError())
    {
        qDebug() << "Error: Failed to parse file "
                 << qPrintable(filename) << ": "
                 << qPrintable(xmlReader.errorString());
        return;
    }
    else if (file.error() != QFile::NoError)
    {
        qDebug() << "Error: Cannot read file " << qPrintable(filename)
                 << ": " << qPrintable(file.errorString());
        return;
    }

    progressDialog.setValue(2);
    QMessageBox::information(this, tr("Finished") ,tr("Import finished."));
}

// 当状态改变，则切换语言
void MainWindow::onLanguageActionTriggered(bool toggled)
{
    QAction* action = dynamic_cast<QAction*>(sender());
    if (action == ui->chineseLanguageAction)
    {
        ui->englishLanguageAction->setChecked(!toggled);
        if (toggled) // 切换为中文
        {
            qApp->installTranslator(&chineseTranslator);
            qApp->installTranslator(&chineseQtTranslator);
            curLocale = QLocale(QLocale::Chinese, QLocale::China);
            QLocale::setDefault(curLocale);
            ui->retranslateUi(this);
            initInterface();
        }
    }
    else
    {
        ui->chineseLanguageAction->setChecked(!toggled);
        if (toggled) // 切换为英文
        {
            // qApp->installTranslator(&englishTranslator);
            qApp->removeTranslator(&chineseTranslator);
            qApp->removeTranslator(&chineseQtTranslator);
            curLocale = QLocale(QLocale::English, QLocale::UnitedStates);
            QLocale::setDefault(curLocale);
            ui->retranslateUi(this);
            initInterface();
        }
    }
}

void MainWindow::initInterface()
{
    on_quickCalendar_selectionChanged();
    on_month_calendar_currentPageChanged(QDate::currentDate().year(),
                                         QDate::currentDate().month());
    // 初始化day, week, 4 days日历
    initCalendarTable(1, ui->dayTableWidget);
    initCalendarTable(7, ui->weekTableWidget);
    initCalendarTable(4, ui->fourDaysTableWidget);
    // initCalendarTable();
}

void MainWindow::on_freezeCheckBox_toggled(bool checked)
{
    freeze(checked);
}

void MainWindow::on_createButton_clicked()
{
    CreateNewEventDialog* editEventDialog = new CreateNewEventDialog(this);
    editEventDialog->init(cacheEventModel, QSharedPointer<Event>(NULL), QDateTime::currentDateTime(),
                          QDateTime::currentDateTime());

    int result = editEventDialog->exec();

    // delete editEventDialog;

    if (result == QDialog::Accepted)
    {
        QSharedPointer<Event> newEvent(editEventDialog->getEvent());
    }
    ui->month_calendar->showPreviousMonth();
    ui->month_calendar->showNextMonth();
    refreshCalendarTable();
}
