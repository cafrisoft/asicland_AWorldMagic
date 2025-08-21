#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(DESIGN_LIB)
#  define DESIGN_EXPORT Q_DECL_EXPORT
# else
#  define DESIGN_EXPORT Q_DECL_IMPORT
# endif
#else
# define DESIGN_EXPORT
#endif
