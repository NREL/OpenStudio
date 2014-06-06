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
