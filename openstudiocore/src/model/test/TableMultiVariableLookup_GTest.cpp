/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "ModelFixture.hpp"

#include "../TableMultiVariableLookup.hpp"
#include "../TableMultiVariableLookup_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,TableMultiVariableLookup)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
  {
    Model m;
    TableMultiVariableLookup table(m,2);

    exit(0);
  } ,
    ::testing::ExitedWithCode(0), "" );

  {
    Model m;
    TableMultiVariableLookup table(m,2);

    ASSERT_TRUE(table.addPoint(70,32,0.1));
    ASSERT_TRUE(table.addPoint(72,32,0.3));
    ASSERT_TRUE(table.addPoint(74,32,0.5));
    ASSERT_TRUE(table.addPoint(76,32,0.7));
    ASSERT_TRUE(table.addPoint(78,32,0.9));

    ASSERT_TRUE(table.addPoint(70,45,0.2));
    ASSERT_TRUE(table.addPoint(72,45,0.4));
    ASSERT_TRUE(table.addPoint(74,45,0.6));
    ASSERT_TRUE(table.addPoint(76,45,0.8));
    ASSERT_TRUE(table.addPoint(78,45,1.0));

    ASSERT_TRUE(table.addPoint(70,68,0.3));
    ASSERT_TRUE(table.addPoint(72,68,0.5));
    ASSERT_TRUE(table.addPoint(74,68,0.7));
    ASSERT_TRUE(table.addPoint(76,68,0.9));
    ASSERT_TRUE(table.addPoint(78,68,1.1));

    ASSERT_TRUE(table.addPoint(70,81,0.4));
    ASSERT_TRUE(table.addPoint(72,81,0.6));
    ASSERT_TRUE(table.addPoint(74,81,0.8));
    ASSERT_TRUE(table.addPoint(76,81,1.0));
    ASSERT_TRUE(table.addPoint(78,81,1.2));

    ASSERT_TRUE(table.addPoint(70,94,0.5));
    ASSERT_TRUE(table.addPoint(72,94,0.7));
    ASSERT_TRUE(table.addPoint(74,94,0.9));
    ASSERT_TRUE(table.addPoint(76,94,1.1));
    ASSERT_TRUE(table.addPoint(78,94,1.3));

    ASSERT_TRUE(table.addPoint(70,107,0.6));
    ASSERT_TRUE(table.addPoint(72,107,0.8));
    ASSERT_TRUE(table.addPoint(74,107,1.0));
    ASSERT_TRUE(table.addPoint(76,107,1.2));
    ASSERT_TRUE(table.addPoint(78,107,1.4));

    // Make sure you cant add a point twice
    ASSERT_FALSE(table.addPoint(78,107,1.4));

    ASSERT_EQ(2,table.numberofIndependentVariables());

    ASSERT_EQ(30,table.points().size());
    ASSERT_EQ(5,table.xValues(0).size());
    ASSERT_EQ(6,table.xValues(1).size());

    ASSERT_DOUBLE_EQ(70,table.xValues(0)[0]);
    ASSERT_DOUBLE_EQ(32,table.xValues(1)[0]);

    ASSERT_DOUBLE_EQ(78,table.xValues(0)[4]);
    ASSERT_DOUBLE_EQ(107,table.xValues(1)[5]);

    std::vector<double> c(2);
    c[0] = 70;
    c[1] = 81;
    boost::optional<double> yValue = table.yValue(c);
    ASSERT_TRUE(yValue);
    ASSERT_DOUBLE_EQ(0.4,yValue.get());

    c[0] = 78;
    c[1] = 107;
    yValue = table.yValue(c);
    ASSERT_TRUE(yValue);
    ASSERT_DOUBLE_EQ(1.4,yValue.get());
  }
}

