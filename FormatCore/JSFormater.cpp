#include<iostream>
#include<algorithm>
#include "JSFormater.h"



string JSFormater::format(const char* content){
	JSFormater f(content);
	return f.doFormat();
}


JSFormater::JSFormater(const char* content):cur_intent_num(0),cur_token(Token::EMPTY)
{
	this->lexer=new JSLexer(content);
	intent_char= ' ';
	intent_num = 4;
	for(int i=0;i<intent_num;i++)
		intent_str.push_back(intent_char);
}


JSFormater::~JSFormater(void)
{
	delete this->lexer;
	this->lexer=NULL;
}
JSFormater& JSFormater::append(const char c){
	cout<<"c:"<<c<<endl;
	output.push_back(c);
	return *this;
}
JSFormater& JSFormater::append(const char* c){
	cout<<"c:"<<c<<endl;
	output.append(c);
	return *this;
}
JSFormater& JSFormater::append(const string& c){
	cout<<"c:"<<c<<endl;
	output.append(c);
	return *this;
}
JSFormater& JSFormater::appendNewLine(int intent_num){
	if(output.length()>0)
		append('\n');
	for(int i=0;i<intent_num;i++){
		append(this->intent_str);
	}
	return *this;
}

string JSFormater::doFormat(){
	try{
		fProgram();
	}catch(int e){
		while(cur_token!=Token::END){
			output.append(cur_token.value);
			cur_token=lexer->scan();
		}
	}
	
	return output;
}
void JSFormater::fProgram(){
	while(true){
		if(get_next_token()==Token::END)
			break;
		
	}
}
string JSFormater::fStatement(){
	if(is_tag(cur_token.tag,"function")){
		return fFunction();
	}else if(is_tag(cur_token.tag,"var")){
		return fDeclare();
	}else if(is_tag(cur_token.tag,"if")){
		return fIf();
	}
	//... do while...
	else{
		return fExpr();
	}
}
string JSFormater::fFunction(){
	string rtn="function ";
	get_next_token();
	if(is_tag(cur_token.tag,"id")){
		rtn.append(fId());
		get_next_token();
	}
	if(is_tag(cur_token.tag,"(")){
		rtn.push_back('(');
		get_next_token();
		if(!is_tag(cur_token.tag,")")){
			while(true){
				if(is_tag(cur_token.tag,"id")){
					rtn.append(cur_token.value);
					get_next_token();
				}else throw 1;
				if(is_tag(cur_token.tag,",")){
					rtn.append(", ");
					get_next_token();
				}else if(is_tag(cur_token.tag,")"))
					break;
				else
					throw 1;
			}
		}
		rtn.push_back(')');
		get_next_token();
	}
	
	if(is_tag(cur_token.tag,"{")){
		rtn.append("{");
		this->incIntent();
		get_next_token();
		string fb=fStatement();
		if(!is_tag(cur_token.tag,"}"))
			throw 1;
		this->desIntent();
		if(fb.empty())
			rtn.append(" }");
		else
			rtn.append("\n").append(cur_intent).push_back('}');
	}else
		throw 1;
	get_next_token();
	return rtn;
}
string JSFormater::fId(){
	if(cur_token.tag!=JSLexer::get_tag("id"))
		throw 1;
	else{
		return cur_token.value;
	}
}
string JSFormater::fDeclare(){
	string rtn="var";
	get_next_token();
	if(!is_tag(cur_token.tag,"id"))
		throw 1;
	while(true){
		if(is_tag(cur_token.tag,"id")){

		}
	}
}