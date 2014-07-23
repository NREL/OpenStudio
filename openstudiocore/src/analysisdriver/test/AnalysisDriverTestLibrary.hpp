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

#ifndef ANALYSISDRIVER_TEST_ANALYSISDRIVERTESTLIBRARY_HPP
#define ANALYSISDRIVER_TEST_ANALYSISDRIVERTESTLIBRARY_HPP

#include "../../utilities/core/Enum.hpp"
#include "../../utilities/core/Path.hpp"
#include "../../utilities/core/Singleton.hpp"

namespace openstudio {
namespace analysisdriver {

class SimpleProject;

OPENSTUDIO_ENUM(LibraryProblem,
  ((Default))
);

class AnalysisDriverTestLibrarySingleton {
  friend class Singleton<AnalysisDriverTestLibrarySingleton>;
 public:
 
  void initialize(const openstudio::path& outputDataDirectory,
                  const openstudio::path& baselineModelDirectory);
                  
  openstudio::path outputDataDirectory() const;                  
                  
  std::vector<std::string> baselineModelNames() const;
                  
  SimpleProject createProject(const std::string& projectName,
                              bool isPAT,
                              const LibraryProblem& libraryProblem,
                              const std::string& baselineModelName);

  SimpleProject openProject(const std::string& projectName,
                            bool requirePAT);
                            
 private:
  openstudio::path m_outputDataDirectory;
  std::vector<openstudio::path> m_baselineModels;  

  static bool pathStemEquals(const openstudio::path& p, const std::string& str);

  void setProblem(SimpleProject& project,
                  const LibraryProblem& libraryProblem);  
};

typedef openstudio::Singleton<AnalysisDriverTestLibrarySingleton> AnalysisDriverTestLibrary;

} // analysisdriver
} // openstudio

#endif // ANALYSISDRIVER_TEST_ANALYSISDRIVERTESTLIBRARY_HPP
