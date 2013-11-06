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

#include <analysis/OptimizationProblem.hpp>
#include <analysis/Variable.hpp>
#include <analysis/MeasureGroup.hpp>
#include <analysis/Measure.hpp>
#include <analysis/NullMeasure.hpp>
#include <analysis/RubyMeasure.hpp>
#include <analysis/LinearFunction.hpp>
#include <analysis/OutputAttributeVariable.hpp>
#include <analysis/SequentialSearch.hpp>
#include <analysis/SequentialSearchOptions.hpp>
#include <analysis/Analysis.hpp>
#include <analysis/OptimizationDataPoint.hpp>
#include <analysis/OptimizationDataPoint_Impl.hpp>

#include <runmanager/lib/Workflow.hpp>

#include <utilities/document/Table.hpp>
#include <utilities/core/Containers.hpp>
#include <utilities/core/Assert.hpp>

#include <OpenStudio.hxx>

#include <QVariant>

using namespace openstudio;
using namespace openstudio::analysis;

// linear problem hand-solved in excel to provide test expectations
std::vector<double> getObjectiveValues(const std::vector<QVariant>& variableValues) {
  OS_ASSERT(variableValues.size() == 5u);
  DoubleVector result(2u,20.0);
  int val(0);

  // var 1
  val = variableValues[0].toInt();
  if (val == 1) {
    result[0] += -1.0;
    result[1] += -3.0;
  }
  // var 2
  val = variableValues[1].toInt();
  if (val == 1) {
    result[0] += -3.0;
    result[1] += -1.0;
  }
  // var 3
  val = variableValues[2].toInt();
  if (val == 1) {
    result[0] += 1.0;
    result[1] += -2.0;
  }
  // var 4
  val = variableValues[3].toInt();
  if (val == 1) {
    result[0] += 3.0;
    result[1] += -2.0;
  }
  // var 5
  val = variableValues[4].toInt();
  if (val == 1) {
    result[0] += -1.0;
    result[1] += 3.0;
  }

  return result;
}

