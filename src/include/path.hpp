/// @file path.hpp
/// �p�X����.
#pragma once

#include "string.hpp"

namespace mew
{
	namespace io
	{
		//==============================================================================
		// ����t�H���_�̃p�X

		const WCHAR GUID_MyDocument  [] = L"::{450D8FBA-AD25-11D0-98A8-0800361B1103}";
		const WCHAR GUID_Network     []	= L"::{208D2C60-3AEA-1069-A2D7-08002B30309D}"; 
		const WCHAR GUID_RecycleBin  [] = L"::{645FF040-5081-101B-9F08-00AA002F954E}";
		const WCHAR GUID_Internet    []	= L"::{871C5380-42A0-1069-A2EA-08002B30309D}";
		const WCHAR GUID_MyComputer  [] = L"::{20D04FE0-3AEA-1069-A2D8-08002B30309D}";
		const WCHAR GUID_ControlPanel[] = L"::{20D04FE0-3AEA-1069-A2D8-08002B30309D}\\::{21EC2020-3AEA-1069-A2DD-08002B30309D}";
		const WCHAR GUID_Printer     []	= L"::{20D04FE0-3AEA-1069-A2D8-08002B30309D}\\::{2227A280-3AEA-1069-A2DE-08002B30309D}";
		const WCHAR GUID_WebFolder   [] = L"::{20D04FE0-3AEA-1069-A2D8-08002B30309D}\\::{BDEADF00-C265-11d0-BCED-00A0C90AB50F}";
		const WCHAR GUID_DialUp      []	= L"::{20D04FE0-3AEA-1069-A2D8-08002B30309D}\\::{992CFFA0-F557-101A-88EC-00DD010CCC48}";
		const WCHAR GUID_Task        []	= L"::{20D04FE0-3AEA-1069-A2D8-08002B30309D}\\::{D6277990-4C6A-11CF-8D87-00AA0060F5BF}";

		const INT_PTR IDList_Self   = 0;
		const INT_PTR IDList_Parent = 1;
		const INT_PTR IDList_Linked = 0x0000FFFF;

		//==============================================================================
		// �C���^�t�F�[�X

		interface __declspec(uuid("409E5925-F847-44F8-A505-26A619AB793D")) IEntry;
		__interface __declspec(uuid("4F030BAB-544F-4898-A73A-8901BB631D35")) IEntryList;
		__interface __declspec(uuid("B44AEEF2-4EC8-436C-B87D-F28DD1CC2C6A")) IFolder;
		__interface __declspec(uuid("3F2929E4-BCC8-4D51-A9C9-A30CC0C3FA22")) IDragSource;

		//==============================================================================
		// �쐬�\�ȃN���X

		class __declspec(uuid("B9847445-80E8-4EBB-BB30-09FF879A31B2")) Entry;
		class __declspec(uuid("0BB31216-4BB7-4B10-957E-83AFF7B4C212")) EntryList;
		class __declspec(uuid("1D502BC3-DA23-4AB1-8E65-6354D1EAA39D")) FolderMenu;
		class __declspec(uuid("6B69FE4C-66D3-4C06-B112-423732354AF4")) DragSource;

		//==============================================================================
		// �C���^�t�F�[�X��`.

		using IndexOrIDList = IndexOr<const ITEMIDLIST>;

		/// �V�F���G���g��(ITEMIDLIST).
		interface __declspec(novtable) IEntry : IUnknown
		{
			/// �G���g���̖��O�̎��.
			enum NameType
			{
				NAME,			///< �G�N�X�v���[���ŕ\������閼�O.
				PATH,			///< �t���p�X.
				LEAF,			///< ���[�t�@�C����.
				BASE,			///< �g���q�����������[�t�@�C����.
				EXTENSION,		///< �g���q�̂�.
				URL,			///< URL�`��. file:///C:/foo.txt
				IDENTIFIER,		///< �V�F�����O��Ԃł̃t�H���_��.
				PATH_OR_NAME,	///< �t���p�X�A��̏ꍇ�͖��O.
				LEAF_OR_NAME,	///< ���[�t�@�C�����A��̏ꍇ�͖��O.
				BASE_OR_NAME,	///< �g���q�����������[�t�@�C�����A��̏ꍇ�͖��O.
			};

#ifndef DOXYGEN
			string get_Name()	{ return GetName(NAME); }
			string get_Path()	{ return GetName(PATH); }
			virtual LPCITEMIDLIST get_ID() = 0;
			virtual int get_Image() = 0;
#endif
			__declspec(property(get=get_Name )) string Name;
			__declspec(property(get=get_Path )) string Path;
			__declspec(property(get=get_ID   )) LPCITEMIDLIST ID;	///< �V�F��ID���X�g.
			__declspec(property(get=get_Image)) int Image;			///< �C���[�W���X�g�C���f�N�X.

