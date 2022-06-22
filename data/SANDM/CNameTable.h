#pragma once

#include <vector>

//тип строки
typedef char* PChar;

//тип элемента таблицы имен
enum ENameElem {
	neLabel,
	neConst
};

//тип элемента таблицы имен
struct SNameElem {
	PChar name;
	ENameElem kind;
};

//таблица имен
class CNameTable
{
public:
	//конструктор
	CNameTable() {}
	//деструктор
	~CNameTable() {}
	//добавление эл-та в таблицу имен
	SNameElem* AddName(const PChar name);
private:
	//тип контейнера таблицы имен
	typedef std::vector <SNameElem> VStore;
	//контейнер таблицы имен
	VStore store;
};
