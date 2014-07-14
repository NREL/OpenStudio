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