			/// �G���g���̖��O���擾����.
			virtual string GetName(NameType what) = 0;
			/// �G���g���̖��O��ݒ肷��.
			virtual HRESULT SetName(PCWSTR name, HWND hwnd = null) = 0;

			/// ���΃p�X���w�肵�ăI�u�W�F�N�g���擾����.
			/// �t�H���_�łȂ��ꍇ�͎��s����.
			virtual HRESULT QueryObject(
				REFINTF       ppObject,		///< IEntry or IShellFolder.
				IndexOrIDList relpath = 0	///< ���΃p�X / null�̏ꍇ�͎������g / 65535�ȉ��̏ꍇ�͐e����.
			) = 0;
			///
			virtual HRESULT ParseDisplayName(
				REFINTF ppObject,	///< IEntry or IShellFolder.
				PCWSTR relpath		///< ���΃p�X / null�̏ꍇ�͎������g / 65535�ȉ��̏ꍇ�͐e����.
			) = 0;
			/// �G�N�X�v���[���Ńt�H���_�Ƃ��ĕ\���ł��邩�ۂ�.
			virtual bool IsFolder() = 0;
			/// 
			virtual bool Exists() = 0;
			///
			virtual bool Equals(IEntry* rhs, NameType what = IDENTIFIER) = 0;
			///
			virtual ref<IEnumUnknown> EnumChildren(bool includeFiles) = 0;

			// Alias
			HRESULT GetParent(REFINTF ppObject)	{ return QueryObject(ppObject, IDList_Parent); }
			HRESULT GetLinked(REFINTF ppObject)	{ return QueryObject(ppObject, IDList_Linked); }
		};

		//==============================================================================
		// �p�X.

		bool	PathIsRegistory(PCWSTR name);
		bool	PathIsFolder(PCWSTR path);
		inline LPCWSTR PathFindLeaf(STRING path)	{ return ::PathFindFileName(path); }

		enum PathFrom
		{
			None,	// ���΃p�X�̏ꍇ�̓G���[.
			Top,	// AVESTA
			Bin,	// AVESTA/bin
			Man,	// AVESTA/man
			Usr,	// AVESTA/usr
			Var,	// AVESTA/var
		};

		PCWSTR	PathOf(PathFrom from);

		HRESULT	CreatePath(WCHAR dst[MAX_PATH], STRING src, PathFrom from = None);
		HRESULT	CreatePath(ITEMIDLIST** pp, STRING src, PathFrom from = None);
		string	CreatePath(STRING src, PathFrom from = None);

		HRESULT	CreateEntry(IEntry** pp, STRING src, PathFrom from = None);
		HRESULT	CreateEntry(IEntry** pp, LPCITEMIDLIST src, PathFrom from = None);
		ref<IEntry> CreateEntry(LPCITEMIDLIST src, PathFrom from = None);
		ref<IEntry> CreateEntry(STRING src, PathFrom from = None);

	//HICON	LoadIcon(PCWSTR filename, int index, int w, int h);

	//HRESULT	PathFromObject(WCHAR dst[MAX_PATH], Object* src);
	//HRESULT	PathGetLink(ITEMIDLIST** dst, PCWSTR src);
	//HRESULT	PathGetLink(WCHAR dst[MAX_PATH], PCWSTR src);

		//==============================================================================
		// INI�t�@�C��.

		string	IniGetString(PCTSTR filename, PCTSTR group, PCTSTR key, PCTSTR defaultValue);
		bool	IniGetBool  (PCTSTR filename, PCTSTR group, PCTSTR key, bool defaultValue);
		inline INT32 IniGetSint32(PCTSTR filename, PCTSTR group, PCTSTR key, UINT32 defaultValue)
		{
			return ::GetPrivateProfileInt(group, key, defaultValue, filename);
		}
	}
}
