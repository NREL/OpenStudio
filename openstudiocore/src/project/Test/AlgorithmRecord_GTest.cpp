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
#include <project/Test/ProjectFixture.hpp>

#include <project/AnalysisRecord.hpp>
#include <project/AlgorithmRecord.hpp>
#include <project/DesignOfExperimentsRecord.hpp>
#include <project/DesignOfExperimentsRecord_Impl.hpp>
#include <project/ProblemRecord.hpp>
#include <project/AttributeRecord.hpp>
#include <project/DataPointRecord.hpp>

#include <analysis/Analysis.hpp>
#include <analysis/Problem.hpp>
#include <analysis/Variable.hpp>
#include <analysis/DataPoint.hpp>
#include <analysis/DesignOfExperiments.hpp>
#include <analysis/DesignOfExperimentsOptions.hpp>

#include <runmanager/lib/Workflow.hpp>

#include <utilities/core/FileReference.hpp>
#include <utilities/core/Containers.hpp>
#include <utilities/data/Attribute.hpp>
#include <utilities/data/Tag.hpp>

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
                      FileReferenceVector(),
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
                      FileReferenceVector(),
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
                      FileReferenceVector(),
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
