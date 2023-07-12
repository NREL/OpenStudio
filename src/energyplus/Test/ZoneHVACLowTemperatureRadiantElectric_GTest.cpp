/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/ZoneHVACLowTemperatureRadiantElectric.hpp"
#include "../../model/ZoneHVACLowTemperatureRadiantElectric_Impl.hpp"
#include "../../model/ConstructionWithInternalSource.hpp"
#include "../../model/ConstructionWithInternalSource_Impl.hpp"
#include "../../model/DefaultConstructionSet.hpp"
#include "../../model/DefaultConstructionSet_Impl.hpp"
#include "../../model/DefaultSurfaceConstructions.hpp"
#include "../../model/DefaultSurfaceConstructions_Impl.hpp"
#include "../../model/HVACComponent.hpp"
#include "../../model/HVACComponent_Impl.hpp"
#include "../../model/Model.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/ScheduleConstant_Impl.hpp"
#include "../../model/StandardOpaqueMaterial.hpp"
#include "../../model/StandardOpaqueMaterial_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Space.hpp"
#include <utilities/idd/IddEnums.hxx>

#include <utilities/idd/ZoneHVAC_LowTemperatureRadiant_Electric_FieldEnums.hxx>
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ZoneHVACLowTemperatureRadiantElectric_Set_Flow_Fractions) {
  //make the example model
  Model model = model::exampleModel();

  //loop through all zones and add a radiant system to each one
  for (ThermalZone thermalZone : model.getConcreteModelObjects<ThermalZone>()) {

    //make an electric radiant unit
    ScheduleConstant availabilitySched(model);
    ScheduleConstant heatingControlTemperatureSchedule(model);

    availabilitySched.setValue(1.0);
    heatingControlTemperatureSchedule.setValue(10.0);

    ZoneHVACLowTemperatureRadiantElectric testRad(model, availabilitySched, heatingControlTemperatureSchedule);

    //add it to the thermal zone
    testRad.addToThermalZone(thermalZone);

    //attach to ceilings
    testRad.setRadiantSurfaceType("Ceilings");

    //test that "surfaces" method returns 0 since no
    //ceilings have an internal source construction
    EXPECT_EQ(0, testRad.surfaces().size());
  }

  // Create some materials and make an internal source construction
  StandardOpaqueMaterial exterior(model);
  StandardOpaqueMaterial interior(model);
  OpaqueMaterialVector layers;
  layers.push_back(exterior);
  layers.push_back(interior);
  ConstructionWithInternalSource construction(layers);

  //set building's default ceiling construction to internal source construction
  DefaultConstructionSet defConSet = model.getConcreteModelObjects<DefaultConstructionSet>()[0];
  defConSet.defaultExteriorSurfaceConstructions()->setRoofCeilingConstruction(construction);

  //translate the model to EnergyPlus
  ForwardTranslator trans;
  Workspace workspace = trans.translateModel(model);

  //loop through all zones and check the flow fraction for each surface in the surface group.  it should be 0.25
  for (const ThermalZone& thermalZone : model.getConcreteModelObjects<ThermalZone>()) {

    //get the radiant zone equipment
    for (const ModelObject& equipment : thermalZone.equipment()) {
      if (equipment.optionalCast<ZoneHVACLowTemperatureRadiantElectric>()) {
        ZoneHVACLowTemperatureRadiantElectric testRad = equipment.optionalCast<ZoneHVACLowTemperatureRadiantElectric>().get();
        EXPECT_FALSE(testRad.isMaximumElectricalPowertoPanelDefaulted());
        EXPECT_TRUE(testRad.isMaximumElectricalPowertoPanelAutosized());
        EXPECT_EQ("MeanAirTemperature", testRad.temperatureControlType());
        EXPECT_TRUE(testRad.isSetpointControlTypeDefaulted());
        EXPECT_EQ(2.0, testRad.heatingThrottlingRange());
        for (const IdfExtensibleGroup& extGrp : testRad.extensibleGroups()) {
          EXPECT_EQ(0.25, extGrp.getDouble(1, false).get());
        }
      }
    }
  }
}

TEST_F(EnergyPlusFixture, ZoneHVACLowTemperatureRadiantElectric_Crash_no_constructions) {

  ForwardTranslator ft;

  Model m;

  // make a space with some surfaces
  Point3dVector points{
    {0, 0, 0},
    {0, 1, 0},
    {1, 1, 0},
    {1, 0, 0},
  };

  boost::optional<Space> _space1 = Space::fromFloorPrint(points, 3, m);
  ASSERT_TRUE(_space1);

  // make a zone, add the space
  ThermalZone z(m);
  _space1->setThermalZone(z);

  // make some plant loops
  PlantLoop clg_loop(m);
  PlantLoop htg_loop(m);

  ASSERT_NO_THROW(ft.translateModel(m));

  // Make a radiant low temperature system
  Schedule alwaysOn = m.alwaysOnDiscreteSchedule();

  ScheduleConstant heatingControlTemperatureSchedule(m);
  heatingControlTemperatureSchedule.setValue(10.0);

  ZoneHVACLowTemperatureRadiantElectric testRad(m, alwaysOn, heatingControlTemperatureSchedule);
  EXPECT_TRUE(testRad.addToThermalZone(z));

  ASSERT_NO_THROW(ft.translateModel(m));

  // Test for #2797 - When no surfaces, it shouldn't even be translated or E+ will crash
  EXPECT_TRUE(testRad.surfaces().empty());
  Workspace w = ft.translateModel(m);
  WorkspaceObjectVector idf_rads(w.getObjectsByType(IddObjectType::ZoneHVAC_LowTemperatureRadiant_Electric));
  EXPECT_EQ(0u, idf_rads.size());
}
