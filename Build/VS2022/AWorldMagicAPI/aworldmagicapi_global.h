#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(AWORLDMAGICAPI_LIB)
#  define AWORLDMAGICAPI_EXPORT Q_DECL_EXPORT
# else
#  define AWORLDMAGICAPI_EXPORT Q_DECL_IMPORT
# endif
#else
# define AWORLDMAGICAPI_EXPORT
#endif
