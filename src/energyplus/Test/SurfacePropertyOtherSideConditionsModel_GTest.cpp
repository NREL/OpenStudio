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

#include "../../model/SurfacePropertyOtherSideConditionsModel.hpp"
#include "../../model/SurfacePropertyOtherSideConditionsModel_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Surface.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Model.hpp"

#include "../../utilities/geometry/Point3d.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"

// E+ FieldEnums
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/SurfaceProperty_OtherSideConditionsModel_FieldEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_SurfacePropertyOtherSideConditionsModel) {

  ForwardTranslator ft;

  Model m;

  SurfacePropertyOtherSideConditionsModel oscms(m);
  oscms.setName("My Props");
  EXPECT_TRUE(oscms.setTypeOfModeling("GroundCoupledSurface"));

  // Not used = not translated
  {
    const Workspace w = ft.translateModel(m);
    EXPECT_EQ(0u, w.getObjectsByType(IddObjectType::SurfaceProperty_OtherSideConditionsModel).size());
  }

  constexpr double width = 10.0;
  constexpr double height = 3.0;
  // Counterclockwise points
  const std::vector<Point3d> floorPointsSpace1{{0.0, 0.0, 0.0}, {0.0, width, 0.0}, {width, width, 0.0}, {width, 0.0, 0.0}};

  auto space = Space::fromFloorPrint(floorPointsSpace1, height, m).get();
  ThermalZone z(m);
  EXPECT_TRUE(space.setThermalZone(z));
  auto roof = [&space]() -> Surface {
    auto sfs = space.surfaces();
    auto it = std::find_if(sfs.cbegin(), sfs.cend(), [](const auto& s) { return s.surfaceType() == "RoofCeiling"; });
    OS_ASSERT(it != sfs.cend());
    return *it;
  }();

  roof.setSurfacePropertyOtherSideConditionsModel(oscms);

  {
    const Workspace w = ft.translateModel(m);
    const auto idfObjs = w.getObjectsByType(IddObjectType::SurfaceProperty_OtherSideConditionsModel);
    ASSERT_EQ(1u, idfObjs.size());
    const auto& idfObject = idfObjs.front();

    EXPECT_EQ("My Props", idfObject.getString(SurfaceProperty_OtherSideConditionsModelFields::Name).get());
    EXPECT_EQ("GroundCoupledSurface", idfObject.getString(SurfaceProperty_OtherSideConditionsModelFields::TypeofModeling).get());
  }
}

TEST_F(EnergyPlusFixture, ReverseTranslator_SurfacePropertyOtherSideConditionsModel) {

  ReverseTranslator rt;

  Workspace w(StrictnessLevel::Minimal, IddFileType::EnergyPlus);

  auto woProps = w.addObject(IdfObject(IddObjectType::SurfaceProperty_OtherSideConditionsModel)).get();
  woProps.setName("My Props");
  EXPECT_TRUE(woProps.setString(SurfaceProperty_OtherSideConditionsModelFields::TypeofModeling, "GroundCoupledSurface"));

  const Model m = rt.translateWorkspace(w);

  const auto oscms = m.getConcreteModelObjects<SurfacePropertyOtherSideConditionsModel>();
  ASSERT_EQ(1, oscms.size());
  const auto& oscm = oscms.front();

  EXPECT_EQ("My Props", oscm.nameString());
  EXPECT_EQ("GroundCoupledSurface", oscm.typeOfModeling());
}
