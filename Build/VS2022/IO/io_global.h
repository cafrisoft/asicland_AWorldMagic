#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(IO_LIB)
#  define IO_EXPORT Q_DECL_EXPORT
# else
#  define IO_EXPORT Q_DECL_IMPORT
# endif
#else
# define IO_EXPORT
#endif
