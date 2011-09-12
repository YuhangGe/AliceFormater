#pragma once
#include<string>
#include<stack>
using namespace std;

class CSSFormater
{
static const string INLINE_TAGS[];
	int total_len;
	int cur_idx;
	int cur_intent_num;
	char intent_char;
	char intent_num;
	string intent_str;

	char cur_char;
	char pre_char;
	const char* content;
	string output;

	bool new_line;

	string doFormat();
	
	char getNextChar();
	char skipSpace();


	void formatStyleLine();

	void formatDirect(const char until);
	void formatDirect(const char c1,const char c2);

	void formatStyleBlock();
	void formatComment();
	void formatLabel();

	CSSFormater& append(const char c);
	CSSFormater& append(const char* c);
	CSSFormater& append(const string& c);
	CSSFormater& appendNewLine(int intent_num);

	bool isSpace(const char c);
public:
	static string format(const char* content);
	CSSFormater(const char* content);
	~CSSFormater(void);
};