TEST_F(AnalysisFixture, SequentialSearch) {
  // define dummy problem
  VariableVector variables;
  std::stringstream ss;
  for (int i = 0; i < 5; ++i) {
    MeasureVector measures;
    measures.push_back(NullMeasure());
    measures.push_back(RubyMeasure(toPath("in.rb"),FileReferenceType::OSM,FileReferenceType::OSM));
    ss << "var " << i + 1;
    variables.push_back(MeasureGroup(ss.str(),measures));
    ss.str("");
  }
  FunctionVector functions;
  functions.push_back(LinearFunction("",VariableVector(1u,OutputAttributeContinuousVariable("f1","f1"))));
  functions.push_back(LinearFunction("",VariableVector(1u,OutputAttributeContinuousVariable("f2","f2"))));
  OptimizationProblem problem("By-Hand Problem",functions,variables,runmanager::Workflow());

  // solve dummy problem and check results
  SequentialSearch algorithm(SequentialSearchOptions(0));
  FileReference seed(toPath("in.osm"));
  Analysis analysis("By-Hand Analysis",problem,algorithm,seed);

  int numAdded = algorithm.createNextIteration(analysis);
  EXPECT_EQ(1,numAdded);
  OptimizationDataPointVector nextIteration =
      castVector<OptimizationDataPoint>(analysis.dataPointsToQueue());
  EXPECT_EQ(1u,nextIteration.size());
  while (!nextIteration.empty()) {
    int n = analysis.dataPoints().size();
    LOG(Debug,"Conducting iteration " << algorithm.iter() << " of Sequential Search.");
    OptimizationDataPointVector completeDataPoints =
        castVector<OptimizationDataPoint>(analysis.completeDataPoints());
    OptimizationDataPointVector currentDataPoints =
        castVector<OptimizationDataPoint>(analysis.getDataPoints("current"));
    EXPECT_EQ(1u,currentDataPoints.size());
    ASSERT_FALSE(currentDataPoints.empty());
    EXPECT_EQ(unsigned(algorithm.iter()),analysis.getDataPoints("explored").size());
    if (algorithm.iter() == 0) {
      EXPECT_EQ(1,n);
      EXPECT_EQ(0u,completeDataPoints.size());
    }
    else if (algorithm.iter() == 1) {
      EXPECT_DOUBLE_EQ(20.0,currentDataPoints[0].objectiveValues()[0]);
      EXPECT_DOUBLE_EQ(20.0,currentDataPoints[0].objectiveValues()[1]);
      EXPECT_EQ(6,n);
      EXPECT_EQ(1u,completeDataPoints.size());
      EXPECT_TRUE(currentDataPoints[0] == completeDataPoints[0]);
      EXPECT_TRUE(completeDataPoints[0].isTag("curve0"));
      EXPECT_TRUE(completeDataPoints[0].isTag("current"));
      EXPECT_TRUE(completeDataPoints[0].isTag("explored"));
      EXPECT_EQ(5u,nextIteration.size());
    }
    else if (algorithm.iter() == 2) {
      EXPECT_DOUBLE_EQ(17.0,currentDataPoints[0].objectiveValues()[0]);
      EXPECT_DOUBLE_EQ(19.0,currentDataPoints[0].objectiveValues()[1]);
      EXPECT_EQ(10,n);
      EXPECT_EQ(6u,completeDataPoints.size());
      EXPECT_EQ(2u,analysis.getDataPoints("curve0").size());
      EXPECT_EQ(4u,nextIteration.size());
    }
    else if (algorithm.iter() == 3) {
      EXPECT_DOUBLE_EQ(16.0,currentDataPoints[0].objectiveValues()[0]);
      EXPECT_DOUBLE_EQ(16.0,currentDataPoints[0].objectiveValues()[1]);
      EXPECT_EQ(13,n);
      EXPECT_EQ(10u,completeDataPoints.size());
      EXPECT_EQ(3u,analysis.getDataPoints("curve0").size());
      EXPECT_EQ(3u,nextIteration.size());
    }
    else if (algorithm.iter() == 4) {
      // backtracking
      EXPECT_DOUBLE_EQ(15.0,currentDataPoints[0].objectiveValues()[0]);
      EXPECT_DOUBLE_EQ(19.0,currentDataPoints[0].objectiveValues()[1]);
      EXPECT_EQ(16,n);
      EXPECT_EQ(13u,completeDataPoints.size());
      EXPECT_EQ(2u,analysis.getDataPoints("curve0").size());
      EXPECT_EQ(3u,nextIteration.size());
    }
    else if (algorithm.iter() == 5) {
      // backtracking
      EXPECT_DOUBLE_EQ(18.0,currentDataPoints[0].objectiveValues()[0]);
      EXPECT_DOUBLE_EQ(20.0,currentDataPoints[0].objectiveValues()[1]);
      EXPECT_EQ(18,n);
      EXPECT_EQ(16u,completeDataPoints.size());
      EXPECT_EQ(2u,analysis.getDataPoints("curve0").size());
      EXPECT_EQ(2u,nextIteration.size());
    }
    else if (algorithm.iter() == 6) {
      EXPECT_DOUBLE_EQ(17.0,currentDataPoints[0].objectiveValues()[0]);
      EXPECT_DOUBLE_EQ(14.0,currentDataPoints[0].objectiveValues()[1]);
      EXPECT_EQ(20,n);
      EXPECT_EQ(18u,completeDataPoints.size());
      EXPECT_EQ(5u,analysis.getDataPoints("curve0").size());
      EXPECT_EQ(2u,nextIteration.size());
    }
    else if (algorithm.iter() == 7) {
      EXPECT_DOUBLE_EQ(20.0,currentDataPoints[0].objectiveValues()[0]);
      EXPECT_DOUBLE_EQ(12.0,currentDataPoints[0].objectiveValues()[1]);
      EXPECT_EQ(23,n);
      EXPECT_EQ(20u,completeDataPoints.size());
      EXPECT_EQ(6u,analysis.getDataPoints("curve0").size());
      EXPECT_EQ(3u,nextIteration.size());
    }
    EXPECT_EQ(static_cast<size_t>(n),completeDataPoints.size() + nextIteration.size());
    BOOST_FOREACH(const OptimizationDataPoint& point,nextIteration) {
      std::vector<QVariant> values = point.variableValues();
      DoubleVector objectiveValues = getObjectiveValues(values);
      for (int i = 0; i < 5; ++i) {
        int value = values[i].toInt();
        if (value == 0) {
          ss << "  ";
        }
        else {
          EXPECT_EQ(1,value);
          ss << i + 1 << " ";
        }
      }
      ss << ": " << objectiveValues[0] << ", " << objectiveValues[1];
      LOG(Debug,ss.str()); ss.str("");

      completeDataPoints.push_back(OptimizationDataPoint(point.uuid(),
                                                         createUUID(),
                                                         "","","",
                                                         problem,
                                                         true,
                                                         false,
                                                         true,
                                                         DataPointRunType::Local,
                                                         values,
                                                         DoubleVector(),
                                                         objectiveValues,
                                                         openstudio::path(),
                                                         boost::none,
                                                         boost::none,
                                                         boost::none,
                                                         FileReferenceVector(),
                                                         boost::none,
                                                         std::vector<openstudio::path>(),
                                                         point.tags(),
                                                         point.outputAttributes())); // DLM: Elaine is this ok?
    }
    EXPECT_EQ(static_cast<size_t>(n),completeDataPoints.size());
    analysis = Analysis(analysis.uuid(),
                        createUUID(),
                        analysis.name(),
                        analysis.displayName(),
                        analysis.description(),
                        problem,
                        algorithm,
                        seed,
                        boost::none,
                        castVector<DataPoint>(completeDataPoints),
                        false,
                        false);
    algorithm.createNextIteration(analysis);
    nextIteration = castVector<OptimizationDataPoint>(analysis.dataPointsToQueue());
  }

  Table summaryTable = algorithm.getSummaryTable(analysis);
  bool ok = summaryTable.save(toPath("./By-Hand Problem Summary.csv"),true);
  EXPECT_TRUE(ok);

  EXPECT_EQ(23u,analysis.successfulDataPoints().size());
  OptimizationDataPointVector minimumCurve = algorithm.getMinimumCurve(0,analysis);
  ASSERT_EQ(6u,minimumCurve.size());
  DoubleVector values = minimumCurve[0].objectiveValues();
  EXPECT_DOUBLE_EQ(20.0,values[0]); EXPECT_DOUBLE_EQ(20.0,values[1]);
  values = minimumCurve[1].objectiveValues();
  EXPECT_DOUBLE_EQ(18.0,values[0]); EXPECT_DOUBLE_EQ(20.0,values[1]);
  values = minimumCurve[2].objectiveValues();
  EXPECT_DOUBLE_EQ(15.0,values[0]); EXPECT_DOUBLE_EQ(19.0,values[1]);
  values = minimumCurve[3].objectiveValues();
  EXPECT_DOUBLE_EQ(16.0,values[0]); EXPECT_DOUBLE_EQ(16.0,values[1]);
  values = minimumCurve[4].objectiveValues();
  EXPECT_DOUBLE_EQ(17.0,values[0]); EXPECT_DOUBLE_EQ(14.0,values[1]);
  values = minimumCurve[5].objectiveValues();
  EXPECT_DOUBLE_EQ(20.0,values[0]); EXPECT_DOUBLE_EQ(12.0,values[1]);
  OptimizationDataPointVector paretoFront = algorithm.getParetoFront(analysis);
  BOOST_FOREACH(const OptimizationDataPoint& paretoPoint,paretoFront) {
    EXPECT_FALSE(std::find(minimumCurve.begin(),minimumCurve.end(),paretoPoint) == minimumCurve.end());
  }
  ASSERT_EQ(4u,paretoFront.size());
  // expect same order as curve0, although different method for calculating
  values = paretoFront[0].objectiveValues();
  EXPECT_DOUBLE_EQ(15.0,values[0]); EXPECT_DOUBLE_EQ(19.0,values[1]); // minimizes f0
  values = paretoFront[1].objectiveValues();
  EXPECT_DOUBLE_EQ(16.0,values[0]); EXPECT_DOUBLE_EQ(16.0,values[1]);
  values = paretoFront[2].objectiveValues();
  EXPECT_DOUBLE_EQ(17.0,values[0]); EXPECT_DOUBLE_EQ(14.0,values[1]);
  values = paretoFront[3].objectiveValues();
  EXPECT_DOUBLE_EQ(20.0,values[0]); EXPECT_DOUBLE_EQ(12.0,values[1]); // minimizes f1
}
