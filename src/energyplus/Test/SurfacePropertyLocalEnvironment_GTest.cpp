/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../../model/ThermalZone.hpp"
#include "../../model/Space.hpp"
#include "../../model/SurfacePropertyLocalEnvironment.hpp"
#include "../../model/SurfacePropertySurroundingSurfaces.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/Surface.hpp"
#include "../../model/SubSurface.hpp"

#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/geometry/Point3d.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/SurfaceProperty_LocalEnvironment_FieldEnums.hxx>

#include <vector>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_SurfacePropertyLocalEnvironment) {

  Model m;
  ThermalZone tz(m);
  Space space(m);
  EXPECT_TRUE(space.setThermalZone(tz));

  std::vector<Point3d> sPoints{
    {0, 2, 0},
    {0, 0, 0},
    {2, 0, 0},
    {2, 2, 0},
  };
  Surface surface(sPoints, m);
  surface.setSpace(space);

  std::vector<Point3d> ssPoints{
    {0, 1, 0},
    {0, 0, 0},
    {1, 0, 0},
    {1, 1, 0},
  };

  SubSurface subSurface(ssPoints, m);
  subSurface.setSurface(surface);
  subSurface.assignDefaultSubSurfaceType();

  SurfacePropertyLocalEnvironment localEnv(surface);
  localEnv.setName(surface.nameString() + " LocalEnv");
  EXPECT_EQ(surface, localEnv.exteriorSurface());
  ASSERT_TRUE(localEnv.exteriorSurfaceAsSurface());
  EXPECT_EQ(surface, localEnv.exteriorSurfaceAsSurface().get());

  ScheduleConstant shadingSch(m);
  shadingSch.setName("External Shading Fraction Schedule");
  shadingSch.setValue(0.5);
  EXPECT_TRUE(localEnv.setExternalShadingFractionSchedule(shadingSch));
  ASSERT_TRUE(localEnv.externalShadingFractionSchedule());
  EXPECT_EQ(shadingSch, localEnv.externalShadingFractionSchedule().get());

  SurfacePropertySurroundingSurfaces sp(m);
  sp.setName("SurfaceProperty Surrounding Surfaces");

  EXPECT_TRUE(localEnv.setSurfacePropertySurroundingSurfaces(sp));
  ASSERT_TRUE(localEnv.surfacePropertySurroundingSurfaces());
  EXPECT_EQ(sp, localEnv.surfacePropertySurroundingSurfaces().get());
  ScheduleConstant tempSch(m);
  tempSch.setName("External Surface Temperature Schedule");
  tempSch.setValue(23.5);
  EXPECT_TRUE(sp.addSurroundingSurfaceGroup("External Surface 1", 0.4, tempSch));
  EXPECT_EQ(1, sp.numberofSurroundingSurfaceGroups());

  ForwardTranslator ft;

  {
    // Assigned to a Surface
    Workspace w = ft.translateModel(m);

    std::vector<WorkspaceObject> idfObjs = w.getObjectsByType(IddObjectType::SurfaceProperty_LocalEnvironment);
    ASSERT_EQ(1, idfObjs.size());
    auto& wo_localEnv = idfObjs[0];
    EXPECT_EQ(localEnv.nameString(), wo_localEnv.nameString());

    auto wo_extSf_ = wo_localEnv.getTarget(SurfaceProperty_LocalEnvironmentFields::ExteriorSurfaceName);
    ASSERT_TRUE(wo_extSf_);
    EXPECT_EQ(wo_extSf_->iddObject().type(), IddObjectType::BuildingSurface_Detailed);

    EXPECT_EQ(shadingSch.nameString(), wo_localEnv.getString(SurfaceProperty_LocalEnvironmentFields::ExternalShadingFractionScheduleName).get());

    EXPECT_EQ(sp.nameString(), wo_localEnv.getString(SurfaceProperty_LocalEnvironmentFields::SurroundingSurfacesObjectName).get());
    auto wo_sp_ = wo_localEnv.getTarget(SurfaceProperty_LocalEnvironmentFields::SurroundingSurfacesObjectName);
    ASSERT_TRUE(wo_sp_);
    EXPECT_EQ(wo_sp_->iddObject().type(), IddObjectType::SurfaceProperty_SurroundingSurfaces);
  }

  EXPECT_TRUE(localEnv.setExteriorSurface(subSurface));
  EXPECT_EQ(subSurface, localEnv.exteriorSurface());
  ASSERT_TRUE(localEnv.exteriorSurfaceAsSubSurface());
  EXPECT_EQ(subSurface, localEnv.exteriorSurfaceAsSubSurface().get());

  {
    // Assigned to a SubSurface
    Workspace w = ft.translateModel(m);

    std::vector<WorkspaceObject> idfObjs = w.getObjectsByType(IddObjectType::SurfaceProperty_LocalEnvironment);
    ASSERT_EQ(1, idfObjs.size());
    auto& wo_localEnv = idfObjs[0];
    EXPECT_EQ(localEnv.nameString(), wo_localEnv.nameString());

    auto wo_extSf_ = wo_localEnv.getTarget(SurfaceProperty_LocalEnvironmentFields::ExteriorSurfaceName);
    ASSERT_TRUE(wo_extSf_);
    EXPECT_EQ(wo_extSf_->iddObject().type(), IddObjectType::FenestrationSurface_Detailed);

    EXPECT_EQ(shadingSch.nameString(), wo_localEnv.getString(SurfaceProperty_LocalEnvironmentFields::ExternalShadingFractionScheduleName).get());

    EXPECT_EQ(sp.nameString(), wo_localEnv.getString(SurfaceProperty_LocalEnvironmentFields::SurroundingSurfacesObjectName).get());
    auto wo_sp_ = wo_localEnv.getTarget(SurfaceProperty_LocalEnvironmentFields::SurroundingSurfacesObjectName);
    ASSERT_TRUE(wo_sp_);
    EXPECT_EQ(wo_sp_->iddObject().type(), IddObjectType::SurfaceProperty_SurroundingSurfaces);
  }
}
