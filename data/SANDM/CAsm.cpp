#include "CAsm.h"

#include <stdio.h>
#include <string.h>

//пропуск пробельных символов
bool SkipSpaces(FILE *f, char &ch)
{
	while (!feof(f) && (' ' == ch || 9 == ch)) ch = fgetc(f);
	return !feof(f);
}

//обработка файла ассемблера
int CAsm::Open(const PChar FName)
{
	FILE *f = fopen(FName, "rb");
	if (!f) return ErrFile;

	char ch;

	while (!feof(f)) {
		ch = fgetc(f);
	}//while

	return 0;
}

//обработка строки ассемблера
int CAsm::ParseString()
{
	return 0;
}
