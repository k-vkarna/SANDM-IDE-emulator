#include "mainwindow.h"

#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QtGui>
#include <QTextStream>

//точка входа в программу
int main(int argc, char *argv[])
{
    //-------------------------------------------------------------------------------------------------------------------------------

#ifdef _WIN32
    if (AttachConsole(ATTACH_PARENT_PROCESS)) {
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
    }
#endif

    QApplication a(argc, argv);

    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&qtTranslator);

    MainWindow w;
    w.showMaximized();
    return a.exec();

    //-------------------------------------------------------------------------------------------------------------------------------

    //    if (argc != 3)
    //        Error(0, 0);
    //    if (!stricmp(argv[1],"-p") || !stricmp(argv[1],"--plain"))
    //    {
    //        /*
    //            CAsm asem;
    //            int err = asem.Open(argv[2]);
    //            if (err) {
    //                printf("CAsm::Open ERROR: %i\n", err);
    //                if (asem.IndexStr()) printf("Current string: %i\n", asem.IndexStr());
    //            } else
    //                printf("OK\n");
    //            return 0;
    //    */
    //        Compiler a;
    //        int res = a.Open(argv[2]);
    //        if (res) Error(res, a.NumLine());
    //        VM b;
    //        res = b.execute(a.CountCmd(), a.GetByteCode());
    //        if(res) Error(res, 0);
    //    }
    //    else if (!stricmp(argv[1],"-b") || !stricmp(argv[1],"--byte"))
    //    {
    //        VM b;
    //        int res = b.executeFile(argv[2]);
    //        if(res) Error(res, 0);
    //    }
    //    else
    //        Error(0, 0);
    //    printf("\n");
    //    system("pause");
    //    return 0;
}
