/// @file shell.hpp
/// �V�F���ƃt�@�C���V�X�e��.
#pragma once

#include <shellapi.h>
#include <shlobj.h>

#include "mew.hpp"
#include "path.hpp"
#include "widgets.hpp"

namespace mew {
/// �V�F���A�t�@�C���V�X�e���A�v���Z�X�ԒʐM.
namespace io {
//==============================================================================
// �C���^�t�F�[�X��`.

/// �V�F���G���g���z��(CIDA).
__interface IEntryList : IUnknown {
#ifndef DOXYGEN
  size_t get_Count();
  LPCITEMIDLIST get_Parent();
  LPCITEMIDLIST get_Leaf(size_t index);
#endif  // DOXYGEN

  __declspec(property(get = get_Count)) size_t Count;
  __declspec(property(get = get_Parent)) LPCITEMIDLIST Parent;
  __declspec(property(get = get_Leaf)) LPCITEMIDLIST Leaf[];

  const CIDA* GetCIDA();
  HRESULT GetAt(IEntry * *ppShellItem, size_t index);

  /// ���̃G���g���z��̂����A�w��̃G���g�������𒊏o����.
  HRESULT CloneSubset(REFINTF pp,        ///< ���o���ꂽ�z��.
                      size_t subsets[],  ///< ���o����G���g���C���f�b�N�X�z��.
                      size_t length      ///< subsets�̒���.
  );
};

/// �V�F���t�H���_���j���[.
__interface IFolder : ui::ITreeItem {
#ifndef DOXYGEN
  IEntry* get_Entry();
  bool get_IncludeFiles();
  void set_IncludeFiles(bool value);
  int get_Depth();
  void set_Depth(int depth);
#endif
  __declspec(property(get = get_Entry)) IEntry* Entry;                                     ///< �G���g��.
  __declspec(property(get = get_IncludeFiles, put = set_IncludeFiles)) bool IncludeFiles;  ///< �t�@�C�����܂ނ��ۂ�.
  __declspec(property(get = get_Depth, put = set_Depth)) int Depth;                        ///< �W�J����[��.

  void Reset();
};

/// �h���b�v�G�t�F�N�g.
enum DropEffect {
  DropEffectNone = 0x00000000,
  DropEffectCopy = 0x00000001,
  DropEffectMove = 0x00000002,
  DropEffectLink = 0x00000004,
  // DropEffectScroll= 0x80000000
};

/// �h���b�O���h���b�v�\�[�X.
__interface IDragSource : IDataObject {
  DropEffect DoDragDrop(DWORD dwSupportsEffect);
  DropEffect DoDragDrop(DWORD dwSupportsEffect, HWND hWndDrag, POINT ptCursor);
  DropEffect DoDragDrop(DWORD dwSupportsEffect, HBITMAP hBitmap, COLORREF colorkey, POINT ptHotspot);
  /// �f�[�^��ǉ�����.
  HRESULT AddData(CLIPFORMAT cfFormat, const void* data, size_t size);
  /// �O���[�o���n���h����ǉ�����.
  HRESULT AddGlobalData(CLIPFORMAT cfFormat, HGLOBAL hGlobal);
  /// �ЂƂ�IDList��ǉ�����.
  HRESULT AddIDList(LPCITEMIDLIST pIDList);
  /// IDList�z���ǉ�����.
  HRESULT AddIDList(const CIDA* pCIDA);
  /// �e�L�X�g��ǉ�����.
  HRESULT AddText(string text);
  /// URL��ǉ�����.
  HRESULT AddURL(string url);
};

//======================================================================
// Version

class Version {
 private:
  string m_Path;
  BYTE* m_VersionInfo;
  WORD m_Language;
  WORD m_CodePage;

 public:
  Version();
  ~Version();
  string GetPath() const;
  bool Open(string filename);
  void Close();
  PCWSTR QueryValue(PCWSTR what);
};
}  // namespace io
}  // namespace mew
