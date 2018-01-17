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

