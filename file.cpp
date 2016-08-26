#include "file.h"

#include <QDebug>

File::File()
{

}

//拷贝文件：
bool File::copyFileToPath(const QString& sourceDir, QString toDir, bool coverFileIfExist)
{
    qDebug() << "bool File::copyFileToPath(const QString& sourceDir, QString toDir, bool coverFileIfExist)";
    qDebug() << sourceDir << toDir;
    toDir.replace("\\","/");
    if (sourceDir == toDir){
        return true;
    }
    if (!QFile::exists(sourceDir))
    {
        qDebug() << "source dir does not exist";
        return false;
    }
    QDir *createfile = new QDir;
    bool exist = createfile->exists(toDir);
    if (exist){
        if(coverFileIfExist){
            createfile->remove(toDir);
        }
    }//end if

    if(!QFile::copy(sourceDir, toDir))
    {
        qDebug() << "copy failed";
        return false;
    }
    return true;
}

bool File::copyFileToPath(const QString &sourceDir, const QDate& date, bool coverFileIfExist)
{
    qDebug() << "bool File::copyFileToPath(const QString &sourceDir, const QDate& date, bool coverFileIfExist)";
    QFileInfo sourceDirInfo(sourceDir);

    QString toDirName(QDir::currentPath() + "/Appdata/" + date.toString("yyyy_MM_dd"));
    QDir toDir(toDirName);

    // 如果目录不存在则新建
    if (!toDir.exists())
    {
        toDir.mkpath(toDirName);
    }

    toDirName += "/" + sourceDirInfo.fileName();
    // QString toDir("./Appdata/" + sourceDirInfo.fileName());

    qDebug() << toDirName;
    return copyFileToPath(sourceDir, toDirName, coverFileIfExist);
}

QStringList File::getAllFileNames(const QDate &date)
{
    QString dirName(QDir::currentPath() + "/Appdata/" + date.toString("yyyy_MM_dd"));
    QDir dir(dirName);
    return dir.entryList(QDir::Files);
}
