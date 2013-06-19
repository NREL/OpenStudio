/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <rulesengine/test/RulesEngineFixture.hpp>

#include <runmanager/lib/Workflow.hpp>
#include <runmanager/lib/Job.hpp>
#include <runmanager/lib/RunManager.hpp>

#include <energyplus/ReverseTranslator.hpp>

#include <model/Model.hpp>
#include <model/Model_Impl.hpp>
#include <model/WeatherFile.hpp>

#include <utilities/filetypes/EpwFile.hpp>
#include <utilities/core/Path.hpp>
#include <utilities/sql/SqlFile.hpp>

#include <runmanager/Test/ToolBin.hxx>
#include <resources.hxx>

using namespace openstudio;
using namespace openstudio::model;

// static variables
boost::optional<openstudio::FileLogSink> RulesEngineFixture::logFile;
openstudio::path RulesEngineFixture::runDir;

void RulesEngineFixture::SetUp() {}

void RulesEngineFixture::TearDown() {}

void RulesEngineFixture::SetUpTestCase() {
  // set up logging
  logFile = FileLogSink(toPath("./RulesEngineFixture.log"));
  logFile->setLogLevel(Debug);
  openstudio::Logger::instance().standardOutLogger().disable();
}

void RulesEngineFixture::TearDownTestCase() {
  logFile->disable();
  exampleModel(false,true); // should call SqlFile destructor
  boost::filesystem::remove_all(runDir);
}

openstudio::model::Model RulesEngineFixture::exampleModel(bool withResults,bool clearModels) {
  static Model model, modelWithResults;

  if (clearModels) {
    model = Model();
    modelWithResults = Model();
  }

  if (model.numObjects() == 0) {
    // set up basic model
    model = model::exampleModel();
  }
  Model result = model;

  if (withResults) {
    if (modelWithResults.numObjects() == 0u) {
      // set weather file
      openstudio::path weatherFilePath = energyPlusWeatherDataPath() / toPath("USA_CO_Golden-NREL.724666_TMY3.epw");
      EpwFile epwFile(weatherFilePath);
      WeatherFile::setWeatherFile(model,epwFile);

      // set up run directory
      modelWithResults = model.clone().cast<Model>();
      runDir = resourcesPath() / toPath("rulesengine/ExampleModel_" + toString(createUUID()));
      boost::filesystem::create_directory(runDir);
      openstudio::path modelPath = runDir / toPath("example.osm");
      modelWithResults.save(modelPath,true);

      // run model
      runmanager::Workflow workflow;
      workflow.addJob(openstudio::runmanager::JobType::ModelToIdf);
      workflow.addJob(openstudio::runmanager::JobType::EnergyPlus);
      openstudio::path nullPath;
      workflow.add(openstudio::runmanager::ConfigOptions::makeTools(
          energyPlusExePath().parent_path(),nullPath,nullPath,nullPath,nullPath));
      runmanager::Job job = workflow.create(runDir,modelPath,weatherFilePath);
      openstudio::path runDbPath = runDir / toPath("run.db");
      if (boost::filesystem::exists(runDbPath)) {
        boost::filesystem::remove(runDbPath);
      }
      runmanager::RunManager runManager(runDbPath,false,false,false);
      runManager.enqueue(job,false);
      runManager.waitForFinished();

      // attach sql file
      openstudio::path sqlPath = job.treeAllFiles().getLastByExtension("sql").fullPath;
      SqlFile sqlFile(sqlPath);
      EXPECT_TRUE(modelWithResults.setSqlFile(sqlFile));
    }
    result = modelWithResults;
  }

  return result;
}

