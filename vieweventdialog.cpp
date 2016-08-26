#include "vieweventdialog.h"
#include "ui_vieweventdialog.h"

ViewEventDialog::ViewEventDialog(QWidget *parent) :
    CreateNewEventDialog(parent),
    ui(new Ui::ViewEventDialog)
    // ui(new Ui::CreateNewEventDialog)
{
    ui->setupUi(this);
    disableAllEdits(true);
    setButtonsToViewSet();
}

void ViewEventDialog::init(CacheEventModel *cacheEventModel, QSharedPointer<Event> event)
{
    fillWithEvent(event);

}

ViewEventDialog::~ViewEventDialog()
{
    delete ui;
}
