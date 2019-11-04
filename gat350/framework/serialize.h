#pragma once
#include "../engine/engine.h"

class Serialize
{
public:
	Serialize(const std::string& name);
	~Serialize();

	virtual Serialize& operator >> (int& value);

private:
	//rapidjson::Document m_document;

};