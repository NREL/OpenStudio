/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

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
