#pragma once
#include <QString>

// 网页版视觉 token 的 Qt 对应
namespace Style {
inline const char* const Paper   = "#f4f1ea";
inline const char* const Surface = "#fbfaf6";
inline const char* const Ink     = "#1c1a16";
inline const char* const InkSoft = "#55503f";
inline const char* const InkFaint= "#8b8570";
inline const char* const Line    = "#ddd6c6";
inline const char* const LineStrong = "#c9c1ac";
inline const char* const Accent  = "#c2571a";
inline const char* const AccentDeep = "#9a430f";
inline const char* const AccentSoft = "#f6e3d2";
inline const char* const Wave    = "#2b6a5e";
inline const char* const Ok      = "#3a7d44";
inline const char* const Err     = "#b03a2e";

QString appStyleSheet();
}
