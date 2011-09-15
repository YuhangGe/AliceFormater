#include<iostream>
#include<algorithm>
#include "JSFormater.h"





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

string JSFormater::format(){
	//try{
		fProgram();
	/*}catch(int e){
		std::cout<<"Wrong! at line:"<<lexer->line<<endl;
		while(cur_token!=Token::END){
			output.append(cur_token.value);
			cur_token=lexer->scan();
		}
	}
	*/
	
	return output;
}
void JSFormater::fProgram(){
	get_next_token();
	while(true){
		if(cur_token==Token::END)
			break;
		output.append(fStatement()).push_back('\n');
	}
}
string JSFormater::fBlock(){
	string rtn="{";
	get_next_token();
	while(!is_tag(cur_token,"}")){
		rtn.append(fStatement());
	}
	rtn.append("}");
	get_next_token();
	return rtn;
}
string JSFormater::fStatement(){
	if(is_tag(cur_token,"function")){
		return fFunction();
	}else if(is_tag(cur_token,"var")){
		return fDeclare();
	}else if(is_tag(cur_token,"for")){
		return fFor();
	}
	//... do while...
	else if(is_tag(cur_token,"{")){
		return fBlock();
	}else if(is_tag(cur_token,";")){
		string rtn=cur_token.value;
		get_next_token();
		return rtn;
	}else{
		return fExpr();
	}
}
string JSFormater::fExpr(){
	string rtn;
	rtn.append(fAssignExpr());
	while(is_tag(cur_token,",")){
		get_next_token();
		rtn.append(fAssignExpr());
	}
	if(is_tag(cur_token,";")){
		rtn.push_back(';');
		get_next_token();
	}
	return rtn;
}
string JSFormater::fAssignExpr(){
	string rtn;
	static const char* tmp0[9]={"+","-","~","!","delete","typeof",
		"void","++","--"};
	if(is_tags(cur_token,tmp0,9)){
		rtn.append(cur_token.value);
		get_next_token();
	}
	rtn.append(fLeftExpr());
	static const char* tmp1[32]={"=","*=","/=","%=","+=",
		"-=","^=","&=","|=",">>=",">>>=","<<=",
		"+","-","*","/","&","%","|"
		,"<<",">>",">>>","<","<=",">",">=",
		"instanceof","in","==","!=","===","!=="};
	if(is_tag(cur_token,"++")||is_tag(cur_token,"--")){
		rtn.append(cur_token.value);
		get_next_token();
	}
	else if(is_tags(cur_token,tmp1,32)){
		rtn.append(" ").append(cur_token.value).append(" ");
		get_next_token();
		rtn.append(fAssignExpr());
	}else if(is_tag(cur_token,"?")){
		rtn.append(" ? ");
		get_next_token();
		rtn.append(fAssignExpr());
		if(!is_tag(cur_token,":"))
			throw 1;
		else
			get_next_token();
		rtn.append(" : ");
		rtn.append(fAssignExpr());
	}
	return rtn;

}
string JSFormater::fLeftExpr(){
	string rtn;

	while(is_tag(cur_token,"new")){
		rtn.append("new ");
		get_next_token();
	}
	rtn.append(fPriExpr());
	while(true){
		if(is_tag(cur_token,"(")){
			rtn.append("(");
			get_next_token();
			if(!is_tag(cur_token,")")){
				rtn.append(fAssignExpr());
				while(is_tag(cur_token,",")){
					rtn.append(", ");
					get_next_token();
					rtn.append(fAssignExpr());
				}
			}
			if(!is_tag(cur_token,")"))
				throw 1;
			else{
				rtn.append(")");
				get_next_token();
			}
		}else if(is_tag(cur_token,".")){
			get_next_token();
			rtn.append(".").append(fId());
		}else if(is_tag(cur_token,"[")){
			rtn.append("[");
			get_next_token();
			rtn.append(fExpr());
			if(!is_tag(cur_token,"]"))
				throw 1;
			else{
				rtn.append("]");
				get_next_token();
			}
		}else
			break;

	}
	return rtn;
}

