/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "AnalysisDriverTestLibrary.hpp"

#include "../SimpleProject.hpp"

#include "../../model/Model.hpp"
#include "../../model/WeatherFile.hpp"

#include "../../utilities/bcl/BCLMeasure.hpp"
#include "../../utilities/core/Assert.hpp"
#include "../../utilities/core/Containers.hpp"
#include "../../utilities/core/FileReference.hpp"
#include "../../utilities/filetypes/EpwFile.hpp"

#include <boost/filesystem.hpp>

#include <QDir>
#include <QFile>
#include <QFileInfo>

#include <runmanager/Test/ToolBin.hxx>

using namespace openstudio::model;

namespace openstudio {
namespace analysisdriver {

void AnalysisDriverTestLibrarySingleton::initialize(const openstudio::path& outputDataDirectory,
                                                    const openstudio::path& baselineModelDirectory)
{
  // create outputDataDirectory and save path as member data
  if (!boost::filesystem::exists(outputDataDirectory)) {
    boost::filesystem::create_directory(outputDataDirectory);
  }
  m_outputDataDirectory = outputDataDirectory;

  // create example model and put in outputDataDirectory
  Model model = exampleModel();
  openstudio::path exampleModelDir = m_outputDataDirectory / toPath("ExampleModel");
  openstudio::path exampleWeatherFileDir = exampleModelDir / toPath("example/files");
  openstudio::path weatherFilePath = exampleWeatherFileDir / toPath("USA_CO_Golden-NREL.724666_TMY3.epw");
  if (!boost::filesystem::exists(exampleModelDir)) {
    boost::filesystem::create_directory(exampleModelDir);
    boost::filesystem::create_directory(exampleWeatherFileDir.parent_path());
    boost::filesystem::create_directory(exampleWeatherFileDir);
    openstudio::path originalWeatherFilePath = energyPlusWeatherDataPath() / toPath("USA_CO_Golden-NREL.724666_TMY3.epw");
    boost::filesystem::copy_file(originalWeatherFilePath,weatherFilePath);
  }
  EpwFile epwFile(weatherFilePath);
  WeatherFile::setWeatherFile(model,epwFile);
  openstudio::path exampleModelPath = exampleModelDir / toPath("example.osm");
  model.save(exampleModelPath,true);
  m_baselineModels.push_back(exampleModelPath);

  // loop through files in baselineModelDirectory (non-recursive) and save .osm paths
  QDir srcDir(toQString(baselineModelDirectory));

  for (const QFileInfo& info : srcDir.entryInfoList(QDir::Files)) {
    if ((info.suffix() == "osm") || (info.suffix() == "idf")) {
      m_baselineModels.push_back(baselineModelDirectory / toPath(info.fileName()));
    }
  }

}
                
openstudio::path AnalysisDriverTestLibrarySingleton::outputDataDirectory() const {
  return m_outputDataDirectory;
}
                
std::vector<std::string> AnalysisDriverTestLibrarySingleton::baselineModelNames() const {
  StringVector modelNames;
  for (const openstudio::path& modelPath : m_baselineModels) {
    modelNames.push_back(toString(modelPath.stem()));
  }
  return modelNames;
}
                
SimpleProject AnalysisDriverTestLibrarySingleton::createProject(
    const std::string& projectName,
    bool isPAT,
    const LibraryProblem& libraryProblem,
    const std::string& baselineModelName)
{
  openstudio::path projectDir = outputDataDirectory() / toPath(projectName);
  if (boost::filesystem::exists(projectDir)) {
    boost::filesystem::remove_all(projectDir);
  }

  OptionalSimpleProject result;
  SimpleProjectOptions options;
  options.setLogLevel(Debug);
  if (isPAT) {
    result = createPATProject(projectDir,options);
  }
  else {
    result = SimpleProject::create(projectDir,options);
  }
  OS_ASSERT(result);

  setProblem(*result,libraryProblem);
  
  std::vector<openstudio::path>::const_iterator it;
  it = std::find_if(m_baselineModels.begin(),
                    m_baselineModels.end(),
                    std::bind(AnalysisDriverTestLibrarySingleton::pathStemEquals,std::placeholders::_1,baselineModelName));
  OS_ASSERT(it != m_baselineModels.end());
  bool test = result->setSeed(FileReference(*it)).first;
  OS_ASSERT(test);

  return *result;  
}

SimpleProject AnalysisDriverTestLibrarySingleton::openProject(
    const std::string& projectName,
    bool requirePAT)
{
  openstudio::path projectDir = outputDataDirectory() / toPath(projectName);

  OptionalSimpleProject result;
  SimpleProjectOptions options;
  options.setLogLevel(Debug);
  if (requirePAT) {
    result = openPATProject(projectDir,options);
  }
  else {
    result = SimpleProject::open(projectDir,options);
  }
  OS_ASSERT(result);

  return *result;
}

bool AnalysisDriverTestLibrarySingleton::pathStemEquals(const openstudio::path& p, const std::string& str) {
  return (toString(p.stem()) == str);
}

void AnalysisDriverTestLibrarySingleton::setProblem(
    SimpleProject& project,
    const LibraryProblem& libraryProblem)
{
  if (libraryProblem == LibraryProblem::Default) {
    return;
  }
  OS_ASSERT(false);
}

} // analysisdriver
} // openstudio
