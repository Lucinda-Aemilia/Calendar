#ifndef EVENTDBCONTRACT_H
#define EVENTDBCONTRACT_H

#include <QString>

class EventDbContract
{
public:
    EventDbContract();
    static const QString DB_NAME;
    static const QString TABLE_NAME;
    static const QString HEADING;
    static const QString DESCRIPTION;
    static const QString START_DATE;
    static const QString START_TIME;
    static const QString END_DATE;
    static const QString END_TIME;
    // 我决定不管repeat了，直接搞成很多个
    // static const QString REPEAT_TYPE;
    // static const QString REPEAT_FREQUENCY;
    // static const QString REPEAT_TIMES;
    static const QString LOCATION;
    static const QString COLOR;
};

#endif // EVENTDBCONTRACT_H
