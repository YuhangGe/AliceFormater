#include<iostream>
#include<string>
#include<hash_map>
#include "JSFormaterA.h"

using namespace std;



int main(){

	FILE* f=fopen("input.txt","r");
	//FILE* fo=fopen("output.txt","w");
	char buf[10240];
	int size=fread(buf,1,10240,f);
	fclose(f);
	buf[size]=0;
	//fwrite(out.c_str(),out.length(),1,fo);
	//fclose(fo);

	/*JSLexer lex(buf);
	Token t=lex.scan();
	while(t!=Token::END){
		cout<<t.value<<':'<<t.tag<<endl;
		t=lex.scan();
	}*/
	JSFormaterA* jsf=new JSFormaterA(buf);
	cout<<jsf->format();


	system("pause");
}