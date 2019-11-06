#pragma once

template<typename TBase>
class smart_ptr
{
public:
	smart_ptr(TBase* ptr) : m_ptr(ptr) {}
	~smart_ptr() 
	{ 
		if (m_ptr) delete m_ptr; 
	}
	smart_ptr(const smart_ptr<TBase>&) = delete;
	smart_ptr& operator = (const smart_ptr&) = delete;

	TBase* get() { return m_ptr; }

	TBase& operator * () { return *m_ptr; }
	TBase* operator -> () { return m_ptr; }

private:
	TBase* m_ptr = nullptr;
};