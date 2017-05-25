/// @file io.hpp
/// �ėp���o��.

#pragma once

#include "string.hpp"
#include <shlwapi.h>

namespace mew
{
	namespace io
	{
		const DWORD STGM_DEFAULT_READ  = STGM_READ | STGM_SHARE_DENY_WRITE;
		const DWORD STGM_DEFAULT_WRITE = STGM_WRITE | STGM_CREATE | STGM_SHARE_EXCLUSIVE;

		void   StreamReadExact  (IStream* stream, void* buffer, size_t size);
		size_t StreamReadSome   (IStream* stream, void* buffer, size_t size);
		void   StreamReadObject (IStream* stream, REFINTF obj);
		void   StreamWriteExact (IStream* stream, const void* buffer, size_t size);
		size_t StreamWriteSome  (IStream* stream, const void* buffer, size_t size);
		void   StreamWriteObject(IStream* stream, IUnknown* obj);
		void   StreamSeekAbs    (IStream* stream, UINT64 pos, UINT64* newpos = null);
		void   StreamSeekRel    (IStream* stream, INT64 mov, UINT64* newpos = null);
        HGLOBAL	StreamCreateOnHGlobal(IStream** pp, size_t size, bool bDeleteOnRelease);
	}

	//==============================================================================

	/// �X�g���[��.
	/// @see IStream
	template <> class ref<IStream> : public ref_base<IStream>
	{
		using super = ref_base<IStream>;

	public:
		Stream() throw() {}
		Stream(IStream* p) throw() : super(p) {}
		Stream(const Stream& p) throw() : super(p)		{}
		explicit ref(REFCLSID clsid, IUnknown* arg = null) throw(...)		{ create(clsid, arg); }
		Stream& operator = (const Stream& s) throw()
		{
			super::operator = (s);
			return *this;
		}
		bool read(void* buffer, size_t size, size_t& done) const		{ done = io::StreamReadSome(m_ptr, buffer, size); return done > 0; }
		const Stream& read(void* buffer, size_t size) const				{ io::StreamReadExact(m_ptr, buffer, size); return *this; }
		const Stream& read(REFINTF obj) const							{ io::StreamReadObject(m_ptr, obj); return *this; }
		bool write(const void* buffer, size_t size, size_t& done) const	{ done = io::StreamWriteSome(m_ptr, buffer, size); return done > 0; }
		const Stream& write(const void* buffer, size_t size) const		{ io::StreamWriteExact(m_ptr, buffer, size); return *this; }
		const Stream& write(IUnknown* obj) const						{ io::StreamWriteObject(m_ptr, obj); return *this; }
		void seek(INT64 mov, UINT64* newpos = null) const				{ io::StreamSeekRel(m_ptr, mov, newpos); }
		UINT64 get_position() const										{ UINT64 newpos; io::StreamSeekRel(m_ptr, 0, &newpos); return newpos; }
		void set_position(UINT64 pos) const								{ io::StreamSeekAbs(m_ptr, pos); } 
		void set_position(UINT64 pos, UINT64* newpos) const				{ io::StreamSeekAbs(m_ptr, pos, newpos); }
		__declspec(property(get=get_position, put=set_position)) UINT64 position;

		template < typename T >
		friend const Stream& operator >> (const Stream& stream, ref<T>& ptr)	{ stream.read(&ptr); return stream; }
		friend const Stream& operator << (const Stream& stream, IUnknown* ptr)	{ stream.write(ptr); return stream; }

		operator IStream& () const	{ return *m_ptr; }
	};

	/// �ėp���o��.
	namespace io
	{
		//==============================================================================
		// �쐬�\�ȃN���X

		class __declspec(uuid("4B1AE91F-B6B5-49CA-9669-3565E011EA41")) Reader;
		class __declspec(uuid("E67D7711-8A8F-4FC8-8147-D805DB641CE6")) Writer;
		class __declspec(uuid("3D1B9CCB-AB14-44CF-8D83-C4ED7CE547A4")) FileReader;
		class __declspec(uuid("9963A69D-3F35-4C88-BCE5-201C213088DF")) FileWriter;

		//==============================================================================
		// �p�X����.

		/// ����t�H���_�p�X����������.
		/// pair<CSIDL, NextChar>
		std::pair<int, PCWSTR> PathResolveCSIDL(PCWSTR src, PCWSTR approot, int appcsidl);

		/// 
		string PathResolvePath(PCWSTR src, PCWSTR approot, int appcsidl);

		inline LPCWSTR PathFindLeaf(const string& path)
		{
			if(!path)
				return null;
			return ::PathFindFileName(path.str());
		}

		//==============================================================================

