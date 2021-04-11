// AvestaSDK.hpp

#pragma once

namespace avesta {
/// .
enum AvestaComponent {
  AvestaForm,
  AvestaTab,
  AvestaFolder,
  AvestaStatus,
  AvestaPreview,
};

/// .
enum NotifyPriority {
  NotifyIdle = 0,      //
  NotifyInfo = 10,     // �V�X�e������̏��
  NotifyResult = 20,   // ���[�U�̑���̌���
  NotifyWarning = 30,  // ���[�U�̌둀��
  NotifyError = 40,    // �V�X�e���̓���s��
};

/// .
enum Navigation {
  NaviCancel,      ///< �ړ���������
  NaviGoto,        ///< �ړ��{�L�[
  NaviGotoAlways,  ///< �ړ�
  NaviOpen,        ///< �J���{�L�[
  NaviOpenAlways,  ///< �J��
  NaviAppend,      ///< �ǉ�
  NaviReserve,     ///< �ǉ��{�B��
  NaviSwitch,      ///< �ǉ��{�t�H�[�J�X
  NaviReplace,     ///< �ǉ��{�t�H�[�J�X�{�����B��
};

///.
enum Fonts {
  FontTab,
  FontAddress,
  FontList,
  FontStatus,
  NumOfFonts,
};

/// .
__interface IAvesta {
  /// �R���|�[�l���g�����擾����.
  size_t GetComponentCount(AvestaComponent type);
  /// �R���|�[�l���g���擾����.
  HRESULT GetComponent(REFINTF pp, AvestaComponent type, size_t index = (size_t)-1);
  /// IShellListView ��񋓂���.
  ref<IEnumUnknown> EnumFolders(Status status) const;

  ref<IShellListView> CurrentView() const;
  ref<IEntry> CurrentFolder() const;
  string CurrentPath() const;

  /// �t�H���_�Ƃ��ĊJ���܂�.
  ref<IShellListView> OpenFolder(IEntry * entry, Navigation navi, Navigation* naviResult = null);
  /// �t�H���_�Ƃ��ĊJ�����A���s���܂�.
  HRESULT OpenOrExecute(IEntry * entry);

  /// �ʒm�����[�U�̎ז��ɂȂ�Ȃ��悤�ɕ\������.
  void Notify(DWORD priority, string msg);
  ///
  void ParseCommandLine(PCWSTR args);
  ///
  void Restart(PCWSTR newDLL = null);
};

/// �֘A�t�����s���܂�.
HRESULT AvestaExecute(IEntry* entry);
}  // namespace avesta

namespace ave {
MEW_API void GetDriveLetter(PCWSTR path, PWSTR buffer);
MEW_API UINT64 GetTotalBytes(IShellListView* view);
MEW_API UINT64 GetSelectedBytes(IShellListView* view);

inline string ResolvePath(PCWSTR path) {
  const int CSIDL_AVESTA = 0xFFFFFFFF;
  return io::PathResolvePath(path, L"AVESTA", CSIDL_AVESTA);
}
inline string ResolvePath(const string& path) { return ResolvePath(path.str()); }
}  // namespace ave

//==============================================================================

void NewFolder(IShellListView* view);

void DlgNew(IShellListView* view);
void DlgSelect(IShellListView* view);
void DlgPattern(IShellListView* view);
void DlgRename(IShellListView* view, bool paste);

// void DlgOpen(IShellListView* view);
