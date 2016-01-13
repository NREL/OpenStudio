/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/ShadingSurface.hpp"
#include "../../model/ShadingSurface_Impl.hpp"
#include "../../model/ShadingSurfaceGroup.hpp"
#include "../../model/ShadingSurfaceGroup_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"

#include <utilities/idd/Shading_Site_Detailed_FieldEnums.hxx>
#include <utilities/idd/Shading_Building_Detailed_FieldEnums.hxx>
#include <utilities/idd/Shading_Zone_Detailed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture,ForwardTranslator_ShadingSurface_Site)
{
  Model model;
  ShadingSurfaceGroup shadingSurfaceGroup(model);
  EXPECT_TRUE(shadingSurfaceGroup.setShadingSurfaceType("Site"));

  Point3dVector points;
  points.push_back(Point3d(0,1,0));
  points.push_back(Point3d(1,1,0));
  points.push_back(Point3d(1,0,0));
  points.push_back(Point3d(0,0,0));
    
  ShadingSurface shadingSurface(points, model);
  EXPECT_TRUE(shadingSurface.setShadingSurfaceGroup(shadingSurfaceGroup));

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Shading_Site_Detailed).size());
}

TEST_F(EnergyPlusFixture,ForwardTranslator_ShadingSurface_Building)
{
  Model model;
  ShadingSurfaceGroup shadingSurfaceGroup(model);
  EXPECT_TRUE(shadingSurfaceGroup.setShadingSurfaceType("Building"));

  Point3dVector points;
  points.push_back(Point3d(0,1,0));
  points.push_back(Point3d(1,1,0));
  points.push_back(Point3d(1,0,0));
  points.push_back(Point3d(0,0,0));
    
  ShadingSurface shadingSurface(points, model);
  EXPECT_TRUE(shadingSurface.setShadingSurfaceGroup(shadingSurfaceGroup));

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Shading_Building_Detailed).size());
}

TEST_F(EnergyPlusFixture,ForwardTranslator_ShadingSurface_Space)
{
  Model model;
  
  ThermalZone thermalZone(model);

  Point3dVector points;
  points.push_back(Point3d(0,1,0));
  points.push_back(Point3d(1,1,0));
  points.push_back(Point3d(1,0,0));
  points.push_back(Point3d(0,0,0));

  boost::optional<Space> space = Space::fromFloorPrint(points, 1, model);
  ASSERT_TRUE(space);
  space->setThermalZone(thermalZone);

  ShadingSurfaceGroup shadingSurfaceGroup(model);
  EXPECT_TRUE(shadingSurfaceGroup.setSpace(*space));

  ShadingSurface shadingSurface(points, model);
  EXPECT_TRUE(shadingSurface.setShadingSurfaceGroup(shadingSurfaceGroup));

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  
  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Shading_Zone_Detailed).size());

  WorkspaceObject zoneObject = workspace.getObjectsByType(IddObjectType::Zone)[0];
  WorkspaceObject shadingSurfaceObject = workspace.getObjectsByType(IddObjectType::Shading_Zone_Detailed)[0];

  EXPECT_TRUE(shadingSurfaceObject.getTarget(Shading_Zone_DetailedFields::BaseSurfaceName));
}
