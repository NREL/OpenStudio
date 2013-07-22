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
#include <analysis/test/AnalysisFixture.hpp>

#include <analysis/Analysis.hpp>
#include <analysis/Problem.hpp>
#include <analysis/Variable.hpp>
#include <analysis/DataPoint.hpp>
#include <analysis/ModelRulesetContinuousVariable.hpp>
#include <analysis/DiscreteVariable.hpp>
#include <analysis/DiscreteVariable_Impl.hpp>
#include <analysis/DiscretePerturbation.hpp>
#include <analysis/NullPerturbation.hpp>
#include <analysis/RubyPerturbation.hpp>
#include <analysis/RubyPerturbation_Impl.hpp>
#include <analysis/ParameterStudyAlgorithm.hpp>
#include <analysis/ParameterStudyAlgorithm_Impl.hpp>
#include <analysis/ParameterStudyAlgorithmOptions.hpp>
#include <analysis/WorkflowStep.hpp>

#include <runmanager/lib/Workflow.hpp>

#include <ruleset/ModelObjectFilterType.hpp>

#include <utilities/core/Containers.hpp>
#include <utilities/data/Tag.hpp>

using namespace openstudio;
using namespace openstudio::analysis;
using namespace openstudio::ruleset;

TEST_F(AnalysisFixture, Analysis_Constructors) {
  // minimal information
  Analysis analysis("Analysis",
                    Problem("Problem",VariableVector(),runmanager::Workflow()),
                    FileReference(toPath("./in.osm")));
  EXPECT_TRUE(analysis.isDirty());

  analysis = Analysis("Analysis",
                      Problem("Problem",VariableVector(),runmanager::Workflow()),
                      FileReferenceType::OSM);
  EXPECT_TRUE(analysis.isDirty());
  EXPECT_EQ("*.osm",toString(analysis.seed().path()));
}

TEST_F(AnalysisFixture, Analysis_SetSeed) {
  Analysis analysis("Analysis",
                    Problem("Problem",VariableVector(),runmanager::Workflow()),
                    FileReferenceType::OSM);
  EXPECT_TRUE(analysis.isDirty());
  bool ok = analysis.clearDirtyFlag();
  EXPECT_TRUE(ok);
  EXPECT_FALSE(analysis.isDirty());
  EXPECT_FALSE(analysis.problem().isDirty());

  ok = analysis.setSeed(FileReference(toPath("MyModel.osm")));
  EXPECT_TRUE(ok);
  EXPECT_TRUE(analysis.isDirty());
  EXPECT_FALSE(analysis.problem().isDirty());
  EXPECT_FALSE(analysis.resultsAreInvalid()); // no results in analysis to be invalidated
  EXPECT_FALSE(analysis.dataPointsAreInvalid());
  ok = analysis.clearDirtyFlag();
  EXPECT_TRUE(ok);

  // if have data point with no results, results are not invalid

  OptionalDataPoint dataPoint = analysis.problem().createDataPoint(std::vector<QVariant>());
  ASSERT_TRUE(dataPoint);
  ok = analysis.addDataPoint(*dataPoint);
  EXPECT_TRUE(ok);
  dataPoint = analysis.problem().createDataPoint(std::vector<QVariant>());
  ASSERT_TRUE(dataPoint);
  ok = analysis.addDataPoint(*dataPoint);
  EXPECT_FALSE(ok); // cannot add second data point with same variable values
  ASSERT_FALSE(analysis.dataPoints().empty());
  EXPECT_TRUE(analysis.dataPoints()[0].isDirty());
  analysis.clearDirtyFlag();
  EXPECT_FALSE(analysis.dataPoints()[0].isDirty());
  ok = analysis.setSeed(FileReference(toPath("YourModel.osm")));
  EXPECT_TRUE(ok);
  EXPECT_TRUE(analysis.isDirty());
  EXPECT_FALSE(analysis.problem().isDirty());
  ASSERT_FALSE(analysis.dataPoints().empty());
  EXPECT_FALSE(analysis.dataPoints()[0].isDirty());
  EXPECT_FALSE(analysis.resultsAreInvalid());
  EXPECT_FALSE(analysis.dataPointsAreInvalid());
  ok = analysis.clearDirtyFlag();
  EXPECT_TRUE(ok);

  // if data point has results, results are invalidated by changing seed

  analysis.removeAllDataPoints();
  dataPoint = DataPoint(createUUID(),
                        createUUID(),
                        "Fake Data Point",
                        "",
                        "",
                        true,
                        false,
                        analysis.problem(),
                        std::vector<QVariant>(),
                        DoubleVector(),
                        toPath("dataPoint1"),
                        FileReference(toPath("out.osm")),
                        FileReference(toPath("in.idf")),
                        OptionalFileReference(),
                        OptionalFileReference(),
                        TagVector(),
                        boost::none,
                        std::vector<openstudio::path>()); //DLM: Elaine is this ok?
  ok = analysis.addDataPoint(*dataPoint);
  EXPECT_TRUE(ok);
  analysis.clearDirtyFlag();
  ok = analysis.setSeed(FileReference(toPath("HerModel.osm")));
  EXPECT_TRUE(ok);
  EXPECT_TRUE(analysis.isDirty());
  EXPECT_FALSE(analysis.problem().isDirty());
  ASSERT_FALSE(analysis.dataPoints().empty());
  EXPECT_FALSE(analysis.dataPoints()[0].isDirty());
  EXPECT_TRUE(analysis.resultsAreInvalid());
  EXPECT_FALSE(analysis.dataPointsAreInvalid());
  ok = analysis.clearDirtyFlag();
  EXPECT_FALSE(ok); // have to clear results first, before declaring clean
  analysis.clearAllResults();
  EXPECT_FALSE(analysis.resultsAreInvalid());
  ASSERT_FALSE(analysis.dataPoints().empty());
  EXPECT_TRUE(analysis.dataPoints()[0].isDirty()); // results were deleted
  ok = analysis.clearDirtyFlag();
  EXPECT_TRUE(ok);
}

