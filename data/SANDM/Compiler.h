#pragma once

#include <stdio.h>

#include "growbuf.h"

const int MAX_LINELENGTH = 256;

class Compiler
{
public:
	Compiler();
	//получение номера текущей строки
	int NumLine() {return numLine;}
	//получение количества команд байт-кода
	int CountCmd() {return cntCmd;}
	//получение байт-кода
	const char *GetByteCode() {return (const char *)byte_code.get();}
	int Open(const char *filename);
	int Save(const char *filename);
    //количество команд в байт-коде
    short int cntCmd;
    //байт-код
    GrowBuf byte_code;
    //текущая строка в ассемблируемом файле (1-based)
    int numLine;
private:
	bool is_valid_token(char *s);
    int get_commandtoken(char *s, bool *ht, bool *cc, bool *cp, bool *ha);
	int parseScript();
	int doParse(const char *str);
	FILE *fp;
};
