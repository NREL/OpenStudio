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
#include "ProjectFixture.hpp"

#include "../AnalysisRecord.hpp"
#include "../AlgorithmRecord.hpp"
#include "../DesignOfExperimentsRecord.hpp"
#include "../DesignOfExperimentsRecord_Impl.hpp"
#include "../ProblemRecord.hpp"
#include "../AttributeRecord.hpp"
#include "../DataPointRecord.hpp"

#include "../../analysis/Analysis.hpp"
#include "../../analysis/Problem.hpp"
#include "../../analysis/Variable.hpp"
#include "../../analysis/DataPoint.hpp"
#include "../../analysis/DesignOfExperiments.hpp"
#include "../../analysis/DesignOfExperimentsOptions.hpp"

#include "../../runmanager/lib/Workflow.hpp"

#include "../../utilities/core/FileReference.hpp"
#include "../../utilities/core/Containers.hpp"
#include "../../utilities/data/Attribute.hpp"
#include "../../utilities/data/Tag.hpp"

using namespace openstudio;
using namespace openstudio::runmanager;
using namespace openstudio::analysis;
using namespace openstudio::project;

TEST_F(ProjectFixture, DesignOfExperiments_RoundTrip_PreRun) {
  DesignOfExperimentsOptions options(DesignOfExperimentsType::FullFactorial);
  options.setMaxIter(0);
  DesignOfExperiments algorithm(options);

  Analysis analysis("Analysis",
                    Problem("Problem",VariableVector(),Workflow()),
                    algorithm,
                    FileReference(toPath("in.osm")));

  ProjectDatabase database = getCleanDatabase("DesignOfExperiments_RoundTrip_PreRun");
  AnalysisRecord analysisR(analysis,database);

  ASSERT_TRUE(analysisR.algorithmRecord());
  ASSERT_TRUE(analysisR.algorithmRecord()->optionalCast<DesignOfExperimentsRecord>());
  DesignOfExperimentsRecord doeR = analysisR.algorithmRecord()->cast<DesignOfExperimentsRecord>();
  EXPECT_EQ(toString(algorithm.versionUUID()),toString(doeR.uuidLast()));
  EXPECT_EQ(algorithm.name(),doeR.name());

  // Check round-tripped objects
  DesignOfExperiments algorithmCopy = doeR.designOfExperiments();
  DesignOfExperimentsOptions optionsCopy = algorithmCopy.designOfExperimentsOptions();
  // Standard AlgorithmOptions Items
  ASSERT_TRUE((options.maxIter() && optionsCopy.maxIter()) ||
              (!options.maxIter() && !optionsCopy.maxIter()));
  if (options.maxIter()) {
    EXPECT_EQ(options.maxIter().get(),optionsCopy.maxIter().get());
  }
  ASSERT_TRUE((options.maxSims() && optionsCopy.maxSims()) ||
              (!options.maxSims() && !optionsCopy.maxSims()));
  if (options.maxSims()) {
    EXPECT_EQ(options.maxSims().get(),optionsCopy.maxSims().get());
  }
  // DesignOfExperimentsOptions Items
  EXPECT_TRUE(options.designType() == optionsCopy.designType());
  // Standard Algorithm Items
  EXPECT_EQ(algorithm.isComplete(),algorithmCopy.isComplete());
  EXPECT_EQ(algorithm.failed(),algorithmCopy.failed());
  EXPECT_EQ(algorithm.iter(),algorithmCopy.iter());
  // DesignOfExperiments Items
  EXPECT_EQ(DesignOfExperiments::standardName(),algorithm.name());
  EXPECT_EQ(DesignOfExperiments::standardName(),algorithmCopy.name());
}

