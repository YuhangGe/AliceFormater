#include<iostream>
#include<string>
#include"HTMLFormater.h"
using namespace std;



int main(){
	char buf[10240]={0};
	FILE* f=fopen("input.txt","r");
	FILE* wf=fopen("output.txt","w");
	if(f==NULL || wf==NULL){
		cout<<"wrong"<<endl;
		exit(-1);
	}
	int size=fread(buf,1,10240,f);
	buf[size]=EOF;
	cout<<buf<<endl;
	cout<<"--------------------"<<endl;
	fclose(f);
	//char text[]="<!doctype html><html>\n\n\n<head><link rel=\"stylesheet\"    href=\"a.css\"/><title>  Hello dsd  </title></head><body><p>hello formater   \n   love you alice</p></body></html>";
	string out=HTMLFormater::format(buf,size);
	cout<<out<<endl;
	const char* cs=out.c_str();
	fwrite(cs,1,strlen(cs),wf);
	fclose(wf);
	//free((void*)cs);
	system("pause");
}