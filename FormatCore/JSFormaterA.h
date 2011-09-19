#pragma once
#include<string>
#include "JSLexer.h"
using namespace std;

class JSFormaterA
{

	char intent_char;
	char intent_num;
	int cur_intent_num;
	int pre_intent_num;
	string intent_str;
	string cur_intent;
	const string& get_intent_str(){
		if(pre_intent_num!=cur_intent_num){
			intent_str.clear();
			for(int i=0;i<intent_num*cur_intent_num;i++)
				intent_str.push_back(intent_char);
			pre_intent_num=cur_intent_num;
		}
		return intent_str;
	}
	JSLexer* lexer;
	Token cur_token;
	string output;

	JSFormaterA& append(const char c);
	JSFormaterA& append(const char* c);
	JSFormaterA& append(const string& c);
	JSFormaterA& appendNewLine();
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
	void fStatement();
	void fFunction();
	void fDeclare();
	void fId();
	void fBlock();
	void fFor();
	void fExpr();
	void fPriExpr();
	void fObject();
	void fArray();
	void fAssignExpr();
	void fLeftExpr();
	void fReturn();
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
	JSFormaterA(const char* content);
	~JSFormaterA(void);
};

