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

