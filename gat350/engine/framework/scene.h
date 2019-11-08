#pragma once
#include "actor.h"

class Scene : public Object
{
public:
	OBJECT_DECLARATION(Scene, Object)
	virtual ~Scene() {}

	virtual void Update() = 0;
	virtual void Draw() = 0;

	void AddObject(std::unique_ptr<Actor> actor)
	{
		m_actors.push_back(std::move(actor));
	}

	template<typename T>
	T* GetObject(const Name& name)
	{
		for (auto& actor : m_actors)
		{
			if (actor->GetName() == name)
			{
				return dynamic_cast<T*>(actor.get());
			}
		}

		return nullptr;
	}

	template<typename T>
	std::vector<T*> GetObjects()
	{
		std::vector<T*> actors;
		for (auto actor : m_actors)
		{
			if (dynamic_cast<T*>(actor) != nullptr)
			{
				actors.push_back(dynamic_cast<T*>(actor));
			}
		}

		return actors;
	}

protected:
	std::vector<std::unique_ptr<class Actor>> m_actors;
};
