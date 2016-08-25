#ifndef VIEWEVENTDIALOG_H
#define VIEWEVENTDIALOG_H

#include "createneweventdialog.h"

#include <QObject>

class ViewEventDialog : public CreateNewEventDialog
{
    Q_OBJECT

public:

    explicit ViewEventDialog(QWidget *parent = 0);
    ~ViewEventDialog();

    void init(CacheEventModel* cacheEventModel, Event* event);

private:
    Event* getEvent();
};

#endif // VIEWEVENTDIALOG_H
