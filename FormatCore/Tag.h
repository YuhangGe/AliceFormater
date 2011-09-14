#pragma once
#include<string>
using namespace std;
class Tag
{
public:
	string value;
	bool is_inline;
	bool is_empty;
	int intent_num;
	Tag(string value,int intent_num=0,bool is_inline=false,bool is_empty=true);
	~Tag(void);
};

