#ifndef LOGGER_H
#define LOGGER_H

    #include <QString>
    #include <QFile>
    #include <QTextStream>

    class Logger
    {
        public:
            enum LogLevel {
                Debug,
                Info,
                Warning,
                Error,
                Fatal
            };

        private:
            Logger();

        public:
            static Logger* Instance();
            void createLog(const QString& fileName, bool overwrite = false);
            void close();
            void addKeyWord(const QString& kw);
            void removeKeyWord();
            template<class ...Args> void debug(Args... args) { reset(); log(Debug, args...); }
            template<class ...Args> void info(Args... args) { reset(); log(Info, args...); }
            template<class ...Args> void warning(Args... args) { reset(); log(Warning, args...); }
            template<class ...Args> void error(Args... args) { reset(); log(Error, args...); }
            template<class ...Args> void fatal(Args... args) { reset(); log(Fatal, args...); }

        private:
            void reset();
            void printInfo(LogLevel lvl);
            template<class T> void log(LogLevel lvl, const T& t) { m_buffer << t; printInfo(lvl); }
            template<class T, class ...Args> void log(LogLevel lvl, const T& t, Args... args) { m_buffer << t << ' '; log(lvl, args...); }

        private:
            static Logger* m_instance;
            QTextStream m_buffer;
            QString m_output;
            QFile m_logFile;
            QTextStream m_logStream;
            QStringList m_kw;
    };

    #define qLog Logger::Instance()

#endif // LOGGER_H
