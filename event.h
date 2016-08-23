#ifndef EVENT_H
#define EVENT_H

#include <QDateTime>
#include <QObject>
#include <QString>
#include <QColor>

class Event : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QDateTime startDate READ startDate WRITE setStartDate NOTIFY startDateChanged)
    Q_PROPERTY(QDateTime endDate READ endDate WRITE setEndDate NOTIFY endDateChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString location READ location WRITE setLocation NOTIFY locationChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(int repeat READ repeat WRITE setRepeat NOTIFY repeatChanged)

public:
    explicit Event(QObject *parent = 0);

    QString name() const;
    void setName(const QString &name);

    QDateTime startDate() const;
    void setStartDate(const QDateTime &startDate);

    QDateTime endDate() const;
    void setEndDate(const QDateTime &endDate);

    QString description() const;
    void setDescription(const QString &description);

    QString location() const;
    void setLocation(const QString &location);

    QColor color() const;
    void setColor(const QColor &color);

    int id() const;
    void setId(const int& id);

    int repeat() const;
    void setRepeat(const int& repeat);

signals:
    void nameChanged(const QString &name);
    void startDateChanged(const QDateTime &startDate);
    void endDateChanged(const QDateTime &endDate);
    void descriptionChanged(const QString &description);
    void locationChanged(const QString &location);
    void colorChanged(const QColor &color);
    void idChanged(const int& id);
    void repeatChanged(const int& repeat);


private:
    QString mName;
    QDateTime mStartDate;
    QDateTime mEndDate;
    QString mDescription;
    QString mLocation;
    QColor mColor;
    int mId;
    int mRepeat;
};

#endif
