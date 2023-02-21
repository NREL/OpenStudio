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

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Space.hpp"
#include "../../model/Surface.hpp"
#include "../../model/SubSurface.hpp"
#include "../../model/DaylightingDeviceLightWell.hpp"
#include "../../model/DaylightingDeviceLightWell_Impl.hpp"

#include "../../utilities/geometry/Point3d.hpp"
#include <utilities/idd/DaylightingDevice_LightWell_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_DaylightingDeviceLightWell) {
  Model model;
  ThermalZone zone(model);
  Space space(model);
  space.setThermalZone(zone);

  Point3dVector points{
    {0, 10, 3},
    {0, 10, 0},
    {0, 0, 0},
    {0, 0, 3},
  };
  Surface surface(points, model);
  surface.setSpace(space);
  EXPECT_EQ("Wall", surface.surfaceType());

  Point3dVector points2{
    {0, 0, 1},
    {0, 0, 0},
    {0, 1, 0},
    {0, 1, 1},
  };
  SubSurface window(points2, model);
  window.setSubSurfaceType("FixedWindow");
  window.setSurface(surface);

  EXPECT_FALSE(window.daylightingDeviceLightWell());
  DaylightingDeviceLightWell lightWell(window, 1, 2, 3, 0.5);
  EXPECT_TRUE(window.daylightingDeviceLightWell());
  EXPECT_EQ(1u, model.getConcreteModelObjects<DaylightingDeviceLightWell>().size());

  ForwardTranslator ft;
  Workspace w = ft.translateModel(model);

  std::vector<WorkspaceObject> wos = w.getObjectsByType(IddObjectType::DaylightingDevice_LightWell);
  ASSERT_EQ(1u, wos.size());
  WorkspaceObject wo(wos[0]);

  boost::optional<WorkspaceObject> idf_window(wo.getTarget(DaylightingDevice_LightWellFields::ExteriorWindowName));
  ASSERT_TRUE(idf_window);
  EXPECT_EQ(window.nameString(), idf_window->nameString());

  EXPECT_EQ(1, wo.getDouble(DaylightingDevice_LightWellFields::HeightofWell, false).get());
  EXPECT_EQ(2, wo.getDouble(DaylightingDevice_LightWellFields::PerimeterofBottomofWell, false).get());
  EXPECT_EQ(3, wo.getDouble(DaylightingDevice_LightWellFields::AreaofBottomofWell, false).get());
  EXPECT_EQ(0.5, wo.getDouble(DaylightingDevice_LightWellFields::VisibleReflectanceofWellWalls, false).get());
}
