#include "Logger.hpp"

#include <QDateTime>
#include <QDebug>

#define LOG_CODEC "UTF-16"

Logger* Logger::m_instance = nullptr;

Logger::Logger()
{
    reset();
}

Logger* Logger::Instance()
{
    if(!m_instance)
        m_instance = new Logger();
    return m_instance;
}

void Logger::createLog(const QString& fileName, bool overwrite)
{
    if(m_logFile.isOpen())
    {
        close();
    }

    m_logFile.setFileName(fileName);
    QFile::OpenMode openMode = QIODevice::WriteOnly;
    if(overwrite)
    {
        openMode |= QIODevice::Truncate;
    }
    if(m_logFile.open(openMode))
    {
        m_logStream.setDevice(&m_logFile);
        m_logStream.setCodec(LOG_CODEC);
    }
}

void Logger::close()
{
    m_logFile.close();
}

void Logger::addKeyWord(const QString& kw)
{
    m_kw << kw;
}

void Logger::removeKeyWord()
{
    m_kw.removeLast();
}

void Logger::reset()
{
    m_buffer.setString(&m_output, QIODevice::WriteOnly | QIODevice::Truncate);
    m_buffer.setCodec(LOG_CODEC);
    m_output = "";
}

void Logger::printInfo(LogLevel lvl)
{
    if(m_logFile.isOpen())
    {
        // Info log
        QDateTime datetime = QDateTime::currentDateTime();
        QString datetimeStr = datetime.toString("yyyy-MM-dd HH:mm:ss.zzz | ");
        QString errStr;
        switch(lvl)
        {
            case Debug:
                errStr = "DBG | ";
                qDebug() << m_output;
                break;
            case Info:
                errStr = "INF | ";
                qInfo() << m_output;
                break;
            case Warning:
                errStr = "WRN | ";
                qWarning() << m_output;
                break;
            case Error:
                errStr = "ERR | ";
                qCritical() << m_output;
                break;
            case Fatal:
                errStr = "FTL | ";
                qCritical() << m_output.toStdString().c_str();
                break;
        }
        QString kw = m_kw.join(" - ");
        if(kw != "")
            kw += " - ";

        QStringList lines = m_output.split('\n');
        // Write
        for(const QString& str: lines)
        {
            if(str.trimmed() != "")
            {
                m_logStream << datetimeStr << errStr << kw << str.trimmed() << endl;
                errStr = "    | "; // Reset du mot clé d'erreur pour une meilleure lecture des logs
            }
        }
    }
}
