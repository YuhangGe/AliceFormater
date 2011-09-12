#pragma once
#include<hash_map>
#include<string>
using namespace std;

 
class Token{
public:
	int tag;
	string value;
	Token(int tag, string& v){
		this->tag=tag;
		this->value=v;
	}
	Token(int tag, const char* value){
		this->tag=tag;
		this->value=string(value);
	}
	bool operator ==(const Token& another){
		return this->tag==another.tag && this->value==another.value;
	}
	bool operator !=(const Token& another){
		return !(this->operator==(another));
	}
	static Token END,EMPTY,WRONG;
};


class JSLexer
{
private:
	char peek;
	int cur_idx;
	const char* source;
	int end_idx;
	char read_ch();
	void unread_ch();
	bool read_the_ch(char c);
	Token help_get_token(char c1,char c2,char c3);
	
	int line;
	bool is_digit(char ch){
		return ch != NULL && ch >= '0' && ch <= '9';
	}
	bool is_id_letter(char ch){
		return ch != NULL && ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch=='$' ||ch=='_');
	}
	string read_str(char quote);
	Token get_buffer_token(const char* word);
	void save_buffer_token(Token t);
	hash_map<const char* ,Token> buf_token;

	static const hash_map<const char* ,int> tag_list;
	
	static bool is_same_tag(const char* k1,const char * k2 ){
		int r1=get_tag(k1);
		int r2=get_tag(k2);
		if(r1==-1 || r2==-1 || r1!=r2)
			return false;
		return true;
	}
	static int get_tag(const char* word){
		hash_map<const char*,int>::const_iterator r=JSLexer::tag_list.find(word);
		if(r==tag_list.end())
			return -1;
		else
			return r->second;
	}
public:
	Token scan();
	JSLexer(const char* source);
	~JSLexer(void);
};

