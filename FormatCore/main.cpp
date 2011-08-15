#include<iostream>
#include<string>
#include"HTMLFormater.h"
using namespace std;



int main(){
	char text[]="<!doctype html><html>\n\n\n<head><link rel=\"stylesheet\"    href=\"a.css\"/><title>  Hello dsd  </title></head><body><p>hello formater   \n   love you alice</p></body></html>";
	string out=HTMLFormater::format(text);
	cout<<out<<endl;

	system("pause");
}