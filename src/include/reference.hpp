/// @file reference.hpp
/// �Q�ƃJ�E���g.
#pragma once

#include "mew.hpp"
#include "std/algorithm.hpp"
#include "std/vector.hpp"

namespace mew
{
	//==============================================================================
	// IUnknown operations.

#define MEW_PP_NEW(n)									\
	template < typename T PP_TYPENAMES_CAT(n) >			\
	inline ref<T> objnew( PP_ARGS_CONST(n) )			\
	{ return ref<T>::from(new T( PP_ARG_VALUES(n) )); }
PP_REPEAT(10, MEW_PP_NEW)
#undef MEW_PP_NEW

	template < class T > inline void objinc(T*      p) throw()					{ if(p) { p->AddRef(); } }
	template < class T > inline void objdec(T*&     p) throw()					{ if(p) { T* tmp = p; p = null; tmp->Release(); } }
	template < class T > inline void objdec(ref<T>& p) throw()					{ p.clear(); }
	template < class T > inline HRESULT objcpy(IUnknown* p, T**     pp) throw()	{ return p ? p->QueryInterface(__uuidof(T), (void**)pp) : (*pp = null, E_POINTER); }
	                     inline HRESULT objcpy(IUnknown* p, REFINTF pp) throw()	{ return p ? p->QueryInterface(pp.iid, pp.pp) : (*pp.pp = null, E_POINTER); }
	inline bool objcmp(IUnknown* lhs, IUnknown* rhs) throw()
	{
		if(lhs == rhs)   return true;
		if(!lhs || !rhs) return false;
		IUnknown* punk1 = null;
		IUnknown* punk2 = null;
		lhs->QueryInterface(__uuidof(IUnknown), (void**)&punk1);
		rhs->QueryInterface(__uuidof(IUnknown), (void**)&punk2);
		bool result = (punk1 == punk2);
		punk1->Release();
		punk2->Release();
		return result;
	}

	//==============================================================================

	/// �X�}�[�g�|�C���^�x�[�X.
	template < typename T > class ref_base
	{
	private:
#pragma warning( disable : 4510 )
#pragma warning( disable : 4610 )
		class thunk : public T
		{
		public:
			using T::QueryInterface;
			HRESULT QueryInterface(REFINTF pp)								{ return QueryInterface(pp.iid, pp.pp); }
			template < class Q > HRESULT QueryInterface(Q** pp, REFIID iid)	{ return QueryInterface(iid, (void**)pp); }
		private:
			ULONG __stdcall AddRef()  = 0;
			ULONG __stdcall Release() = 0;
			~thunk();	// operator delete is forbidden.
		};
#pragma warning( default : 4510 )
#pragma warning( default : 4610 )

	public:
		using pointer_in = T*;
		using reference_in = T&;
		using pointer_out = thunk*;
		using reference_out = thunk&;

	protected:
		T*	m_ptr;

	private:
		void operator [] (size_t) const; // �z��ɑ΂���X�}�[�g�|�C���^�͗L�蓾�Ȃ�.
	public:
		ref_base() throw() : m_ptr(null)	{}
		ref_base(const ref_base& p) throw() : m_ptr(p.m_ptr)	{ if(m_ptr) m_ptr->AddRef(); }
		ref_base(pointer_in ptr) throw()    : m_ptr(ptr)		{ if(m_ptr) m_ptr->AddRef(); }
		~ref_base() throw()	{ clear(); }
		ref_base& operator = (ref_base& p) throw()	{ return operator = (p.m_ptr); }
		ref_base& operator = (pointer_in p) throw()
		{
			if(p) p->AddRef();
			clear();
			m_ptr = p;
			return *this;
		}
		// operators
		operator pointer_out     () const throw()		{ return static_cast<pointer_out>(m_ptr); }
		reference_out operator * () const throw()		{ ASSERT(m_ptr); return *static_cast<pointer_out>(m_ptr); }
		pointer_in* operator &  () throw()				{ ASSERT(!m_ptr); return &m_ptr; }
		pointer_out operator -> () const throw()		{ ASSERT(m_ptr); return static_cast<pointer_out>(m_ptr); }
		bool operator !  () const throw()				{ return !m_ptr; }
		bool operator <  (pointer_in p) const throw()	{ return m_ptr < p; }
		bool operator == (pointer_in p) const throw()	{ return m_ptr == p; }
		bool operator != (pointer_in p) const throw()	{ return m_ptr != p; }
		// methods
		pointer_in get() const			throw()	{ return m_ptr; }
		pointer_in get() const volatile	throw()	{ return m_ptr; }
		void clear()					throw()	{ if(m_ptr) detach()->Release(); }
		void attach(pointer_in p)		throw()	{ clear(); m_ptr = p; }
		pointer_in detach()				throw()	{ pointer_in tmp = m_ptr; m_ptr = null; return tmp; }
		static ref<T> from(pointer_in p) throw()
		{
			ref<T> ptr;
			ptr.attach(p);
			return ptr;
		}
		void create(REFCLSID clsid, IUnknown* arg = null) throw(...)
		{
			ASSERT(!m_ptr);
			mew::CreateInstance(clsid, &m_ptr, arg);
			ASSERT(m_ptr);
		}
		void dispose() throw()
		{
			if(T* p = detach())
			{
				__if_exists(T::Dispose)
				{
					p->Dispose();
				}
				__if_not_exists(T::Dispose)
				{
					IDisposable* d = 0;
					if SUCCEEDED(p->QueryInterface(__uuidof(IDisposable), (void**)&d))
					{
						d->Dispose();
						d->Release();
					}
				}
				p->Release();
			}
		}
		template < typename T >
		HRESULT copyto(T** pp) const throw()		{ return mew::objcpy(m_ptr, pp); }
		HRESULT copyto(REFINTF pp) const throw()	{ return mew::objcpy(m_ptr, pp); }
	};