TEST_F(AnalysisFixture, Analysis_DataPointsAreInvalid) {
  Analysis analysis("Analysis",
                    Problem("Problem",VariableVector(),runmanager::Workflow()),
                    FileReferenceType::OSM);
  EXPECT_FALSE(analysis.dataPointsAreInvalid());

  // add a single, null-only variable
  // ETH@20130206 - Alternate code for this test.
  // Problem problem = analysis.problem();
  // DiscreteVariable dv("South Facade WWR",
  //                     DiscretePerturbationVector(1u,NullPerturbation()));
  bool test = analysis.problem().push(
        DiscreteVariable("South Facade WWR",
                         DiscretePerturbationVector(1u,NullPerturbation())));
  // ETH@20130206
  // bool test = problem.push(dv);
  EXPECT_TRUE(test);
  ASSERT_FALSE(analysis.problem().variables().empty());
  EXPECT_FALSE(analysis.dataPointsAreInvalid());

  // create a data point
  OptionalDataPoint dataPoint = analysis.problem().createDataPoint(std::vector<QVariant>(1u,0));
  ASSERT_TRUE(dataPoint);
  test = analysis.addDataPoint(*dataPoint);
  EXPECT_TRUE(test);

  // pushing perturbations does not invalidate data point
  RubyPerturbation measure1(toPath("myMeasure.rb"),
                           FileReferenceType::OSM,
                           FileReferenceType::OSM,
                           true);
  // ETH@20130206
  // test = dv.push(measure1);
  // EXPECT_EQ(1u,dv.numPerturbations(false));
  // EXPECT_EQ(2u,analysis.problem().variables()[0].cast<DiscreteVariable>().numPerturbations(false));
  test = analysis.problem().variables()[0].cast<DiscreteVariable>().push(measure1);
  EXPECT_TRUE(test);
  EXPECT_FALSE(analysis.dataPointsAreInvalid());
  // should be able to add another data point
  dataPoint = analysis.problem().createDataPoint(std::vector<QVariant>(1u,1));
  ASSERT_TRUE(dataPoint);
  test = analysis.addDataPoint(*dataPoint);
  EXPECT_TRUE(test);

  RubyPerturbation measure2 = measure1.clone().cast<RubyPerturbation>();
  test = analysis.problem().variables()[0].cast<DiscreteVariable>().push(measure2);
  EXPECT_TRUE(test);
  EXPECT_FALSE(analysis.dataPointsAreInvalid());

  // swapping perturbations invalidates data points
  test = analysis.problem().variables()[0].cast<DiscreteVariable>().swap(measure1,measure2);
  EXPECT_TRUE(test);
  EXPECT_TRUE(analysis.dataPointsAreInvalid());
  // and should not be able to add data points now
  dataPoint = analysis.problem().createDataPoint(std::vector<QVariant>(1u,2));
  ASSERT_TRUE(dataPoint);
  test = analysis.addDataPoint(*dataPoint);
  EXPECT_FALSE(test);

  // removing all data points clears state
  analysis.removeAllDataPoints();
  EXPECT_FALSE(analysis.dataPointsAreInvalid());
  // and can now add points again
  dataPoint = analysis.problem().createDataPoint(std::vector<QVariant>(1u,1));
  ASSERT_TRUE(dataPoint);
  test = analysis.addDataPoint(*dataPoint);
  EXPECT_TRUE(test);

  // adding a new variable re-invalidates them
  test = analysis.problem().push(DiscreteVariable("West Facade WWR",DiscretePerturbationVector(1u,NullPerturbation())));
  EXPECT_TRUE(test);
  EXPECT_TRUE(analysis.dataPointsAreInvalid());
  std::vector<QVariant> values;
  values.push_back(QVariant(2));
  values.push_back(QVariant(0));
  dataPoint = analysis.problem().createDataPoint(values);
  ASSERT_TRUE(dataPoint);
  test = analysis.addDataPoint(*dataPoint);
  EXPECT_FALSE(test);

  // removing all data points clears state
  analysis.removeAllDataPoints();
  EXPECT_FALSE(analysis.dataPointsAreInvalid());
  // and can now add points again
  test = analysis.addDataPoint(*dataPoint);
  EXPECT_TRUE(test);

  // erasing a variable re-invalidates ...
  test = analysis.problem().erase(analysis.problem().workflow()[0]);
  EXPECT_TRUE(test);
  EXPECT_TRUE(analysis.dataPointsAreInvalid());
}

