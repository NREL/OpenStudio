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

#include <gtest/gtest.h>
#include <analysisdriver/test/AnalysisDriverFixture.hpp>

#include <analysisdriver/SimpleProject.hpp>
#include <analysisdriver/AnalysisDriver.hpp>
#include <analysisdriver/CurrentAnalysis.hpp>
#include <analysisdriver/AnalysisRunOptions.hpp>
#include <analysisdriver/AnalysisDriverWatcher.hpp>

#include <project/ProjectDatabase.hpp>

#include <analysis/Problem.hpp>
#include <analysis/DDACEAlgorithm.hpp>
#include <analysis/DDACEAlgorithmOptions.hpp>
#include <analysis/Analysis.hpp>
#include <analysis/DataPoint.hpp>

#include <runmanager/Test/ToolBin.hxx>

#include <model/Model.hpp>

#include <utilities/core/FileReference.hpp>

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

  virtual void onDataPointQueued(const openstudio::UUID& dataPoint) {
    analysis::DataPoint dp = getDataPoint(dataPoint);
    ++m_numDataPointQueued;
  }

  virtual void onDataPointComplete(const openstudio::UUID& dataPoint) {
    analysis::DataPoint dp = getDataPoint(dataPoint);
    ++m_numDataPointComplete;
  }

  virtual void onAnalysisComplete(const openstudio::UUID& analysis) {
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
  model::Model model = model::exampleModel();
  openstudio::path p = toPath("./example.osm");
  model.save(p,true);
  FileReference seedModel(p);
  analysis.setSeed(seedModel);

  // SET ALGORITHM
  DDACEAlgorithmOptions algOptions(DDACEAlgorithmType::lhs);
  algOptions.setSamples(2);
  DDACEAlgorithm algorithm(algOptions);
  analysis.setAlgorithm(algorithm);

  if (!dakotaExePath().empty()) {
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
}