	/// �X�}�[�g�|�C���^.
	template < typename T > class ref : public ref_base<T>
	{
	private:
		using super = ref_base<T>;
	public:
		ref() throw()	{}
		ref(const Null&) throw()	{}
		ref(const ref& p) throw() : super(p)	{}
		ref(pointer_in p) throw() : super(p)	{}
		template < class U > ref(const ref_base<U>& p) throw() : super(p)	{}
		explicit ref(REFCLSID clsid, IUnknown* arg = null) throw(...)		{ create(clsid, arg); }
		ref& operator = (const ref& p) throw()	{ super::operator = (p); return *this; }
		ref& operator = (pointer_in p) throw()	{ super::operator = (p); return *this; }
		ref& operator = (const Null&)  throw()	{ clear(); return *this; }
		template < class U > ref& operator = (const ref_base<U>& p) throw()	{ super::operator = (p); return *this; }
	};

	//==============================================================================

	namespace detail { class auto_cast_t; }
	detail::auto_cast_t cast(IUnknown* ptr) throw();
	namespace detail
	{
		class auto_cast_t
		{
			friend auto_cast_t mew::cast(IUnknown* ptr) throw();
		private:
			IUnknown* m_ptr;
			auto_cast_t(IUnknown* p) throw() : m_ptr(p)	{}
		public:
			template < typename T > operator ref<T> () const throw()
			{
				if(!m_ptr)
					return ref<T>();
				ref<T> p;
				m_ptr->QueryInterface(&p);
				return p;
			}
			template < typename T > operator ref_base<T> () const throw()
			{
				if(!m_ptr)
					return ref_base<T>();
				ref_base<T> p;
				m_ptr->QueryInterface(&p);
				return p;
			}
		};
	}

	/// QueryInterface��p����L���X�g�i�e���v���[�g�����ȗ��Łj.
	inline detail::auto_cast_t cast(IUnknown* ptr) throw()	{ return detail::auto_cast_t(ptr); }

	/// QueryInterface��p����L���X�g.
	template < typename T > inline ref<T> cast(IUnknown* ptr) throw()	{ ref<T> p; objcpy(ptr, &p); return p; }

	//==============================================================================

	/// IEnumUnknown ���ȒP�Ɏg�����߂̃w���p.
	template < typename T > class each : public ref<T>
	{
	private:
		using super = ref_base<T>;
		ref<IEnumUnknown> m_enum;
	public:
		each(IEnumUnknown* e) throw() : m_enum(e)	{}
		each(const each& p) throw() : m_enum(p.m_enum)	{}
		each& operator = (const each& p) throw()
		{
			super::operator = (p);
			m_enum = p.m_enum;
			return *this;
		}
		bool next()
		{
			if(!m_enum)
				return false;
			clear();
			ref<IUnknown> unk;
			if(m_enum->Next(1, &unk, null) == S_OK && SUCCEEDED(unk->QueryInterface(&m_ptr)))
				return true;
			m_enum.clear();
			return false;
		}
	};

	///
	template < typename TSequence >
	class EnumUnknownBase : public IEnumUnknown
	{
		// TODO: TSequence �������_���A�N�Z�X�\�łȂ��ꍇ�ւ̑Ή�.
	public:
		using sequence = TSequence;

