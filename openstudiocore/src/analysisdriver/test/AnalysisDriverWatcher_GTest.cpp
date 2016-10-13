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

#include <gtest/gtest.h>
#include "AnalysisDriverFixture.hpp"

#include "../SimpleProject.hpp"
#include "../AnalysisDriver.hpp"
#include "../CurrentAnalysis.hpp"
#include "../AnalysisRunOptions.hpp"
#include "../AnalysisDriverWatcher.hpp"

#include "../../project/ProjectDatabase.hpp"

#include "../../analysis/Problem.hpp"
#include "../../analysis/DDACEAlgorithm.hpp"
#include "../../analysis/DDACEAlgorithmOptions.hpp"
#include "../../analysis/Analysis.hpp"
#include "../../analysis/DataPoint.hpp"

#include <runmanager/Test/ToolBin.hxx>

#include "../../model/Model.hpp"

#include "../../utilities/core/FileReference.hpp"

#include <OpenStudio.hxx>

using namespace openstudio;
using namespace openstudio::analysis;
using namespace openstudio::analysisdriver;

namespace openstudio {
namespace analysisdriver {

class TestWatcher : public AnalysisDriverWatcher {
 public:
  TestWatcher(const AnalysisDriver& analysisDriver)
    : AnalysisDriverWatcher(analysisDriver),
      m_numDataPointQueued(0),
      m_numDataPointComplete(0),
      m_numAnalysisComplete(0)
  {}

  virtual ~TestWatcher() {}

  virtual void onDataPointQueued(const openstudio::UUID& dataPoint) override {
    analysis::DataPoint dp = getDataPoint(dataPoint);
    ++m_numDataPointQueued;
  }

  virtual void onDataPointComplete(const openstudio::UUID& dataPoint) override {
    analysis::DataPoint dp = getDataPoint(dataPoint);
    ++m_numDataPointComplete;
  }

  virtual void onAnalysisComplete(const openstudio::UUID& analysis) override {
    analysis::Analysis a = getAnalysis(analysis);
    ++m_numAnalysisComplete;
  }

  int numDataPointQueued() {
    return m_numDataPointQueued;
  }

  int numDataPointComplete() {
    return m_numDataPointComplete;
  }

  int numAnalysisComplete() {
    return m_numAnalysisComplete;
  }

 private:
  int m_numDataPointQueued, m_numDataPointComplete, m_numAnalysisComplete;
};

} // analysisdriver
} // openstudio

TEST_F(AnalysisDriverFixture,AnalysisDriverWatcher_OneAnalysis) {

  // GET SIMPLE PROJECT
  SimpleProject project = getCleanSimpleProject("AnalysisDriverWatcher_OneAnalysis");
  Analysis analysis = project.analysis();

  // SET PROBLEM
  Problem problem = retrieveProblem("Continuous",true,false);
  analysis.setProblem(problem);

  // SET SEED
  model::Model model = fastExampleModel();
  openstudio::path p = toPath("./example.osm");
  model.save(p,true);
  FileReference seedModel(p);
  analysis.setSeed(seedModel);

  // CREATE DATA POINTS
  std::vector<QVariant> values;
  values.push_back(QVariant(double(34.21689)));
  values.push_back(QVariant(double(0.7234532)));
  values.push_back(QVariant(int(0)));
  OptionalDataPoint dataPoint = problem.createDataPoint(values);
  ASSERT_TRUE(dataPoint);
  bool test = analysis.addDataPoint(*dataPoint);
  EXPECT_TRUE(test);
  values[0] = QVariant(double(183.28938));
  values[1] = QVariant(double(0.82975899));
  dataPoint = problem.createDataPoint(values);
  ASSERT_TRUE(dataPoint);
  test = analysis.addDataPoint(*dataPoint);
  EXPECT_TRUE(test);

  // CREATE WATCHER
  AnalysisDriver driver = project.analysisDriver();
  TestWatcher watcher(driver);
  watcher.watch(analysis.uuid());

  // RUN AND WATCH ANALYSIS
  AnalysisRunOptions runOptions = standardRunOptions(project.projectDir());
  driver.run(analysis,runOptions);
  driver.waitForFinished();

  // CHECK WATCHING RESULTS
  EXPECT_EQ(2,watcher.numDataPointQueued());
  EXPECT_EQ(2,watcher.numDataPointComplete());
  EXPECT_EQ(1,watcher.numAnalysisComplete());
}
