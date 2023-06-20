/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
