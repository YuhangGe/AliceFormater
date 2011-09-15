#include "JSLexer.h"

Token Token::EMPTY=Token(-2,"\0");
Token Token::END=Token(-1,"\0");
Token Token::WRONG=Token(-3,"\0");

JSLexer::JSLexer(const char* source):peek(' '),cur_idx(-1),line(1)
{
	this->source=source;
	this->end_idx=strlen(this->source);
	this->init_tokens();
}


JSLexer::~JSLexer(void)
{
	hash_map<const char*,Token >::const_iterator f,e;
	for(f=buf_token.begin(),e=buf_token.end();f!=e;f++){
		delete f->first;
	}
}

void JSLexer::init_tokens(){
	hash_map<const char*,int>::const_iterator f,e;
	for(f=tag_list.begin(),e=tag_list.end();f!=e;f++){
		save_buffer_token(Token(f->second,f->first));
	}
}

Token JSLexer::help_get_token(char c1,char c2,char c3){
	string str;
	str.push_back(c1);
	char t_c = read_ch();
		if(t_c == c2){
			str.push_back(c2);
		}
		else if(t_c == c3){
			str.push_back(c3);
		}
		else {
			unread_ch();
		}
		return Token(get_tag(str.c_str()),str);
}
Token JSLexer::scan(){
	while(read_ch() != NULL) {
			if(peek == ' ' || peek == '\t' || peek== '\r')
				continue;
			else if(peek == '\n')
				line++;
			else
				break;
		}
		if(peek ==NULL)
			return Token::END;

		char cur_peek = peek;
		char t_c='\0';
		char t_str[2]={0};
		switch(peek) {
		case '/':
			t_c=read_ch();
			if(t_c=='/'){
				return Token(get_tag("inlinecomment"),"//"+ read_str('\n'));
				break;
			}else if(t_c=='*'){
				string comment_str="/*"+read_str('*');
				t_c=read_ch();
				while(t_c!=NULL && t_c!='/'){
					comment_str.append(read_str('*'));
					t_c=read_ch();
				}
				comment_str.append("*/");
				return Token(get_tag("comment"),comment_str);
				break;
			}else{
				unread_ch();
			}
		case '+':
		case '-':
		case '&':
		case '|':
			return help_get_token(cur_peek, '=', cur_peek);
			break;
		case '*':
		case '%':
		case '^':
			return help_get_token(cur_peek, '=', NULL);
			break;
		case '=':
				if(read_the_ch('=') == true) {
					if(read_the_ch('=') == true)
						return Token(get_tag("==="),"===");
					else {
						unread_ch();
						return Token(get_tag("=="),"==");
					}
				} else {
					unread_ch();
					return Token(get_tag("="),"=");
				}

				break;
			case '!':
				if(read_the_ch('=') == true) {
					if(read_the_ch('=') == true)
						return Token(get_tag("!=="),"!==");
					else {
						unread_ch();
						return Token(get_tag("!="),"!=");
					}
				} else {
					unread_ch();
					return Token(get_tag("!"),"!");
				}
				break;
			case '<':
				t_c = read_ch();
				if(t_c == '=')
					return Token(get_tag("<="), "<=");
				else if(t_c == '<') {
					if(read_the_ch('=') == true)
						return Token(get_tag("<<="), "<<=");
					else {
						unread_ch();
						return Token(get_tag("<<"), "<<");
					}
				} else {
					unread_ch();
					return Token(get_tag("<"), "<");
				}
				break;
			case '>':
				t_c = read_ch();
				if(t_c == '=')
					return Token(get_tag(">="), ">=");
				else if(t_c == '>') {
					if(read_the_ch('=') == true)
						return Token(get_tag(">>="), ">>=");
					else {
						unread_ch();
						return Token(get_tag(">>"), ">>");
					}
				} else {
					unread_ch();
					return Token(get_tag(">"), ">");
				}
				break;
			case '~':
			case '(':
			case ')':
			case '[':
			case ']':
			case '{':
			case '}':
			case '?':
			case ':':
			case ';':
			case '.':
			case ',':
			case '\\':
				t_str[0]=cur_peek;
				return Token(get_tag(t_str),t_str);
				break;
			case '\'':
			case '\"':
				string r_str;
				r_str.push_back(cur_peek);
				r_str.append(read_str(cur_peek)).push_back(cur_peek);
				return Token(get_tag("string"),r_str);
				break;
		}

		string word;
		word.push_back(peek);
		
		if(is_digit(peek)==true) {
			while(is_digit(read_ch())==true) {
				word.push_back(peek);
			}
			unread_ch();
			return Token(get_tag("number"),word);
		}
	
		if(is_id_letter(peek) ==true) {
			while(is_id_letter(read_ch()) == true) {
				word.push_back(peek);
			}
			unread_ch();
			Token t = get_buffer_token(word.c_str());
			if(t==Token::EMPTY){
				t = Token(get_tag("id"),word);
				save_buffer_token(t);
				return t;
			}
			else {
				return t;	
			}
		}

		return Token::WRONG;
}
Token JSLexer::get_buffer_token(const char* word){
	hash_map<const char*,Token>::const_iterator r=buf_token.find(word);
	if(r!=buf_token.end())
		return r->second;
	return Token::EMPTY;
}
void JSLexer::save_buffer_token(Token t){
	char * k=new char[t.value.size()];
	strcpy(k,t.value.c_str());
	buf_token.insert(hash_map<const char*,Token>::value_type(k,t));
}

