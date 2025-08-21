#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(AWORLDMAGICLIB_LIB)
#  define AWORLDMAGICLIB_EXPORT Q_DECL_EXPORT
# else
#  define AWORLDMAGICLIB_EXPORT Q_DECL_IMPORT
# endif
#else
# define AWORLDMAGICLIB_EXPORT
#endif