TEST_F(ProjectFixture, DesignOfExperiments_RoundTrip_PostRun) {
  DesignOfExperimentsOptions options(DesignOfExperimentsType::FullFactorial);
  options.setMaxSims(13);
  DesignOfExperiments algorithm(createUUID(),
                                createUUID(),
                                std::string(),
                                std::string(),
                                true,
                                true,
                                0,
                                options);

  Analysis analysis("Analysis",
                    Problem("Problem",VariableVector(),Workflow()),
                    algorithm,
                    FileReference(toPath("in.osm")));

  DataPoint dataPoint(createUUID(),
                      createUUID(),
                      std::string(),
                      std::string(),
                      std::string(),
                      analysis.problem(),
                      true,
                      true,
                      true,
                      DataPointRunType::Local,
                      std::vector<QVariant>(),
                      DoubleVector(),
                      openstudio::path(),
                      boost::none,
                      boost::none,
                      boost::none,
                      boost::none,
                      std::vector<openstudio::path>(),
                      TagVector(),
                      AttributeVector());
  EXPECT_TRUE(analysis.addDataPoint(dataPoint));

  ProjectDatabase database = getCleanDatabase("DesignOfExperiments_RoundTrip_PostRun");
  AnalysisRecord analysisR(analysis,database);

  ASSERT_TRUE(analysisR.algorithmRecord());
  ASSERT_TRUE(analysisR.algorithmRecord()->optionalCast<DesignOfExperimentsRecord>());
  DesignOfExperimentsRecord doeR = analysisR.algorithmRecord()->cast<DesignOfExperimentsRecord>();
  EXPECT_EQ(toString(algorithm.versionUUID()),toString(doeR.uuidLast()));
  EXPECT_EQ(algorithm.name(),doeR.name());

  // Check round-tripped objects
  DesignOfExperiments algorithmCopy = doeR.designOfExperiments();
  DesignOfExperimentsOptions optionsCopy = algorithmCopy.designOfExperimentsOptions();
  // Standard AlgorithmOptions Items
  ASSERT_TRUE((options.maxIter() && optionsCopy.maxIter()) ||
              (!options.maxIter() && !optionsCopy.maxIter()));
  if (options.maxIter()) {
    EXPECT_EQ(options.maxIter().get(),optionsCopy.maxIter().get());
  }
  ASSERT_TRUE((options.maxSims() && optionsCopy.maxSims()) ||
              (!options.maxSims() && !optionsCopy.maxSims()));
  if (options.maxSims()) {
    EXPECT_EQ(options.maxSims().get(),optionsCopy.maxSims().get());
  }
  // DesignOfExperimentsOptions Items
  EXPECT_TRUE(options.designType() == optionsCopy.designType());
  // Standard Algorithm Items
  EXPECT_EQ(algorithm.isComplete(),algorithmCopy.isComplete());
  EXPECT_EQ(algorithm.failed(),algorithmCopy.failed());
  EXPECT_EQ(algorithm.iter(),algorithmCopy.iter());
  // DesignOfExperiments Items
  EXPECT_EQ(DesignOfExperiments::standardName(),algorithm.name());
  EXPECT_EQ(DesignOfExperiments::standardName(),algorithmCopy.name());
}

