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

#include "../FSUDaceAlgorithmOptions.hpp"
#include "../FSUDaceAlgorithm.hpp"

using namespace openstudio;
using namespace openstudio::analysis;

TEST_F(AnalysisFixture, FSUDaceAlgorithmOptions) {

  //Testing FSUDaceAlgorithmOptions:
  FSUDaceAlgorithmOptions test1(FSUDaceAlgorithmType::halton);
  
  FSUDaceCvtTrialType obj0(FSUDaceCvtTrialType::grid);
  test1.setTrialType(obj0);
  EXPECT_EQ(test1.trialType(),obj0);
  test1.clearTrialType();
  EXPECT_FALSE(test1.trialType());
  
  test1.setSamples(20);
  test1.setFixedSequence(true);
  test1.setLatinize(true);
  test1.setVarianceBasedDecomp(true);
  test1.setQualityMetrics(true);
  test1.setSeed(5);
  test1.setNumTrials(1000000);
  test1.setFixedSeed(true);
  EXPECT_EQ(test1.samples(),20);
  EXPECT_TRUE(test1.latinize());
  EXPECT_TRUE(test1.varianceBasedDecomp());
  EXPECT_TRUE(test1.qualityMetrics());
  EXPECT_EQ(test1.seed(),5);
  EXPECT_EQ(test1.numTrials(),1000000);
  EXPECT_TRUE(test1.fixedSeed());
  test1.clearSeed();
  test1.clearNumTrials();
  EXPECT_FALSE(test1.seed());
  EXPECT_FALSE(test1.numTrials());

  std::vector<int> vectorOne(3);
  vectorOne[0]=1;
  vectorOne[1]=2;
  vectorOne[2]=3;
  test1.setSequenceStart(vectorOne);
  test1.setSequenceLeap(vectorOne);
  EXPECT_EQ(test1.sequenceStart(),vectorOne);
  EXPECT_EQ(test1.sequenceLeap(),vectorOne);

  vectorOne[1]=-1;
  //The next two lines should put errors in the log file.
  EXPECT_FALSE(test1.setSequenceStart(vectorOne));
  EXPECT_FALSE(test1.setSequenceLeap(vectorOne));

  test1.clearSequenceStart();
  test1.clearSequenceLeap();

  EXPECT_TRUE(test1.sequenceStart().empty());
  EXPECT_TRUE(test1.sequenceLeap().empty());

  //Testing FSUDaceAlgorithm:

  //resetting the vector value (was -1, which is invalid)
  vectorOne[1]=2;

  FSUDaceAlgorithmOptions testHaltonOptions(FSUDaceAlgorithmType::halton);
  FSUDaceAlgorithmOptions testHammersleyOptions(FSUDaceAlgorithmType::hammersley);
  FSUDaceAlgorithmOptions testCvtOptions(FSUDaceAlgorithmType::cvt);
  
  //Halton- and Hammersley-only options:
  testHaltonOptions.setSequenceStart(vectorOne);
  testHaltonOptions.setSequenceLeap(vectorOne);
  testHaltonOptions.setFixedSequence(true);
  testHammersleyOptions.setSequenceStart(vectorOne);
  testHammersleyOptions.setSequenceLeap(vectorOne);
  testHammersleyOptions.setFixedSequence(true);
  testCvtOptions.setSequenceStart(vectorOne);
  testCvtOptions.setSequenceLeap(vectorOne);
  testCvtOptions.setFixedSequence(true);

  //CVT-only options:
  testHaltonOptions.setSeed(3);
  testHaltonOptions.setFixedSeed(true);
  testHaltonOptions.setNumTrials(100000);
  testHaltonOptions.setTrialType(FSUDaceCvtTrialType::halton);
  testHammersleyOptions.setSeed(3);
  testHammersleyOptions.setFixedSeed(true);
  testHammersleyOptions.setNumTrials(100000);
  testHammersleyOptions.setTrialType(FSUDaceCvtTrialType::halton);
  testCvtOptions.setSeed(3);
  testCvtOptions.setFixedSeed(true);
  testCvtOptions.setNumTrials(100000);
  testCvtOptions.setTrialType(FSUDaceCvtTrialType::halton);

  //General Options:
  testHaltonOptions.setLatinize(true);
  testHaltonOptions.setVarianceBasedDecomp(true);
  testHaltonOptions.setQualityMetrics(true);
  testHammersleyOptions.setLatinize(true);
  testHammersleyOptions.setVarianceBasedDecomp(true);
  testHammersleyOptions.setQualityMetrics(true);
  testCvtOptions.setLatinize(true);
  testCvtOptions.setVarianceBasedDecomp(true);
  testCvtOptions.setQualityMetrics(true);

  FSUDaceAlgorithm testHalton(testHaltonOptions);
  FSUDaceAlgorithm testHammersley(testHammersleyOptions);
  FSUDaceAlgorithm testCvt(testCvtOptions);

  std::string testHaltonStrDakotaInDescription = testHalton.dakotaInFileDescription();
  LOG(Debug,"Printing the DakotaInFileDescription for the Halton Test" << std::endl <<
    testHaltonStrDakotaInDescription );

  std::string testHammersleyStrDakotaInDescription = testHammersley.dakotaInFileDescription();
  LOG(Debug,"Printing the DakotaInFileDescription for the Hammersley Test" << std::endl <<
    testHammersleyStrDakotaInDescription );

  std::string testCvtStrDakotaInDescription = testCvt.dakotaInFileDescription();
  LOG(Debug,"Printing the DakotaInFileDescription for the CVT Test" << std::endl <<
    testCvtStrDakotaInDescription );

  EXPECT_TRUE(testHalton.requiresContinuousVariables());
}

