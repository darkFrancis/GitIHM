#include "TagsWindow.hpp"
#include "ui_TagsWindow.h"
#include <QMessageBox>

/**
 * @param parent Le QWidget parent de cette fenêtre
 * @param tags Liste de tags à afficher
 *
 * Contructeur de la classe TagsWindow.@n
 * Ce constructeur hérite de celui de QMainWindow et utilise le système des fichiers
 * d'interface utilisateur.@n
 * Ce constructeur va appeler la fonction TagsWindow::update_tags pour initialiser la
 * liste des tags et rend ensuite la fenêtre modale.
 */
TagsWindow::TagsWindow(QWidget *parent, QStringList tags) :
    QMainWindow(parent),
    ui(new Ui::TagsWindow)
{
    ui->setupUi(this);
    update_tags(tags);

    this->setWindowModality(Qt::ApplicationModal);
    this->setAttribute(Qt::WA_QuitOnClose, false);
}

/**
 * Destructeur de la classe TagsWindow.
 */
TagsWindow::~TagsWindow()
{
    delete ui;
}

/**
 * @param tags Nouvelle liste des tags
 *
 * Met à jour la liste des tags de la fenêtre en la remplaçant par celle
 * passée en argument.
 */
void TagsWindow::update_tags(QStringList tags)
{
    for(int i = 0; i < tags.length(); i++)
    {
        if(tags[i].simplified() == "")
        {
            tags.removeAt(i);
            i--;
        }
    }
    ui->listWidget_tags->clear();
    ui->listWidget_tags->addItems(tags);
    ui->listWidget_tags->sortItems();
}

/**
 * Efface les champs à renseigner pour ajouter un nouveau tag.
 */
void TagsWindow::clean_tag_name()
{
    ui->lineEdit_msg->clear();
    ui->lineEdit_name->clear();
    ui->lineEdit_commit->clear();
}

/**
 * Ce connecteur est activé suite à un clic souris de l'utilisateur sur le
 * bouton Ajouter.@n
 * Ajoute le tag renseigné en émettant le signal TagsWindow::action puis
 * l'ajoute à la liste.
 */
void TagsWindow::on_pushButton_add_clicked()
{
    if(ui->lineEdit_name->text().simplified() == "")
    {
        QMessageBox::warning(this,
                             "Action impossible",
                             "Il faut remplir au moins le champs nom pour ajouter un tag !");
    }
    else
    {
        QStringList args = QStringList() << "tag" << ui->lineEdit_name->text().simplified();
        if(ui->lineEdit_msg->text().simplified() != "")
        {
            args << "-m" << ui->lineEdit_msg->text().simplified();
        }
        if(ui->lineEdit_commit->text().simplified() != "")
        {
            args << ui->lineEdit_commit->text().simplified();
        }
        emit action(args);
    }
}

/**
 * Ce connecteur est activé suite à un clic souris de l'utilisateur sur le
 * bouton Push.@n
 * Pousse les tags en émettant le signal TagsWindow::action.
 */
void TagsWindow::on_pushButton_push_clicked()
{
    emit action(QStringList() << "push");
}

/**
 * Ce connecteur est activé suite à un clic souris de l'utilisateur sur le
 * bouton Push.@n
 * Supprime les tags en émettant le signal TagsWindow::action.
 */
void TagsWindow::on_pushButton_remove_clicked()
{
    QStringList tmp;
    for(int i = 0; i < ui->listWidget_tags->count(); i++)
    {
        if(ui->listWidget_tags->item(i)->isSelected())
        {
            tmp << ui->listWidget_tags->item(i)->text();
        }
    }
    emit action(QStringList() << "tag" << "-d" << tmp);
}
