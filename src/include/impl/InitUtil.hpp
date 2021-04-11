// InitUtil.hpp
#pragma once

#include "io.hpp"

namespace mew {
namespace util {
#ifdef _GDIPLUS_H
class GdiInit {
 private:
  ULONG_PTR m_Token;
  Gdiplus::GdiplusStartupInput m_StartupInput;

 public:
  // �R���X�g���N�^.
  GdiInit() throw() { Gdiplus::GdiplusStartup(&m_Token, &m_StartupInput, null); }
  ~GdiInit() throw() { Gdiplus::GdiplusShutdown(m_Token); }
};

#endif

inline void InitModule() throw() {
#ifdef _DEBUG
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
#endif
  io::Path path;
  ::GetModuleFileName(null, path, MAX_PATH);
  path.RemoveLeaf();
  ::SetCurrentDirectory(path);
}
}  // namespace util
}  // namespace mew