#ifndef C_JSON_H
#define C_JSON_H

#include "../../Shared/debug.h"

#include <QObject>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>
#include <QDir>

class C_Json
{

private:
#ifdef __APPLE__
    QString static getConfigFileName(){ return QString("config.json"); }
    QString static getConfigFilePath()
    {
        QString path(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
        QDir dirPath(path);
        dirPath.mkdir("Secret_Voldemort_Save");
        return QString(path + "/Secret_Voldemort_Save/" + getConfigFileName());
    }
#else
    QString static getConfigFileName(){ return QString("config.json"); }
    QString static getConfigFilePath(){ return QString(getConfigFileName()); }
#endif

public:
    void static initConfigFile()
    {
        QString fileContent;
        QFile file(getConfigFilePath());

        if(!file.open(QIODevice::ReadOnly))
        {
            LOG_DBG("Config file not found, file initialization ...");
            file.open(QIODevice::WriteOnly | QIODevice::Text);
            file.write("{\n"
                            "\t\"Enable music\": true,\n"
                            "\t\"Enable sounds\": true,\n"
                            "\t\"IP\": \"127.0.0.1\",\n"
                            "\t\"Name\": \"MyName\",\n"
                            "\t\"Port\": 30000,\n"
                            "\t\"Volume\": 50\n"
                        "}");
            file.close();
        }
    }
    void static saveParameter(const QString &paramName, const QString &value)
    {
        QString fileContent;
        QFile file(getConfigFilePath());

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
        QFile file(getConfigFilePath());

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
        QFile file(getConfigFilePath());

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
        QFile file(getConfigFilePath());

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
        QFile file(getConfigFilePath());

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
        QFile file(getConfigFilePath());

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
