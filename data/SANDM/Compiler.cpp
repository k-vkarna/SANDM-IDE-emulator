#include <stdio.h>
#include <string.h>

#include "Compiler.h"
#include "LineParser.h"
#include "byte_code.h"
#include "tokens.h"

Compiler::Compiler()
{
	fp = 0;
	numLine = 0;
	cntCmd = 0;
	byte_code.resize(0);
}

int Compiler::parseScript()
{
	char str[MAX_LINELENGTH];

	for (;;)
	{
		char *p=str;
		*p=0;
		numLine++;
		fgets(str,MAX_LINELENGTH,fp);
		if (feof(fp)&&!str[0]) break;

		// remove trailing whitespace
		while (*p) p++;
		if (p > str) p--;
		while (p >= str && (*p == '\r' || *p == '\n' || *p == ' ' || *p == '\t')) p--;
		*++p=0;

		int ret=doParse(str);

		if (ret != PS_OK) return ret;
	}

	return PS_OK;
}

int Compiler::doParse(const char *str)
{
	LineParser lp;
	int res = lp.Parse((char *)str);
	if (res)
	{
		return PS_ERROR;
	}

	if(!stricmp(lp.GetCommand(),""))
	{
		return PS_OK;
	}

	bool ht, cc, cp, ha;
	int tkid = get_commandtoken(lp.GetCommand(),&ht,&cc,&cp,&ha);
	if (tkid == PS_FILE_ERROR)
	{
		return PS_ERROR;
	}

	char type = lp.GetType();
	bool pointermode = lp.GetPointerFlag();
	int arg = lp.GetArgument();

	if (!ht && type != 'D')
	{
		return PS_ERROR;
	}

	if (!cc && type == 'C')
	{
		return PS_ERROR;
	}

	if (!cp && pointermode)
	{
		return PS_ERROR;
	}

	if (!ha && arg)
	{
		return PS_ERROR;
	}

	char command_code[5];
	for (int i=0; i<5; i++)
	{
		command_code[i] = 0;
	}
	command_code[0] = tkid << 4;
	command_code[0] |= RESERVED;
	switch (type)
	{
	case 'C':
		command_code[0] |= TYPE_CHAR;
		break;
	case 'W':
		command_code[0] |= TYPE_WORD;
		break;
	case 'D':
		command_code[0] |= TYPE_DWORD;
		break;
	default:
		command_code[0] |= TYPE_BYTE;
		break;
	}
	command_code[0] |= pointermode ? POINTERMODE_ON : POINTERMODE_OFF;
	*(int *)(command_code + 1) = arg;

	byte_code.add(command_code, 5);
	cntCmd++;

	return PS_OK;
}

int Compiler::Open(const char *filename)
{
	fp = fopen(filename, "r");
	if (!fp)
	{
		return PS_FILE_ERROR;
	}
	int res = parseScript();
	fclose(fp);
	return res;
}

int Compiler::Save(const char *filename)
{
	FILE *f = fopen(filename, "wb");
	if (!fp)
	{
		return PS_FILE_ERROR;
	}
	fwrite(&cntCmd, sizeof(cntCmd), 1, f);
	fwrite(GetByteCode(), 5, cntCmd, f);
	fclose(f);
	return 0;
}

bool Compiler::is_valid_token(char *s)
{
	for (int x = 0; x < TOK__LAST; x++) {
		if (!stricmp(tokenlist[x].name,s)) return true;
	}
	return false;
}

int Compiler::get_commandtoken(char *s, bool *ht, bool *cc, bool *cp, bool *ha)
{
	int x;
	for (x = 0; x < TOK__LAST; x++) {
		if (!stricmp(tokenlist[x].name,s)) {
			*ht = tokenlist[x].has_type;
			*cc = tokenlist[x].can_char;
			*cp = tokenlist[x].can_pointer;
			*ha = tokenlist[x].has_argument;
			return tokenlist[x].id;
		}
	}
	return PS_FILE_ERROR;
}
