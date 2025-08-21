#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(IP_LIB)
#  define IP_EXPORT Q_DECL_EXPORT
# else
#  define IP_EXPORT Q_DECL_IMPORT
# endif
#else
# define IP_EXPORT
#endif
