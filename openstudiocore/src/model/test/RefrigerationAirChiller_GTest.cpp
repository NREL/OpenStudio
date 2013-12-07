/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <model/RefrigerationAirChiller.hpp>
#include <model/RefrigerationAirChiller_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, RefrigerationAirChiller_DefaultConstructor)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
    Model m;
    Schedule s = m.alwaysOnDiscreteSchedule();
    RefrigerationAirChiller testObject = RefrigerationAirChiller(m, s);

    exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture, RefrigerationAirChiller_Remove)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  RefrigerationAirChiller testObject = RefrigerationAirChiller(m, s);

  std::vector<RefrigerationAirChiller> refrigerationAirChillers = m.getModelObjects<RefrigerationAirChiller>();
  EXPECT_EQ(1, refrigerationAirChillers.size());

  testObject.remove();

  refrigerationAirChillers = m.getModelObjects<RefrigerationAirChiller>();
  EXPECT_EQ(0, refrigerationAirChillers.size());
}

TEST_F(ModelFixture, RefrigerationAirChiller_CloneOneModelWithDefaultData)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  RefrigerationAirChiller testObject = RefrigerationAirChiller(m, s);

  RefrigerationAirChiller testObjectClone = testObject.clone(m).cast<RefrigerationAirChiller>();

}

TEST_F(ModelFixture, RefrigerationAirChiller_CloneOneModelWithCustomData)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  RefrigerationAirChiller testObject = RefrigerationAirChiller(m, s);

  RefrigerationAirChiller testObjectClone = testObject.clone(m).cast<RefrigerationAirChiller>();

}

TEST_F(ModelFixture, RefrigerationAirChiller_CloneTwoModelsWithDefaultData)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  RefrigerationAirChiller testObject = RefrigerationAirChiller(m, s);

  RefrigerationAirChiller testObjectClone = testObject.clone(m).cast<RefrigerationAirChiller>();

  Model m2;

  RefrigerationAirChiller testObjectClone2 = testObject.clone(m2).cast<RefrigerationAirChiller>();

  EXPECT_NE(testObjectClone2.handle(), testObjectClone.handle());
}
