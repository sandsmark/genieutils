#include "widget.h"
#include <genie/util/Utility.h>
#include <genie/util/Logger.h>
#include <QApplication>
#include <QFileInfo>
#include <QDir>
#include <QCommandLineParser>
#include <QDebug>

int main(int argc, char *argv[])
{
    genie::Logger::setLogLevel(genie::Logger::L_DEBUG);
    QApplication a(argc, argv);
    a.setApplicationName("drsexplorer");
    a.setOrganizationName("genieutils");

    QCommandLineParser parser;

    parser.setApplicationDescription("GENIE picture (SLP, SMP, etc.) viewer");

    QCommandLineOption paletteOption("palette", "Palette to use (50500 from is the default)", "identifier", "50500");
    QCommandLineOption palDrs("palettedrs", "DRS file to load palette from (interfac.drs is the default)", "palette drs path");

    parser.addHelpOption();
    parser.addOption(paletteOption);
    parser.addOption(palDrs);
    parser.addOption({"swgb", "Star Wars: Galactic Battlegrounds mode"});
    parser.addPositionalArgument("drsfile", QCoreApplication::translate("file", "DRS file to open"));
    parser.process(a);

    const QStringList files = parser.positionalArguments();
    if (files.isEmpty()) {
        qWarning() << "No drs file specified";
        parser.showHelp(1); // automatically exits
    }


    const QString drsPath = QString::fromStdString(genie::util::resolvePathCaseInsensitive(files.first().toStdString()));
    if (drsPath.isEmpty() || !QFileInfo::exists(drsPath)) {
        qWarning() << "DRS path does not exist:" << drsPath;
        parser.showHelp(1); // automatically exits
    }

    QString palDrsPath = QString::fromStdString(genie::util::resolvePathCaseInsensitive(parser.value(palDrs).toStdString()));
    if (palDrsPath.isEmpty()) {
        palDrsPath = QFileInfo(drsPath).dir().absoluteFilePath("interfac.drs");
    }
    if (!QFileInfo::exists(palDrsPath)) {
        qWarning() << "Palette DRS path does not exist" << palDrsPath;
        parser.showHelp(1); // automatically exits
    }
    bool ok;
    int palette = parser.value(paletteOption).toInt(&ok);
    if (!ok) {
        qWarning() << "Invalid palette ID";
        parser.showHelp(1); // automatically exits
    }

    Widget w;
    w.swgbMode = parser.isSet("swgb");
    if (!w.loadPalette(palette, palDrsPath)) {
        qWarning() << "Failed to load palette";
        parser.showHelp(1); // automatically exits
    }
    if (!w.loadDrs(drsPath)) {
        qWarning() << "Failed to load palette";
        parser.showHelp(1); // automatically exits
    }

    w.show();

    return a.exec();
}
