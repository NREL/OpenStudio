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

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/PhotovoltaicPerformanceSandia.hpp"
#include "../../model/PhotovoltaicPerformanceSandia_Impl.hpp"

#include "../../model/GeneratorPhotovoltaic.hpp"

#include "../../model/Model.hpp"
#include "../../model/ShadingSurfaceGroup.hpp"
#include "../../model/ShadingSurface.hpp"

#include "../../utilities/geometry/Point3d.hpp"

#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/IdfObject_Impl.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/WorkspaceObject_Impl.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/PhotovoltaicPerformance_Sandia_FieldEnums.hxx>
#include <utilities/idd/Generator_Photovoltaic_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/Schedule_Constant_FieldEnums.hxx>
#include <utilities/idd/GlobalGeometryRules_FieldEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_PhotovoltaicPerformanceSandia) {

  ForwardTranslator ft;

  Model m;

  // Not linked to a Generator:Phovoltaic => not translated
  {
    auto perf = PhotovoltaicPerformanceSandia(m);
    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idf_perfs(w.getObjectsByType(IddObjectType::PhotovoltaicPerformance_Sandia));
    ASSERT_EQ(0u, idf_perfs.size());
    perf.remove();
  }


  auto perfNames = PhotovoltaicPerformanceSandia::sandiaModulePerformanceNames();
  ASSERT_GT(perfNames.size(), 5);
  auto perfName = perfNames[5];

  GeneratorPhotovoltaic panel = GeneratorPhotovoltaic::fromSandiaDatabase(m, perfName);
  PhotovoltaicPerformanceSandia sandiaPerf = panel.photovoltaicPerformance().cast<PhotovoltaicPerformanceSandia>();

  // Make a ShadingSurface, and a ShadingSurfaceGroup as it's needed
  Point3dVector points;
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(1, 0, 0));
  points.push_back(Point3d(1, 1, 0));
  ShadingSurface shadingSurface(points, m);

  ShadingSurfaceGroup shadingSurfaceGroup(m);
  EXPECT_TRUE(shadingSurface.setShadingSurfaceGroup(shadingSurfaceGroup));

  EXPECT_TRUE(panel.setSurface(shadingSurface));
  EXPECT_TRUE(panel.surface());

  Workspace w = ft.translateModel(m);

  WorkspaceObjectVector idf_panels(w.getObjectsByType(IddObjectType::Generator_Photovoltaic));
  ASSERT_EQ(1u, idf_panels.size());
  WorkspaceObject idf_panel(idf_panels[0]);

  WorkspaceObjectVector idf_perfs(w.getObjectsByType(IddObjectType::PhotovoltaicPerformance_Sandia));
  ASSERT_EQ(1u, idf_perfs.size());

  EXPECT_EQ("PhotovoltaicPerformance:Sandia", idf_panel.getString(Generator_PhotovoltaicFields::PhotovoltaicPerformanceObjectType).get());
  ASSERT_TRUE(idf_panel.getTarget(Generator_PhotovoltaicFields::ModulePerformanceName));
  WorkspaceObject idf_perf = idf_panel.getTarget(Generator_PhotovoltaicFields::ModulePerformanceName).get();

}
