#pragma once

#include "../core/name.h"

template <typename T, size_t N = 65536>
class Manager
{
public:
	Manager();
	virtual ~Manager();

	virtual void Remove(const Name& name) = 0;
	virtual void RemoveAll() = 0;

protected:
	T** m_elements;
};

template<typename T, size_t N>
inline Manager<T, N>::Manager()
{
	m_elements = new T * [N]();
}

template<typename T, size_t N>
inline Manager<T, N>::~Manager()
{
	delete m_elements;
}
