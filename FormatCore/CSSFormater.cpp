#include<iostream>
#include<algorithm>
#include "CSSFormater.h"

#define INLINE_TAG_NUMS 5



string CSSFormater::format(const char* content){
	CSSFormater f(content);
	return f.doFormat();
}


CSSFormater::CSSFormater(const char* content):cur_idx(0),cur_char(0),cur_intent_num(0)
{
	total_len=strlen(content);
	this->content=content;
	intent_char= ' ';
	intent_num = 4;
	for(int i=0;i<intent_num;i++)
		intent_str.push_back(intent_char);
}


CSSFormater::~CSSFormater(void)
{
}
CSSFormater& CSSFormater::append(const char c){
	//cout<<"c:"<<c<<endl;
	output.push_back(c);
	return *this;
}
CSSFormater& CSSFormater::append(const char* c){
	//cout<<"c:"<<c<<endl;
	output.append(c);
	return *this;
}
CSSFormater& CSSFormater::append(const string& c){
	//cout<<"c:"<<c<<endl;
	output.append(c);
	return *this;
}
CSSFormater& CSSFormater::appendNewLine(int intent_num){
	if(output.length()>0)
		append('\n');
	for(int i=0;i<intent_num;i++){
		append(this->intent_str);
	}
	return *this;
}
char CSSFormater::getNextChar(){
	if(cur_idx==total_len){
		cur_char=NULL;
		return NULL;
	}
	cur_char=content[cur_idx];
	cout<<cur_char;
	cur_idx++;
	return cur_char;
}
char CSSFormater::skipSpace(){
	while(isSpace(cur_char)){
		getNextChar();
	}
	return cur_char;
}

string CSSFormater::doFormat(){
	getNextChar();
	skipSpace();
	while(cur_char!=NULL){
		if(cur_char==NULL){
			break;
		}else if(cur_char=='/'){
			formatComment();
		}
		else if(isSpace(cur_char)){
			skipSpace();
		}else if(cur_char=='{'){
			getNextChar();
			this->cur_intent_num++;
			formatStyleBlock();
			this->cur_intent_num--;
		}else{
			formatLabel();
		}

	}
	return output;
}
bool CSSFormater::isSpace(const char c){
	return c=='\t' || c==' '||c=='\n'||c=='\r';
}
void CSSFormater::formatLabel(){
	skipSpace();
	while(cur_char!=NULL){
		if(isSpace(cur_char)==true){
			skipSpace();
			if(cur_char!=',' && cur_char!='{')
				append(' ');
		}else if(cur_char==','){
			append(',');
			if(!isSpace(getNextChar()))
				append(' ');
		}else if(cur_char=='{'){
			append(" {");
			break;
		}else{
			append(cur_char);
			getNextChar();
		}
	}
}
void CSSFormater::formatComment(){
	if(getNextChar()=='*'){
		appendNewLine(0).append("/*");
	}
}
void CSSFormater::formatStyleBlock(){
	while(cur_char!=NULL && cur_char!='}'){
		formatStyleLine();
	}
	appendNewLine(0).append('}').appendNewLine(0);
	getNextChar();
}
void CSSFormater::formatStyleLine(){

	skipSpace();
	if(cur_char!=NULL && cur_char!='}')
		appendNewLine(this->cur_intent_num);
	while(cur_char!=NULL){
		if(cur_char==':'||cur_char=='}'){
			break;
		}else if(isSpace(cur_char)){
			skipSpace();
		}else{
			append(cur_char);
			getNextChar();
		}	
	}
	if(cur_char==':'){
		append(": ");
		getNextChar();
		skipSpace();
	}
	while(cur_char!=NULL){
		if(cur_char=='}'){
			break;
		}else if(cur_char==';'){
			append(';');
			getNextChar();
			break;
		}else if(cur_char=='\"'){
			append('\"');
			getNextChar();
			this->formatDirect('\"');
			append('\"');
			getNextChar();
		}else if(isSpace(cur_char)==true){
			skipSpace();
			if(cur_char!=';')
				append(' ');
		}else{
			append(cur_char);
			getNextChar();
		}
		
	}
}


void CSSFormater::formatDirect(const char until){
	while(cur_char!=NULL && cur_char!=until){
		append(cur_char);
		pre_char=cur_char;
		getNextChar();
	}
}
