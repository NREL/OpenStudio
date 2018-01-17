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

