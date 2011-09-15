#pragma once
#include<string>
#include "JSLexer.h"
using namespace std;

class JSFormater
{

	char intent_char;
	char intent_num;
	int cur_intent_num;
	string intent_str;
	string cur_intent;

	JSLexer* lexer;
	Token cur_token;
	string output;

	JSFormater& append(const char c);
	JSFormater& append(const char* c);
	JSFormater& append(const string& c);
	JSFormater& appendNewLine(int intent_num);
	string incIntent(){
		this->cur_intent_num++;
		cur_intent.clear();
		for(int i=0;i<this->cur_intent_num;i++)
			cur_intent.append(this->intent_str);
		return cur_intent;
	}
	string desIntent(){
		this->cur_intent_num--;
		cur_intent.clear();
		for(int i=0;i<this->cur_intent_num;i++)
			cur_intent.append(this->intent_str);
		return cur_intent;
	}
	Token get_next_token(){
		cur_token=lexer->scan();
		return cur_token;
	}

	void fProgram();
	string fStatement();
	string fFunction();
	string fDeclare();
	string fExpression();
	string fId();
	string fBlock();
	string fFor();
	string fExpr();
	string fPriExpr();
	string fObject();
	string fArray();
	string fAssignExpr();
	string fLeftExpr();
	bool is_tag(const Token& t, const char* ctag){
		return t.tag==JSLexer::get_tag(ctag);
	}
	bool is_tags(const Token& t,const char* ctags[], int n){
		for(int i=0;i<n;i++){
			if(t.tag==JSLexer::get_tag(ctags[i]))
				return true;
		}
		return false;
	}
	
public:
	string format();
	JSFormater(const char* content);
	~JSFormater(void);
};

