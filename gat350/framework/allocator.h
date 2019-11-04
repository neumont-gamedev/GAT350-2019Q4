#pragma once

#include "../core/core.h"
#include <map>

template <typename T>
class Allocator
{
public:
	std::shared_ptr<T> Get(const Name& name);
	
	std::shared_ptr<T> Add(const Name& name);
	void Remove(const Name& name);

private:
	std::map<Name, std::shared_ptr<T>> m_resources;
};

template<typename T>
inline std::shared_ptr<T> Allocator<T>::Get(const Name& name)
{
	std::shared_ptr<T> resource;

	auto iter = m_resources.find(name);
	if (iter != m_resources.end())
	{
		resource = iter->second;
	}
	else
	{
		resource = std::make_shared<T>();
		if (resource->Create(name))
		{
			m_resources[name] = resource;
		}
	}

	return resource;
}

template<typename T>
inline std::shared_ptr<T> Allocator<T>::Add(const Name& name)
{
	bool success = false;

	std::shared_ptr<T> resource;

	auto iter = m_resources.find(name);
	if (iter != m_resources.end())
	{
		resource = iter->second;
	}
	else
	{
		resource = std::make_shared<T>();
		if (resource->Create(name))
		{
			m_resources[name] = resource;
		}
	}

	return resource;
}

template<typename T>
inline void Allocator<T>::Remove(const Name& name)
{
	auto iter = m_resources.find(name);
	if (iter == m_resources.end())
	{
		m_resources.erase();
	}
}
