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
#include <model/Model_Impl.hpp>
#include <model/Space.hpp>
#include <model/Space_Impl.hpp>
#include <model/SpaceType.hpp>
#include <model/SpaceType_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/Surface.hpp>
#include <model/Surface_Impl.hpp>
#include <model/DesignSpecificationOutdoorAir.hpp>
#include <model/DesignSpecificationOutdoorAir_Impl.hpp>

#include <utilities/geometry/Point3d.hpp>

#include <boost/foreach.hpp>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, DesignSpecificationOutdoorAir_Space)
{
  Model model;

  DesignSpecificationOutdoorAir designSpecificationOutdoorAir(model);

  Space space(model);
  EXPECT_FALSE(space.designSpecificationOutdoorAir());
  EXPECT_TRUE(space.setDesignSpecificationOutdoorAir(designSpecificationOutdoorAir));
  ASSERT_TRUE(space.designSpecificationOutdoorAir());
  EXPECT_EQ(designSpecificationOutdoorAir.handle(), space.designSpecificationOutdoorAir()->handle());
}

TEST_F(ModelFixture, DesignSpecificationOutdoorAir_SpaceType)
{
  Model model;

  DesignSpecificationOutdoorAir designSpecificationOutdoorAir(model);
  
  SpaceType spaceType(model);
  EXPECT_FALSE(spaceType.designSpecificationOutdoorAir());
  EXPECT_TRUE(spaceType.setDesignSpecificationOutdoorAir(designSpecificationOutdoorAir));
  ASSERT_TRUE(spaceType.designSpecificationOutdoorAir());
  EXPECT_EQ(designSpecificationOutdoorAir.handle(), spaceType.designSpecificationOutdoorAir()->handle());

  Space space(model);
  EXPECT_TRUE(space.setSpaceType(spaceType));
  ASSERT_TRUE(space.designSpecificationOutdoorAir());
  EXPECT_EQ(designSpecificationOutdoorAir.handle(), space.designSpecificationOutdoorAir()->handle());

  DesignSpecificationOutdoorAir designSpecificationOutdoorAir2(model);
  EXPECT_TRUE(space.setDesignSpecificationOutdoorAir(designSpecificationOutdoorAir2));
  ASSERT_TRUE(space.designSpecificationOutdoorAir());
  EXPECT_EQ(designSpecificationOutdoorAir2.handle(), space.designSpecificationOutdoorAir()->handle());
}

TEST_F(ModelFixture, DesignSpecificationOutdoorAir_ThermalZone_2SameSpaces)
{
  Model model;

  ThermalZone thermalZone(model);

  DesignSpecificationOutdoorAir designSpecificationOutdoorAir(model);

  Space space1(model);
  EXPECT_TRUE(space1.setThermalZone(thermalZone));
  EXPECT_TRUE(space1.setDesignSpecificationOutdoorAir(designSpecificationOutdoorAir));

  Space space2(model);
  EXPECT_TRUE(space2.setThermalZone(thermalZone));
  EXPECT_TRUE(space2.setDesignSpecificationOutdoorAir(designSpecificationOutdoorAir));

  EXPECT_EQ(1u, model.getModelObjects<DesignSpecificationOutdoorAir>().size());
  boost::optional<Space> combinedSpace = thermalZone.combineSpaces();
  ASSERT_TRUE(combinedSpace);
  EXPECT_FALSE(combinedSpace->isDesignSpecificationOutdoorAirDefaulted());
  ASSERT_TRUE(combinedSpace->designSpecificationOutdoorAir());
  EXPECT_EQ(1u, model.getModelObjects<DesignSpecificationOutdoorAir>().size());
  EXPECT_EQ(designSpecificationOutdoorAir.handle(), combinedSpace->designSpecificationOutdoorAir()->handle());
}

TEST_F(ModelFixture, DesignSpecificationOutdoorAir_ThermalZone_2DifferentSpaces)
{
  Model model;

  // 1 m^2
  Point3dVector points;
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(1, 1, 0));
  points.push_back(Point3d(1, 0, 0));
  

  ThermalZone thermalZone(model);

  DesignSpecificationOutdoorAir designSpecificationOutdoorAir1(model);
  EXPECT_TRUE(designSpecificationOutdoorAir1.setOutdoorAirFlowperPerson(0.0));
  EXPECT_TRUE(designSpecificationOutdoorAir1.setOutdoorAirFlowperFloorArea(2.0));

  DesignSpecificationOutdoorAir designSpecificationOutdoorAir2(model);
  EXPECT_TRUE(designSpecificationOutdoorAir2.setOutdoorAirFlowperPerson(3.0));

  Space space1(model);
  EXPECT_TRUE(space1.setThermalZone(thermalZone));
  EXPECT_TRUE(space1.setDesignSpecificationOutdoorAir(designSpecificationOutdoorAir1));
  
  Surface surface1(points, model);
  surface1.setParent(space1);
  EXPECT_EQ(1.0, space1.floorArea());

  space1.setPeoplePerFloorArea(1.0);
  EXPECT_EQ(1.0, space1.numberOfPeople());

  Space space2(model);
  EXPECT_TRUE(space2.setThermalZone(thermalZone));
  EXPECT_TRUE(space2.setDesignSpecificationOutdoorAir(designSpecificationOutdoorAir2));
  
  Surface surface2(points, model);
  surface2.setParent(space2);
  EXPECT_EQ(1.0, space2.floorArea());

  space2.setPeoplePerFloorArea(2.0);
  EXPECT_EQ(2.0, space2.numberOfPeople());

  EXPECT_EQ(2u, model.getModelObjects<DesignSpecificationOutdoorAir>().size());
  boost::optional<Space> combinedSpace = thermalZone.combineSpaces();
  ASSERT_TRUE(combinedSpace);
  EXPECT_FALSE(combinedSpace->isDesignSpecificationOutdoorAirDefaulted());
  ASSERT_TRUE(combinedSpace->designSpecificationOutdoorAir());
  EXPECT_EQ(3u, model.getModelObjects<DesignSpecificationOutdoorAir>().size());
  EXPECT_NE(designSpecificationOutdoorAir1.handle(), combinedSpace->designSpecificationOutdoorAir()->handle());
  EXPECT_NE(designSpecificationOutdoorAir2.handle(), combinedSpace->designSpecificationOutdoorAir()->handle());

  EXPECT_EQ(1.0, combinedSpace->designSpecificationOutdoorAir()->outdoorAirFlowperFloorArea()); // (2*1 + 0*1)/2
  EXPECT_EQ(2.0, combinedSpace->designSpecificationOutdoorAir()->outdoorAirFlowperPerson()); // (0*1 + 3*2)/3
}