string JSLexer::read_str(char quote){
	string rtn;
		char t_c = read_ch();
		while(t_c != quote && t_c != NULL) {
			rtn.push_back(t_c);
			t_c = read_ch();
		}
		return rtn;
}

char JSLexer::read_ch(){
	if(cur_idx < end_idx) {
			cur_idx++;
			peek = source[cur_idx];
			if(peek == '\n')
				line++;
			//printf("c:%c\n",peek);
		} else {
			peek = NULL;
		}
	
		return peek;
}
bool JSLexer::read_the_ch(char c){
	return read_ch()==c;
}
void JSLexer::unread_ch () {
		if(peek == NULL)
			return;
		cur_idx--;
		if(peek == '\n')
			line--;
		peek = source[cur_idx];
}


#define LIST_LENGTH 82
typedef hash_map<const char*,int,hash_compare<const char*, str_hash>>::value_type TokenTag;
TokenTag init_list[LIST_LENGTH]={
	TokenTag("function",2),
	TokenTag("(",4),
	TokenTag(")",5),
	TokenTag("{",6),
	TokenTag("}",7),
	TokenTag(",",8),
	TokenTag("try",9),
	TokenTag("catch",10),
	TokenTag("finally",11),
	TokenTag("return",12),
	TokenTag("throw",13),
	TokenTag("with",14),
	TokenTag("continue",15),
	TokenTag("break",16),
	TokenTag("for",17),
	TokenTag(";",18),
	TokenTag("in",19),
	TokenTag("var",20),
	TokenTag("do",21),
	TokenTag("while",22),
	TokenTag("switch",23),
	TokenTag("case",24),
	TokenTag(":",25),
	TokenTag("default",26),
	TokenTag("if",27),
	TokenTag("else",28),
	TokenTag("=",29),
	TokenTag("*=",30),
	TokenTag("/=",31),
	TokenTag("%=",32),
	TokenTag("+=",33),
	TokenTag("-=",34),
	TokenTag("<<=",35),
	TokenTag(">>=",36),
	TokenTag(">>>=",37),
	TokenTag("&=",38),
	TokenTag("^=",39),
	TokenTag("|=",40),
	TokenTag("?",41),
	TokenTag("&&",42),
	TokenTag("||",43),
	TokenTag("&",44),
	TokenTag("^",45),
	TokenTag("|",46),
	TokenTag("==",47),
	TokenTag("!=",48),
	TokenTag("===",49),
	TokenTag("!==",50),
	TokenTag("<",51),
	TokenTag(">",52),
	TokenTag("<=",53),
	TokenTag(">=",54),
	TokenTag("instanceof",55),
	TokenTag("<<",56),
	TokenTag(">>",57),
	TokenTag(">>>",58),
	TokenTag("+",59),
	TokenTag("-",60),
	TokenTag("*",61),
	TokenTag("/",62),
	TokenTag("%",63),
	TokenTag("delete",64),
	TokenTag("void",65),
	TokenTag("typeof",66),
	TokenTag("++",67),
	TokenTag("--",68),
	TokenTag("~",69),
	TokenTag("!",70),
	TokenTag("new",71),
	TokenTag("[",72),
	TokenTag("]",73),
	TokenTag(".",74),
	TokenTag("this",75),
	TokenTag("null",76),
	TokenTag("true",77),
	TokenTag("false",78),
	TokenTag("number",79),
	TokenTag("string",80),
	TokenTag("regexp",81),
	TokenTag("id",82),
	TokenTag("inlinecomment",83),
	TokenTag("comment",84)
};
const  hash_map<const char *,int, hash_compare<const char*,str_hash>> JSLexer::tag_list(init_list,init_list+LIST_LENGTH);


