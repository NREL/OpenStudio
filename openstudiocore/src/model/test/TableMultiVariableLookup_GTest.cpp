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

#include <model/test/ModelFixture.hpp>

#include <model/TableMultiVariableLookup.hpp>
#include <model/TableMultiVariableLookup_Impl.hpp>

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

