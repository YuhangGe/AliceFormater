#include<iostream>
#include<algorithm>
#include "HTMLFormater.h"

#define INLINE_TAG_NUMS 5

const string HTMLFormater::INLINE_TAGS[INLINE_TAG_NUMS]={
	string("span"),string("a"),string("img"),string("title"),string("link")
};

string HTMLFormater::format(const char* content,int size){
	HTMLFormater f(content,size);
	return f.doFormat();
}


HTMLFormater::HTMLFormater(const char* content,int size):cur_idx(0),cur_char(0),cur_intent_num(0)
{
	total_len=size;
	this->content=content;
	intent_char= ' ';
	intent_num = 4;
	for(int i=0;i<intent_num;i++)
		intent_str.push_back(intent_char);
}


HTMLFormater::~HTMLFormater(void)
{
}
HTMLFormater& HTMLFormater::append(const char c){
	//cout<<"c:"<<c<<endl;
	output.push_back(c);
	return *this;
}
HTMLFormater& HTMLFormater::append(const char* c){
	//cout<<"c:"<<c<<endl;
	output.append(c);
	return *this;
}
HTMLFormater& HTMLFormater::append(const string& c){
	//cout<<"c:"<<c<<endl;
	output.append(c);
	return *this;
}
HTMLFormater& HTMLFormater::appendNewLine(int intent_num){
	if(output.length()>0)
		append('\n');
	for(int i=0;i<intent_num;i++){
		append(this->intent_str);
	}
	return *this;
}
char HTMLFormater::getNextChar(){
	if(cur_idx==total_len){
		cur_char=NULL;
		return NULL;
	}
	cur_char=content[cur_idx];
	
	cur_idx++;
	return cur_char;
}

string HTMLFormater::doFormat(){
	getNextChar();
	while(cur_char!=NULL){
		if(cur_char==NULL){
			break;
		}else if(cur_char=='<'){
			getNextChar();
			if(cur_char=='?'){
				getNextChar();
				formatPhp();
			}
			else if(cur_char=='!'){
				if(getNextChar()=='-')
					if(getNextChar()=='-')
						formatComment();
					else
					{
						append('-');
						formatDirect('>');
					}
				else
					formatDoc();
			}
			else if(cur_char=='/'){
				getNextChar();
				formatCloseTag();
			}
			else{
				formatTag();
			}
			getNextChar();
		}else{
			formatText();
		}

	}
	return output;
}
void HTMLFormater::formatComment(){
	bool go_on=true;
	appendNewLine(this->cur_intent_num).append("<!-");
	while(go_on && cur_char!=NULL){
		formatDirect('-');
		append('-');
		if(getNextChar()=='-'){
			append('-');
			if(getNextChar()=='>'){
				append('>');
				go_on=false;
			}
		}
	}
	
}
void HTMLFormater::formatText(){
	string text;
	while(cur_char==' '||cur_char=='\t'||cur_char=='\r')
		getNextChar();
	while(cur_char!='<' && cur_char!='\n'){
		text.push_back(cur_char);
		getNextChar();
	}
	if(cur_char=='\n')
		getNextChar();
	if(text.size()==0)
		return;
	text.erase(text.find_last_not_of(" \t\n\r") + 1);
	Tag t=tags.top();
	if(t.is_inline==false)
		appendNewLine(t.intent_num+1);
	append(text);
	if(!tags.empty());
		tags.top().is_empty=false;
}
void HTMLFormater::formatPhp(){
}
void HTMLFormater::formatScript(){
}
void HTMLFormater::formatTag(){

	string tag_name;

	while(cur_char!=' ' && cur_char!='\t' && cur_char!='>'){
		tag_name.push_back(cur_char);
		getNextChar();
	}
	appendNewLine(this->cur_intent_num).append('<').append(tag_name);
	if(!tags.empty())
		tags.top().is_empty=false;
	Tag t(tag_name,cur_intent_num,this->isInlineTag(tag_name));
	tags.push(t);
	cur_intent_num++;
	formatAttr();
	if(pre_char=='/'){
		tags.pop();
		this->cur_intent_num--;
	}
	append('>');
}

void HTMLFormater::formatAttr(){
	pre_char='\0';
	formatDirect('>');
}

void HTMLFormater::formatDirect(const char until){
	while(cur_char!=until){
		append(cur_char);
		pre_char=cur_char;
		getNextChar();
	}
}

void HTMLFormater::formatCloseTag(){
	string tag_name;
	while(true){
		if(cur_char=='>')
			break;
		if(cur_char==' ' || cur_char=='\t')
			continue;
		tag_name.push_back(cur_char);
		getNextChar();
	}
	Tag e_t=tags.top();
	if(e_t.value==tag_name){
		//cout<<e_t.value<<':'<<e_t.is_empty<<endl;
		if(e_t.is_inline==false && e_t.is_empty==false)
			appendNewLine(e_t.intent_num);
		tags.pop();
		this->cur_intent_num--;
	}
	append("</").append(tag_name).append('>');
}

void HTMLFormater::formatDoc(){
	append("<!");
	formatDirect('>');
	append('>');
}

bool HTMLFormater::isInlineTag(string tag_name){
	transform(tag_name.begin(), tag_name.end(), tag_name.begin(), ::tolower);
	for(int i=0;i<INLINE_TAG_NUMS;i++){
		if(tag_name==INLINE_TAGS[i])
			return true;
	}
	return false;
}