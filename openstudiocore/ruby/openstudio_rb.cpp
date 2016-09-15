#include "init_openstudio.hpp"
#include <RubyAPI.hpp>
#include <QtPlugin>
#include <iostream>
#include <ruby.h>

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


inline void initResources() { 

  #ifdef QT_STATIC
    Q_INIT_RESOURCE(modeleditorlib); 
  #endif // QT_STATIC

}

extern "C" {

void Init_openstudiomodeleditor();

RUBY_API void Init_openstudio(void) {

  initResources();

  init_openstudio_internal();

  Init_openstudiomodeleditor();
  rb_provide("openstudiomodeleditor");
}

}

