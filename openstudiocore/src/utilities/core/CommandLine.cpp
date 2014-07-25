/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
*  All rights reserved.
*  
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*  
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*  
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#include "CommandLine.hpp"

namespace openstudio{

// platform specific

#ifdef _WINDOWS

  #include <Windows.h> // for SetConsoleOutputCP

  /** Manage the console. */
  class ConsoleInitializer{
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
  ConsoleInitializer::ConsoleInitializer()
    : m_inputCP(GetConsoleCP()), m_outputCP(GetConsoleOutputCP())
  {
    // http://msdn.microsoft.com/en-us/library/ms686036%28VS.85%29.aspx
    //bool b1 = SetConsoleCP(65001); // set to UTF-8
    //bool b2 = SetConsoleOutputCP(65001); // set to UTF-8
    //bool b1 = SetConsoleCP(1200); // set to UTF-16LE
    //bool b2 = SetConsoleOutputCP(1200); // set to UTF-16LE
    //unsigned inputCP = GetConsoleCP();
    //unsigned outputCP = GetConsoleOutputCP();
  }

  /** Restore the console. */
  ConsoleInitializer::~ConsoleInitializer()
  {
    SetConsoleCP(m_inputCP);
    SetConsoleOutputCP(m_outputCP);
  }

  // Global ConsoleInitializer object
  static ConsoleInitializer GlobalConsoleOutputInitializer;

#endif

} // openstudio
