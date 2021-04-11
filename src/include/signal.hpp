// signal.hpp
#pragma once

#include "message.hpp"

namespace mew {
//==============================================================================
// �C���^�t�F�[�X

__interface __declspec(uuid("91E42850-31CF-4B5E-A731-E24D46618E68")) ISignal;
__interface __declspec(uuid("55A305E9-3FAC-4DC1-B992-F25B33D699DE")) IMessenger;

//==============================================================================
// �쐬�\�ȃN���X

/// ISignal�̋@�\�����ׂĎ��������w���p.
/// �W��ɂ��ISignal�̎����ɗ��p�ł���.
class __declspec(uuid("9E76A64B-99FA-4F4C-A4BC-ED8EC3CFC31E")) Messenger;

/// ���b�Z�[�W�\�[�X.
__interface ISignal : IDisposable {
  /// �V���N��o�^����.
  /// �V���N�ƃ��b�Z�[�W�R�[�h�����ɓ������V���N���o�^�ς݂̏ꍇ�́A�֘A�t����ꂽ���b�Z�[�W���㏑�������.
  /// @retval S_OK         �o�^����.
  /// @retval E_INVALIDARG handler���s���A�܂���code���T�|�[�g���Ă��Ȃ�.
  HRESULT Connect(EventCode code,  ///< �o�^���郁�b�Z�[�W�R�[�h. �T�|�[�g���Ă��Ȃ����b�Z�[�W�R�[�h�̏ꍇ�͌Ăяo�������s����.
                  function fn,     ///< �o�^����V���N. null�̏ꍇ�͌Ăяo�������s����.
                  message msg = null  ///< ���M����郁�b�Z�[�W.
                  ) throw();
  /// �V���N��o�^��������.
  /// @result �폜���ꂽ�V���N�̌�.
  size_t Disconnect(EventCode code,  ///< �폜���郁�b�Z�[�W�R�[�h. 0�̏ꍇ�͑S�ẴR�[�h���폜�����.
                    function fn,     ///< �폜����V���N. null�̏ꍇ�͑S�Ă̊֐����폜�����.
                    IUnknown* obj = null  ///< �폜����V���N�I�u�W�F�N�g. fn��null�ȊO�̏ꍇ�͖��������.
                    ) throw();
};

/// ���b�Z���W���[.
__interface IMessenger : ISignal {
  /// ���b�Z�[�W�̔z�M�Ɏg�p�����֐��I�u�W�F�N�g��Ԃ�.
  function Invoke(EventCode code) throw();
};

///
template <int code>
struct Event {
  template <class T>
  static void event(message& msg, T* from) {
    msg["from"] = from;
  }
};

/// ���b�Z�[�W�\�[�X�����̂��߂̃��b�p.
template <class TBase>
class __declspec(novtable) SignalImpl : public TBase {
 protected:
  ref<IMessenger> m_msgr;

 protected:  // invoke
#define MEW_PP_MSGR_FMT(n)                      \
  template <int code PP_TYPENAMES_CAT(n)>       \
  message InvokeEvent(PP_ARGS_CONST(n)) const { \
    if (!m_msgr) return null;                   \
    function fn = m_msgr->Invoke(code);         \
    if (!fn) return null;                       \
    message msg;                                \
    Event<code>::event(msg, PP_ARG_VALUES(n));  \
    fn(msg);                                    \
    return msg;                                 \
  }

  PP_REPEAT(10, MEW_PP_MSGR_FMT)

#undef MEW_PP_MSGR_FMT

 public:
  HRESULT Connect(EventCode code, function fn, message msg = null) throw() {
    return !m_msgr ? E_UNEXPECTED : m_msgr->Connect(code, fn, msg);
  }
  size_t Disconnect(EventCode code, function fn, IUnknown* obj = null) throw() {
    return !m_msgr ? 0 : m_msgr->Disconnect(code, fn, obj);
  }
};
}  // namespace mew
