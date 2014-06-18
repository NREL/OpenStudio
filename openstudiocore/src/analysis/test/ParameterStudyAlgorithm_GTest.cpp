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

#include <gtest/gtest.h>
#include "AnalysisFixture.hpp"

#include "../ParameterStudyAlgorithm.hpp"
#include "../ParameterStudyAlgorithmOptions.hpp"

using namespace openstudio;
using namespace openstudio::analysis;

TEST_F(AnalysisFixture, ParameterStudyAlgorithmOptions) {

  //Testing ParameterStudyAlgorithmOptions:
  ParameterStudyAlgorithmOptions testVectorParameterStudyOptions(
    ParameterStudyAlgorithmType::vector_parameter_study);
  ParameterStudyAlgorithmOptions testListParameterStudyOptions(
    ParameterStudyAlgorithmType::list_parameter_study);
  ParameterStudyAlgorithmOptions testCenteredParameterStudyOptions(
    ParameterStudyAlgorithmType::centered_parameter_study);
  ParameterStudyAlgorithmOptions testMultidimParameterStudyOptions(
    ParameterStudyAlgorithmType::multidim_parameter_study);

  std::vector<int> testVector(0);
  std::vector<double> testVector2(0);

  EXPECT_EQ(testVectorParameterStudyOptions.finalPoint(),testVector2);
  EXPECT_EQ(testVectorParameterStudyOptions.stepVector(),testVector2);
  EXPECT_EQ(testVectorParameterStudyOptions.listOfPoints(),testVector2);
  EXPECT_EQ(testVectorParameterStudyOptions.stepsPerVariable(),testVector);
  EXPECT_EQ(testVectorParameterStudyOptions.partitions(),testVector);
  EXPECT_FALSE(testVectorParameterStudyOptions.numSteps());

  EXPECT_EQ(testListParameterStudyOptions.finalPoint(),testVector2);
  EXPECT_EQ(testListParameterStudyOptions.stepVector(),testVector2);
  EXPECT_EQ(testListParameterStudyOptions.listOfPoints(),testVector2);
  EXPECT_EQ(testListParameterStudyOptions.stepsPerVariable(),testVector);
  EXPECT_EQ(testListParameterStudyOptions.partitions(),testVector);
  EXPECT_FALSE(testListParameterStudyOptions.numSteps());

  EXPECT_EQ(testCenteredParameterStudyOptions.finalPoint(),testVector2);
  EXPECT_EQ(testCenteredParameterStudyOptions.stepVector(),testVector2);
  EXPECT_EQ(testCenteredParameterStudyOptions.listOfPoints(),testVector2);
  EXPECT_EQ(testCenteredParameterStudyOptions.stepsPerVariable(),testVector);
  EXPECT_EQ(testCenteredParameterStudyOptions.partitions(),testVector);
  EXPECT_FALSE(testCenteredParameterStudyOptions.numSteps());

  EXPECT_EQ(testMultidimParameterStudyOptions.finalPoint(),testVector2);
  EXPECT_EQ(testMultidimParameterStudyOptions.stepVector(),testVector2);
  EXPECT_EQ(testMultidimParameterStudyOptions.listOfPoints(),testVector2);
  EXPECT_EQ(testMultidimParameterStudyOptions.stepsPerVariable(),testVector);
  EXPECT_EQ(testMultidimParameterStudyOptions.partitions(),testVector);
  EXPECT_FALSE(testMultidimParameterStudyOptions.numSteps());

  std::vector<int> vectorOne(3);
  vectorOne[0]=1;
  vectorOne[1]=2;
  vectorOne[2]=3;

  std::vector<double> vectorTwo(3);
  vectorTwo[0]=1.1;
  vectorTwo[1]=2.2;
  vectorTwo[2]=3.3;

  testVectorParameterStudyOptions.setFinalPoint(vectorTwo);
  testVectorParameterStudyOptions.setStepVector(vectorTwo);
  testVectorParameterStudyOptions.setListOfPoints(vectorTwo);
  testVectorParameterStudyOptions.setStepsPerVariable(vectorOne);
  testVectorParameterStudyOptions.setPartitions(vectorOne);
  testVectorParameterStudyOptions.setNumSteps(4);

  testListParameterStudyOptions.setFinalPoint(vectorTwo);
  testListParameterStudyOptions.setStepVector(vectorTwo);
  testListParameterStudyOptions.setListOfPoints(vectorTwo);
  testListParameterStudyOptions.setStepsPerVariable(vectorOne);
  testListParameterStudyOptions.setPartitions(vectorOne);
  testListParameterStudyOptions.setNumSteps(4);

  testCenteredParameterStudyOptions.setFinalPoint(vectorTwo);
  testCenteredParameterStudyOptions.setStepVector(vectorTwo);
  testCenteredParameterStudyOptions.setListOfPoints(vectorTwo);
  testCenteredParameterStudyOptions.setStepsPerVariable(vectorOne);
  testCenteredParameterStudyOptions.setPartitions(vectorOne);
  testCenteredParameterStudyOptions.setNumSteps(4);

  testMultidimParameterStudyOptions.setFinalPoint(vectorTwo);
  testMultidimParameterStudyOptions.setStepVector(vectorTwo);
  testMultidimParameterStudyOptions.setListOfPoints(vectorTwo);
  testMultidimParameterStudyOptions.setStepsPerVariable(vectorOne);
  testMultidimParameterStudyOptions.setPartitions(vectorOne);
  testMultidimParameterStudyOptions.setNumSteps(4);

  EXPECT_EQ(testListParameterStudyOptions.finalPoint(),vectorTwo);
  EXPECT_EQ(testListParameterStudyOptions.stepVector(),vectorTwo);
  EXPECT_EQ(testListParameterStudyOptions.listOfPoints(),vectorTwo);
  EXPECT_EQ(testListParameterStudyOptions.stepsPerVariable(),vectorOne);
  EXPECT_EQ(testListParameterStudyOptions.partitions(),vectorOne);
  EXPECT_EQ(testListParameterStudyOptions.numSteps(),4);

  testListParameterStudyOptions.clearFinalPoint();
  testListParameterStudyOptions.clearStepVector();
  testListParameterStudyOptions.clearListOfPoints();
  testListParameterStudyOptions.clearStepsPerVariable();
  testListParameterStudyOptions.clearPartitions();
  testListParameterStudyOptions.clearNumSteps();

  EXPECT_EQ(testListParameterStudyOptions.finalPoint(),testVector2);
  EXPECT_EQ(testListParameterStudyOptions.stepVector(),testVector2);
  EXPECT_EQ(testListParameterStudyOptions.listOfPoints(),testVector2);
  EXPECT_EQ(testListParameterStudyOptions.stepsPerVariable(),testVector);
  EXPECT_EQ(testListParameterStudyOptions.partitions(),testVector);
  EXPECT_FALSE(testListParameterStudyOptions.numSteps());

  testListParameterStudyOptions.setFinalPoint(vectorTwo);
  testListParameterStudyOptions.setStepVector(vectorTwo);
  testListParameterStudyOptions.setListOfPoints(vectorTwo);
  testListParameterStudyOptions.setStepsPerVariable(vectorOne);
  testListParameterStudyOptions.setPartitions(vectorOne);
  testListParameterStudyOptions.setNumSteps(4);

  //Testing Parameter Study Algorithm Class:
  ParameterStudyAlgorithm testVectorParameterStudy(testVectorParameterStudyOptions);
  ParameterStudyAlgorithm testListParameterStudy(testListParameterStudyOptions);
  ParameterStudyAlgorithm testCenteredParameterStudy(testCenteredParameterStudyOptions);
  ParameterStudyAlgorithm testMultidimParameterStudy(testMultidimParameterStudyOptions);

  std::string testVectorParameterStudyStrDakotaInDescription = testVectorParameterStudy.dakotaInFileDescription();
  LOG(Debug,"Printing the DakotaInFileDescription for the Vector Parameter Study Test" << std::endl <<
    testVectorParameterStudyStrDakotaInDescription );

  std::string testListParameterStudyStrDakotaInDescription = testListParameterStudy.dakotaInFileDescription();
  LOG(Debug,"Printing the DakotaInFileDescription for the List Parameter Study Test" << std::endl <<
    testListParameterStudyStrDakotaInDescription );

  std::string testCenteredParameterStudyStrDakotaInDescription = testCenteredParameterStudy.dakotaInFileDescription();
  LOG(Debug,"Printing the DakotaInFileDescription for the Centered Parameter Study Test" << std::endl <<
    testCenteredParameterStudyStrDakotaInDescription );

  std::string testMultidimParameterStudyStrDakotaInDescription = testMultidimParameterStudy.dakotaInFileDescription();
  LOG(Debug,"Printing the DakotaInFileDescription for the Multidim Parameter Study Test" << std::endl <<
    testMultidimParameterStudyStrDakotaInDescription );

  EXPECT_TRUE(testVectorParameterStudy.requiresContinuousVariables());
  

}

