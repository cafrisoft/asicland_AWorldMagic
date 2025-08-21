#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(CONSTRUCTOR_LIB)
#  define CONSTRUCTOR_EXPORT Q_DECL_EXPORT
# else
#  define CONSTRUCTOR_EXPORT Q_DECL_IMPORT
# endif
#else
# define CONSTRUCTOR_EXPORT
#endif
