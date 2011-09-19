#include<iostream>
#include<algorithm>
#include "JSFormaterA.h"





JSFormaterA::JSFormaterA(const char* content):cur_intent_num(0),pre_intent_num(0),cur_token(Token::EMPTY)
{
	this->lexer=new JSLexer(content);
	intent_char= ' ';
	intent_num = 4;
}


JSFormaterA::~JSFormaterA(void)
{
	delete this->lexer;
	this->lexer=NULL;
}
JSFormaterA& JSFormaterA::append(const char c){
	//cout<<"c:"<<c<<endl;
	output.push_back(c);
	return *this;
}
JSFormaterA& JSFormaterA::append(const char* c){
	//cout<<"c:"<<c<<endl;
	output.append(c);
	return *this;
}
JSFormaterA& JSFormaterA::append(const string& c){
	//cout<<"c:"<<c<<endl;
	output.append(c);
	return *this;
}
JSFormaterA& JSFormaterA::appendNewLine(){
	append('\n').append(get_intent_str());
	return *this;
}

string JSFormaterA::format(){
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
void JSFormaterA::fProgram(){
	get_next_token();
	while(true){
		if(cur_token==Token::END)
			break;
		fStatement();
	}
}
void JSFormaterA::fBlock(){
	
	append('{');
	get_next_token();
	if(is_tag(cur_token,"}")){
		append('}');
		get_next_token();
		return;
	}
	cur_intent_num++;
	while(!is_tag(cur_token,"}")){
		fStatement();
	}
	cur_intent_num--;
	appendNewLine().append('}');
	get_next_token();

}
void JSFormaterA::fStatement(){
	if(is_tag(cur_token,"function")){
		fFunction();
	}else if(is_tag(cur_token,"var")){
		fDeclare();
	}else if(is_tag(cur_token,"for")){
		fFor();
	}
	//... do while...
	else if(is_tag(cur_token,"{")){
		fBlock();
	}else if(is_tag(cur_token,";")){
		appendNewLine().append(";");
		get_next_token();
	}else if(is_tag(cur_token,"return")){
		fReturn();
	}else{
		appendNewLine();
		fExpr();
	}
}
void JSFormaterA::fReturn(){
	appendNewLine().append("return");
	get_next_token();
	if(is_tag(cur_token,";"))
		append(';');
	else if(is_tag(cur_token,"}"))
		return;
	else{
		append(' ');
		fExpr();
		if(is_tag(cur_token,";")){
			append(';');
			get_next_token();
		}
	}
}
void JSFormaterA::fExpr(){
	fAssignExpr();
	while(is_tag(cur_token,",")){
		get_next_token();
		fAssignExpr();
	}
	if(is_tag(cur_token,";")){
		append(';');
		get_next_token();
	}

}
void JSFormaterA::fAssignExpr(){
	static const char* tmp0[9]={"+","-","~","!","delete","typeof",
		"void","++","--"};
	if(is_tags(cur_token,tmp0,9)){
		append(cur_token.value);
		get_next_token();
	}
	fLeftExpr();
	static const char* tmp1[32]={"=","*=","/=","%=","+=",
		"-=","^=","&=","|=",">>=",">>>=","<<=",
		"+","-","*","/","&","%","|"
		,"<<",">>",">>>","<","<=",">",">=",
		"instanceof","in","==","!=","===","!=="};
	if(is_tag(cur_token,"++")||is_tag(cur_token,"--")){
		append(cur_token.value);
		get_next_token();
	}
	else if(is_tags(cur_token,tmp1,32)){
		append(" ").append(cur_token.value).append(" ");
		get_next_token();
		fAssignExpr();
	}else if(is_tag(cur_token,"?")){
		append(" ? ");
		get_next_token();
		fAssignExpr();
		if(!is_tag(cur_token,":"))
			throw 1;
		else
			get_next_token();
		append(" : ");
		fAssignExpr();
	}

}
void JSFormaterA::fLeftExpr(){
	while(is_tag(cur_token,"new")){
		append("new ");
		get_next_token();
	}
	fPriExpr();
	while(true){
		if(is_tag(cur_token,"(")){
			append('(');
			get_next_token();
			if(!is_tag(cur_token,")")){
				fAssignExpr();
				while(is_tag(cur_token,",")){
					append(", ");
					get_next_token();
					fAssignExpr();
				}
			}
			if(!is_tag(cur_token,")"))
				throw 1;
			else{
				append(')');
				get_next_token();
			}
		}else if(is_tag(cur_token,".")){
			get_next_token();
			output.append(".");
			fId();
		}else if(is_tag(cur_token,"[")){
			append('[');
			get_next_token();
			fExpr();
			if(!is_tag(cur_token,"]"))
				throw 1;
			else{
				append(']');
				get_next_token();
			}
		}else
			break;

	}

}

void JSFormaterA::fPriExpr(){
	if(is_tag(cur_token,"function")){
		fFunction();
	}else if(is_tag(cur_token,"{")){
		fObject();
	}else if(is_tag(cur_token,"[")){
		fArray();
	}else if(is_tag(cur_token,"(")){
		output.append("(");
		get_next_token();
		fExpr();
		if(!is_tag(cur_token,")"))
			throw 1;
		else{
			output.append(")");
			get_next_token();
		}
	}else{
		output.append(cur_token.value);
		get_next_token();
	}

}
void JSFormaterA::fObject(){
	append('{');
	get_next_token();
	if(is_tag(cur_token,"}")){
		append('}');
		return ;
	}
	cur_intent_num++;
	appendNewLine();
	fId();
	if(!is_tag(cur_token,":"))
		throw 1;
	else{
		append(" : ");
		get_next_token();
	}
	fAssignExpr();
	while(is_tag(cur_token,",")){
		append(',').appendNewLine();
		get_next_token();
		fId();
		if(!is_tag(cur_token,":"))
			throw 1;
		else{
			append(" : ");
			get_next_token();
		}
		fAssignExpr();
	}
	cur_intent_num--;
	appendNewLine().append('}');
	get_next_token();
}
void JSFormaterA::fArray(){
	output.append("[");
	get_next_token();
	if(is_tag(cur_token,"]")){
		append(']');
		get_next_token();
		return;
	}
	fAssignExpr();
	while(is_tag(cur_token,",")){
		append(", ");
		get_next_token();
		fAssignExpr();
	}
	if(!is_tag(cur_token,"]"))
		throw 1;
	else{
		append(']');
		get_next_token();
	}

}
void JSFormaterA::fFor(){
	append("for");
	if(!is_tag(get_next_token(),"("))
		throw 1;
	append('(');
	if(is_tag(get_next_token(),"var")){
		fDeclare();	
	}else{
		fExpr();
	}
	if(is_tag(cur_token,";"))
		append(';');
	else
		throw 1;
	get_next_token();
	fAssignExpr();
	if(is_tag(cur_token,";"))
		append(';');
	else
		throw 1;

}
void JSFormaterA::fFunction(){
	append("function ");
	get_next_token();
	if(is_tag(cur_token,"id")){
		fId();
	}
	if(is_tag(cur_token,"(")){
		append('(');
		get_next_token();
		if(!is_tag(cur_token,")")){
			while(true){
				if(is_tag(cur_token,"id")){
					append(cur_token.value);
					get_next_token();
				}else throw 1;
				if(is_tag(cur_token,",")){
					append(", ");
					get_next_token();
				}else if(is_tag(cur_token,")"))
					break;
				else
					throw 1;
			}
		}
		append(") ");
		get_next_token();
	}
	
	if(is_tag(cur_token,"{")){
		fBlock();
	}else
		throw 1;

}
void JSFormaterA::fId(){
	if(!is_tag(cur_token,"id"))
		throw 1;
	else{
		append(cur_token.value);
		get_next_token();
	}
}
void JSFormaterA::fDeclare(){
	appendNewLine().append("var ");
	get_next_token();
	fId();
	bool inl1,inl2=false;
	if(is_tag(cur_token,"=")){
		get_next_token();
		append(" = ");
		fAssignExpr();
		inl1=true;
	}

	while(is_tag(cur_token,",")){
		append(',');
		int idx=output.size();
		get_next_token();
		fId();
		if(is_tag(cur_token,"=")){
			inl2=true;
			get_next_token();
			append(" = ");
		}else
			inl2=false;
		if(inl1==true || inl2==true){
			cur_intent_num++;
			string new_line="\n"+get_intent_str();
			output.insert(idx,new_line);
			if(inl2==true)
				fAssignExpr();
			cur_intent_num--;
		}else
			output.insert(idx," ");
		inl1=inl2, inl2=false;
	}
	if(is_tag(cur_token,";")){
		append(';');
		get_next_token();
	}
	
}