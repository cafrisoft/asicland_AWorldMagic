#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(PYTHON_LIB)
#  define PYTHON_EXPORT Q_DECL_EXPORT
# else
#  define PYTHON_EXPORT Q_DECL_IMPORT
# endif
#else
# define PYTHON_EXPORT
#endif