		/// �p�X����̂��߂̌Œ蒷������.
		class Path
		{
		private:
			TCHAR	m_Path[MAX_PATH];
		public:
			Path()					{ str::clear(m_Path); }
			Path(PCTSTR path)		{ str::fullpath(m_Path, path, MAX_PATH); }

			PTSTR  str()				{ return m_Path; }
			PCTSTR str() const			{ return m_Path; }
			operator PTSTR  ()			{ return str(); }
			operator PCTSTR () const	{ return str(); }

			/// �t�@�C�����̈ʒu��Ԃ�.
			PCTSTR FindLeaf() const			{ return ::PathFindFileName(m_Path); }
			/// �t�@�C�����̈ʒu��Ԃ�.
			PTSTR  FindLeaf()				{ return ::PathFindFileName(m_Path); }
			/// �g���q�̈ʒu��Ԃ�.
			PCTSTR FindExtension() const	{ return ::PathFindExtension(m_Path); }
			/// �g���q�̈ʒu��Ԃ�.
			PTSTR  FindExtension()			{ return ::PathFindExtension(m_Path); }
			/// �����Ƀo�b�N�X���b�V����ǉ�����.
			/// ���łɖ������o�b�N�X���b�V���̏ꍇ�͉������Ȃ�.
			Path& AddSeparator()
			{
				::PathAddBackslash(m_Path);
				return *this;
			}
			/// �p�X��A������.
			/// �ԂɃo�b�N�X���b�V����t�����邩�ۂ��͎����I�ɔ��f�����.
			Path& Append(PCTSTR appended)
			{
				PathAppend(m_Path, appended);
				return *this;
			}
			/// �t�@�C���^�C�g������菜��.
			Path& RemoveLeaf()
			{
				PathRemoveFileSpec(m_Path);
				return *this;
			}
			/// �t�@�C���^�C�g����ύX����.
			Path& ChangeLeaf(PCTSTR leaf)
			{
				PathRemoveFileSpec(m_Path);
				PathAppend(m_Path, leaf);
				return *this;
			}
			/// �g���q��ǉ�����.
			/// ���łɊg���q�������Ă���ꍇ�͉������Ȃ�.
			/// @param ext  �s���I�h�t���g���q. ��F".txt"
			Path& AddExtension(PCTSTR ext)
			{
				::PathAddExtension(m_Path, ext);
				return *this;
			}
			/// �g���q����菜��.
			Path& RemoveExtension()
			{
				::PathRemoveExtension(m_Path);
				return *this;
			}
			/// �g���q��ύX����.
			/// @param ext �s���I�h�t���g���q. ��F".txt"
			Path& ChangeExtension(PCTSTR ext)
			{
				::PathRenameExtension(m_Path, ext);
				return *this;
			}
		};
	}
}

//==============================================================================
// POD�^�ɑ΂���Binary���o�̓I�y���[�^

/// POD�^�Ƃ��Ĉ���.
#define AVESTA_POD(POD)															\
	inline IStream& operator << (IStream& stream, const POD& pod)				\
	{ mew::io::StreamWriteExact(&stream, &pod, sizeof(POD)); return stream; }	\
	inline IStream& operator >> (IStream& stream, POD& pod)						\
	{ mew::io::StreamReadExact(&stream, &pod, sizeof(POD)); return stream; }	\
	template <> struct mew::IsPOD<POD> { enum { value = 1 }; };

AVESTA_POD( signed   char )
AVESTA_POD( unsigned char )
AVESTA_POD( signed   short )
AVESTA_POD( unsigned short )
AVESTA_POD( signed   int )
AVESTA_POD( unsigned int )
AVESTA_POD( signed   long )
AVESTA_POD( unsigned long )
AVESTA_POD( signed   __int64 )
AVESTA_POD( unsigned __int64 )
AVESTA_POD( bool )
AVESTA_POD( char )
AVESTA_POD( __wchar_t )
AVESTA_POD( float )
AVESTA_POD( double )
AVESTA_POD( long double )
AVESTA_POD( SIZE )
AVESTA_POD( POINT )
AVESTA_POD( RECT )
AVESTA_POD( GUID )

template < typename T >
inline IStream& operator << (IStream& stream, const mew::POD<T>& pod)
{
	mew::io::StreamWriteExact(&stream, static_cast<const T*>(&pod), sizeof(T)); return stream;
}

template < typename T >
inline IStream& operator >> (IStream& stream, mew::POD<T>& pod)
{
	mew::io::StreamReadExact(&stream, static_cast<T*>(&pod), sizeof(T)); return stream;
}

template < typename T >
inline IStream& operator >> (IStream& stream, mew::ref<T>& ptr)
{
	mew::io::StreamReadObject(&stream, &ptr);
	return stream;
}

inline IStream& operator << (IStream& stream, IUnknown* ptr)
{
	mew::io::StreamWriteObject(&stream, ptr);
	return stream;
}
