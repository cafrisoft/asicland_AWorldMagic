#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(COMMON01_LIB)
#  define COMMON01_EXPORT Q_DECL_EXPORT
# else
#  define COMMON01_EXPORT Q_DECL_IMPORT
# endif
#else
# define COMMON01_EXPORT
#endif
