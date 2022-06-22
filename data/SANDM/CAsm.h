#pragma once

#include "CNameTable.h"

const int SzWord = sizeof(short);

const int ErrFile = 1;

//описание машинной команды S&M
struct SCmd
{
	char code;
	int data;
};

//транслятор ассемблера S&M в байт-код
class CAsm
{
	public:
		//конструктор
		CAsm() : iCmd(0), iStr(0) {}
		//обработка файла ассемблера
		int Open(const PChar FName);
		//текущее число команд байт-кода
		short SzDump() {return iCmd;}
		//номер текущей строки файла ассемблера или 0
		int IndexStr() {return iStr;}
	private:
		//массив байт-кодов
		SCmd dump[SzWord];
		//таблица имен
		CNameTable nTab;
		//индекс нового байт-кода
		short iCmd;
		//номер обрабатываемой строки ассемблера (1-based)
		int iStr;
		//обработка строки ассемблера
		int ParseString();
};
