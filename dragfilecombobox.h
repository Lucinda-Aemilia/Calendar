#ifndef DRAGFILECOMBOBOX_H
#define DRAGFILECOMBOBOX_H

#include <QWidget>
#include <QComboBox>
#include <QDate>
#include <QDebug>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QGraphicsSceneMouseEvent>

namespace Ui {
class DragFileComboBox;
}

class DragFileComboBox : public QComboBox
{
    Q_OBJECT

public:
    explicit DragFileComboBox(QWidget *parent = 0);
    ~DragFileComboBox();
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent* event);

public slots:
    void refreshFileList(const QDate& date);

private:
    Ui::DragFileComboBox *ui;
    QDate mDate;
    QStringList mFileList;
};

#endif // DRAGFILECOMBOBOX_H
