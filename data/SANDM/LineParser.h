#pragma once

class LineParser
{
public:
	LineParser();
	int Parse(char *line);
	char *GetCommand();
	char GetType();
	bool GetPointerFlag();
	int GetArgument();
private:
	void Clean();
	char *m_command;
	char m_type;
	bool m_pointerflag;
	int m_argument;
};
