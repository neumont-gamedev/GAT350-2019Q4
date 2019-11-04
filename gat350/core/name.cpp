#include "name.h"
#include "assert.h"
#include "string.h"

size_t Name::ms_unique_id = 0;
char* Name::ms_names = nullptr;

Name::Name(const char* string, bool unique)
{
	ASSERT(strlen(string) < MAX_NAME_SIZE);

	std::string name_string(string);
	std::transform(name_string.begin(), name_string.end(), name_string.begin(), std::tolower);

	if (unique)
	{
		name_string += std::to_string(ms_unique_id);
		ms_unique_id++;
	}

	m_id = std::hash<std::string>{}(name_string.c_str());
	m_index = m_id % MAX_ENTRIES;
	strcpy_s(ms_names + (m_index * MAX_NAME_SIZE), MAX_NAME_SIZE, name_string.c_str());
}

bool Name::operator==(const Name& other) const
{
	return (m_id == other.m_id);
}

bool Name::operator!=(const Name& other) const
{
	return (m_id != other.m_id);
}

bool Name::operator<(const Name& other) const
{
	return (m_id < other.m_id);
}

std::string Name::ToString() const
{
	return std::string(c_str());
}

const char* Name::c_str() const
{
	return ms_names + (m_index * MAX_NAME_SIZE);
}

void Name::AllocNames()
{
	ms_names = new char[MAX_ENTRIES * MAX_NAME_SIZE];
}

void Name::FreeNames()
{
	delete ms_names;
}
