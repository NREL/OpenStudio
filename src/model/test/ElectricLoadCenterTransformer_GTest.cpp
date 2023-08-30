/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../ElectricLoadCenterTransformer.hpp"
#include "../ElectricLoadCenterTransformer_Impl.hpp"
#include "../ElectricLoadCenterDistribution.hpp"
#include "../ElectricLoadCenterDistribution_Impl.hpp"

#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"
#include "../ScheduleCompact.hpp"
#include "../ScheduleCompact_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../OutputMeter.hpp"
#include "../OutputMeter_Impl.hpp"

#include "../Building.hpp"
#include "../Building_Impl.hpp"
#include "../Model.hpp"
#include "../Model_Impl.hpp"

#include "../../utilities/idd/IddEnums.hpp"
#include "../../utilities/idf/ValidityReport.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/core/Compare.hpp"
#include "../../utilities/core/Optional.hpp"

using namespace openstudio;
using namespace openstudio::model;
using std::string;

TEST_F(ModelFixture, ElectricLoadCenterTransformer_Constructor) {
  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);

  ElectricLoadCenterTransformer elct(model);

  EXPECT_TRUE(elct.availabilitySchedule());
  EXPECT_EQ("PowerOutToGrid", elct.transformerUsage());
  EXPECT_FALSE(elct.isTransformerUsageDefaulted());
  EXPECT_FALSE(elct.zone());
  EXPECT_EQ(0, elct.radiativeFraction());
  EXPECT_TRUE(elct.isRadiativeFractionDefaulted());
  EXPECT_TRUE(elct.ratedCapacity());
  EXPECT_EQ(15000, elct.ratedCapacity().get());
  EXPECT_EQ("3", elct.phase());
  EXPECT_TRUE(elct.isPhaseDefaulted());
  EXPECT_EQ("Aluminum", elct.conductorMaterial());
  EXPECT_TRUE(elct.isConductorMaterialDefaulted());
  EXPECT_EQ(150, elct.fullLoadTemperatureRise());
  EXPECT_TRUE(elct.isFullLoadTemperatureRiseDefaulted());
  EXPECT_EQ(0.1, elct.fractionofEddyCurrentLosses());
  EXPECT_TRUE(elct.isFractionofEddyCurrentLossesDefaulted());
  EXPECT_EQ("NominalEfficiency", elct.performanceInputMethod());
  EXPECT_FALSE(elct.isPerformanceInputMethodDefaulted());
  EXPECT_FALSE(elct.ratedNoLoadLoss());
  EXPECT_FALSE(elct.ratedLoadLoss());
  EXPECT_EQ(0.98, elct.nameplateEfficiency());
  EXPECT_TRUE(elct.isNameplateEfficiencyDefaulted());
  EXPECT_EQ(0.35, elct.perUnitLoadforNameplateEfficiency());
  EXPECT_TRUE(elct.isPerUnitLoadforNameplateEfficiencyDefaulted());
  EXPECT_EQ(75, elct.referenceTemperatureforNameplateEfficiency());
  EXPECT_TRUE(elct.isReferenceTemperatureforNameplateEfficiencyDefaulted());
  EXPECT_FALSE(elct.perUnitLoadforMaximumEfficiency());
  EXPECT_TRUE(elct.considerTransformerLossforUtilityCost());
  EXPECT_TRUE(elct.isConsiderTransformerLossforUtilityCostDefaulted());
  EXPECT_EQ(0u, elct.meters().size());
  EXPECT_FALSE(elct.getMeter(0));

  // model.save(toPath("./ELCT_ctor.osm"), true);
}
TEST_F(ModelFixture, ElectricLoadCenterTransformer_ModelObjects) {
  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);

  //add schedule
  Schedule s = model.alwaysOffDiscreteSchedule();

  ElectricLoadCenterTransformer elct(model);
  //AvailSchedule
  EXPECT_TRUE(elct.setAvailabilitySchedule(s));
  EXPECT_TRUE(elct.availabilitySchedule());
  EXPECT_EQ(s.handle(), elct.availabilitySchedule().get().handle());
  elct.resetAvailabilitySchedule();
  EXPECT_FALSE(elct.availabilitySchedule());
  //Zone
  EXPECT_TRUE(elct.setZone(zone1));
  EXPECT_TRUE(elct.zone());
  EXPECT_EQ(zone1.handle(), elct.zone().get().handle());
  elct.resetZone();
  EXPECT_FALSE(elct.zone());

  // model.save(toPath("./ELCT_modelobject.osm"), true);
}
TEST_F(ModelFixture, ElectricLoadCenterTransformer_Extensible) {
  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);

  // add Meter
  OutputMeter meter(model);
  meter.setName("Electricity:Building");
  OutputMeter meter2(model);
  meter2.setName("Electricity:HVAC");
  OutputMeter meter3(model);
  meter3.setName("Electricity:Fans");

  ElectricLoadCenterTransformer elct(model);

  //add meters
  EXPECT_TRUE(elct.addMeter(meter.nameString()));
  EXPECT_EQ(1u, elct.meters().size());
  EXPECT_TRUE(elct.getMeter(0));
  EXPECT_FALSE(elct.getMeter(1));
  EXPECT_EQ(meter.nameString(), elct.getMeter(0).get());
  //2nd
  EXPECT_TRUE(elct.addMeter(meter2.nameString()));
  EXPECT_EQ(2u, elct.meters().size());
  EXPECT_TRUE(elct.getMeter(0));
  EXPECT_TRUE(elct.getMeter(1));
  EXPECT_FALSE(elct.getMeter(2));
  EXPECT_EQ(meter2.nameString(), elct.getMeter(1).get());
  //3rd
  EXPECT_TRUE(elct.addMeter(meter3.nameString()));
  EXPECT_EQ(3u, elct.meters().size());
  EXPECT_TRUE(elct.getMeter(0));
  EXPECT_TRUE(elct.getMeter(1));
  EXPECT_TRUE(elct.getMeter(2));
  EXPECT_EQ(meter3.nameString(), elct.getMeter(2).get());
  //remove 2nd (0-indexed)
  EXPECT_TRUE(elct.eraseMeter(1));
  EXPECT_EQ(2u, elct.meters().size());
  EXPECT_TRUE(elct.getMeter(0));
  EXPECT_TRUE(elct.getMeter(1));
  EXPECT_FALSE(elct.getMeter(2));
  //meter3 is now in the second slot (0-indexed)
  EXPECT_EQ(meter3.nameString(), elct.getMeter(1).get());
  //remove all
  elct.eraseMeters();
  EXPECT_EQ(0u, elct.meters().size());

  // model.save(toPath("./ELCT_extensible.osm"), true);
}
