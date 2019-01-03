#include "openstudio_csharp.hpp"

#include <QtPlugin>

#if defined(__APPLE__)
  Q_IMPORT_PLUGIN(QCocoaIntegrationPlugin);
#elif (defined (_WIN32) || defined (_WIN64))
  Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin);
#elif defined(__linux__)
  Q_IMPORT_PLUGIN(QXcbIntegrationPlugin);
#endif

extern "C" {


void Init_openstudio_csharp(void) {

}

}

