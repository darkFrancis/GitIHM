#include "BranchWindow.hpp"
#include "ui_BranchWindow.h"

#include <QMessageBox>

#include "Logger.hpp"

/**
 * @param parent Le QWidget parent de cette fenêtre
 *
 * Contructeur de la classe BranchWindow.@n
 * Ce constructeur hérite de celui de QMainWindow et utilise le système des fichiers
 * d'interface utilisateur.@n
 * Ce constructeur rend la fenêtre modale.
 */
BranchWindow::BranchWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BranchWindow)
{
    ui->setupUi(this);

    this->setWindowModality(Qt::ApplicationModal);
    this->setAttribute(Qt::WA_QuitOnClose);
}

/**
 * Destructeur de la classe BranchWindow.
 */
BranchWindow::~BranchWindow()
{
    delete ui;
}

/**
 * @param branches Nouvelle liste des branches
 *
 * Met à jour la liste des branches en la remplaçant par la liste
 * @c branches fournies en argument de cette fonction. Si l'élément
 * qui était précédemment sélectionné est modifié/supprimé, le
 * premier élément de la liste devient l'élément sélectionné.
 */
void BranchWindow::update_branches(QStringList branches)
{
    qLog->info("BranchWindow - Mise à jour des branches");
    ui->lineEdit_add->clear();

    QString branch;
    QString selected = get_selected();
    for(int i = 0; i < branches.length(); i++)
    {
        branch = branches[i];
        if(branch.simplified() == "")
        {
            branches.removeAt(i);
            i--;
        }
        else if(branch[0] == QChar('*'))
        {
            branches[i] = branch.split(' ').at(1).simplified();
        }
        else
        {
            branches[i] = branch.simplified();
        }
    }

    // Ajout/Suppression
    for(int i = 0; i < ui->listWidget_branch->count(); i++)
    {
        branch = ui->listWidget_branch->item(i)->text().simplified();
        if(branches.contains(branch))
        {
            branches.takeAt(branches.indexOf(branch));
        }
        else
        {
            ui->listWidget_branch->takeItem(i);
            i--;
        }
    }
    if(branches.length() > 0)
    {
        ui->listWidget_branch->addItems(branches);
        ui->listWidget_branch->sortItems();
    }

    // Gestion sélection
    for(int i = 0; i < ui->listWidget_branch->count(); i++)
    {
        if(ui->listWidget_branch->item(i)->text() == selected)
        {
            ui->listWidget_branch->item(i)->setSelected(true);
            on_listWidget_branch_currentItemChanged(ui->listWidget_branch->item(i), nullptr);
            return;
        }
    }
    ui->listWidget_branch->item(0)->setSelected(true);
    on_listWidget_branch_currentItemChanged(ui->listWidget_branch->item(0), nullptr);
}

/**
 * @param current Nouvel item sélectionné
 *
 * Ce connecteur est activé lorsque l'utilisateur sélectionne un
 * nouvel item dans la liste.@n
 * Change le label de la branche sélectionnée avec le nom de l'item
 * @c current sélectionné.
 */
void BranchWindow::on_listWidget_branch_currentItemChanged(QListWidgetItem *current, QListWidgetItem*)
{
    ui->label_selected->setText(current->text() + " ->");
}

/**
 * Ce connecteur est activé par un clic souris de l'utilisateur sur le
 * bouton Ajouter.@n
 * Si la fonction BranchWindow::check_branch_name appliquée au nom de
 * branche renseignée par l'utilisateur renvoie @b true, ajoute une
 * nouvelle branche grâce à l'émission du signal BranchWindow::action.
 */
void BranchWindow::on_pushButton_add_clicked()
{
    QString name = ui->lineEdit_add->text().simplified();
    if(check_branch_name(name)) emit action(QStringList() << "branch" << name);
}

/**
 * Ce connecteur est activé par un clic souris de l'utilisateur sur le
 * bouton Renommer.@n
 * Si la fonction BranchWindow::check_branch_name appliquée au nom de
 * branche renseignée par l'utilisateur renvoie @b true, renomme la
 * branche sélectionnée grâce à l'émission du signal BranchWindow::action.
 */
void BranchWindow::on_pushButton_rename_clicked()
{
    QString name = ui->lineEdit_add->text().simplified();
    if(check_branch_name(name))
    {
        QString branch = get_selected();
        if(branch != "") emit action(QStringList() << "branch" << "-m" << branch << name);
    }
}

/**
 * Ce connecteur est activé par un clic souris de l'utilisateur sur le
 * bouton Copier.@n
 * Si la fonction BranchWindow::check_branch_name appliquée au nom de
 * branche renseignée par l'utilisateur renvoie @b true, copie la branche
 * sélectionnée grâce à l'émission du signal BranchWindow::action.
 */
void BranchWindow::on_pushButton_copy_clicked()
{
    QString name = ui->lineEdit_add->text().simplified();
    if(check_branch_name(name))
    {
        QString branch = get_selected();
        if(branch != "") emit action(QStringList() << "branch" << "-c" << branch << name);
    }
}

/**
 * Ce connecteur est activé par un clic souris de l'utilisateur sur le
 * bouton Supprimer.@n
 * Supprime la branche sélectionnée grâce à l'émission du signal
 * BranchWindow::action.
 */
void BranchWindow::on_pushButton_remove_clicked()
{
    QString branch = get_selected();
    if(branch != "") emit action(QStringList() << "branch" << "-d" << branch);
}

bool _check_branch_global_name(const QString& name)
{
    if(name.contains("..") ||
            name.endsWith(".lock") ||
            name.endsWith("/") ||
            name.contains("/.") ||
            name.startsWith('.') ||
            name.contains("@{"))
        return false;
    return true;
}

bool _check_branch_name_char(char ch)
{
        if(unsigned(ch) <= ' ' ||
                ch == '~'  ||
                ch == '^'  ||
                ch == ':'  ||
                ch == '\\' ||
                ch == '?'  ||
                ch == '['  ||
                ch == '*')
                return false;
        return true;
}

/**
 * @param name Nom de branche Git
 * @return Booléen de vérification
 *
 * Vérifie que le nom de la branche ne contient que des caractères autorisés.
 * Sinon, affiche une popup d'erreur.
 */
bool BranchWindow::check_branch_name(QString name)
{
    auto raiseErr = [this]() {
        QMessageBox::warning(this,
                             "Attention",
                             "Nom de branche impossible !");
        return false;
    };
    if(!_check_branch_global_name(name))
        return raiseErr();
    for(int i = 1; i < name.length()-1; i++)
    {
        if(!_check_branch_name_char(name[i].toLatin1()))
        {
            return raiseErr();
        }
    }
    return true;
}

/**
 * @return Elément sélectionné
 *
 * Cherche l'élément sélectionné dans la liste et renvoie son texte.
 */
QString BranchWindow::get_selected()
{
    if(ui->listWidget_branch->selectedItems().length() > 0)
    {
        return ui->listWidget_branch->selectedItems().at(0)->text().simplified();
    }
    return "";
}
