#include "Tag.h"


Tag::Tag(string value,int intent_num,bool is_inline)
{
	this->value=value;
	this->intent_num=intent_num;
	this->is_inline=is_inline;
}


Tag::~Tag(void)
{
}
