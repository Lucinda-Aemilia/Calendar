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

/*
void DragFileComboBox::dragEnterEvent(QDragEnterEvent* event)
{
    qDebug() << "DragFileComboBox::dragEnterEvent(QDragEnterQSharedPointer<Event> event)";
    event->acceptProposedAction();
}

//! [dragMoveEvent() function]
void DragFileComboBox::dragMoveEvent(QDragMoveEvent* event)
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
    qDebug() << "DragFileComboBox::dragLeaveEvent(QDragLeaveQSharedPointer<Event> event)";
    event->accept();
}
//! [dragLeaveEvent() function]
*/

void DragFileComboBox::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "DragFileComboBox::mousePressEvent(QGraphicsSceneMouseQSharedPointer<Event> event)";
    /*
    QMimeData *data = new QMimeData;
    // data->setColorData();

    QDrag *drag = new QDrag(event->widget());
    drag->setMimeData(data);
    drag->start();
    */
    mDragStartPosition = event->pos();
    /*
    if (currentIndex() < 0)
        setCurrentIndex(0);
    else
        setCurrentIndex(-1);
    */
    showPopup();
    // QComboBox::mousePressEvent(event);
}

void DragFileComboBox::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << "DragFileComboBox::mouseMoveEvent(QMouseQSharedPointer<Event> event)";

    if (!(event->buttons() & Qt::LeftButton) ||
            // (event->pos() - mDragStartPosition).manhattanLength()
                        // < QApplication::startDragDistance() ||
            // < height() ||
            count() == 0)
    {
        QComboBox::mouseMoveEvent(event);
        return;
    }

    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;

    QString fileAbsolutePath = currentData().toString();
    qDebug() << fileAbsolutePath;

    QList<QUrl> fileList;
    fileList.append(QUrl::fromLocalFile(fileAbsolutePath));
    mimeData->setUrls(fileList);
    // mimeData->setText("Hello World");
    drag->setMimeData(mimeData);

    // Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction);
    Qt::DropAction dropAction = drag->exec(Qt::CopyAction, Qt::CopyAction);

    refreshFileList(mDate);
    QComboBox::mouseMoveEvent(event);
}

void DragFileComboBox::refreshFileList(const QDate &date)
{
    qDebug() << "DragFileComboBox::refreshFileList(const QDate &date)";
    mDate = date;
    mFileList = File::getAllFileNames(date);
    clear();
    for (int i = 0; i < mFileList.size(); i++)
    {
        QFileInfo fileInfo(mFileList.at(i));
        addItem(fileInfo.fileName(), fileInfo.absoluteFilePath());
        qDebug() << fileInfo.absoluteFilePath();
    }
}
