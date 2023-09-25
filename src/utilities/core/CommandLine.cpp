/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "CommandLine.hpp"

namespace openstudio {

// platform specific

#if (defined(_WIN32) || defined(_WIN64))

#  include <Windows.h>  // for SetConsoleOutputCP

/** Manage the console. */
class ConsoleInitializer
{
 public:
  /** Initialize the console. */
  ConsoleInitializer();
  /** Restore the console. */
  virtual ~ConsoleInitializer();

 private:
  unsigned m_inputCP;
  unsigned m_outputCP;
};

/** Initialize the console. */
ConsoleInitializer::ConsoleInitializer() : m_inputCP(GetConsoleCP()), m_outputCP(GetConsoleOutputCP()) {
  // http://msdn.microsoft.com/en-us/library/ms686036%28VS.85%29.aspx
  //bool b1 = SetConsoleCP(65001); // set to UTF-8
  //bool b2 = SetConsoleOutputCP(65001); // set to UTF-8
  //bool b1 = SetConsoleCP(1200); // set to UTF-16LE
  //bool b2 = SetConsoleOutputCP(1200); // set to UTF-16LE
  //unsigned inputCP = GetConsoleCP();
  //unsigned outputCP = GetConsoleOutputCP();
}

/** Restore the console. */
ConsoleInitializer::~ConsoleInitializer() {
  SetConsoleCP(m_inputCP);
  SetConsoleOutputCP(m_outputCP);
}

// Global ConsoleInitializer object
static ConsoleInitializer GlobalConsoleOutputInitializer;

#endif

}  // namespace openstudio
