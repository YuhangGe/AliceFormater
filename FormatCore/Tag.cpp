#include "Tag.h"


Tag::Tag(string value,int intent_num,bool is_inline,bool is_empty)
{
	this->value=value;
	this->intent_num=intent_num;
	this->is_inline=is_inline;
	this->is_empty=is_empty;
}


Tag::~Tag(void)
{
}
