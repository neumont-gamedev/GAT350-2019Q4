#pragma once
#include "../engine.h"
#include "../../framework/factory.h"

#define OBJECT_DECLARATION(class_name, super_class_name) \
	class_name() {} \
	class_name(const Name& name) : super_class_name(name) {} \
	virtual const char* GetClassName() { return #class_name; }

#define OBJECT_FACTORY(class_name) \
	class FactoryRegister \
	{ \
	public: \
		FactoryRegister() { ObjectFactory::Register(#class_name, new Creator<class_name, Object>()); } \
	} instance;

class Object
{
public:
	Object() {}
	Object(const Name& name) { m_name = name; }
	virtual ~Object() {}

	virtual bool Create(const Name& name) = 0;
	virtual void Destroy() = 0;

	virtual const char* GetClassName() = 0;

	Name& GetName() { return m_name; }

	inline virtual void Edit() { ImGui::Text(m_name.c_str()); }

protected:
	Name m_name;
};
