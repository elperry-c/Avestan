/// @file application.hpp
/// �A�v���P�[�V�����t���[�����[�N.
#pragma once

#include "message.hpp"

namespace mew {
/// �ėp�X�e�[�^�X.
enum Status {
  StatusNone = 0x00000000,  ///< .

  FOCUSED = 0x00000001,   ///< �t�H�[�J�X.
  SELECTED = 0x00000002,  ///< �I��.
  CHECKED = 0x00000004,   ///< �`�F�b�N.
  ENABLED = 0x00000008,   ///< �L����.
  EXPANDED = 0x00000010,  ///< �W�J.
  HOT = 0x00000020,       ///< �z�b�g�g���b�N.

  UNFOCUSED = 0x00010000,   ///< .
  UNSELECTED = 0x00020000,  ///< .
  UNCHECKED = 0x00040000,   ///< .
  DISABLED = 0x00080000,    ///< .
  COLLAPSED = 0x00100000,   ///< .
  UNHOT = 0x00200000,       ///< .

  ToggleFocus = 0x00010001,   ///< .
  ToggleSelect = 0x00020002,  ///< .
  ToggleCheck = 0x00040004,   ///< .
  ToggleEnable = 0x00080008,  ///< .
  ToggleExpand = 0x00100010,  ///< .
  ToggleHot = 0x00200020,     ///< .
};

__interface __declspec(uuid("2EFEAB15-24BF-498B-A1F7-6E85087026C0")) ICommand;
__interface __declspec(uuid("E2B2FC4C-561B-4A38-96EB-B484E47237D8")) ICommands;

/// �R�}���h�e�[�u��.
class __declspec(uuid("E35429F9-AA06-42C3-BDAF-9361FF54501E")) Commands;

/// �R�}���h.
__interface ICommand : IUnknown {
#ifndef DOXYGEN
  string get_Description();
#endif  // DOXYGEN

  /// �R�}���h�����s����.
  void Invoke();
  /// �R�}���h�̐��� [get].
  __declspec(property(get = get_Description)) string Description;

  /// �X�e�[�^�X���擾����.
  UINT32 QueryState(IUnknown* owner = null);
};

/// �R�}���h�e�[�u��.
__interface ICommands : IUnknown {
  HRESULT Add(string name, ICommand * command);
  HRESULT Add(string name, string description);
  HRESULT Alias(string alias, string name);
  HRESULT Find(REFINTF ppInterface, string name);
  HRESULT Remove(string name);
  HRESULT SetHandler(string name, function fn, message msg = null);
  HRESULT SetObserver(string name, function fn, message msg = null);
};

/// �R�}���h���b�Z�[�W�R�[�h.
enum CommandMessage {
  EventInvoke = 'CMIV',      ///< �R�}���h�����s���ꂽ.
  EventQueryState = 'CMST',  ///< �R�}���h�̃X�e�[�^�X���₢���킹��ꂽ.
};
}  // namespace mew
