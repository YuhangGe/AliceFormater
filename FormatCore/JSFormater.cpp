#include<iostream>
#include<algorithm>
#include "JSFormater.h"



//string JSFormater::format(const char* content){
//	JSFormater f(content);
//	return f.doFormat();
//}
//
//
//JSFormater::JSFormater(const char* content):cur_idx(0),cur_char(0),cur_intent_num(0)
//{
//	total_len=strlen(content);
//	this->content=content;
//	intent_char= ' ';
//	intent_num = 4;
//	for(int i=0;i<intent_num;i++)
//		intent_str.push_back(intent_char);
//}
//
//
//JSFormater::~JSFormater(void)
//{
//}
//JSFormater& JSFormater::append(const char c){
//	//cout<<"c:"<<c<<endl;
//	output.push_back(c);
//	return *this;
//}
//JSFormater& JSFormater::append(const char* c){
//	//cout<<"c:"<<c<<endl;
//	output.append(c);
//	return *this;
//}
//JSFormater& JSFormater::append(const string& c){
//	//cout<<"c:"<<c<<endl;
//	output.append(c);
//	return *this;
//}
//JSFormater& JSFormater::appendNewLine(int intent_num){
//	if(output.length()>0)
//		append('\n');
//	for(int i=0;i<intent_num;i++){
//		append(this->intent_str);
//	}
//	return *this;
//}
//char JSFormater::getNextChar(){
//	if(cur_idx==total_len){
//		cur_char=NULL;
//		return NULL;
//	}
//	cur_char=content[cur_idx];
//	
//	cur_idx++;
//	return cur_char;
//}
//
//string JSFormater::doFormat(){
//	getNextChar();
//	while(cur_char!=NULL){
//		if(cur_char==NULL){
//			break;
//		}else if(cur_char=='<'){
//			getNextChar();
//			if(cur_char=='?'){
//				getNextChar();
//				formatPhp();
//			}
//			else if(cur_char=='!'){
//				getNextChar();
//				formatDoc();
//			}
//			else if(cur_char=='/'){
//				getNextChar();
//				formatCloseTag();
//			}
//			else{
//				formatTag();
//			}
//			getNextChar();
//		}else{
//			formatText();
//		}
//
//	}
//	return output;
//}
//void JSFormater::formatText(){
//	string text;
//	while(cur_char==' '||cur_char=='\t'||cur_char=='\r')
//		getNextChar();
//	while(cur_char!='<' && cur_char!='\n'){
//		text.push_back(cur_char);
//		getNextChar();
//	}
//	if(cur_char=='\n')
//		getNextChar();
//	if(text.size()==0)
//		return;
//	text.erase(text.find_last_not_of(" \t\n\r") + 1);
//	Tag t=tags.top();
//	if(t.is_inline==false)
//		appendNewLine(t.intent_num+1);
//	append(text);
//}
//void JSFormater::formatPhp(){
//}
//void JSFormater::formatScript(){
//}
//void JSFormater::formatTag(){
//
//	string tag_name;
//	string tag;
//	while(cur_char!=' ' && cur_char!='\t' && cur_char!='>'){
//		tag_name.push_back(cur_char);
//		getNextChar();
//	}
//	appendNewLine(this->cur_intent_num).append('<').append(tag_name);
//	Tag t(tag_name,cur_intent_num,this->isInlineTag(tag_name));
//	tags.push(t);
//	cur_intent_num++;
//	formatAttr();
//	if(pre_char=='/'){
//		tags.pop();
//		this->cur_intent_num--;
//	}
//	append('>');
//}
//
//void JSFormater::formatAttr(){
//	pre_char='\0';
//	formatDirect('>');
//}
//
//void JSFormater::formatDirect(const char until){
//	while(cur_char!=until){
//		append(cur_char);
//		pre_char=cur_char;
//		getNextChar();
//	}
//}
//
//void JSFormater::formatCloseTag(){
//	string tag_name;
//	while(true){
//		if(cur_char=='>')
//			break;
//		if(cur_char==' ' || cur_char=='\t')
//			continue;
//		tag_name.push_back(cur_char);
//		getNextChar();
//	}
//	Tag e_t=tags.top();
//	if(e_t.value==tag_name){
//		if(e_t.is_inline==false)
//			appendNewLine(e_t.intent_num);
//		tags.pop();
//		this->cur_intent_num--;
//	}
//	append("</").append(tag_name).append('>');
//}
//
//void JSFormater::formatDoc(){
//	append("<!");
//	formatDirect('>');
//	append('>');
//}
//
//bool JSFormater::isInlineTag(string tag_name){
//	transform(tag_name.begin(), tag_name.end(), tag_name.begin(), ::tolower);
//	for(int i=0;i<INLINE_TAG_NUMS;i++){
//		if(tag_name==INLINE_TAGS[i])
//			return true;
//	}
//	return false;
//}