string JSFormater::fPriExpr(){
	string rtn;
	if(is_tag(cur_token,"function")){
		return fFunction();
	}else if(is_tag(cur_token,"{")){
		return fObject();
	}else if(is_tag(cur_token,"[")){
		return fArray();
	}else if(is_tag(cur_token,"(")){
		rtn.append("(");
		get_next_token();
		rtn.append(fExpr());
		if(!is_tag(cur_token,")"))
			throw 1;
		else{
			rtn.append(")");
			get_next_token();
		}
	}else{
		rtn=cur_token.value;
		get_next_token();
	}
	return rtn;
}
string JSFormater::fObject(){
	string rtn="{";
	get_next_token();
	if(is_tag(cur_token,"}"))
		return rtn.append("}");
	rtn.append(fId());
	if(!is_tag(cur_token,":"))
		throw 1;
	else{
		rtn.append(" : ");
		get_next_token();
	}
	rtn.append(fAssignExpr());
	while(is_tag(cur_token,",")){
		rtn.append(", ");
		get_next_token();
		rtn.append(fId());
		if(!is_tag(cur_token,":"))
			throw 1;
		else{
			rtn.append(" : ");
			get_next_token();
		}
		rtn.append(fAssignExpr());
	}
	rtn.push_back('}');
	get_next_token();
	return rtn;
}
string JSFormater::fArray(){
	string rtn="[";
	get_next_token();
	if(is_tag(cur_token,"]")){
		rtn.push_back(']');
		get_next_token();
		return rtn;
	}
	rtn.append(fAssignExpr());
	while(is_tag(cur_token,",")){
		rtn.append(", ");
		get_next_token();
		rtn.append(fAssignExpr());
	}
	if(!is_tag(cur_token,"]"))
		throw 1;
	else{
		rtn.push_back(']');
		get_next_token();
	}
	return rtn;
}
string JSFormater::fFor(){
	string rtn="for";
	if(!is_tag(get_next_token(),"("))
		throw 1;
	rtn.push_back('(');
	if(is_tag(get_next_token(),"var")){
		rtn.append(fDeclare());	
	}else{
		rtn.append(fExpr());
	}
	if(is_tag(cur_token,";"))
		rtn.push_back(';');
	else
		throw 1;
	rtn.append(fAssignExpr());
	if(is_tag(cur_token,";"))
		rtn.push_back(';');
	else
		throw 1;

}
string JSFormater::fFunction(){
	string rtn="function ";
	get_next_token();
	if(is_tag(cur_token,"id")){
		rtn.append(fId());
	}
	if(is_tag(cur_token,"(")){
		rtn.push_back('(');
		get_next_token();
		if(!is_tag(cur_token,")")){
			while(true){
				if(is_tag(cur_token,"id")){
					rtn.append(cur_token.value);
					get_next_token();
				}else throw 1;
				if(is_tag(cur_token,",")){
					rtn.append(", ");
					get_next_token();
				}else if(is_tag(cur_token,")"))
					break;
				else
					throw 1;
			}
		}
		rtn.push_back(')');
		get_next_token();
	}
	
	if(is_tag(cur_token,"{")){
		rtn.append(fBlock());
	}else
		throw 1;
	return rtn;
}
string JSFormater::fId(){
	string rtn=cur_token.value;
	if(!is_tag(cur_token,"id"))
		throw 1;
	else{
		get_next_token();
		return rtn;
	}
}
string JSFormater::fDeclare(){
	string rtn="var ";
	get_next_token();
	rtn.append(fId());
	bool inl1,inl2=false;
	if(is_tag(cur_token,"=")){
		get_next_token();
		rtn.append(" = ").append(fAssignExpr());
		inl1=true;
	}
	while(is_tag(cur_token,",")){
		string tmp_v;
		rtn.push_back(',');
		get_next_token();
		tmp_v.append(fId());
		if(is_tag(cur_token,"=")){
			inl2=true;
			get_next_token();
			tmp_v.append(" = ").append(fAssignExpr());
		}else
			inl2=false;
		if(inl1==true || inl2==true){
			rtn.append("\n    ");
		}else
			rtn.push_back(' ');
		rtn.append(tmp_v);
		inl1=inl2, inl2=false;
	}
	if(is_tag(cur_token,";")){
		rtn.append(";");
		get_next_token();
	}
	return rtn;
}