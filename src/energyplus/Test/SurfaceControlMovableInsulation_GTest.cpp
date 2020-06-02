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
