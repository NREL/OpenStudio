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

#include "../PSUADEDaceAlgorithmOptions.hpp"
#include "../PSUADEDaceAlgorithm.hpp"

using namespace openstudio;
using namespace openstudio::analysis;

TEST_F(AnalysisFixture, PSUADEDaceAlgorithmOptions) {

  //Testing PSUADEDaceAlgorithmOptions:
  PSUADEDaceAlgorithmOptions test1;
  EXPECT_FALSE(test1.seed());
  EXPECT_FALSE(test1.samples());
  EXPECT_FALSE(test1.partitions()); 
  test1.setSeed(20);
  test1.setSamples(21);
  test1.setPartitions(22);
  EXPECT_EQ(test1.seed(),20);
  EXPECT_EQ(test1.samples(),21);
  EXPECT_EQ(test1.partitions(),22);
  test1.clearSeed();
  test1.clearSamples();
  test1.clearPartitions();
  EXPECT_FALSE(test1.seed());
  EXPECT_FALSE(test1.samples());
  EXPECT_FALSE(test1.partitions()); 

  //Testing FSUDaceAlgorithm:

  PSUADEDaceAlgorithmOptions test2Options;
  PSUADEDaceAlgorithmOptions test3Options;
  
  //General Options:
  test3Options.setSeed(10);
  test3Options.setSamples(11);
  test3Options.setPartitions(12);

  PSUADEDaceAlgorithm test2(test2Options);
  PSUADEDaceAlgorithm test3(test3Options);

  std::string test2StrDakotaInDescription = test2.dakotaInFileDescription();
  LOG(Debug,"Printing the DakotaInFileDescription for test 2 (no options)" << std::endl <<
    test2StrDakotaInDescription );

  std::string test3StrDakotaInDescription = test3.dakotaInFileDescription();
  LOG(Debug,"Printing the DakotaInFileDescription for test 3 (all options)" << std::endl <<
    test3StrDakotaInDescription );

  EXPECT_TRUE(test2.requiresContinuousVariables());
}

