//#pragma once
//#include<string>
//#include<stack>
//using namespace std;
//
//class JSFormater
//{
//static const string INLINE_TAGS[];
//	int total_len;
//	int cur_idx;
//	int cur_intent_num;
//	char intent_char;
//	char intent_num;
//	string intent_str;
//
//	char cur_char;
//	char pre_char;
//	const char* content;
//	string output;
//	string doFormat();
//	
//	char getNextChar();
//	
//
//	void formatTag();
//	void formatCloseTag();
//	void formatDoc();
//	void formatAttr();
//	void formatPhp();
//	void formatStyle();
//	void formatScript();
//	void formatDirect(const char until);
//	void formatText();
//	
//	JSFormater& append(const char c);
//	JSFormater& append(const char* c);
//	JSFormater& append(const string& c);
//	JSFormater& appendNewLine(int intent_num);
//
//	bool isInlineTag(string tag_name);
//
//public:
//	static string format(const char* content);
//	JSFormater(const char* content);
//	~JSFormater(void);
//};
//
