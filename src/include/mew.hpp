/// @file mew.hpp
/// ���C�u�������ʒ�`.
#pragma once

#include "detail/headers.hpp"
#include "basic.hpp"
#include "preprocessor.hpp"
#include "meta.hpp"

#ifndef MEW_API
#	define MEW_API __declspec(dllimport)
#endif

//==============================================================================

/// ���C�u�������[�g���O���.
namespace mew
{
	//==============================================================================
	// �C���^�t�F�[�X.

	__interface __declspec(uuid("8A33A952-42CA-446D-8F1D-7589C2792242")) IString;
	__interface __declspec(uuid("EDB2A453-4879-410E-A3B9-4FCE94E7FA02")) IDisposable;
	__interface __declspec(uuid("E66A357E-3B54-401D-9DBA-7FC2450AAED6")) ISerializable;
	__interface __declspec(uuid("8BC045E4-1A58-4712-ADF3-D4595DBE079E")) IMessage;

	template < class T > class ref;
	using string = ref<IString>;		///< ������. ���ۂɂ͎Q�Ƃ����Aimmutable �̂��ߒl�^���ƍl���Ă��\��Ȃ�.
	using message = ref<IMessage>;	///< ���b�Z�[�W.
	using Stream = ref<IStream>;		///< �X�g���[��.

	//==============================================================================
	// �f�o�b�O�x��.

#ifdef _DEBUG
	MEW_API void Trace(const string& msg);
	MEW_API bool Assert(PCWSTR msg, PCWSTR file, int line, PCWSTR fn);
	MEW_API void RegisterInstance(IUnknown* obj, const char* name);
	MEW_API void UnregisterInstance(IUnknown* obj);
#endif
}

//==============================================================================
// Debug macro

#ifdef _DEBUG
inline void TRACE(const mew::string& msg)	{ mew::Trace(msg); }

#define MEW_PP_TRACE(n)										\
	template < PP_TYPENAMES(n) >							\
	inline void TRACE(PCTSTR msg, PP_ARGS_CONST(n) )		\
	{ mew::Trace(mew::string::format(msg, PP_ARG_VALUES(n) )); }
PP_REPEAT_FROM_1(10, MEW_PP_TRACE)
#undef MEW_PP_TRACE

#	define TRESPASS()			if(mew::Assert(L"TRESPASS", __FILE_W__, __LINE__, __FUNCTION_W__)); else __debugbreak()
#	define DEBUG_ONLY(exp)		exp
#	define ASSERT(exp)			if(exp) ; else if(mew::Assert(L#exp, __FILE_W__, __LINE__, __FUNCTION_W__)); else __debugbreak()
#	define VERIFY(exp)			ASSERT(exp)
#	define ASSERT_HRESULT(exp)	if(SUCCEEDED(exp)); else if(mew::Assert(L#exp, __FILE_W__, __LINE__, __FUNCTION_W__)); else __debugbreak()
#	define VERIFY_HRESULT(exp)	ASSERT_HRESULT(exp)
#else
#	define TRACE				__noop
#	define TRESPASS()			__assume(0)
#	define DEBUG_ONLY(exp)		
#	define ASSERT				__noop
#	define VERIFY(exp)			((void)(exp))
#	define ASSERT_HRESULT(exp)	__noop
#	define VERIFY_HRESULT(exp)	((void)(exp))
#endif
#define TRESPASS_DBG(exp)	if(false); else { TRESPASS(); DEBUG_ONLY(exp); }

//==============================================================================
// STATIC_ASSERT

template < bool x > struct STATIC_ASSERTION_FAILURE;
template <> struct STATIC_ASSERTION_FAILURE<true>{};
template < int x > struct STATIC_ASSERT_TEST{};
/// �R���p�C�����̃A�T�[�V���� : Boost����̃p�N��
#define STATIC_ASSERT(exp)				typedef STATIC_ASSERT_TEST < sizeof(STATIC_ASSERTION_FAILURE<(bool)(exp)>) > __STATIC_ASSERT_TYPEDEF__
#define STATIC_ASSERT_MSG(exp, msg)		typedef STATIC_ASSERT_TEST<1> __ERROR__##msg; typedef STATIC_ASSERT_TEST < sizeof(STATIC_ASSERTION_FAILURE<(bool)(exp)>) > __ERROR__##msg

namespace mew
{
	//==============================================================================
	// �\����.

	/// �ʂ��.
	const struct Null { template < typename T > operator T* () const throw() { return 0; } } null;
	
	/// �^�t�����ꂽ�C���^�t�F�[�X�|�C���^�̎Q��.
	struct REFINTF
	{
		REFIID iid;
		void** const pp;
		template < class T > REFINTF(T** ppT)              : pp((void**)ppT), iid(__uuidof(T))	{ ASSERT(pp); }
		template < class T > REFINTF(REFIID riid, T** ppT) : pp((void**)ppT), iid(riid)			{ ASSERT(pp); }
	};

