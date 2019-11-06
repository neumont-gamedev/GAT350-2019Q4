#pragma once

#include "../core/name.h"

template <typename TBase, size_t N = 65536>
class Manager
{
public:
	Manager();
	virtual ~Manager();

	virtual void Remove(const Name& name) = 0;
	virtual void RemoveAll() = 0;

protected:
	TBase** m_elements;
};

template<typename TBase, size_t N>
inline Manager<TBase, N>::Manager()
{
	m_elements = new TBase * [N]();
}

template<typename TBase, size_t N>
inline Manager<TBase, N>::~Manager()
{
	delete m_elements;
}
