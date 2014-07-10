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

#ifndef ANALYSISDRIVER_TEST_ANALYSISDRIVERFIXTURE_HPP
#define ANALYSISDRIVER_TEST_ANALYSISDRIVERFIXTURE_HPP

#include <gtest/gtest.h>

#include "../../utilities/core/Path.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/FileLogSink.hpp"
#include "../../utilities/core/Enum.hpp"

#include "../../ruleset/RubyUserScriptArgumentGetter.hpp"

namespace openstudio {
namespace runmanager {
  class Workflow;
}
namespace analysis {
  class Problem;
}
namespace project {
  class ProjectDatabase;
}
namespace analysisdriver {
  class SimpleProject;
  class AnalysisRunOptions;
}
}

OPENSTUDIO_ENUM(AnalysisDriverFixtureProblem,
                ((MixedOsmIdf))
                ((IdfOnly))
                ((Continuous))
                ((UserScriptContinuous))
                ((SimpleUQ))
                ((MixedContinuousAndDiscreteUQ))
                ((SimpleLognormalUQ))
                ((SimpleHistogramBinUQ))
                ((BuggyBCLMeasure)));

class AnalysisDriverFixture : public ::testing::Test {
 protected:

  virtual ~AnalysisDriverFixture() {}

  // TEST FIXTURE METHODS

  /// initialize for each test
  virtual void SetUp();

  /// tear down after each test
  virtual void TearDown();

  /// initialize static members
  static void SetUpTestCase();

  /// tear down static members
  static void TearDownTestCase();

  /// static data
  static boost::optional<openstudio::FileLogSink> logFile;
  static std::shared_ptr<openstudio::ruleset::RubyUserScriptArgumentGetter> argumentGetter;

  /** Return Problem saved to resources database by SetUpTestCase. Creates appropriate
   *  simulation workflow based on modelToIdf and energyPlus. */
  static openstudio::analysis::Problem retrieveProblem(const AnalysisDriverFixtureProblem& name,
                                                       bool modelToIdf,
                                                       bool energyPlus);

  static openstudio::analysis::Problem retrieveProblem(const std::string& name,
                                                       bool modelToIdf,
                                                       bool energyPlus);

  /** Returns a clean database for testing. */
  static openstudio::project::ProjectDatabase getCleanDatabase(
      const std::string& projectDatabaseName);

  /** Returns an already existing database for testing. */
  static openstudio::project::ProjectDatabase getDatabase(
      const std::string& projectDatabaseName);

  /** Returns a clean SimpleProject for testing. */
  static openstudio::analysisdriver::SimpleProject getCleanSimpleProject(
      const std::string& projectName);

  /** Returns a clean PAT SimpleProject for testing. */
  static openstudio::analysisdriver::SimpleProject getCleanPATProject(
      const std::string& projectName);

  /** Returns an already existing SimpleProject for testing. */
  static openstudio::analysisdriver::SimpleProject getSimpleProject(
      const std::string& projectName);

  /** Returns an already existing PAT SimpleProject for testing. */
  static openstudio::analysisdriver::SimpleProject getPATProject(
      const std::string& projectName);

  static openstudio::analysisdriver::AnalysisRunOptions standardRunOptions(
      const openstudio::path& workingDirectory);

  /// set up logging
  REGISTER_LOGGER("AnalysisDriverFixture");

 private:

  static openstudio::analysis::Problem createMixedOsmIdfProblem();

  static openstudio::analysis::Problem createIdfOnlyProblem();

  static openstudio::analysis::Problem createContinuousProblem();

  static openstudio::analysis::Problem createUserScriptContinuousProblem();

  static openstudio::analysis::Problem createSimpleUQProblem();

  static openstudio::analysis::Problem createMixedContinuousAndDiscreteUQProblem();

  static openstudio::analysis::Problem createSimpleLognormalUQProblem();

  static openstudio::analysis::Problem createSimpleHistogramBinUQProblem();

  static openstudio::analysis::Problem createBuggyBCLMeasureProblem();

  static void appendDefaultSimulationWorkflow(openstudio::analysis::Problem& problem,
                                              bool modelToIdf,
                                              bool energyPlus);
};

#endif // ANALYSISDRIVER_TEST_ANALYSISDRIVERFIXTURE_HPP
