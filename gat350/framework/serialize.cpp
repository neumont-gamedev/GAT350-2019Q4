#include "serialize.h"

Serialize::Serialize(const std::string& name)
{
	//bool success = json::load(name.c_str(), m_document);
	//ASSERT_MSG(success, "error loading json file.");
}

Serialize::~Serialize()
{
}

Serialize& Serialize::operator>>(int& value)
{
	return *this;
}

// const rapidjson::Value& entities_value = value["entities"];