TEST_F(ProjectFixture, DesignOfExperiments_RemoveAlgorithm) {
  DesignOfExperimentsOptions options(DesignOfExperimentsType::FullFactorial);
  options.setMaxSims(13);
  DesignOfExperiments algorithm(createUUID(),
                                createUUID(),
                                std::string(),
                                std::string(),
                                true,
                                true,
                                0,
                                options);

  Analysis analysis("Analysis",
                    Problem("Problem",VariableVector(),Workflow()),
                    algorithm,
                    FileReference(toPath("in.osm")));

  DataPoint dataPoint(createUUID(),
                      createUUID(),
                      std::string(),
                      std::string(),
                      std::string(),
                      analysis.problem(),
                      true,
                      true,
                      true,
                      DataPointRunType::Local,
                      std::vector<QVariant>(),
                      DoubleVector(),
                      openstudio::path(),
                      boost::none,
                      boost::none,
                      boost::none,
                      boost::none,
                      std::vector<openstudio::path>(),
                      TagVector(),
                      AttributeVector());
  EXPECT_TRUE(analysis.addDataPoint(dataPoint));

  ProjectDatabase database = getCleanDatabase("DesignOfExperiments_RemoveAlgorithm");
  AnalysisRecord analysisR(analysis,database);

  ASSERT_TRUE(analysisR.algorithmRecord());
  ASSERT_TRUE(analysisR.algorithmRecord()->optionalCast<DesignOfExperimentsRecord>());
  DesignOfExperimentsRecord doeR = analysisR.algorithmRecord()->cast<DesignOfExperimentsRecord>();

  // pre-conditions
  EXPECT_EQ(1u,AlgorithmRecord::getAlgorithmRecords(database).size());
  EXPECT_FALSE(AttributeRecord::getAttributeRecords(database).empty()); // lists algorithm options

  // ETH@20120928 Kind of odd that this is allowed. Breaks Analysis interface.
  database.removeRecord(doeR);
  database.save();

  // post-conditions
  EXPECT_TRUE(AlgorithmRecord::getAlgorithmRecords(database).empty());
  EXPECT_TRUE(AttributeRecord::getAttributeRecords(database).empty());

  // ETH@20120928 - This is bad consequence of letting AlgorithmRecord be removed on its own.
  // At the very least, parent pointers back to child should be nulled out when child is removed.
  // But perhaps no child records be removable through the public interface?
  EXPECT_DEATH(Analysis analysisCopy = analysisR.analysis(),".*");
}

TEST_F(ProjectFixture, DesignOfExperiments_RemoveAnalysis) {
  DesignOfExperimentsOptions options(DesignOfExperimentsType::FullFactorial);
  options.setMaxSims(13);
  DesignOfExperiments algorithm(createUUID(),
                                createUUID(),
                                std::string(),
                                std::string(),
                                true,
                                true,
                                0,
                                options);

  Analysis analysis("Analysis",
                    Problem("Problem",VariableVector(),Workflow()),
                    algorithm,
                    FileReference(toPath("in.osm")));

  DataPoint dataPoint(createUUID(),
                      createUUID(),
                      std::string(),
                      std::string(),
                      std::string(),
                      analysis.problem(),
                      true,
                      true,
                      true,
                      DataPointRunType::Local,
                      std::vector<QVariant>(),
                      DoubleVector(),
                      openstudio::path(),
                      boost::none,
                      boost::none,
                      boost::none,
                      boost::none,
                      std::vector<openstudio::path>(),
                      TagVector(),
                      AttributeVector());
  EXPECT_TRUE(analysis.addDataPoint(dataPoint));

  ProjectDatabase database = getCleanDatabase("DesignOfExperiments_RemoveAnalysis");
  AnalysisRecord analysisR(analysis,database);

  // pre-conditions
  EXPECT_EQ(1u,AnalysisRecord::getAnalysisRecords(database).size());
  EXPECT_EQ(1u,ProblemRecord::getProblemRecords(database).size());
  EXPECT_EQ(1u,AlgorithmRecord::getAlgorithmRecords(database).size());
  EXPECT_FALSE(AttributeRecord::getAttributeRecords(database).empty()); // lists algorithm options
  EXPECT_EQ(1u,DataPointRecord::getDataPointRecords(database).size());

  database.removeRecord(analysisR);
  database.save();

  // post-conditions
  EXPECT_TRUE(AnalysisRecord::getAnalysisRecords(database).empty());
  EXPECT_TRUE(ProblemRecord::getProblemRecords(database).empty());
  EXPECT_TRUE(AlgorithmRecord::getAlgorithmRecords(database).empty());
  EXPECT_TRUE(AttributeRecord::getAttributeRecords(database).empty());
  EXPECT_TRUE(DataPointRecord::getDataPointRecords(database).empty());
}
