#pragma once
#include "ref_count.h"

template<typename TBase>
class ref_ptr
{
public:
	ref_ptr(TBase* ptr) : m_ptr(ptr) 
	{
		if (ptr)
		{
			m_ref_count = new ref_count();
			m_ref_count->add_ref();
		}
	}

	~ref_ptr() 
	{ 
		if (m_ptr)
		{
			m_ref_count->release();
			if (m_ref_count->count() == 0)
			{
				delete m_ptr;
				delete m_ref_count;
			}
		}
	}

	ref_ptr(const ref_ptr<TBase>& rp)
	{
		m_ptr = rp.m_ptr;
		m_ref_count = rp.m_ref_count;
		m_ref_count->add_ref();
	}

	ref_ptr& operator = (const ref_ptr& rp)
	{
		if (m_ptr != rp.m_ptr)
		{
			if (m_ptr)
			{
				m_ref_count->release();
				if (m_ref_count->count() == 0)
				{
					delete m_ptr;
					delete m_ref_count;
				}
			}

			m_ptr = rp.m_ptr;
			m_ref_count = rp.m_ref_count;
			m_ref_count->add_ref();
		}

		return *this;
	}


	TBase* get() { return m_ptr; }

	TBase& operator * () { return *m_ptr; }
	TBase* operator -> () { return m_ptr; }

private:
	ref_count* m_ref_count = nullptr;
	TBase* m_ptr = nullptr;
};