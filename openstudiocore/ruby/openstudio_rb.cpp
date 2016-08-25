#include "init_openstudio.hpp"
#include <RubyAPI.hpp>
#include <QtPlugin>
#include <iostream>
#include <ruby.h>

#ifdef _MSC_VER
__declspec(noinline) void importPlugins() {
#else
__attribute__((noinline)) void importPlugins() {
#endif
  #ifdef QT_STATIC
    Q_IMPORT_PLUGIN(QSQLiteDriverPlugin);
    //Q_INIT_RESOURCE(openstudio);
  #endif
  
  #if defined(Q_OS_OSX) && defined(QT_STATIC)
    Q_IMPORT_PLUGIN(QCocoaIntegrationPlugin);
  #elif defined(Q_OS_WIN) && defined(QT_STATIC)
    Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin);
  #endif
}

extern "C" {

void Init_openstudiomodeleditor();
void Init_openstudiolib();

RUBY_API void Init_openstudio(void) {
  importPlugins();

  init_openstudio_internal();

  Init_openstudiomodeleditor();
  rb_provide("openstudiomodeleditor");
}

}

