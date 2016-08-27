#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "windows.h"
#include "createneweventdialog.h"
#include "calendareventfilewidget.h"
#include "event.h"

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
    on_month_calendar_currentPageChanged(QDate::currentDate().year(),
                                         QDate::currentDate().month());


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
    // ui->dayButton->setWindowFlags(Qt::WindowTransparentForInput);
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
        if (mFrozen)
        {
            // qDebug() << "MainWindow::eventFilter" << event->type();
            // show();
            // QApplication::sendEvent(QApplication::desktop(), event);
            return true;
        }
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
    if (frozen && !mFrozen)
    {
        windowMinSize = this->minimumSize();
        windowMaxSize = this->maximumSize();
        windowPos = this->pos();
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

        // setWindowFlags(Qt::FramelessWindowHint);
        // windowFlags();
        // setAttribute(Qt::WA_TranslucentBackground, true);
        // setAttribute(Qt::WA_TransparentForMouseEvents, true);
        // setWindowFlags(Qt::WindowTransparentForInput);
        // show();

        // QMouseEvent event(QEvent::MouseButtonPress, pos, 0, 0, 0);
        // Application::sendEvent(QApplication::desktop(), &event);
        // this->move(windowPos);
        // show();
        mFrozen = true;
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
    // qDebug() << event->type();
    if (mFrozen)
    {
        this->move(windowPos);
    }
    QMainWindow::showEvent(event);
}

void MainWindow::moveEvent(QMoveEvent *event)
{
    qDebug() << event->type();
    if (mFrozen)
    {
        this->move(windowPos);
        // return;
    }
    QMainWindow::moveEvent(event);
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
    // qDebug() << "active day" << toggled;
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
    QLocale lo(QLocale::C);
    ui->dateDisplayLabel->setText(lo.toDate(date.toString("yyyy-MM-dd"), "yyyy-MM-dd")
                                  .toString("yyyy.M.d  dddd"));
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
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();

    QList<QSharedPointer<Event>> events(cacheEventModel->allEvents());

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
    }

    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();
    file.close();
}

void MainWindow::onActionImportTriggered()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("Import XML"), ".",
                                                    tr("XML files (*.xml)"));
    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error: Cannot read file " << qPrintable(filename)
                 << ": " << qPrintable(file.errorString());
    }
    QXmlStreamReader xmlReader(&file);
    xmlReader.readNext();
    while(!xmlReader.atEnd())
    {
        if(xmlReader.isStartElement())
        {
            if(xmlReader.name() == "Parameters")
            {
                xmlReader.readNext();
            }
            else if (xmlReader.name() == "para1")
            {
                int para1 = xmlReader.readElementText().toInt();
                xmlReader.readNext();
            }
            else if (xmlReader.name() == "preFilterCap")
            {
                int para2 = xmlReader.readElementText().toInt();
                xmlReader.readNext();
            }
            else if (xmlReader.name() == "SADWindowSize")
            {
                int para3 = xmlReader.readElementText().toInt();
                xmlReader.readNext();
            }
            else
            {
                xmlReader.raiseError(QObject::tr("Not a options file"));
            }
        }
        else
        {
            xmlReader.readNext();
        }
    }
    file.close();
    if (xmlReader.hasError())
    {
        qDebug() << "Error: Failed to parse file "
                 << qPrintable(filename) << ": "
                 << qPrintable(xmlReader.errorString());
    }
    else if (file.error() != QFile::NoError)
    {
        qDebug() << "Error: Cannot read file " << qPrintable(filename)
                 << ": " << qPrintable(file.errorString());
    }
}
