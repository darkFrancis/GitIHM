#ifndef BRANCHWINDOW_HPP
#define BRANCHWINDOW_HPP

    #include <QMainWindow>
    #include <QListWidgetItem>

    namespace Ui {
        class BranchWindow;
    }

    /**
     * @class BranchWindow
     * @brief La classe BranchWindow défini la fenêtre de gestion des branches GIT.
     *
     * Ces branches peuvent être ajoutées, supprimées, renommées ou copiées. Toute
     * autre commande devra se faire manuellement depuis la fenêtre Git.@n
     * Header : BranchWindow.hpp
     */
    class BranchWindow : public QMainWindow
    {
        Q_OBJECT

        public:
            BranchWindow(QWidget *parent = nullptr);
            ~BranchWindow();

        public slots:
            void update_branches(QStringList branches);

        signals:
            /**
             * @param args Arguments à utiliser
             *
             * Ce signal est émit pour demander un appel à la commande Git depuis
             * la fenêtre Git de la fenêtre principale.
             */
            void action(QStringList args);

        private slots:
            void on_listWidget_branch_currentItemChanged(QListWidgetItem *current, QListWidgetItem*);
            void on_pushButton_add_clicked();
            void on_pushButton_rename_clicked();
            void on_pushButton_copy_clicked();
            void on_pushButton_remove_clicked();

        private:
            Ui::BranchWindow *ui;/**< UI de la classe BranchWindow */
            bool check_branch_name(QString name);
            QString get_selected();
    };

#endif // BRANCHWINDOW_HPP
