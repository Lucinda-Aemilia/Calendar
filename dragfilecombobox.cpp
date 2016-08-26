#include "dragfilecombobox.h"
#include "ui_dragfilecombobox.h"
#include "file.h"

#include <QFileInfo>

DragFileComboBox::DragFileComboBox(QWidget *parent) :
    QComboBox(parent),
    ui(new Ui::DragFileComboBox)
{
    ui->setupUi(this);
}

DragFileComboBox::~DragFileComboBox()
{
    delete ui;
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
