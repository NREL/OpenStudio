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
#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/SurfaceControlMovableInsulation.hpp"
#include "../../model/SurfaceControlMovableInsulation_Impl.hpp"
#include "../../model/StandardOpaqueMaterial.hpp"
#include "../../model/StandardOpaqueMaterial_Impl.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/ScheduleConstant_Impl.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/IdfObject.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/SurfaceControl_MovableInsulation_FieldEnums.hxx>

#include <vector>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_SurfaceControlMovableInsulation) {
  Model model;
  ThermalZone thermalZone(model);
  Space space(model);
  space.setThermalZone(thermalZone);
  std::vector<Point3d> vertices;
  vertices.push_back(Point3d());
  vertices.push_back(Point3d(1, 0, 0));
  vertices.push_back(Point3d(1, 1, 0));
  vertices.push_back(Point3d(0, 1, 0));
  Surface surface(vertices, model);
  surface.setSpace(space);
  StandardOpaqueMaterial material(model);
  ScheduleConstant schedule(model);
  openstudio::model::SurfaceControlMovableInsulation mi(surface, material);
  EXPECT_EQ(surface.handle(), mi.surface().handle());
  ASSERT_TRUE(surface.surfaceControlMovableInsulation());
  EXPECT_EQ("Outside", surface.surfaceControlMovableInsulation().get().insulationType());

  mi.setInsulationType("Inside");
  mi.setMaterial(material);
  EXPECT_EQ(material.handle(), mi.material().handle());
  EXPECT_TRUE(!(schedule.handle() == mi.schedule().handle()));
  mi.setSchedule(schedule);
  EXPECT_EQ(schedule.handle(), mi.schedule().handle());

  ForwardTranslator trans;
  Workspace workspace = trans.translateModel(model);

  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::BuildingSurface_Detailed).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Material).size());
  // Always On Discrete, Always Off Discrete, Always On Continuous, Schedule Constant 1
  EXPECT_EQ(4u, workspace.getObjectsByType(IddObjectType::Schedule_Constant).size());

  std::vector<WorkspaceObject> objVector(workspace.getObjectsByType(IddObjectType::SurfaceControl_MovableInsulation));
  ASSERT_EQ(1u, objVector.size());
  WorkspaceObject wo(objVector.at(0));

  EXPECT_EQ("Inside", wo.getString(SurfaceControl_MovableInsulationFields::InsulationType, false).get());

  boost::optional<WorkspaceObject> woSurface(wo.getTarget(SurfaceControl_MovableInsulationFields::SurfaceName));
  EXPECT_TRUE(woSurface);
  if (woSurface) {
    EXPECT_EQ(woSurface->iddObject().type(), IddObjectType::BuildingSurface_Detailed);
  }

  boost::optional<WorkspaceObject> woMaterial(wo.getTarget(SurfaceControl_MovableInsulationFields::MaterialName));
  EXPECT_TRUE(woMaterial);
  if (woMaterial) {
    EXPECT_EQ(woMaterial->iddObject().type(), IddObjectType::Material);
  }

  boost::optional<WorkspaceObject> woSchedule(wo.getTarget(SurfaceControl_MovableInsulationFields::ScheduleName));
  EXPECT_TRUE(woSchedule);
  if (woSchedule) {
    EXPECT_EQ(woSchedule->iddObject().type(), IddObjectType::Schedule_Constant);
  }
}
