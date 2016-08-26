#ifndef VIEWEVENTDIALOG_H
#define VIEWEVENTDIALOG_H

#include "createneweventdialog.h"

#include <QWidget>
#include <QSharedPointer>

namespace Ui {
class ViewEventDialog;
}

class ViewEventDialog : public CreateNewEventDialog
{
    Q_OBJECT

public:
    explicit ViewEventDialog(QWidget *parent = 0);
    void init(CacheEventModel *cacheEventModel, Event *event);
    ~ViewEventDialog();

private:
    Ui::ViewEventDialog *ui;
    // Ui::CreateNewEventDialog *ui;
};

#endif // VIEWEVENTDIALOG_H
