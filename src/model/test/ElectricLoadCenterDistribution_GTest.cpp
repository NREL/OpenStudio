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
#include "ModelFixture.hpp"
#include "../Schedule.hpp"
#include "../ScheduleCompact.hpp"
#include "../ElectricLoadCenterDistribution.hpp"
#include "../GeneratorPhotovoltaic.hpp"
//#include "../GeneratorMicroTurbine.hpp" // In a different branch right now...
#include "../ElectricLoadCenterInverterSimple.hpp"
#include "../ElectricLoadCenterInverterLookUpTable.hpp"
#include "../ElectricLoadCenterStorageSimple.hpp"
#include "../ElectricLoadCenterTransformer.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture, ElectricLoadCenterDistribution) {
  Model model;

  ElectricLoadCenterDistribution elcd(model);
  EXPECT_EQ(0u, elcd.generators().size());
  EXPECT_EQ("Baseload", elcd.generatorOperationSchemeType());
  EXPECT_TRUE(elcd.isGeneratorOperationSchemeTypeDefaulted());
  EXPECT_EQ("DirectCurrentWithInverter", elcd.electricalBussType());
  EXPECT_TRUE(elcd.isElectricalBussTypeDefaulted());
  EXPECT_FALSE(elcd.inverter());

  EXPECT_TRUE(elcd.setGeneratorOperationSchemeType("Baseload"));
  EXPECT_EQ("Baseload", elcd.generatorOperationSchemeType());
  EXPECT_FALSE(elcd.isGeneratorOperationSchemeTypeDefaulted());
  elcd.resetGeneratorOperationSchemeType();

  GeneratorPhotovoltaic panel1 = GeneratorPhotovoltaic::simple(model);
  GeneratorPhotovoltaic panel2 = GeneratorPhotovoltaic::simple(model);
  EXPECT_TRUE(elcd.addGenerator(panel1));
  ASSERT_EQ(1u, elcd.generators().size());
  EXPECT_EQ(panel1.handle(), elcd.generators()[0].handle());
  EXPECT_TRUE(elcd.addGenerator(panel2));
  ASSERT_EQ(2u, elcd.generators().size());
  EXPECT_EQ(panel1.handle(), elcd.generators()[0].handle());
  EXPECT_EQ(panel2.handle(), elcd.generators()[1].handle());
  EXPECT_TRUE(elcd.removeGenerator(panel1));
  ASSERT_EQ(1u, elcd.generators().size());
  EXPECT_EQ(panel2.handle(), elcd.generators()[0].handle());
  EXPECT_TRUE(elcd.removeGenerator(panel2));
  ASSERT_EQ(0u, elcd.generators().size());

  // can't remove something not already there
  EXPECT_FALSE(elcd.removeGenerator(panel2));

  // can't add something twice
  EXPECT_TRUE(elcd.addGenerator(panel1));
  EXPECT_FALSE(elcd.addGenerator(panel1));

  ElectricLoadCenterInverterSimple inverter(model);
  EXPECT_TRUE(inverter.name());
  EXPECT_TRUE(elcd.setInverter(inverter));
  ASSERT_TRUE(elcd.inverter());
  EXPECT_EQ(inverter.handle(), elcd.inverter()->handle());
  elcd.resetInverter();
}

TEST_F(ModelFixture, ElectricLoadCenterDistribution_Panels) {
  Model model;

  ElectricLoadCenterDistribution elcd(model);
  GeneratorPhotovoltaic panel1 = GeneratorPhotovoltaic::simple(model);
  GeneratorPhotovoltaic panel2 = GeneratorPhotovoltaic::simple(model);

  EXPECT_TRUE(elcd.addGenerator(panel1));
  EXPECT_TRUE(elcd.addGenerator(panel2));
  ASSERT_EQ(2u, elcd.generators().size());
  EXPECT_EQ(panel1.handle(), elcd.generators()[0].handle());
  EXPECT_EQ(panel2.handle(), elcd.generators()[1].handle());
  EXPECT_TRUE(panel1.electricLoadCenterDistribution());
  EXPECT_TRUE(panel2.electricLoadCenterDistribution());

  panel2.remove();

  ASSERT_EQ(1u, elcd.generators().size());
  EXPECT_EQ(panel1.handle(), elcd.generators()[0].handle());

  panel1.remove();

  EXPECT_EQ(0, elcd.generators().size());
}

