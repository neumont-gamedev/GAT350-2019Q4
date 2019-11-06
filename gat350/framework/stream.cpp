#include "stream.h"

Stream::Stream(const std::string& name)
{
	//bool success = json::load(name.c_str(), m_document);
	//ASSERT_MSG(success, "error loading json file.");
}

Stream::~Stream()
{
}

Stream& Stream::operator>>(int& value)
{
	return *this;
}

// const rapidjson::Value& entities_value = value["entities"];