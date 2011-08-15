#pragma once
#include<string>
using namespace std;
class Tag
{
public:
	string value;
	bool is_inline;
	int intent_num;
	Tag(string value,int intent_num=0,bool is_inline=false);
	~Tag(void);
};

