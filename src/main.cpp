#include "MainWindow.hpp"
#include "Context.hpp"
#include <QApplication>
#include <QFile>
#include <QStyleFactory>
#include "Logger.hpp"

#define DARKSTYLE_FILE QString(":/darkstyle/darkstyle.qss")

void setStyle();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qLog->createLog("GitIHM.log");

    setStyle();

    MainWindow w;
    w.show();

    int returnCode = a.exec();
    qLog->info("Code retour de l'application", returnCode);

    qCtx->save();
    qLog->close();
    return returnCode;
}

void setStyle()
{
    qLog->info("Mise en place du style", DARKSTYLE_FILE);
    QFile file(DARKSTYLE_FILE);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qApp->setStyle(QStyleFactory::create("Fusion"));
        QFont defaultFont = QApplication::font();
        defaultFont.setPointSize(defaultFont.pointSize()+2);
        qApp->setFont(defaultFont);
        QPalette darkPalette;
        darkPalette.setColor(QPalette::Window,QColor(33,33,33));
        darkPalette.setColor(QPalette::WindowText,Qt::white);
        darkPalette.setColor(QPalette::Disabled,QPalette::WindowText,QColor(127,127,127));
        darkPalette.setColor(QPalette::Base,QColor(42,42,42));
        darkPalette.setColor(QPalette::AlternateBase,QColor(66,66,66));
        darkPalette.setColor(QPalette::ToolTipBase,Qt::white);
        darkPalette.setColor(QPalette::ToolTipText,Qt::white);
        darkPalette.setColor(QPalette::Text,Qt::white);
        darkPalette.setColor(QPalette::Disabled,QPalette::Text,QColor(127,127,127));
        darkPalette.setColor(QPalette::Dark,QColor(35,35,35));
        darkPalette.setColor(QPalette::Shadow,QColor(20,20,20));
        darkPalette.setColor(QPalette::Button,QColor(53,53,53));
        darkPalette.setColor(QPalette::ButtonText,Qt::white);
        darkPalette.setColor(QPalette::Disabled,QPalette::ButtonText,QColor(127,127,127));
        darkPalette.setColor(QPalette::BrightText,Qt::red);
        darkPalette.setColor(QPalette::Link,QColor(42,130,218));
        darkPalette.setColor(QPalette::Highlight,QColor(42,130,218));
        darkPalette.setColor(QPalette::Disabled,QPalette::Highlight,QColor(80,80,80));
        darkPalette.setColor(QPalette::HighlightedText,Qt::white);
        darkPalette.setColor(QPalette::Disabled,QPalette::HighlightedText,QColor(127,127,127));
        qApp->setPalette(darkPalette);

        QString style = QString(file.readAll());
        file.close();
        qApp->setStyleSheet(style);

        qLog->info("Style créé :", file.fileName());
    }
    else
    {
        qLog->error("Impossible de créer le style :", file.fileName());
    }
}