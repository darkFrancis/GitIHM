#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

    #include <QProcess>
    #include <QListWidget>
    #include <QTimer>
    #include <QMainWindow>

    #define GIT_COMMIT_DEFAULT_MSG QString("Commit without message")/**< Message par défaut pour un commit si aucun message n'est renseigné */
    #define GIT_COMMIT_PLACEHOLDER QString("Ajoutez un message au commit")/**< Affichage dans la ligne d'édition du commit si aucun message renseigné */

    #define GIT_STATUS_LABEL_0 QString("Non modifié")
    #define GIT_STATUS_LABEL_1 QString("Non suivi")
    #define GIT_STATUS_LABEL_2 QString("Ignoré")
    #define GIT_STATUS_LABEL_M QString("Modifié")
    #define GIT_STATUS_LABEL_A QString("Ajouté")
    #define GIT_STATUS_LABEL_D QString("Supprimé")
    #define GIT_STATUS_LABEL_R QString("Renommé")
    #define GIT_STATUS_LABEL_C QString("Copié")
    #define GIT_STATUS_LABEL_U QString("A jour non fusionné")

    namespace Ui {
        class MainWindow;
    }

    /**
     * @class MainWindow
     * @brief La classe MainWindow défini la fenêtre de gestion de Git.
     *
     * @n Header : MainWindow.hpp
     */
    class MainWindow : public QMainWindow
    {
        Q_OBJECT

        public:
            MainWindow(QWidget *parent = nullptr);
            ~MainWindow();
            void init();
            void clear();

        signals:
            /**
             * Ce signal est émit en cas de création d'un nouveau tag.
             */
            void tag_created();
            /**
             * @param tags Liste des tags
             *
             * Ce signal est émit pour mettre à jour la liste des tag dans la fenêtre
             * de gestion des tags.
             */
            void tag_update(QStringList tags);
            /**
             * @param branches Liste des tags
             *
             * Ce signal est émit pour mettre à jour la liste des branches dans la fenêtre
             * de gestion des branches.
             */
            void branch_update(QStringList branches);

        private slots:
            // Update
            void update_all();
            void update_status();
            void update_branches();
            void update_remote();
            // Commit
            void on_pushButton_commit_clicked();
            void on_checkBox_amend_stateChanged(int arg1);
            void on_lineEdit_commit_returnPressed();
            // Branches
            void on_toolButton_branch_clicked();
            void on_pushButton_branchMerge_clicked();
            void on_pushButton_branchSwitch_clicked();
            void on_comboBox_branch_currentIndexChanged(const QString &arg1);
            // Actions
            void on_pushButton_add_clicked();
            void on_pushButton_reset_clicked();
            void on_pushButton_checkout_clicked();
            void on_pushButton_gitk_clicked();
            void on_pushButton_tags_clicked();
            void on_pushButton_push_clicked();
            void on_pushButton_fetch_clicked();
            void on_pushButton_rebase_clicked();
            void on_pushButton_extra_clicked();
            void on_lineEdit_extra_returnPressed();
            void action_tags(QStringList args);
            void action_branch(QStringList args);
            void on_toolButton_gitDir_clicked();
            void on_toolButton_refresh_clicked();
            void on_checkBox_autoRefresh_stateChanged(int arg1);
            void on_spinBox_timerTime_valueChanged(int arg1);
            void on_pushButton_stash_clicked();
            void on_pushButton_pop_clicked();
            void on_pushButton_conflict_clicked();
            void closeMergeTool();

        private:
            bool action(QStringList args, bool status = true);
            QStringList getSelected(QListWidget* list_view, bool only_files = true);
            QStringList getAllItems(QListWidget* list_view, bool only_files = true);
            QString stateChar2Label(QChar c, bool staged = false);
            bool setGitDir(const QString& dirName);
            // Update
            bool checkForGitDir();
            void updateStash();
            // Status
            void status(const QString& msg);

        private:
            Ui::MainWindow *ui;/**< UI de la classe MainWindow */
            QProcess* m_process;/**< Processus pour exécution des commandes Git */
            int m_last_exit_code;/**< Dernier code retour du processus */
            QString m_output;/**< Sortie standard du dernier processus */
            QString m_error;/**< Erreur standard du dernier processus */
            QStringList m_unmerged;/**< Liste des fichiers en conflit */
            bool m_bInGitDir;
            QTimer m_timer;
    };

#endif // MAINWINDOW_HPP
