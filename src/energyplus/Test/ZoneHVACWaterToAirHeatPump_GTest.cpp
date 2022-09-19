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
#include "../../model/ZoneHVACWaterToAirHeatPump.hpp"
#include "../../model/CoilHeatingWaterToAirHeatPumpEquationFit.hpp"
#include "../../model/CoilCoolingWaterToAirHeatPumpEquationFit.hpp"
#include "../../model/FanOnOff.hpp"
#include "../../model/CoilHeatingElectric.hpp"
#include "../../model/ScheduleConstant.hpp"

#include <utilities/idd/ZoneHVAC_WaterToAirHeatPump_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_WaterToAirHeatPump_EquationFit_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_WaterToAirHeatPump_EquationFit_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/IdfObject_Impl.hpp"

#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/WorkspaceObject_Impl.hpp"

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_ZoneHVACWaterToAirHeatPump) {

  Model m;

  Schedule sch = m.alwaysOnDiscreteSchedule();
  CoilHeatingWaterToAirHeatPumpEquationFit htg_coil(m);
  CoilCoolingWaterToAirHeatPumpEquationFit clg_coil(m);
  FanOnOff fan(m);
  CoilHeatingElectric supp_htg_coil(m);

  ZoneHVACWaterToAirHeatPump hp(m, sch, fan, htg_coil, clg_coil, supp_htg_coil);

  // Need to be in a thermal zone to be translated, and TZ needs at least one space
  ThermalZone z(m);
  hp.addToThermalZone(z);
  Space s(m);
  s.setThermalZone(z);

  openstudio::energyplus::ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  WorkspaceObjectVector idf_hps(w.getObjectsByType(IddObjectType::ZoneHVAC_WaterToAirHeatPump));
  EXPECT_EQ(1u, idf_hps.size());
  WorkspaceObject idf_hp(idf_hps[0]);

  WorkspaceObjectVector idf_ccs(w.getObjectsByType(IddObjectType::Coil_Cooling_WaterToAirHeatPump_EquationFit));
  EXPECT_EQ(1u, idf_ccs.size());
  WorkspaceObject idf_cc(idf_ccs[0]);

  WorkspaceObjectVector idf_hcs(w.getObjectsByType(IddObjectType::Coil_Heating_WaterToAirHeatPump_EquationFit));
  EXPECT_EQ(1u, idf_hcs.size());
  WorkspaceObject idf_hc(idf_hcs[0]);

  // Check Node Connections
  EXPECT_EQ("", idf_hp.getString(ZoneHVAC_WaterToAirHeatPumpFields::AirInletNodeName).get());
  EXPECT_EQ("", idf_hp.getString(ZoneHVAC_WaterToAirHeatPumpFields::AirOutletNodeName).get());

  EXPECT_EQ("Zone HVAC Water To Air Heat Pump 1 Cooling Coil Outlet Node",
            idf_hc.getString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::AirInletNodeName).get());
  EXPECT_EQ("Zone HVAC Water To Air Heat Pump 1 Heating Coil Outlet Node",
            idf_hc.getString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::AirOutletNodeName).get());

  EXPECT_EQ("", idf_cc.getString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::AirInletNodeName).get());
  EXPECT_EQ("", idf_cc.getString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::AirOutletNodeName).get());

  EXPECT_EQ(hp.nameString(), idf_hp.getString(ZoneHVAC_WaterToAirHeatPumpFields::Name).get());
  EXPECT_EQ(sch.nameString(), idf_hp.getString(ZoneHVAC_WaterToAirHeatPumpFields::AvailabilityScheduleName).get());
  EXPECT_EQ("OutdoorAir:Mixer", idf_hp.getString(ZoneHVAC_WaterToAirHeatPumpFields::OutdoorAirMixerObjectType, false).get());
  boost::optional<WorkspaceObject> woMixer(idf_hp.getTarget(ZoneHVAC_WaterToAirHeatPumpFields::OutdoorAirMixerName));
  ASSERT_TRUE(woMixer);
  EXPECT_EQ(woMixer->iddObject().type(), IddObjectType::OutdoorAir_Mixer);
  EXPECT_EQ("Autosize", idf_hp.getString(ZoneHVAC_WaterToAirHeatPumpFields::CoolingSupplyAirFlowRate, false).get());
  EXPECT_EQ("Autosize", idf_hp.getString(ZoneHVAC_WaterToAirHeatPumpFields::HeatingSupplyAirFlowRate, false).get());
  EXPECT_EQ("Autosize", idf_hp.getString(ZoneHVAC_WaterToAirHeatPumpFields::NoLoadSupplyAirFlowRate, false).get());
  EXPECT_EQ("Autosize", idf_hp.getString(ZoneHVAC_WaterToAirHeatPumpFields::CoolingOutdoorAirFlowRate, false).get());
  EXPECT_EQ("Autosize", idf_hp.getString(ZoneHVAC_WaterToAirHeatPumpFields::HeatingOutdoorAirFlowRate, false).get());
  EXPECT_EQ("Autosize", idf_hp.getString(ZoneHVAC_WaterToAirHeatPumpFields::NoLoadOutdoorAirFlowRate, false).get());
  EXPECT_EQ("Fan:OnOff", idf_hp.getString(ZoneHVAC_WaterToAirHeatPumpFields::SupplyAirFanObjectType, false).get());
  EXPECT_EQ(fan.nameString(), idf_hp.getString(ZoneHVAC_WaterToAirHeatPumpFields::SupplyAirFanName).get());
  EXPECT_EQ("Coil:Heating:WaterToAirHeatPump:EquationFit", idf_hp.getString(ZoneHVAC_WaterToAirHeatPumpFields::HeatingCoilObjectType, false).get());
  EXPECT_EQ(htg_coil.nameString(), idf_hp.getString(ZoneHVAC_WaterToAirHeatPumpFields::HeatingCoilName).get());
  EXPECT_EQ("Coil:Cooling:WaterToAirHeatPump:EquationFit", idf_hp.getString(ZoneHVAC_WaterToAirHeatPumpFields::CoolingCoilObjectType, false).get());
  EXPECT_EQ(clg_coil.nameString(), idf_hp.getString(ZoneHVAC_WaterToAirHeatPumpFields::CoolingCoilName).get());
  EXPECT_EQ(2.5, idf_hp.getDouble(ZoneHVAC_WaterToAirHeatPumpFields::MaximumCyclingRate, false).get());
  EXPECT_EQ(60.0, idf_hp.getDouble(ZoneHVAC_WaterToAirHeatPumpFields::HeatPumpTimeConstant, false).get());
  EXPECT_EQ(0.01, idf_hp.getDouble(ZoneHVAC_WaterToAirHeatPumpFields::FractionofOnCyclePowerUse, false).get());
  EXPECT_EQ(60.0, idf_hp.getDouble(ZoneHVAC_WaterToAirHeatPumpFields::HeatPumpFanDelayTime, false).get());
  EXPECT_EQ("Coil:Heating:Electric", idf_hp.getString(ZoneHVAC_WaterToAirHeatPumpFields::SupplementalHeatingCoilObjectType, false).get());
  EXPECT_EQ(supp_htg_coil.nameString(), idf_hp.getString(ZoneHVAC_WaterToAirHeatPumpFields::SupplementalHeatingCoilName).get());
  EXPECT_EQ("Autosize", idf_hp.getString(ZoneHVAC_WaterToAirHeatPumpFields::MaximumSupplyAirTemperaturefromSupplementalHeater, false).get());
  EXPECT_EQ(21.0, idf_hp.getDouble(ZoneHVAC_WaterToAirHeatPumpFields::MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation, false).get());
  EXPECT_TRUE(idf_hp.isEmpty(ZoneHVAC_WaterToAirHeatPumpFields::OutdoorDryBulbTemperatureSensorNodeName));
  EXPECT_EQ("BlowThrough", idf_hp.getString(ZoneHVAC_WaterToAirHeatPumpFields::FanPlacement, false).get());
  EXPECT_TRUE(idf_hp.isEmpty(ZoneHVAC_WaterToAirHeatPumpFields::SupplyAirFanOperatingModeScheduleName));
  EXPECT_TRUE(idf_hp.isEmpty(ZoneHVAC_WaterToAirHeatPumpFields::AvailabilityManagerListName));
  EXPECT_EQ("Cycling", idf_hp.getString(ZoneHVAC_WaterToAirHeatPumpFields::HeatPumpCoilWaterFlowMode, false).get());
  EXPECT_TRUE(idf_hp.isEmpty(ZoneHVAC_WaterToAirHeatPumpFields::DesignSpecificationZoneHVACSizingObjectName));

  EXPECT_EQ(htg_coil.nameString(), idf_hc.getString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::Name).get());
  EXPECT_TRUE(idf_hc.isEmpty(Coil_Heating_WaterToAirHeatPump_EquationFitFields::WaterInletNodeName));
  EXPECT_TRUE(idf_hc.isEmpty(Coil_Heating_WaterToAirHeatPump_EquationFitFields::WaterOutletNodeName));
  EXPECT_EQ("Autosize", idf_hc.getString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedAirFlowRate, false).get());
  EXPECT_EQ("Autosize", idf_hc.getString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedWaterFlowRate, false).get());
  EXPECT_EQ("Autosize", idf_hc.getString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::GrossRatedHeatingCapacity, false).get());
  EXPECT_EQ(3.0, idf_hc.getDouble(Coil_Heating_WaterToAirHeatPump_EquationFitFields::GrossRatedHeatingCOP, false).get());
  EXPECT_EQ(20.0, idf_hc.getDouble(Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedEnteringWaterTemperature, false).get());
  EXPECT_EQ(20.0, idf_hc.getDouble(Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedEnteringAirDryBulbTemperature, false).get());
  EXPECT_EQ(1.0, idf_hc.getDouble(Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatioofRatedHeatingCapacitytoRatedCoolingCapacity, false).get());
  boost::optional<WorkspaceObject> woCurve1(idf_hc.getTarget(Coil_Heating_WaterToAirHeatPump_EquationFitFields::HeatingCapacityCurveName));
  ASSERT_TRUE(woCurve1);
  EXPECT_EQ(woCurve1->iddObject().type(), IddObjectType::Curve_QuadLinear);
  boost::optional<WorkspaceObject> woCurve2(idf_hc.getTarget(Coil_Heating_WaterToAirHeatPump_EquationFitFields::HeatingPowerConsumptionCurveName));
  ASSERT_TRUE(woCurve2);
  EXPECT_EQ(woCurve2->iddObject().type(), IddObjectType::Curve_QuadLinear);

  EXPECT_EQ(clg_coil.nameString(), idf_cc.getString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::Name).get());
  EXPECT_TRUE(idf_cc.isEmpty(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::WaterInletNodeName));
  EXPECT_TRUE(idf_cc.isEmpty(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::WaterOutletNodeName));
  EXPECT_EQ("Autosize", idf_cc.getString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedAirFlowRate, false).get());
  EXPECT_EQ("Autosize", idf_cc.getString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedWaterFlowRate, false).get());
  EXPECT_EQ("Autosize", idf_cc.getString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::GrossRatedTotalCoolingCapacity, false).get());
  EXPECT_EQ("Autosize", idf_cc.getString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::GrossRatedSensibleCoolingCapacity, false).get());
  EXPECT_EQ(3.0, idf_cc.getDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::GrossRatedCoolingCOP, false).get());
  EXPECT_EQ(30.0, idf_cc.getDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedEnteringWaterTemperature, false).get());
  EXPECT_EQ(27.0, idf_cc.getDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedEnteringAirDryBulbTemperature, false).get());
  EXPECT_EQ(19.0, idf_cc.getDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedEnteringAirWetBulbTemperature, false).get());
  boost::optional<WorkspaceObject> woCurve3(idf_cc.getTarget(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::TotalCoolingCapacityCurveName));
  ASSERT_TRUE(woCurve3);
  EXPECT_EQ(woCurve3->iddObject().type(), IddObjectType::Curve_QuadLinear);
  boost::optional<WorkspaceObject> woCurve4(idf_cc.getTarget(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::SensibleCoolingCapacityCurveName));
  ASSERT_TRUE(woCurve4);
  EXPECT_EQ(woCurve4->iddObject().type(), IddObjectType::Curve_QuintLinear);
  boost::optional<WorkspaceObject> woCurve5(idf_cc.getTarget(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::CoolingPowerConsumptionCurveName));
  ASSERT_TRUE(woCurve5);
  EXPECT_EQ(woCurve5->iddObject().type(), IddObjectType::Curve_QuadLinear);
  EXPECT_EQ(0, idf_cc.getDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::NominalTimeforCondensateRemovaltoBegin, false).get());
  EXPECT_EQ(
    0.0,
    idf_cc.getDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity).get());
}