	protected:
		const sequence&	m_range;
		size_t			m_iter;
		volatile LONG	m_refcount;

	private: // non-copyable
		EnumUnknownBase(const EnumUnknownBase&);
		EnumUnknownBase& operator = (const EnumUnknownBase&);

	protected:
		EnumUnknownBase(const sequence& seq, size_t iter = 0) : m_range(seq), m_iter(iter), m_refcount(1)
		{
			DEBUG_ONLY( mew::RegisterInstance(this, "EnumUnknownBase") );
		}
	public:
		virtual ~EnumUnknownBase()
		{
			ASSERT(m_refcount == 0);
			DEBUG_ONLY( mew::UnregisterInstance(this) );
		}
		HRESULT __stdcall QueryInterface(REFIID iid, void** pp)
		{
			if(!pp)
				return E_POINTER;
			if(iid == __uuidof(IEnumUnknown) || iid == __uuidof(IUnknown))
			{
				*pp = static_cast<IEnumUnknown*>(this);
				AddRef();
				return S_OK;
			}
			else
			{
				*pp = null;
				return E_NOINTERFACE;
			}
		}
		ULONG __stdcall AddRef()
		{
			ASSERT(m_refcount > 0);
			return ::InterlockedIncrement(&m_refcount);
		}
		ULONG __stdcall Release()
		{
			ASSERT(m_refcount > 0);
			ULONG refcount = ::InterlockedDecrement(&m_refcount);
			if(refcount == 0)
				delete this;
			return refcount;
		}
		HRESULT __stdcall Next(ULONG count, IUnknown** output, ULONG* fetched)
		{
			if(m_iter >= m_range.size())
			{
				if(fetched) *fetched = 0;
				return S_FALSE;
			}
			const ULONG remain = m_range.size() - m_iter;
			HRESULT hr;
			ULONG done;
			if(count <= remain)	{ hr = S_OK; done = count; } else { hr = S_FALSE; done = remain; }
			for(ULONG i = 0; i < done; ++i)
			{
				output[i] = m_range[m_iter];
				objinc(output[i]);
			}
			m_iter += done;
			return hr;
		}
		HRESULT __stdcall Skip(ULONG count)
		{
			m_iter += count;
			return m_iter <= m_range.size();
		}
		HRESULT __stdcall Reset()
		{
			m_iter = 0;
			return S_OK;
		}
	};

	/// STL�R���e�i�ւ̎Q�Ƃ�񋓂���.
	template < typename TSequence >
	class EnumUnknown : public EnumUnknownBase<TSequence>
	{
		using super = EnumUnknownBase<TSequence>;
	protected:
		ref<IUnknown>	m_owner;

	private:
		EnumUnknown(IUnknown* owner, const sequence& seq, size_t iter = 0) : m_owner(owner), super(seq, iter)
		{}
	public:
		static ref<IEnumUnknown> newobj(IUnknown* owner, const sequence& seq, size_t iter = 0)
		{
			return ref<IEnumUnknown>::from(new EnumUnknown(owner, seq, iter));
		}
		HRESULT __stdcall Clone(IEnumUnknown** ppEnum)
		{
			if(!ppEnum) return E_POINTER;
			EnumUnknown* clone = new EnumUnknown(m_owner, m_range, m_iter);
			*ppEnum = clone;
			return S_OK;
		}
	};

	/// �X�}�[�g�|�C���^�̔z��.
	template
	<
		class T,
		template < class T, class TAlloc > class TSequence = std::vector,
		template < class T > class TAlloc = std::allocator
	>
	class array
	{
	public:
		using pointer_out = typename ref<T>::pointer_out;
		using pointer_in  = typename ref<T>::pointer_in;
		using allocator_type = TAlloc<pointer_in>;
		using sequence = TSequence<pointer_in,  allocator_type>;
		using value_type = typename sequence::value_type;
		using size_type = typename sequence::size_type;
		using iterator = typename sequence::iterator;
		using const_iterator = typename sequence::const_iterator;
		using enum_type = EnumUnknown<sequence>;

	protected:
		sequence m_items;

