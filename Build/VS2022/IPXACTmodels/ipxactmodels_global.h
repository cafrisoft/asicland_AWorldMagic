#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(IPXACTMODELS_LIB)
#  define IPXACTMODELS_EXPORT Q_DECL_EXPORT
# else
#  define IPXACTMODELS_EXPORT Q_DECL_IMPORT
# endif
#else
# define IPXACTMODELS_EXPORT
#endif
