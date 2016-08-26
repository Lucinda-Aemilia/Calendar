#include "dragfilecombobox.h"
#include "ui_dragfilecombobox.h"
#include "file.h"

#include <QFileInfo>
#include <QDrag>

DragFileComboBox::DragFileComboBox(QWidget *parent) :
    QComboBox(parent),
    ui(new Ui::DragFileComboBox)
{
    ui->setupUi(this);
    setAcceptDrops(true);
}

DragFileComboBox::~DragFileComboBox()
{
    delete ui;
}

//! [dragEnterEvent() function]
void DragFileComboBox::dragEnterEvent(QDragEnterEvent *event)
{
    /*
    setText(tr("<drop content>"));
    setBackgroundRole(QPalette::Highlight);


    emit changed(event->mimeData());
    */
    qDebug() << "DragFileComboBox::dragEnterEvent(QDragEnterEvent *event)";
    event->acceptProposedAction();
}
//! [dragEnterEvent() function]

//! [dragMoveEvent() function]
void DragFileComboBox::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}
//! [dragMoveEvent() function]

//! [dropEvent() function part1]
void DragFileComboBox::dropEvent(QDropEvent *event)
{
    event->acceptProposedAction();
}
//! [dropEvent() function part3]

//! [dragLeaveEvent() function]
void DragFileComboBox::dragLeaveEvent(QDragLeaveEvent *event)
{
    // clear();
    qDebug() << "DragFileComboBox::dragLeaveEvent(QDragLeaveEvent *event)";
    event->accept();
}
//! [dragLeaveEvent() function]

void DragFileComboBox::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "DragFileComboBox::mousePressEvent(QGraphicsSceneMouseEvent *event)";
    /*
    QMimeData *data = new QMimeData;
    // data->setColorData();

    QDrag *drag = new QDrag(event->widget());
    drag->setMimeData(data);
    drag->start();
    */
    QComboBox::mousePressEvent(event);
}

void DragFileComboBox::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << "DragFileComboBox::mouseMoveEvent(QMouseEvent *event)";
    QComboBox::mouseMoveEvent(event);
}

void DragFileComboBox::refreshFileList(const QDate &date)
{
    mDate = date;
    mFileList = File::getAllFileNames(date);
    clear();
    for (int i = 0; i < mFileList.size(); i++)
    {
        QFileInfo fileInfo(mFileList.at(i));
        addItem(fileInfo.fileName());
    }
}
