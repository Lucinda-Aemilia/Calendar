#ifndef DRAGFILECOMBOBOX_H
#define DRAGFILECOMBOBOX_H

#include <QWidget>
#include <QComboBox>
#include <QDate>

namespace Ui {
class DragFileComboBox;
}

class DragFileComboBox : public QComboBox
{
    Q_OBJECT

public:
    explicit DragFileComboBox(QWidget *parent = 0);
    ~DragFileComboBox();

public slots:
    void refreshFileList(const QDate& date);

private:
    Ui::DragFileComboBox *ui;
    QDate mDate;
    QStringList mFileList;
};

#endif // DRAGFILECOMBOBOX_H
