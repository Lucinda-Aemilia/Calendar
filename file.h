#ifndef FILE_H
#define FILE_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QDate>
#include <QStringList>
#include <QSharedPointer>

class File : public QObject
{
    Q_OBJECT

public:
    File();
    static bool copyFileToPath(const QString& sourceDir, QString toDir, bool coverFileIfExist);
    // 拷贝某一个文件到特定日期下
    static bool copyFileToPath(const QString &sourceDir, const QDate& date, bool coverFileIfExist);
    // 得到一个日期对应的文件的绝对路径列表
    static QList<QFileInfo> getAllFileNames(const QDate& date);
};

#endif // FILE_H