TEST_F(ModelFixture, ElectricLoadCenterDistribution_Panels2) {
  Model model;

  ElectricLoadCenterDistribution elcd1(model);
  ElectricLoadCenterDistribution elcd2(model);
  GeneratorPhotovoltaic panel = GeneratorPhotovoltaic::simple(model);

  EXPECT_TRUE(elcd1.addGenerator(panel));
  EXPECT_EQ(1u, elcd1.generators().size());
  EXPECT_EQ(0, elcd2.generators().size());
  ASSERT_TRUE(panel.electricLoadCenterDistribution());
  EXPECT_EQ(elcd1.handle(), panel.electricLoadCenterDistribution()->handle());

  EXPECT_TRUE(elcd2.addGenerator(panel));
  EXPECT_EQ(0, elcd1.generators().size());
  EXPECT_EQ(1u, elcd2.generators().size());
  ASSERT_TRUE(panel.electricLoadCenterDistribution());
  EXPECT_EQ(elcd2.handle(), panel.electricLoadCenterDistribution()->handle());

  elcd2.remove();
  EXPECT_FALSE(panel.electricLoadCenterDistribution());
}

TEST_F(ModelFixture, ElectricLoadCenterDistribution_Inverters) {
  Model model;

  ElectricLoadCenterDistribution elcd(model);
  ElectricLoadCenterInverterSimple inverter1(model);
  ElectricLoadCenterInverterLookUpTable inverter2(model);

  EXPECT_FALSE(elcd.inverter());
  EXPECT_FALSE(inverter1.electricLoadCenterDistribution());
  EXPECT_FALSE(inverter2.electricLoadCenterDistribution());

  EXPECT_TRUE(elcd.setInverter(inverter1));
  ASSERT_TRUE(elcd.inverter());
  EXPECT_EQ(inverter1.handle(), elcd.inverter()->handle());
  EXPECT_TRUE(inverter1.electricLoadCenterDistribution());
  EXPECT_FALSE(inverter2.electricLoadCenterDistribution());

  inverter1.remove();

  EXPECT_FALSE(elcd.inverter());
  EXPECT_FALSE(inverter2.electricLoadCenterDistribution());

  EXPECT_TRUE(elcd.setInverter(inverter2));
  ASSERT_TRUE(elcd.inverter());
  EXPECT_EQ(inverter2.handle(), elcd.inverter()->handle());
  EXPECT_TRUE(inverter2.electricLoadCenterDistribution());

  elcd.remove();
  EXPECT_FALSE(inverter2.electricLoadCenterDistribution());
}

TEST_F(ModelFixture, ElectricLoadCenterDistribution_Inverters2) {
  Model model;

  ElectricLoadCenterDistribution elcd1(model);
  ElectricLoadCenterDistribution elcd2(model);
  ElectricLoadCenterInverterSimple inverter(model);

  EXPECT_FALSE(elcd1.inverter());
  EXPECT_FALSE(elcd1.inverter());
  EXPECT_FALSE(inverter.electricLoadCenterDistribution());

  EXPECT_TRUE(elcd1.setInverter(inverter));
  EXPECT_TRUE(elcd1.inverter());
  EXPECT_FALSE(elcd2.inverter());
  ASSERT_TRUE(inverter.electricLoadCenterDistribution());
  EXPECT_EQ(elcd1.handle(), inverter.electricLoadCenterDistribution()->handle());

  EXPECT_TRUE(elcd2.setInverter(inverter));
  EXPECT_FALSE(elcd1.inverter());
  EXPECT_TRUE(elcd2.inverter());
  ASSERT_TRUE(inverter.electricLoadCenterDistribution());
  EXPECT_EQ(elcd2.handle(), inverter.electricLoadCenterDistribution()->handle());
}

