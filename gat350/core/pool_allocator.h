#pragma once
#include <list>

template <typename TBase>
class PoolAllocator
{
public:
	using ptr_type = BYTE*;

public:
	PoolAllocator(size_t size);
	~PoolAllocator();

	TBase* Get();
	void Free(void* address);

private:
	void* m_pool = nullptr;
	std::list<void*> m_freelist;
};

template<typename TBase>
inline PoolAllocator<TBase>::PoolAllocator(size_t size)
{
	m_pool = new char[size * sizeof(TBase)];
	for (size_t i = 0; i < size; i++)
	{
		ptr_type current = static_cast<ptr_type>(m_pool) + (i * sizeof(TBase));
		m_freelist.push_back(current);
	}
}

template<typename TBase>
inline PoolAllocator<TBase>::~PoolAllocator()
{
	delete m_pool;
}

template<typename TBase>
inline TBase* PoolAllocator<TBase>::Get()
{
	TBase* element = nullptr;

	if (m_freelist.empty() == false)
	{
		element = reinterpret_cast<TBase*>(m_freelist.front());
		m_freelist.pop_front();
	}

	return element;
}

template<typename TBase>
inline void PoolAllocator<TBase>::Free(void* element)
{
	m_freelist.push_front(element);
}
