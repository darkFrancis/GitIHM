#ifndef CONTEXT_HPP
#define CONTEXT_HPP

    #include <QString>

    class Context
    {
        private:
            Context();

        public:
            static Context* Instance();
            void save();
            void setCurrentGitDir(const QString& dir)   { m_currentGitDir = dir;    }
            QString currentGitDir() const               { return m_currentGitDir;   }
            void setTimer(bool enable)                  { m_bTimerAuto = enable;    }
            bool timer() const                          { return m_bTimerAuto;      }
            void setTimerTime(int seconds)              { m_timerRefresh = seconds; }
            int timerTime() const                       { return m_timerRefresh;    }

        private:
            void init();

        private:
            static Context* m_instance;
            QString m_currentGitDir;
            bool m_bTimerAuto;
            int m_timerRefresh;
    };

    #define qCtx Context::Instance()

#endif // CONTEXT_HPP
