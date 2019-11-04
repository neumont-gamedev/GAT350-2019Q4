#pragma once

#include "..\\framework\manager.h"

template <typename TBase, size_t N = 65536>
class ResourceManager : public Manager<TBase, N>
{
public:
	ResourceManager(Renderer* renderer) : m_renderer(renderer) {}
	~ResourceManager();

	template<typename T = TBase>
	T* Get(const Name& name);

	void Add(const Name& name, TBase* element);
	void Remove(const Name& name) override;
	void RemoveAll() override;
};

template<typename TBase, size_t N>
inline ResourceManager<TBase, N>::~ResourceManager()
{
	RemoveAll();
}

template<typename TBase, size_t N>
template<typename T>
T* ResourceManager<TBase, N>::Get(const Name& name)
{
	u32 index = name.GetID() % N;
	TBase* element = Manager<TBase, N>::m_elements[index];

	if (element == nullptr)
	{
		element = new T(m_renderer);
		element->Create(name);
		Manager<TBase, N>::m_elements[index] = element;
	}

	return dynamic_cast<T*>(element);
}

template<typename TBase, size_t N>
inline void ResourceManager<TBase, N>::Add(const Name& name, TBase* element)
{
	u32 index = name.GetID() % N;
	TBase* manager_element = Manager<TBase, N>::m_elements[index];
	ASSERT(manager_element == nullptr);

	Manager<TBase, N>::m_elements[index] = element;
}

template<typename TBase, size_t N>
void ResourceManager<TBase, N>::Remove(const Name& name)
{
	u32 index = name.GetID() % N;
	TBase* element = Manager<TBase, N>::m_elements[index];
	if (element)
	{
		element->Destroy();
		delete element;
		Manager<TBase, N>::m_elements[index] = nullptr;
	}
}

template<typename TBase, size_t N>
void ResourceManager<TBase, N>::RemoveAll()
{
	for (size_t i = 0; i < N; i++)
	{
		TBase* element = Manager<TBase, N>::m_elements[i];
		if (element)
		{
			element->Destroy();
			delete element;
			Manager<TBase, N>::m_elements[i] = nullptr;
		}
	}
}
