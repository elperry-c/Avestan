/// @file error.hpp
/// ��O.
#pragma once

#include "mew.hpp"
#include "string.hpp"

namespace mew {
/// ��O.
namespace exceptions {
/// ��O���N���X.
class Error {
 public:
  virtual ~Error() {}

#ifndef DOXYGEN
  virtual string get_Message() throw() = 0;
  virtual HRESULT get_Code() throw() = 0;
#endif  // DOXYGEN

  /// ��O���b�Z�[�W [get].
  __declspec(property(get = get_Message)) string Message;
  /// ��O�R�[�h [get].
  __declspec(property(get = get_Code)) HRESULT Code;
};

/// �V�X�e���G���[.
class SystemError : public Error {
 protected:
  string m_Message;
  HRESULT m_Code;

 public:
  SystemError(const string& msg, HRESULT code) : m_Message(msg), m_Code(code) {}

#ifndef DOXYGEN
  virtual string get_Message() throw() { return m_Message; }
  virtual HRESULT get_Code() throw() { return m_Code; }
#endif  // DOXYGEN
};

/// ���W�b�N�G���[.
class LogicError : public SystemError {
 public:
  LogicError(const string& msg, HRESULT code) : SystemError(msg, code) {}
};

/// �����^�C���G���[.
class RuntimeError : public SystemError {
 public:
  RuntimeError(const string& msg, HRESULT code) : SystemError(msg, code) {}
};

/// �s���ȃ��\�b�h�Ăяo��.
class ArgumentError : public LogicError {
 public:
  ArgumentError(const string& msg, HRESULT code = E_INVALIDARG) : LogicError(msg, code) {}
};

/// �L���X�g�G���[.
class CastError : public RuntimeError {
 public:
  CastError(const string& msg, HRESULT code = E_NOINTERFACE) : RuntimeError(msg, code) {}
};

/// �C���X�^���X�쐬�G���[.
class ClassError : public RuntimeError {
 public:
  ClassError(const string& msg, HRESULT code) : RuntimeError(msg, code) {}
};

/// IO�G���[.
class IOError : public RuntimeError {
 public:
  IOError(const string& msg, HRESULT code) : RuntimeError(msg, code) {}
};
}  // namespace exceptions
}  // namespace mew
