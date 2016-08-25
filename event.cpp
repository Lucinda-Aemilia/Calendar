#include "event.h"

Event::Event(QString name, QDateTime startDate, QDateTime endDate, QString description,
             QString location, QColor color, QString repeat, int id, QObject *parent) :
    mName(name), mStartDate(startDate), mEndDate(endDate), mDescription(description),
    mLocation(location), mColor(color), mRepeat(repeat), mId(id), QObject(parent)
{ }

QString Event::name() const
{
    return mName;
}

void Event::setName(const QString &name)
{
    if (name != mName) {
        mName = name;
        emit nameChanged(mName);
    }
}

QDateTime Event::startDate() const
{
    return mStartDate;
}

void Event::setStartDate(const QDateTime &startDate)
{
    if (startDate != mStartDate) {
        mStartDate = startDate;
        emit startDateChanged(mStartDate);
    }
}

QDateTime Event::endDate() const
{
    return mEndDate;
}

void Event::setEndDate(const QDateTime &endDate)
{
    if (endDate != mEndDate) {
        mEndDate = endDate;
        emit endDateChanged(mEndDate);
    }
}

QString Event::description() const
{
    return mDescription;
}

void Event::setDescription(const QString &description)
{
    if (description != mDescription) {
        mDescription = description;
        emit descriptionChanged(mDescription);
    }
}

QString Event::location() const
{
    return mLocation;
}

void Event::setLocation(const QString &location)
{
    if (location != mLocation) {
        mLocation = location;
        emit locationChanged(mLocation);
    }
}

QColor Event::color() const
{
    return mColor;
}

void Event::setColor(const QColor &color)
{
    if (color != mColor) {
        mColor = color;
        emit colorChanged(mColor);
    }
}

int Event::id() const
{
    return mId;
}

void Event::setId(const int &id)
{
    if (id != mId)
    {
        mId = id;
        emit idChanged(mId);
    }
}

QString Event::repeat() const
{
    return mRepeat;
}

void Event::setRepeat(const QString &repeat)
{
    if (repeat != mRepeat)
    {
        mRepeat = repeat;
        emit repeatChanged(mRepeat);
    }
}