	template < typename T >
	struct bitof_t
	{
		T& value;
		const T mask;

		bitof_t(T& v, T m) throw() : value(v), mask(m) {}
		void operator = (bool b) throw()	{ if(b) { value |= mask; } else { value &= ~mask; } }
		operator T () const throw()			{ return value & mask; }
	};

	/// �r�b�g���擾�E�ݒ肷��.
	template < typename T, typename M > inline bitof_t<T> bitof(T& value, M mask) throw()	{ return bitof_t<T>(value, static_cast<T>(mask)); }

	/// POD(Plain Old Data) �ł��邱�Ƃ������^�O.
	/// POD�ł��邱�Ƃ������ɂ́A������p�����邩�AIsPOD����ꉻ����.
	template < class T > struct POD
	{
		friend IStream& operator << (IStream& stream, const POD& pod)	{ io::StreamWriteExact(&stream, static_cast<const T*>(&pod), sizeof(T)); return stream; }
		friend IStream& operator >> (IStream& stream,       POD& pod)	{ io::StreamReadExact(&stream, static_cast<T*>(&pod), sizeof(T)); return stream; }
	};

	/// ����^��POD�^���ۂ��𔻒肷��.
	template < typename T              > struct IsPOD			{ enum { value = meta::Convertable<T, POD<T> >::value }; };
	template < typename T, size_t size > struct IsPOD<T[size]>	{ enum { value = false }; };

	/// �C���f�b�N�X�܂��̓|�C���^.
	template < class T > struct IndexOr
	{
		INT_PTR	value;
		
		IndexOr(INT_PTR index) : value(index)		{ ASSERT(is_index()); }
		IndexOr(T* p)          : value((INT_PTR)p)	{ ASSERT(is_ptr()); }
		template < class U > IndexOr(const ref<U>& p) : value((INT_PTR)(T*)p)	{ ASSERT(is_ptr()); }
		bool is_ptr()   const	{ return value == 0 || !IS_INTRESOURCE(value); }
		bool is_index() const	{ return IS_INTRESOURCE(value); }
		INT_PTR as_index() const	{ ASSERT(is_index());   return value; }
		T*      as_ptr()   const	{ ASSERT(is_ptr()); return (T*)value; }
		operator INT_PTR () const	{ return as_index(); }
		operator T*      () const	{ return as_ptr(); }
		T* operator -> () const	{ return as_ptr(); }
		bool operator == (INT_PTR index) const	{ return value == index; }
		bool operator == (const T* p)    const	{ return value == (INT_PTR)p; }
		bool operator ! () const				{ return value == 0; }

    #ifdef _WIN64
    IndexOr(__unaligned T* p) : value((INT_PTR)p)	{ ASSERT(is_ptr()); }
    IndexOr(int index) : value(static_cast<INT_PTR>(index)) { ASSERT(is_index()); }
    IndexOr(size_t index) : value(static_cast<INT_PTR>(index)) { ASSERT(is_index()); }
    IndexOr(UINT index) : value(static_cast<INT_PTR>(index)) { ASSERT(is_index()); }
    bool operator == (int index) const { return value == static_cast<INT_PTR>(index); }
    #endif
	};

	/// 4�̕����ŕ\���ꂽ���ʎq.
	struct FourCC : POD<FourCC>
	{
		UINT32	Value;
		FourCC() throw() {}
		FourCC(INT32 x) throw() : Value((UINT32)x) {}
		operator UINT32 () const throw()	{ return Value; }
		friend bool operator < (const FourCC& lhs, const FourCC& rhs) throw()	{ return lhs.Value < rhs.Value; }
	};

	/// GUID for index.
	/// {????????-69BE-4828-A21F-064B816EA451}
	const GUID GUID_Index = { 0x00000000, 0x69be, 0x4828, { 0xa2, 0x1f, 0x6, 0x4b, 0x81, 0x6e, 0xa4, 0x51 } };

	/// GUID for Clipboard Formats.
	/// {????????-C4F1-4667-8A96-ECCC6B91B971}
	const GUID GUID_ClipboardFormat = { 0x00000000, 0xc4f1, 0x4667, { 0x8a, 0x96, 0xec, 0xcc, 0x6b, 0x91, 0xb9, 0x71 } };

	/// GUID for FourCC. ����̓I�t�B�V�����ɋK�肳��Ă���炵��.
	/// {????????-0000-0010-8000-00AA00389B71}
	const GUID GUID_FourCC = { 0x00000000, 0x0000, 0x0010, { 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71 } };

