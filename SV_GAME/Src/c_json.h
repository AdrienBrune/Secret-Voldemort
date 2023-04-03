#ifndef C_JSON_H
#define C_JSON_H

#include "../../Shared/debug.h"

#include <QObject>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

class C_Json
{

public:
    void static saveParameter(const QString &paramName, const QString &value)
    {
        QString fileContent;
#ifdef __APPLE__
        QFile file("../config.json");
#else
        QFile file("config.json");
#endif

        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            LOG_DBG("Config file not found");
            return;
        }
        fileContent = file.readAll();
        file.close();

        QJsonDocument jsonFile = QJsonDocument::fromJson(fileContent.toUtf8());
        QJsonObject config = jsonFile.object();
        if(config.contains(paramName) && config[paramName].isString())
        {
            config[paramName] = value;
        }
        else
        {
            LOG_DBG("Parameter from config file couldn't have been saved");
        }

        if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            LOG_DBG("Config file not found");
            return;
        }
        file.write(QJsonDocument(config).toJson());
        file.close();
    }
    void static saveParameter(const QString &paramName, const double &value)
    {
        QString fileContent;
#ifdef __APPLE__
        QFile file("../config.json");
#else
        QFile file("config.json");
#endif

        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            LOG_DBG("Config file not found");
            return;
        }
        fileContent = file.readAll();
        file.close();

        QJsonDocument jsonFile = QJsonDocument::fromJson(fileContent.toUtf8());
        QJsonObject config = jsonFile.object();
        if(config.contains(paramName) && config[paramName].isDouble())
        {
            config[paramName] = value;
        }
        else
        {
            LOG_DBG("Parameter from config file couldn't have been saved");
        }

        if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            LOG_DBG("Config file not found");
            return;
        }
        file.write(QJsonDocument(config).toJson());
        file.close();
    }
    void static saveParameter(const QString &paramName, const bool &value)
    {
        QString fileContent;
#ifdef __APPLE__
        QFile file("../config.json");
#else
        QFile file("config.json");
#endif

        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            LOG_DBG("Config file not found");
            return;
        }
        fileContent = file.readAll();
        file.close();

        QJsonDocument jsonFile = QJsonDocument::fromJson(fileContent.toUtf8());
        QJsonObject config = jsonFile.object();
        if(config.contains(paramName) && config[paramName].isBool())
        {
            config[paramName] = value;
        }
        else
        {
            LOG_DBG("Parameter from config file couldn't have been saved");
        }

        if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            LOG_DBG("Config file not found");
            return;
        }
        file.write(QJsonDocument(config).toJson());
        file.close();
    }
    void static loadParameter(const QString &paramName, QString *value)
    {
        QString fileContent;
#ifdef __APPLE__
        QFile file("../config.json");
#else
        QFile file("config.json");
#endif
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            LOG_DBG("Config file not found");
            return;
        }
        fileContent = file.readAll();
        file.close();

        QJsonDocument jsonFile = QJsonDocument::fromJson(fileContent.toUtf8());
        QJsonObject config = jsonFile.object();
        if(config.contains(paramName) && config[paramName].isString())
        {
            *value = config[paramName].toString();
        }
        else
        {
            LOG_DBG("Parameter from config file couldn't have been loaded");
        }
    }
    void static loadParameter(const QString &paramName, double *value)
    {
        QString fileContent;
#ifdef __APPLE__
        QFile file("../config.json");
#else
        QFile file("config.json");
#endif
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            LOG_DBG("Config file not found");
            return;
        }
        fileContent = file.readAll();
        file.close();

        QJsonDocument jsonFile = QJsonDocument::fromJson(fileContent.toUtf8());
        QJsonObject config = jsonFile.object();
        if(config.contains(paramName) && config[paramName].isDouble())
        {
            *value = config[paramName].toDouble();
        }
        else
        {
            LOG_DBG("Parameter from config file couldn't have been loaded");
        }
    }
    void static loadParameter(const QString &paramName, bool *value)
    {
        QString fileContent;
#ifdef __APPLE__
        QFile file("../config.json");
#else
        QFile file("config.json");
#endif
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            LOG_DBG("Config file not found");
            return;
        }
        fileContent = file.readAll();
        file.close();

        QJsonDocument jsonFile = QJsonDocument::fromJson(fileContent.toUtf8());
        QJsonObject config = jsonFile.object();
        if(config.contains(paramName) && config[paramName].isBool())
        {
            *value = config[paramName].toBool();
        }
        else
        {
            LOG_DBG("Parameter from config file couldn't have been loaded");
        }
    }
};

#endif // C_JSON_H
