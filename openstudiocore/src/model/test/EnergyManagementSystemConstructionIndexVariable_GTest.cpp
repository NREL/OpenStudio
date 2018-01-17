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

#include "ModelFixture.hpp"
#include "../Model.hpp"
#include "../Model_Impl.hpp"
#include "../Building.hpp"
#include "../Building_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../Component.hpp"
#include "../CFactorUndergroundWallConstruction.hpp"
#include "../CFactorUndergroundWallConstruction_Impl.hpp"
#include "../Construction.hpp"
#include "../Construction_Impl.hpp"
#include "../DefaultConstructionSet.hpp"
#include "../DefaultConstructionSet_Impl.hpp"
#include "../DefaultSurfaceConstructions.hpp"
#include "../DefaultSurfaceConstructions_Impl.hpp"
#include "../DefaultSubSurfaceConstructions.hpp"
#include "../DefaultSubSurfaceConstructions_Impl.hpp"
#include "../EnergyManagementSystemConstructionIndexVariable.hpp"
#include "../ConstructionWithInternalSource.hpp"
#include "../ConstructionWithInternalSource_Impl.hpp"
#include "../FFactorGroundFloorConstruction.hpp"
#include "../FFactorGroundFloorConstruction_Impl.hpp"
#include "../WindowDataFile.hpp"
#include "../WindowDataFile_Impl.hpp"
#include "../StandardsInformationConstruction.hpp"
#include "../StandardsInformationConstruction_Impl.hpp"

#include "../Material.hpp"
#include "../Material_Impl.hpp"
#include "../AirGap.hpp"
#include "../AirWallMaterial.hpp"
#include "../StandardOpaqueMaterial.hpp"
#include "../StandardOpaqueMaterial_Impl.hpp"
#include "../StandardGlazing.hpp"
#include "../Space.hpp"
#include "../Space_Impl.hpp"
#include "../Surface.hpp"
#include "../Surface_Impl.hpp"
#include "../SubSurface.hpp"
#include "../SubSurface_Impl.hpp"
#include "../LifeCycleCost.hpp"

#include "../../utilities/geometry/Point3d.hpp"
#include "../../utilities/idf/IdfFile.hpp"
#include <utilities/idd/OS_Construction_FieldEnums.hxx>
#include <utilities/idd/OS_Material_FieldEnums.hxx>
#include <utilities/idd/OS_Material_AirGap_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, EMS_ConstructionIndexVariable)
{
  Model model;

  // Create some materials
  StandardOpaqueMaterial exterior(model);
  AirGap air(model);
  StandardOpaqueMaterial interior(model);

  OpaqueMaterialVector layers;
  layers.push_back(exterior);
  layers.push_back(air);
  layers.push_back(interior);

  Construction construction(layers);

  EnergyManagementSystemConstructionIndexVariable emsCIV(model, construction);
  //emsCIV.setConstructionObject(construction);
  EXPECT_EQ(construction.handle(), emsCIV.constructionObject().handle());
  model.save(toPath("./EMS_constructiontest.osm"), true);
}
