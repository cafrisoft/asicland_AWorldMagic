#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(PLUGINS_LIB)
#  define PLUGINS_EXPORT Q_DECL_EXPORT
# else
#  define PLUGINS_EXPORT Q_DECL_IMPORT
# endif
#else
# define PLUGINS_EXPORT
#endif
