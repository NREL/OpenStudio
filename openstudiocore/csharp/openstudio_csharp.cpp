#include "openstudio_csharp.hpp"

#include <QtPlugin>

// this uses the static initialization trick to import plugins
#ifdef QT_STATIC
  Q_IMPORT_PLUGIN(QSQLiteDriverPlugin);

  #if defined(Q_OS_OSX)
    Q_IMPORT_PLUGIN(QCocoaIntegrationPlugin);
  #elif defined(Q_OS_WIN)
    Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin);
  #elif defined(Q_OS_LINUX)
    Q_IMPORT_PLUGIN(QXcbIntegrationPlugin);  
  #endif

#endif // QT_STATIC

extern "C" {


void Init_openstudio_csharp(void) {
  
}

}