TEST_F(ModelFixture, ElectricLoadCenterDistribution_newFields) {

  Model model;

  ElectricLoadCenterDistribution elcd(model);
  EXPECT_EQ(0u, elcd.generators().size());
  EXPECT_EQ("Baseload", elcd.generatorOperationSchemeType());
  EXPECT_TRUE(elcd.isGeneratorOperationSchemeTypeDefaulted());
  EXPECT_EQ("DirectCurrentWithInverter", elcd.electricalBussType());
  EXPECT_TRUE(elcd.isElectricalBussTypeDefaulted());
  EXPECT_FALSE(elcd.inverter());

  EXPECT_TRUE(elcd.setGeneratorOperationSchemeType("FollowThermal"));
  EXPECT_EQ("FollowThermal", elcd.generatorOperationSchemeType());
  EXPECT_FALSE(elcd.isGeneratorOperationSchemeTypeDefaulted());
  elcd.resetGeneratorOperationSchemeType();
  EXPECT_TRUE(elcd.isGeneratorOperationSchemeTypeDefaulted());

  /* GeneratorMicroTurbine mt(model);
  GeneratorPhotovoltaic pv(model);
  EXPECT_TRUE(elcd.addGenerator(mt));
  ASSERT_EQ(1u, elcd.generators().size());
  EXPECT_EQ(mt.handle(), elcd.generators()[0].handle());
  EXPECT_TRUE(elcd.addGenerator(pv));
  ASSERT_EQ(2u, elcd.generators().size());
  EXPECT_EQ(mt.handle(), elcd.generators()[0].handle());
  EXPECT_EQ(pv.handle(), elcd.generators()[1].handle());
  EXPECT_TRUE(elcd.removeGenerator(mt));
  ASSERT_EQ(1u, elcd.generators().size());
  EXPECT_EQ(panel2.handle(), elcd.generators()[0].handle());
  EXPECT_TRUE(elcd.removeGenerator(pv));
  ASSERT_EQ(0u, elcd.generators().size());

  // can't remove something not already there
  EXPECT_FALSE(elcd.removeGenerator(pv));

  // can't add something twice
  EXPECT_TRUE(elcd.addGenerator(mt));
  EXPECT_FALSE(elcd.addGenerator(mt)); */

  GeneratorPhotovoltaic panel1 = GeneratorPhotovoltaic::simple(model);
  GeneratorPhotovoltaic panel2 = GeneratorPhotovoltaic::simple(model);
  EXPECT_TRUE(elcd.addGenerator(panel1));
  ASSERT_EQ(1u, elcd.generators().size());
  EXPECT_EQ(panel1.handle(), elcd.generators()[0].handle());
  EXPECT_TRUE(elcd.addGenerator(panel2));
  ASSERT_EQ(2u, elcd.generators().size());
  EXPECT_EQ(panel1.handle(), elcd.generators()[0].handle());
  EXPECT_EQ(panel2.handle(), elcd.generators()[1].handle());
  EXPECT_TRUE(elcd.removeGenerator(panel1));
  ASSERT_EQ(1u, elcd.generators().size());
  EXPECT_EQ(panel2.handle(), elcd.generators()[0].handle());
  EXPECT_TRUE(elcd.removeGenerator(panel2));
  ASSERT_EQ(0u, elcd.generators().size());

  // can't remove something not already there
  EXPECT_FALSE(elcd.removeGenerator(panel1));

  // can't add something twice
  EXPECT_TRUE(elcd.addGenerator(panel1));
  EXPECT_FALSE(elcd.addGenerator(panel1));

  // Demand Limit Scheme Purchased Electric Demand Limit, Optional
  EXPECT_FALSE(elcd.demandLimitSchemePurchasedElectricDemandLimit());
  EXPECT_TRUE(elcd.setDemandLimitSchemePurchasedElectricDemandLimit(100500));
  ASSERT_TRUE(elcd.demandLimitSchemePurchasedElectricDemandLimit());
  EXPECT_EQ(100500, elcd.demandLimitSchemePurchasedElectricDemandLimit().get());
  elcd.resetDemandLimitSchemePurchasedElectricDemandLimit();
  ASSERT_FALSE(elcd.demandLimitSchemePurchasedElectricDemandLimit());

  // Generator Track Schedule Name Scheme Schedule Name, Optional
  Schedule sch = model.alwaysOnDiscreteSchedule();
  EXPECT_FALSE(elcd.trackScheduleSchemeSchedule());
  EXPECT_TRUE(elcd.setTrackScheduleSchemeSchedule(sch));
  ASSERT_TRUE(elcd.trackScheduleSchemeSchedule());
  EXPECT_EQ(sch.handle(), (elcd.trackScheduleSchemeSchedule())->handle());
  elcd.resetTrackScheduleSchemeSchedule();
  ASSERT_FALSE(elcd.trackScheduleSchemeSchedule());

  // Generator Track Meter Scheme Meter Name, Optional String
  EXPECT_FALSE(elcd.trackMeterSchemeMeterName());
  EXPECT_TRUE(elcd.setTrackMeterSchemeMeterName("Electricity:Facility"));
  ASSERT_TRUE(elcd.trackMeterSchemeMeterName());
  ASSERT_EQ("Electricity:Facility", elcd.trackMeterSchemeMeterName().get());
  elcd.resetTrackMeterSchemeMeterName();
  ASSERT_FALSE(elcd.trackMeterSchemeMeterName());

  // Electrical Buss type, defaults to DirectCurrentWithInverter
  EXPECT_EQ("DirectCurrentWithInverter", elcd.electricalBussType());
  EXPECT_TRUE(elcd.isElectricalBussTypeDefaulted());
  EXPECT_TRUE(elcd.setElectricalBussType("AlternatingCurrentWithStorage"));
  ASSERT_FALSE(elcd.isElectricalBussTypeDefaulted());
  ASSERT_EQ("AlternatingCurrentWithStorage", elcd.electricalBussType());
  elcd.resetElectricalBussType();
  ASSERT_TRUE(elcd.isElectricalBussTypeDefaulted());

  // Test inverter
  ElectricLoadCenterInverterSimple inverter(model);
  EXPECT_TRUE(inverter.name());
  EXPECT_TRUE(elcd.setInverter(inverter));
  ASSERT_TRUE(elcd.inverter());
  EXPECT_EQ(inverter.handle(), elcd.inverter()->handle());
  elcd.resetInverter();
  ASSERT_FALSE(elcd.inverter());

  // Test Storage
  ElectricLoadCenterStorageSimple battery(model);
  ASSERT_FALSE(elcd.electricalStorage());
  EXPECT_TRUE(battery.name());
  EXPECT_TRUE(elcd.setElectricalStorage(battery));
  ASSERT_TRUE(elcd.electricalStorage());
  EXPECT_EQ(battery.handle(), elcd.electricalStorage()->handle());
  elcd.resetElectricalStorage();
  ASSERT_FALSE(elcd.electricalStorage());

  // Test Transformer:
  ElectricLoadCenterTransformer elct(model);
  EXPECT_FALSE(elcd.transformer());
  EXPECT_TRUE(elcd.setTransformer(elct));
  EXPECT_TRUE(elcd.transformer());
  EXPECT_EQ(elct.handle(), elcd.transformer()->handle());
  elcd.resetTransformer();
  EXPECT_FALSE(elcd.transformer());

  // Storage Operation Scheme, defaults
  EXPECT_TRUE(elcd.isStorageOperationSchemeDefaulted());
  EXPECT_EQ("TrackFacilityElectricDemandStoreExcessOnSite", elcd.storageOperationScheme());
  ASSERT_TRUE(elcd.setStorageOperationScheme("FacilityDemandLeveling"));
  ASSERT_FALSE(elcd.isStorageOperationSchemeDefaulted());
  ASSERT_EQ("FacilityDemandLeveling", elcd.storageOperationScheme());
  elcd.resetStorageOperationScheme();
  ASSERT_TRUE(elcd.isStorageOperationSchemeDefaulted());

  // Storage Control Track Meter Name, Optional String
  EXPECT_FALSE(elcd.storageControlTrackMeterName());
  ASSERT_TRUE(elcd.setStorageControlTrackMeterName("Electricity:Building"));
  ASSERT_TRUE(elcd.storageControlTrackMeterName());
  ASSERT_EQ("Electricity:Building", elcd.storageControlTrackMeterName().get());
  elcd.resetStorageControlTrackMeterName();
  EXPECT_FALSE(elcd.storageControlTrackMeterName());

  // Storage Converter: not yet

  // Maximum Storage State of Charge Fraction, defaults
  EXPECT_TRUE(elcd.isMaximumStorageStateofChargeFractionDefaulted());
  EXPECT_EQ(1.0, elcd.maximumStorageStateofChargeFraction());
  ASSERT_TRUE(elcd.setMaximumStorageStateofChargeFraction(0.85));
  ASSERT_FALSE(elcd.isMaximumStorageStateofChargeFractionDefaulted());
  ASSERT_EQ(0.85, elcd.maximumStorageStateofChargeFraction());
  elcd.resetMaximumStorageStateofChargeFraction();
  ASSERT_TRUE(elcd.isMaximumStorageStateofChargeFractionDefaulted());

  // Minimum Storage State of Charge Fraction, defaults
  EXPECT_TRUE(elcd.isMinimumStorageStateofChargeFractionDefaulted());
  EXPECT_EQ(0.0, elcd.minimumStorageStateofChargeFraction());
  ASSERT_TRUE(elcd.setMinimumStorageStateofChargeFraction(0.05));
  ASSERT_FALSE(elcd.isMinimumStorageStateofChargeFractionDefaulted());
  ASSERT_EQ(0.05, elcd.minimumStorageStateofChargeFraction());
  elcd.resetMinimumStorageStateofChargeFraction();
  ASSERT_TRUE(elcd.isMinimumStorageStateofChargeFractionDefaulted());

  // Design Storage Control Charge Power, Optional Double
  EXPECT_FALSE(elcd.designStorageControlChargePower());
  EXPECT_TRUE(elcd.setDesignStorageControlChargePower(100000));
  ASSERT_TRUE(elcd.designStorageControlChargePower());
  EXPECT_EQ(100000, elcd.designStorageControlChargePower().get());
  elcd.resetDesignStorageControlChargePower();
  ASSERT_FALSE(elcd.designStorageControlChargePower());

  // Storage Charge Power Fraction Schedule Name, optional Schedule
  ScheduleCompact schChP = ScheduleCompact(model);
  EXPECT_FALSE(elcd.storageChargePowerFractionSchedule());
  EXPECT_TRUE(elcd.setStorageChargePowerFractionSchedule(schChP));
  ASSERT_TRUE(elcd.storageChargePowerFractionSchedule());
  EXPECT_EQ(schChP.handle(), elcd.storageChargePowerFractionSchedule()->handle());
  elcd.resetStorageChargePowerFractionSchedule();
  ASSERT_FALSE(elcd.storageChargePowerFractionSchedule());

  // Design Storage Control Discharge Power, Optional Double
  EXPECT_FALSE(elcd.designStorageControlDischargePower());
  EXPECT_TRUE(elcd.setDesignStorageControlDischargePower(100000));
  ASSERT_TRUE(elcd.designStorageControlDischargePower());
  EXPECT_EQ(100000, elcd.designStorageControlDischargePower().get());
  elcd.resetDesignStorageControlDischargePower();
  ASSERT_FALSE(elcd.designStorageControlDischargePower());

  // Storage Discharge Power Fraction Schedule Name, optional Schedule
  ScheduleCompact schDisChP = ScheduleCompact(model);
  EXPECT_FALSE(elcd.storageDischargePowerFractionSchedule());
  EXPECT_TRUE(elcd.setStorageDischargePowerFractionSchedule(schDisChP));
  ASSERT_TRUE(elcd.storageDischargePowerFractionSchedule());
  EXPECT_EQ(schDisChP.handle(), elcd.storageDischargePowerFractionSchedule()->handle());
  elcd.resetStorageDischargePowerFractionSchedule();
  ASSERT_FALSE(elcd.storageDischargePowerFractionSchedule());

  // Storage Control Utility Demand Target, optional double
  EXPECT_FALSE(elcd.storageControlUtilityDemandTarget());
  EXPECT_TRUE(elcd.setStorageControlUtilityDemandTarget(50000));
  ASSERT_TRUE(elcd.storageControlUtilityDemandTarget());
  EXPECT_EQ(50000, elcd.storageControlUtilityDemandTarget().get());
  elcd.resetStorageControlUtilityDemandTarget();
  ASSERT_FALSE(elcd.storageControlUtilityDemandTarget());

  // Storage Control Utility Demand Target Fraction Schedule, defaults to alwaysOnDiscrete
  ScheduleCompact schSControl = ScheduleCompact(model);
  EXPECT_TRUE(elcd.isStorageControlUtilityDemandTargetFractionScheduleDefaulted());
  EXPECT_EQ(model.alwaysOnDiscreteSchedule().handle(), elcd.storageControlUtilityDemandTargetFractionSchedule().handle());
  EXPECT_TRUE(elcd.setStorageControlUtilityDemandTargetFractionSchedule(schSControl));
  ASSERT_FALSE(elcd.isStorageControlUtilityDemandTargetFractionScheduleDefaulted());
  EXPECT_EQ(schSControl.handle(), elcd.storageControlUtilityDemandTargetFractionSchedule().handle());
  elcd.resetStorageControlUtilityDemandTargetFractionSchedule();
  ASSERT_TRUE(elcd.isStorageControlUtilityDemandTargetFractionScheduleDefaulted());
}
TEST_F(ModelFixture, ElectricLoadCenterDistribution_TransformerDelete) {
  Model model;

  ElectricLoadCenterDistribution elcd(model);
  ElectricLoadCenterTransformer elct(model);
  EXPECT_TRUE(elcd.setTransformer(elct));
  EXPECT_EQ(1u, model.getObjectsByType(ElectricLoadCenterDistribution::iddObjectType()).size());
  EXPECT_EQ(1u, model.getObjectsByType(ElectricLoadCenterTransformer::iddObjectType()).size());
  elcd.remove();
  EXPECT_EQ(0u, model.getObjectsByType(ElectricLoadCenterDistribution::iddObjectType()).size());
  EXPECT_EQ(0u, model.getObjectsByType(ElectricLoadCenterTransformer::iddObjectType()).size());
}