/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../ErrorFile.hpp"
#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/Space.hpp"
#include "../../model/ZonePropertyUserViewFactorsBySurfaceName.hpp"
#include "../../model/Surface.hpp"
#include "../../model/InternalMass.hpp"
#include "../../model/InternalMassDefinition.hpp"
#include "../../model/ThermalZone.hpp"

#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/ZoneProperty_UserViewFactors_BySurfaceName_FieldEnums.hxx>

#include <vector>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_ZonePropertyUserViewFactorsBySurfaceName) {
  Model model;
  ThermalZone zone(model);

  Space space(model);
  space.setThermalZone(zone);

  InternalMassDefinition intMassDefn(model);
  intMassDefn.setSurfaceArea(20);
  InternalMass intMass(intMassDefn);
  intMass.setSpace(space);

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d());
  vertices.push_back(Point3d(1, 0, 0));
  vertices.push_back(Point3d(1, 1, 0));
  vertices.push_back(Point3d(0, 1, 0));

  Surface surface(vertices, model);
  surface.setSurfaceType("Floor");
  surface.setSpace(space);
  surface.setOutsideBoundaryCondition("Outdoors");

  ZonePropertyUserViewFactorsBySurfaceName zoneProp = zone.getZonePropertyUserViewFactorsBySurfaceName();
  EXPECT_TRUE(zoneProp.addViewFactor(surface, intMass, 0.5));
  EXPECT_TRUE(zoneProp.addViewFactor(intMass, surface, 0.25));

  ForwardTranslator ft;

  Workspace workspace = ft.translateModel(model);

  std::vector<WorkspaceObject> idf_zoneProps = workspace.getObjectsByType(IddObjectType::ZoneProperty_UserViewFactors_BySurfaceName);
  ASSERT_EQ(idf_zoneProps.size(), 1);
  WorkspaceObject idf_zoneProp(idf_zoneProps[0]);

  EXPECT_EQ(2u, idf_zoneProp.numExtensibleGroups());
}
