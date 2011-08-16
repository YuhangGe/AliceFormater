#pragma once
#include<string>
#include<stack>
#include "Tag.h"
using namespace std;
class HTMLFormater
{
	static const string INLINE_TAGS[];
	int total_len;
	int cur_idx;
	int cur_intent_num;
	char intent_char;
	char intent_num;
	string intent_str;
	stack<Tag> tags;

	char cur_char;
	char pre_char;
	const char* content;
	string output;
	string doFormat();
	
	char getNextChar();
	

	void formatTag();
	void formatCloseTag();
	void formatDoc();
	void formatAttr();
	void formatPhp();
	void formatStyle();
	void formatScript();
	void formatDirect(const char until);
	void formatText();
	void formatComment();

	HTMLFormater& append(const char c);
	HTMLFormater& append(const char* c);
	HTMLFormater& append(const string& c);
	HTMLFormater& appendNewLine(int intent_num);

	bool isInlineTag(string tag_name);

public:
	static string format(const char* content,int size);
	HTMLFormater(const char* content,int size);
	~HTMLFormater(void);
};

