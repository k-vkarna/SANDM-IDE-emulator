//#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>

//#include "SANDM/CAsm.h"

//#include "SANDM/Compiler.h"
//#include "SANDM/VM.h"
//#include "SANDM/tokens.h"

//#include "mainwindow.h"

//#include <QApplication>
//#include <QTranslator>
//#include <QLibraryInfo>
//#include <QDebug>

////обработка ошибок
//void Error(const int err, const int numLine) {
//    //версия программы
//    const char* const Version = "0.0.4";

//    switch (err) {
//    case PS_FILE_ERROR:
//        printf("ERROR: File I/O\n");
//        break;
//    case PS_ERROR:
//        printf("ERROR: Wrong syntax at line: %i\n", numLine);
//        break;
//    default:
//        printf("SANDM version %s\n", Version);
//        printf("Usage: sandm -(p|b) <file>\n");
//        printf("\t--plain or -p for read plain text\n");
//        printf("\t--byte or -b for read byte-code\n");
//    }
//    system("pause");
//    exit(0);
//}

////точка входа в программу
//int start(QString filename) {

//    const char* buff = filename.toStdString().c_str();

//    try {
//        Compiler a;
//        int res = a.Open(buff);
//        if (res) Error(res, a.NumLine());
//        VM b;
//        res = b.execute(a.CountCmd(), a.GetByteCode());
//        if(res) Error(res, 0);
//        printf("\n");
//        system("pause");
//    } catch (QString err) {
//        Error(0, 0);
//        qDebug() << err;
//    }

//}
