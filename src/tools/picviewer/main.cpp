#include "Window.h"

#include <QApplication>
#include <QCommandLineParser>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("picviewer");
    a.setOrganizationName("genieutils");

    QCommandLineParser parser;

    parser.setApplicationDescription("GENIE picture (SLP, SMP, etc.) viewer");

    QCommandLineOption drsPath("drs", "Path to DRS file to load from", "drs identifier");
    QCommandLineOption palFile("palette", "Palette path/name to use (50500 from interfac.drs is the default)", "identifier", "50500");
    QCommandLineOption palDrs("palettedrs", "DRS file to load palette from", "drs identifier");

    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOption(drsPath);
    parser.addOption(palFile);
    parser.addOption(palDrs);
    parser.addPositionalArgument("picturefile", QCoreApplication::translate("file", "Path/name of picture to show"));
    parser.process(a);

    Window w;
    if (parser.isSet(drsPath)) {
        if (!w.setDrsPath(parser.value(drsPath))) {
            std::cerr << "Failed to load drs" << std::endl;
            return 1;
        }
    }

    if (parser.isSet(palFile)) {
        w.setPalette(parser.value(palFile), parser.value(palDrs));
    }

    if (parser.positionalArguments().isEmpty()) {
        // qdebug doesn't really work before qapplication is running
        std::cerr << "No picture files specified" << std::endl;
        return 1;
    } else if (!w.load(parser.positionalArguments().first())) {
        std::cerr << "Failed to load picture" << std::endl;
        return 1;
    } else {
        w.show();
    }

    return a.exec();
}
