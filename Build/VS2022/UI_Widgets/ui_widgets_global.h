#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(UI_WIDGETS_LIB)
#  define UI_WIDGETS_EXPORT Q_DECL_EXPORT
# else
#  define UI_WIDGETS_EXPORT Q_DECL_IMPORT
# endif
#else
# define UI_WIDGETS_EXPORT
#endif
