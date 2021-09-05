#include "Context.hpp"
#include "Logger.hpp"

#include <QFile>
#include <QTextStream>

#define INIT_FILE       "GitIHM.ini"
#define KW_GITDIR       "git-dir"
#define KW_TIMER        "timer-enable"
#define KW_TIMERTIME    "timer-seconds"

Context* Context::m_instance = nullptr;

Context::Context()
{
    init();
}

Context* Context::Instance()
{
    if(!m_instance)
        m_instance = new Context();
    return m_instance;
}

void Context::save()
{
    qLog->info("Enregistrement du fichier INI :", INIT_FILE);
    QFile file(INIT_FILE);
    if(file.open(QIODevice::Text | QIODevice::WriteOnly | QIODevice::Truncate))
    {
        QTextStream stream(&file);
        stream.setCodec("UTF-8");

        stream << KW_GITDIR << '=' << m_currentGitDir << endl;
        stream << KW_TIMER << '=' << (m_bTimerAuto ? "true" : "false") << endl;
        stream << KW_TIMERTIME << '=' << m_timerRefresh;
        file.close();
    }
    else
    {
        qLog->error("Echec d'ouverture du fichier");
    }
}

void Context::init()
{
    m_currentGitDir = ".";
    m_bTimerAuto = false;
    m_timerRefresh = 1;

    qLog->info("Lecture du fichier INI :", INIT_FILE);
    QFile file(INIT_FILE);
    if(!file.exists())
    {
        save();
    }
    else if(file.open(QIODevice::Text | QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        stream.setCodec("UTF-8");

        QStringList lines= stream.readAll().split('\n');
        file.close();

        for(QString& line : lines)
        {
            line = line.trimmed();
            int idx = line.indexOf('=');
            if(line != "" && idx != -1)
            {
                QString key = line.left(idx).trimmed();
                QString value = line.mid(idx+1).trimmed();
                if(key == KW_GITDIR) m_currentGitDir = value;
                else if(key == KW_TIMER) m_bTimerAuto = value == "true";
                else if(key == KW_TIMERTIME)
                {
                    m_timerRefresh = value.toInt();
                    // Gestion borne inf
                    if(m_timerRefresh < 1)
                        m_timerRefresh = 1;
                }
            }
        }
    }
    else
    {
        qLog->error("Echec d'ouverture du fichier");
    }
}