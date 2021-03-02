/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
