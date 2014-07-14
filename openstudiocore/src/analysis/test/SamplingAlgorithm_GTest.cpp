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

#include "../SamplingAlgorithmOptions.hpp"
#include "../SamplingAlgorithm.hpp"

using namespace openstudio;
using namespace openstudio::analysis;

TEST_F(AnalysisFixture, SamplingAlgorithmOptions) {

  //Testing SamplingAlgorithmOptions:
  SamplingAlgorithmOptions test1;
  
  SamplingAlgorithmRNGType obj0(SamplingAlgorithmRNGType::rnum2);
  SamplingAlgorithmSampleType obj1(SamplingAlgorithmSampleType::random);
  test1.setRNGType(obj0);
  test1.setSampleType(obj1);
  EXPECT_EQ(test1.rngType(),obj0);
  EXPECT_EQ(test1.sampleType(),obj1);
  test1.clearRNGType();
  test1.clearSampleType();
  EXPECT_FALSE(test1.rngType());
  EXPECT_FALSE(test1.sampleType());
  
  test1.setSamples(20);
  test1.setAllVariables(true);
  test1.setVarianceBasedDecomp(true);
  test1.setDropTolerance(true);
  test1.setSeed(5);
  test1.setFixedSeed(true);
  EXPECT_EQ(test1.samples(),20);
  EXPECT_TRUE(test1.varianceBasedDecomp());
  EXPECT_TRUE(test1.allVariables());
  EXPECT_TRUE(test1.dropTolerance());
  EXPECT_EQ(test1.seed(),5);
  EXPECT_TRUE(test1.fixedSeed());
  test1.clearSeed();
  EXPECT_FALSE(test1.seed());

  //Testing SamplingAlgorithm:

  SamplingAlgorithmOptions testOptions;


  testOptions.setAllVariables(true);
  testOptions.setSamples(4);
  testOptions.setSeed(3);
  testOptions.setFixedSeed(true);
  testOptions.setRNGType(SamplingAlgorithmRNGType::mt19937);
  testOptions.setSampleType(SamplingAlgorithmSampleType::lhs);
  testOptions.setVarianceBasedDecomp(true);
  testOptions.setDropTolerance(true);

  SamplingAlgorithm testSamplingOptions(testOptions);

  std::string testSamplingOptionsStrDakotaInDescription = testSamplingOptions.dakotaInFileDescription();
  LOG(Debug,"Printing the DakotaInFileDescription for the Sampling Options Test" << std::endl <<
    testSamplingOptionsStrDakotaInDescription );
}