	public:
		array() throw()	{}
		array(const array& rhs) throw() : m_items(rhs.m_items)
		{
			std::for_each(m_items.begin(), m_items.end(), objinc);
		}
		array& operator = (const array& rhs) throw()
		{
			sequence items(rhs.m_items);
			algorithm::for_all(items, objinc);
			clear();
			std::for_each(items.begin(), items.end(), objinc);
		}
		~array() throw()
		{
			clear();
		}
		pointer_out data() throw()
		{
			return empty() ? null : static_cast<pointer_out>(&m_items[0]);
		}
		ref<IEnumUnknown> enumerate(IUnknown* owner) throw()
		{
			if(m_items.empty())
				return null;
			else
				return enum_type::newobj(owner, m_items);
		}
		HRESULT enumerate(REFINTF pp, IUnknown* owner) throw()
		{
			if(m_items.empty())
				return E_FAIL;
			return objnew<enum_type>(owner, m_items)->QueryInterface(pp);
		}

	public: // vector compatible
		void insert(pointer_in p, size_t index) throw()
		{
			if(p)
				p->AddRef();
			sequence::iterator i = m_items.begin();
			std::advance(i, math::min<size_type>(index, m_items.size())); 
			m_items.insert(i, p);
		}
		void clear() throw()
		{
			for(sequence::iterator i = m_items.begin(); i != m_items.end(); ++i)
			{
				if(*i) (*i)->Release();
			}
			m_items.clear();
		}
		void push_back(pointer_in p) throw()
		{
			if(p) p->AddRef();
			m_items.push_back(p);
		}
		void pop_back() throw()
		{
			if(pointer_in p = m_items.back()) p->Release();
			m_items.pop_back();
		}
		size_type size() const throw()		{ return m_items.size(); }
		bool empty() const throw()			{ return m_items.empty(); }
		pointer_out front() const throw()	{ return static_cast<pointer_out>(m_items.front()); }
		pointer_out back() const throw()	{ return static_cast<pointer_out>(m_items.back()); }
		pointer_out at(size_type index) const throw()				{ return static_cast<pointer_out>(m_items[index]); }
		pointer_out operator [] (size_type index) const throw()		{ return static_cast<pointer_out>(m_items[index]); }
		//void resize(size_type size) throw()
		//{
		//	m_items.resize(size);
		//}
		void reserve(size_type count) throw()	{ m_items.reserve(count); }
		void erase(iterator i) throw()
		{
			if(*i) (*i)->Release();
			m_items.erase(i);
		}
		bool erase(pointer_in p) throw()
		{
			sequence::iterator i = std::find_if(m_items.begin(), m_items.end(), std::bind1st(std::ptr_fun(&objcmp), p));
			if(i == m_items.end())
				return false;
			erase(i);
			return true;
		}
		bool erase(size_t index) throw()
		{
			if((size_type)index >= m_items.size())
				return false;
			if(!p || objcmp(m_items[index], p))
			erase(m_items.begin() + index);
			return true;
		}
		iterator begin() throw()	{ return m_items.begin(); }
		iterator end() throw()		{ return m_items.end(); }
		const_iterator begin() const throw()	{ return m_items.begin(); }
		const_iterator end() const throw()		{ return m_items.end(); }

	public: // algorithm extension
		template < class Op >
		void foreach(Op op, bool acceptNull = false)
		{
			for(sequence::iterator i = m_items.begin(); i != m_items.end(); ++i)
			{
				if(acceptNull || *i) op(*i);
			}
		}
		template < class T, class M >
		void foreach(T* p, M m, bool acceptNull = false)
		{
			for(sequence::iterator i = m_items.begin(); i != m_items.end(); ++i)
			{
				if(acceptNull || *i) (p->*m)(*i);
			}
		}
		template < class U >  iterator find(const U& u)				{ return std::find(m_items.begin(), m_items.end(), u); }
		template < class Op > iterator find_if(Op op)				{ return std::find_if(m_items.begin(), m_items.end(), op); }
		template < class U >  const_iterator find(const U& u) const	{ return std::find(m_items.begin(), m_items.end(), u); }
		template < class Op > const_iterator find_if(Op op) const	{ return std::find_if(m_items.begin(), m_items.end(), op); }
	};

	/// IEnumUnknown �� �z�񂪈�̉�����Ă���.
	template
	<
		class T,
		template < class T, class TAlloc > class TSequence = std::vector,
		template < class T > class TAlloc = std::allocator
	>
	class Enumerator : public array<T, TSequence, TAlloc>, public EnumUnknownBase<typename array<T, TSequence, TAlloc>::sequence>
	{
		using enum_type = EnumUnknownBase<typename array<T, TSequence, TAlloc>::sequence>;
	public:
		Enumerator() : enum_type(m_items, 0)			{}
		HRESULT __stdcall Clone(IEnumUnknown** ppEnum)	{ return enumerate(this).copyto(ppEnum); }
	};
}
