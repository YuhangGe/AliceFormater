#include<iostream>
#include<string>
#include"CSSFormater.h"
using namespace std;



int main(){
	FILE* f=fopen("input.txt","r");
	FILE* fo=fopen("output.txt","w");
	char buf[10240];
	int size=fread(buf,1,10240,f);
	fclose(f);
	buf[size]=0;
	//char text[]="<!doctype html><html>\n\n\n<head><link rel=\"stylesheet\"    href=\"a.css\"/><title>  Hello dsd  </title></head><body><p>hello formater   \n   love you alice</p></body></html>";
	string out=CSSFormater::format(buf);
	cout<<out<<endl;
	fwrite(out.c_str(),out.length(),1,fo);
	fclose(fo);
	system("pause");
}