TEST_F(AnalysisFixture, Analysis_ClearAllResults) {
  // create dummy problem
  ModelObjectFilterType buildingType(IddObjectType::OS_Building);
  ModelRulesetContinuousVariable var("Building Rotation",
                                     ModelObjectFilterClauseVector(1u,buildingType),
                                     "northAxis");
  var.setMinimum(0.0);
  var.setMaximum(360.0);
  Problem problem("Problem",VariableVector(1u,var),runmanager::Workflow());

  // create dummy algorithm (already initialized to be complete)
  ParameterStudyAlgorithmOptions options(ParameterStudyAlgorithmType::list_parameter_study);
  DoubleVector points;
  for (double val = 0.0; val < 360.0; val += 15.0) {
    points.push_back(val);
  }
  options.setListOfPoints(points);
  ParameterStudyAlgorithm algorithm(createUUID(),
                                    createUUID(),
                                    std::string(),
                                    std::string(),
                                    true,
                                    false,
                                    0,
                                    options,
                                    FileReference(toPath("dakota.rst")),
                                    FileReference(toPath("dakota.out")),
                                    boost::none); //DLM: Elaine is this ok?

  // create analysis
  Analysis analysis("Analysis",problem,algorithm,FileReference(toPath("./in.osm")));

  // add data points
  std::stringstream ss;
  int i = 1;
  BOOST_FOREACH(double point,points) {
    ss << "dataPoint" << i;
    DataPoint dataPoint(createUUID(),
                        createUUID(),
                        "",
                        "",
                        "",
                        true,
                        false,
                        problem,
                        std::vector<QVariant>(1u,point),
                        DoubleVector(),
                        toPath(ss.str()),
                        FileReference(toPath(ss.str() + "/out.osm")),
                        FileReference(toPath(ss.str() + "/in.idf")),
                        OptionalFileReference(),
                        OptionalFileReference(),
                        TagVector(),
                        boost::none,
                        std::vector<openstudio::path>()); //DLM: Elaine is this ok?
    ss.str("");
    bool test = analysis.addDataPoint(dataPoint);
    EXPECT_TRUE(test);
  }

  // check current status
  EXPECT_EQ(points.size(),analysis.dataPoints().size());
  EXPECT_EQ(points.size(),analysis.completeDataPoints().size());
  ASSERT_TRUE(analysis.algorithm());
  EXPECT_TRUE(analysis.algorithm()->isComplete());
  EXPECT_FALSE(analysis.algorithm()->failed());
  ASSERT_TRUE(analysis.algorithm()->optionalCast<DakotaAlgorithm>());
  EXPECT_TRUE(analysis.algorithm()->cast<DakotaAlgorithm>().restartFileReference());
  EXPECT_TRUE(analysis.algorithm()->cast<DakotaAlgorithm>().outFileReference());

  // clear results and check status
  analysis.clearAllResults();
  EXPECT_EQ(points.size(),analysis.dataPoints().size());
  EXPECT_TRUE(analysis.completeDataPoints().empty());
  ASSERT_TRUE(analysis.algorithm());
  EXPECT_FALSE(analysis.algorithm()->isComplete());
  EXPECT_FALSE(analysis.algorithm()->failed());
  ASSERT_TRUE(analysis.algorithm()->optionalCast<DakotaAlgorithm>());
  EXPECT_FALSE(analysis.algorithm()->cast<DakotaAlgorithm>().restartFileReference());
  EXPECT_FALSE(analysis.algorithm()->cast<DakotaAlgorithm>().outFileReference());
}

TEST_F(AnalysisFixture,Analysis_JSONSerialization_PreRunPAT) {
  // Create example PAT analysis

  // Add data points

  // Serialize Analysis with no data points

  // Deserialize and check results

  // Serialize Analysis with data points

  // Deserialize and check results

}

