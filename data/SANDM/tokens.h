#pragma once

const int PS_OK = 0;
const int PS_ERROR = 50;
const int PS_FILE_ERROR = -1;
const int PS_INPUT_TIMEOUT = -5;
const int PS_PROGRAMM_EXIT = -14;

enum {
	//Register/memory
	TOK_SET,
	TOK_SAVE,
	TOK_LOAD,
	TOK_PAGE,

	//Input/output
	TOK_READ,
	TOK_WRITE,

	//Arithmetic
	TOK_ADD,
	TOK_SUB,
	TOK_MUL,
	TOK_DIV,
	TOK_MOD,

	//Management
	TOK_JMP,
	TOK_JNZ,
	TOK_JGZ,
	TOK_NOP,

	TOK__LAST
};

typedef struct {
	int id;
	const char *name;
	bool has_type;
	bool can_char;
	bool can_pointer;
	bool has_argument;
} tokenType;

const tokenType tokenlist[TOK__LAST] = {
	{TOK_ADD, "ADD", true, false, true, true},
	{TOK_DIV, "DIV", true, false, true, true},
	{TOK_JGZ, "JGZ", true, false, true, true},
	{TOK_JMP, "JMP", true, false, true, true},
	{TOK_JNZ, "JNZ", true, false, true, true},
	{TOK_LOAD, "LOAD", true, false, false, false},
	{TOK_MOD, "MOD", true, false, true, true},
	{TOK_MUL, "MUL", true, false, true, true},
	{TOK_NOP, "NOP", false, false, false, false},
	{TOK_PAGE, "PAGE", false, false, true, true},
	{TOK_READ, "READ", true, true, false, false},
	{TOK_SAVE, "SAVE", true, false, true, true},
	{TOK_SET, "SET", true, false, true, true},
	{TOK_SUB, "SUB", true, false, true, true},
	{TOK_WRITE, "WRITE", true, true, false, false}
};
