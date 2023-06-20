/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
  ASSERT_EQ(1u, idf_zoneProps.size());
  WorkspaceObject idf_zoneProp(idf_zoneProps[0]);

  EXPECT_EQ(2u, idf_zoneProp.numExtensibleGroups());
}