	/// GUID�܂���16�����܂ł̃}���`�o�C�g������ŕ\���ꂽ���ʎq.
	struct Guid : GUID
	{
		Guid() {}
		Guid(REFGUID x) : GUID(x)	{}
		explicit Guid(PCSTR x)						{ strncpy((PSTR)this, x, sizeof(Guid)); }
		explicit Guid(int x)    : GUID(GUID_Index)	{ this->Data1 = x; }
		explicit Guid(FourCC x) : GUID(GUID_FourCC)	{ this->Data1 = x.Value; }
		operator const void* () const throw()								{ return memcmp(this, &GUID_NULL, sizeof(Guid)) ? this : 0; }
		friend bool operator < (const Guid& lhs, const Guid& rhs) throw()	{ return memcmp(&lhs, &rhs, sizeof(Guid)) < 0; }
	};

	/// �N���e�B�J���Z�N�V����.
	class CriticalSection
	{
	private:
		CRITICAL_SECTION	m_cs;
	public:
		CriticalSection() throw()	{ ::InitializeCriticalSection(&m_cs); }
		~CriticalSection() throw()	{ ::DeleteCriticalSection(&m_cs); }
		void Lock() throw()			{ ::EnterCriticalSection(&m_cs); }
		void Unlock() throw()		{ ::LeaveCriticalSection(&m_cs); }
		BOOL TryLock() throw()		{ return ::TryEnterCriticalSection(&m_cs); }
	private: // non-copyable
		CriticalSection(const CriticalSection&);
		CriticalSection& operator = (const CriticalSection&);
	};

	/// �N���e�B�J���Z�N�V�����̎������b�N.
	class AutoLock
	{
	private:
		CriticalSection&	m_cs;
	public:
		AutoLock(CriticalSection& cs) throw() : m_cs(cs)	{ m_cs.Lock(); }
		~AutoLock() throw()									{ m_cs.Unlock(); }
	private: // non-copyable
		AutoLock(const AutoLock&);
		AutoLock& operator = (const AutoLock&);
	};

	//==============================================================================
	// �C���^�t�F�[�X��`.

	/// �����I�Ȕj�����T�|�[�g����.
	__interface IDisposable : IUnknown
	{
		/// �I�u�W�F�N�g��j������.
		/// �z�Q�Ƃ�l�C�e�B�u���\�[�X��������邽�߂Ɏg�p����.
		/// ���̃��\�b�h�̌ďo����ADispose()�ȊO�̂��ׂẴ��\�b�h�̌Ăяo���͕s���ł���.
		void Dispose() throw();
	};

	/// �X�g���[���ւ̒��񉻂��T�|�[�g����.
	/// ���̃C���^�t�F�[�X���T�|�[�g����ꍇ�́A
	/// readable IStream�������ɂƂ�R���X�g���N�^����������K�v������.
	__interface ISerializable : IUnknown
	{
#ifndef DOXYGEN
		REFCLSID get_Class() throw();
#endif // DOXYGEN

		/// �t�V���A���C�Y�̍ۂɎg�p����N���XID [get].
		__declspec(property(get=get_Class)) CLSID Class;
		/// Stream �֎��g�̒��񉻏��������o��.
		void Serialize(IStream& stream);
	};

	//==============================================================================
	// �֐�.

	using FactoryProc = void(*)(REFINTF, IUnknown*);

	/// �I�u�W�F�N�g�𐶐�����.
	MEW_API void CreateInstance(
		REFCLSID  clsid,		///< �N���XID.
		REFINTF   ppInterface,	///< �쐬���ꂽ�I�u�W�F�N�g.
		IUnknown* arg			///< �R���X�g���N�^����.
	) throw(...);

	/// �N���X��o�^����.
	MEW_API void RegisterFactory(
		REFCLSID    clsid,		///< �N���XID.
		FactoryProc factory		///< �쐬�֐�.
	) throw();

	/// ��������쐬����.
	MEW_API void CreateString(
		IString** ppString,				///< �쐬���ꂽ������.
		PCWSTR    format,				///< ����������.
		size_t    length = (size_t)-1,	///< ����������̒���.
		size_t    argc   = 0,			///< ����������̈����̐�.
		PCWSTR    argv[] = 0			///< ����������̈����̔z��.
	) throw();

	/// ���\�[�X���當������쐬����.
	MEW_API void CreateString(
		IString** ppString,		///< �쐬���ꂽ������.
		UINT      nID,			///< ������ID.
		HMODULE   hModule,		///< ���\�[�X�e�[�u�����܂ރ��W���[��.
		size_t    argc   = 0,	///< ����������̈����̐�.
		PCWSTR    argv[] = 0	///< ����������̈����̔z��.
	) throw();

	/// �I�u�W�F�N�g�̕�����\���𓾂�.
	void ObjectToString(
		IString** ppString,		///< �쐬���ꂽ��������󂯎��.
		IUnknown* pObject		///< ������\���𓾂����I�u�W�F�N�g. null�ł��悢.
	) throw();
}
