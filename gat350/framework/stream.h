#pragma once
#include "../engine/def.h"

class Stream
{
public:
	Stream(const std::string& name);
	~Stream();

	virtual Stream& operator >> (int& value);

private:
	//rapidjson::Document m_document;

};