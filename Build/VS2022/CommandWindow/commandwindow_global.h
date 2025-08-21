#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(COMMANDWINDOW_LIB)
#  define COMMANDWINDOW_EXPORT Q_DECL_EXPORT
# else
#  define COMMANDWINDOW_EXPORT Q_DECL_IMPORT
# endif
#else
# define COMMANDWINDOW_EXPORT
#endif
