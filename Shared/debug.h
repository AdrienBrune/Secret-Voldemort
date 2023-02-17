#ifndef DEBUG_H
#define DEBUG_H

#include <QDebug>
#include <QFile>
#include <QDate>

#ifdef LOG_VIA_FILE
    #define LOG_DBG(str) log(QString("%1 Ligne[%2] Function[%3] : %4\n").arg(QDateTime::currentDateTime().toString()).arg(__LINE__).arg(__FUNCTION__).arg(str))
#elif SERVER_FEATURES
    #define LOG_DBG(str) ui->outputLog->append(QString("%1 Ligne[%2] Function[%3] : %4\n").arg(QDateTime::currentDateTime().toString()).arg(__LINE__).arg(__FUNCTION__).arg(str))
#else
    #define LOG_DBG(str) qDebug() << __LINE__ << __FUNCTION__ << str
#endif

static void log(QString message)
{
    QFile file("Log.txt");

    file.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Append);
    QTextStream stream(&file);
    stream << message;
    file.close();
};

#endif // DEBUG_H
