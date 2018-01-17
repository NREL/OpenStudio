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
  virtual void SetUp() override;

  /// tear down after each test
  virtual void TearDown() override;

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

  static openstudio::model::Model fastExampleModel();

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
