/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "ModelFixture.hpp"

#include "../CoilCoolingDXSingleSpeedThermalStorage.hpp"
#include "../CoilCoolingDXSingleSpeedThermalStorage_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"

#include "../FluidAndGlycol.hpp"
#include "../FluidAndGlycol_Impl.hpp"

#include "../Connection.hpp"
#include "../Connection_Impl.hpp"

#include "../BivariateFunctions.hpp"
#include "../BivariateFunctions_Impl.hpp"

#include "../UnivariateFunctions.hpp"
#include "../UnivariateFunctions_Impl.hpp"

#include "../TrivariateFunctions.hpp"
#include "../TrivariateFunctions_Impl.hpp"

#include "../WaterStorageTank.hpp"
#include "../WaterStorageTank_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CoilCoolingDXSingleSpeedThermalStorage_GettersSetters) {
  Model m;
  // TODO: Check regular Ctor arguments
  CoilCoolingDXSingleSpeedThermalStorage coilCoolingDXSingleSpeedThermalStorage(m);
  // TODO: Or if a UniqueModelObject (and make sure _Impl is included)
  // CoilCoolingDXSingleSpeedThermalStorage coilCoolingDXSingleSpeedThermalStorage = m.getUniqueModelObject<CoilCoolingDXSingleSpeedThermalStorage>();

  coilCoolingDXSingleSpeedThermalStorage.setName("My CoilCoolingDXSingleSpeedThermalStorage");

  // Availability Schedule: Required Object
  Schedule availabilitySchedule(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setAvailabilitySchedule(availabilitySchedule));
  EXPECT_EQ(availabilitySchedule, coilCoolingDXSingleSpeedThermalStorage.availabilitySchedule());

  // Operating Mode Control Method: Required String
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setOperatingModeControlMethod("ScheduledModes"));
  EXPECT_EQ("ScheduledModes", coilCoolingDXSingleSpeedThermalStorage.operatingModeControlMethod());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setOperatingModeControlMethod("BADENUM"));
  EXPECT_EQ("ScheduledModes", coilCoolingDXSingleSpeedThermalStorage.operatingModeControlMethod());

  // Operation Mode Control Schedule: Optional Object
  boost::optional<Schedule> operationModeControlSchedule(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setOperationModeControlSchedule(operationModeControlSchedule));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.operationModeControlSchedule());
  EXPECT_EQ(operationModeControlSchedule, coilCoolingDXSingleSpeedThermalStorage.operationModeControlSchedule().get());

  // Storage Type: Required String
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setStorageType("Water"));
  EXPECT_EQ("Water", coilCoolingDXSingleSpeedThermalStorage.storageType());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setStorageType("BADENUM"));
  EXPECT_EQ("Water", coilCoolingDXSingleSpeedThermalStorage.storageType());

  // User Defined Fluid Type: Optional Object
  boost::optional<FluidAndGlycol> userDefinedFluidType(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setUserDefinedFluidType(userDefinedFluidType));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.userDefinedFluidType());
  EXPECT_EQ(userDefinedFluidType, coilCoolingDXSingleSpeedThermalStorage.userDefinedFluidType().get());

  // Fluid Storage Volume: Optional Double
  // Autocalculate
  coilCoolingDXSingleSpeedThermalStorage.autocalculateFluidStorageVolume();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.isFluidStorageVolumeAutocalculated());
  // Set
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setFluidStorageVolume(0.8));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.fluidStorageVolume());
  EXPECT_EQ(0.8, coilCoolingDXSingleSpeedThermalStorage.fluidStorageVolume().get());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setFluidStorageVolume(-10.0));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.fluidStorageVolume());
  EXPECT_EQ(0.8, coilCoolingDXSingleSpeedThermalStorage.fluidStorageVolume().get());
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.isFluidStorageVolumeAutocalculated());

  // Ice Storage Capacity: Optional Double
  // Autocalculate
  coilCoolingDXSingleSpeedThermalStorage.autocalculateIceStorageCapacity();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.isIceStorageCapacityAutocalculated());
  // Set
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setIceStorageCapacity(0.9));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.iceStorageCapacity());
  EXPECT_EQ(0.9, coilCoolingDXSingleSpeedThermalStorage.iceStorageCapacity().get());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setIceStorageCapacity(-10.0));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.iceStorageCapacity());
  EXPECT_EQ(0.9, coilCoolingDXSingleSpeedThermalStorage.iceStorageCapacity().get());
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.isIceStorageCapacityAutocalculated());

  // Storage Capacity Sizing Factor: Optional Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setStorageCapacitySizingFactor(1.0));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.storageCapacitySizingFactor());
  EXPECT_EQ(1.0, coilCoolingDXSingleSpeedThermalStorage.storageCapacitySizingFactor().get());

  // Storage Tank Ambient Temperature Node: Required Object
  Connection storageTankAmbientTemperatureNode(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setStorageTankAmbientTemperatureNode(storageTankAmbientTemperatureNode));
  EXPECT_EQ(storageTankAmbientTemperatureNode, coilCoolingDXSingleSpeedThermalStorage.storageTankAmbientTemperatureNode());

  // Storage Tank to Ambient U-value Times Area Heat Transfer Coefficient: Required Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setStorageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient(1.2));
  EXPECT_EQ(1.2, coilCoolingDXSingleSpeedThermalStorage.storageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setStorageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient(-10.0));
  EXPECT_EQ(1.2, coilCoolingDXSingleSpeedThermalStorage.storageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient());

  // Fluid Storage Tank Rating Temperature: Optional Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setFluidStorageTankRatingTemperature(1.3));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.fluidStorageTankRatingTemperature());
  EXPECT_EQ(1.3, coilCoolingDXSingleSpeedThermalStorage.fluidStorageTankRatingTemperature().get());

  // Rated Evaporator Air Flow Rate: Required Double
  // Autosize
  coilCoolingDXSingleSpeedThermalStorage.autosizeRatedEvaporatorAirFlowRate();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.isRatedEvaporatorAirFlowRateAutosized());
  // Set
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setRatedEvaporatorAirFlowRate(1.4));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.ratedEvaporatorAirFlowRate());
  EXPECT_EQ(1.4, coilCoolingDXSingleSpeedThermalStorage.ratedEvaporatorAirFlowRate().get());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setRatedEvaporatorAirFlowRate(-10.0));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.ratedEvaporatorAirFlowRate());
  EXPECT_EQ(1.4, coilCoolingDXSingleSpeedThermalStorage.ratedEvaporatorAirFlowRate().get());
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.isRatedEvaporatorAirFlowRateAutosized());

  // Evaporator Air Inlet Node: Required Object
  Connection evaporatorAirInletNode(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setEvaporatorAirInletNode(evaporatorAirInletNode));
  EXPECT_EQ(evaporatorAirInletNode, coilCoolingDXSingleSpeedThermalStorage.evaporatorAirInletNode());

  // Evaporator Air Outlet Node: Required Object
  Connection evaporatorAirOutletNode(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setEvaporatorAirOutletNode(evaporatorAirOutletNode));
  EXPECT_EQ(evaporatorAirOutletNode, coilCoolingDXSingleSpeedThermalStorage.evaporatorAirOutletNode());

  // Cooling Only Mode Available: Required Boolean
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeAvailable(true));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModeAvailable());
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeAvailable(false));
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModeAvailable());

  // Cooling Only Mode Rated Total Evaporator Cooling Capacity: Optional Double
  // Autosize
  coilCoolingDXSingleSpeedThermalStorage.autosizeCoolingOnlyModeRatedTotalEvaporatorCoolingCapacity();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.isCoolingOnlyModeRatedTotalEvaporatorCoolingCapacityAutosized());
  // Set
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeRatedTotalEvaporatorCoolingCapacity(1.8));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModeRatedTotalEvaporatorCoolingCapacity());
  EXPECT_EQ(1.8, coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModeRatedTotalEvaporatorCoolingCapacity().get());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeRatedTotalEvaporatorCoolingCapacity(-10.0));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModeRatedTotalEvaporatorCoolingCapacity());
  EXPECT_EQ(1.8, coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModeRatedTotalEvaporatorCoolingCapacity().get());
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.isCoolingOnlyModeRatedTotalEvaporatorCoolingCapacityAutosized());

  // Cooling Only Mode Rated Sensible Heat Ratio: Required Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeRatedSensibleHeatRatio(0.95));
  EXPECT_EQ(0.95, coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModeRatedSensibleHeatRatio());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeRatedSensibleHeatRatio(-10.0));
  EXPECT_EQ(0.95, coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModeRatedSensibleHeatRatio());

  // Cooling Only Mode Rated COP: Required Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeRatedCOP(2.0));
  EXPECT_EQ(2.0, coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModeRatedCOP());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeRatedCOP(-10.0));
  EXPECT_EQ(2.0, coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModeRatedCOP());

  // Cooling Only Mode Total Evaporator Cooling Capacity Function of Temperature Curve: Optional Object
  boost::optional<BivariateFunctions> coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(
    coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve());
  EXPECT_EQ(coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve().get());

  // Cooling Only Mode Total Evaporator Cooling Capacity Function of Flow Fraction Curve: Optional Object
  boost::optional<UnivariateFunctions> coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(
    coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve());
  EXPECT_EQ(coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve().get());

  // Cooling Only Mode Energy Input Ratio Function of Temperature Curve: Optional Object
  boost::optional<BivariateFunctions> coolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve(
    coolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve());
  EXPECT_EQ(coolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve().get());

  // Cooling Only Mode Energy Input Ratio Function of Flow Fraction Curve: Optional Object
  boost::optional<UnivariateFunctions> coolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve(
    coolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve());
  EXPECT_EQ(coolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve().get());

  // Cooling Only Mode Part Load Fraction Correlation Curve: Optional Object
  boost::optional<UnivariateFunctions> coolingOnlyModePartLoadFractionCorrelationCurve(m);
  EXPECT_TRUE(
    coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModePartLoadFractionCorrelationCurve(coolingOnlyModePartLoadFractionCorrelationCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModePartLoadFractionCorrelationCurve());
  EXPECT_EQ(coolingOnlyModePartLoadFractionCorrelationCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModePartLoadFractionCorrelationCurve().get());

  // Cooling Only Mode Sensible Heat Ratio Function of Temperature Curve: Optional Object
  boost::optional<BivariateFunctions> coolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve(
    coolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve());
  EXPECT_EQ(coolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve().get());

  // Cooling Only Mode Sensible Heat Ratio Function of Flow Fraction Curve: Optional Object
  boost::optional<UnivariateFunctions> coolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve(
    coolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve());
  EXPECT_EQ(coolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve().get());

  // Cooling And Charge Mode Available: Required Boolean
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeAvailable(true));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeAvailable());
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeAvailable(false));
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeAvailable());

  // Cooling And Charge Mode Rated Total Evaporator Cooling Capacity: Optional Double
  // Autocalculate
  coilCoolingDXSingleSpeedThermalStorage.autocalculateCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.isCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacityAutocalculated());
  // Set
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity(2.9));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeRatedTotalEvaporatorCoolingCapacity());
  EXPECT_EQ(2.9, coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeRatedTotalEvaporatorCoolingCapacity().get());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity(-10.0));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeRatedTotalEvaporatorCoolingCapacity());
  EXPECT_EQ(2.9, coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeRatedTotalEvaporatorCoolingCapacity().get());
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.isCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacityAutocalculated());

  // Cooling And Charge Mode Capacity Sizing Factor: Required Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeCapacitySizingFactor(3.0));
  EXPECT_EQ(3.0, coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeCapacitySizingFactor());

  // Cooling And Charge Mode Rated Storage Charging Capacity: Optional Double
  // Autocalculate
  coilCoolingDXSingleSpeedThermalStorage.autocalculateCoolingAndChargeModeRatedStorageChargingCapacity();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.isCoolingAndChargeModeRatedStorageChargingCapacityAutocalculated());
  // Set
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeRatedStorageChargingCapacity(3.1));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeRatedStorageChargingCapacity());
  EXPECT_EQ(3.1, coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeRatedStorageChargingCapacity().get());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeRatedStorageChargingCapacity(-10.0));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeRatedStorageChargingCapacity());
  EXPECT_EQ(3.1, coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeRatedStorageChargingCapacity().get());
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.isCoolingAndChargeModeRatedStorageChargingCapacityAutocalculated());

  // Cooling And Charge Mode Storage Capacity Sizing Factor: Required Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeStorageCapacitySizingFactor(3.2));
  EXPECT_EQ(3.2, coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeStorageCapacitySizingFactor());

  // Cooling And Charge Mode Rated Sensible Heat Ratio: Required Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeRatedSensibleHeatRatio(0.971));
  EXPECT_EQ(0.971, coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeRatedSensibleHeatRatio());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeRatedSensibleHeatRatio(-10.0));
  EXPECT_EQ(0.971, coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeRatedSensibleHeatRatio());

  // Cooling And Charge Mode Cooling Rated COP: Required Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeCoolingRatedCOP(3.4));
  EXPECT_EQ(3.4, coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeCoolingRatedCOP());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeCoolingRatedCOP(-10.0));
  EXPECT_EQ(3.4, coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeCoolingRatedCOP());

  // Cooling And Charge Mode Charging Rated COP: Required Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeChargingRatedCOP(3.5));
  EXPECT_EQ(3.5, coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeChargingRatedCOP());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeChargingRatedCOP(-10.0));
  EXPECT_EQ(3.5, coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeChargingRatedCOP());

  // Cooling And Charge Mode Total Evaporator Cooling Capacity Function of Temperature Curve: Optional Object
  boost::optional<TrivariateFunctions> coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(
    coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve());
  EXPECT_EQ(coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve().get());

  // Cooling And Charge Mode Total Evaporator Cooling Capacity Function of Flow Fraction Curve: Optional Object
  boost::optional<UnivariateFunctions> coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(
    coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve());
  EXPECT_EQ(coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve().get());

  // Cooling And Charge Mode Evaporator Energy Input Ratio Function of Temperature Curve: Optional Object
  boost::optional<TrivariateFunctions> coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve(
    coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve());
  EXPECT_EQ(coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve().get());

  // Cooling And Charge Mode Evaporator Energy Input Ratio Function of Flow Fraction Curve: Optional Object
  boost::optional<UnivariateFunctions> coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve(
    coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve());
  EXPECT_EQ(coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve().get());

  // Cooling And Charge Mode Evaporator Part Load Fraction Correlation Curve: Optional Object
  boost::optional<UnivariateFunctions> coolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve(
    coolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve());
  EXPECT_EQ(coolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve().get());

  // Cooling And Charge Mode Storage Charge Capacity Function of Temperature Curve: Optional Object
  boost::optional<TrivariateFunctions> coolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve(
    coolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve());
  EXPECT_EQ(coolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve().get());

  // Cooling And Charge Mode Storage Charge Capacity Function of Total Evaporator PLR Curve: Optional Object
  boost::optional<UnivariateFunctions> coolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve(
    coolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve());
  EXPECT_EQ(coolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve().get());

  // Cooling And Charge Mode Storage Energy Input Ratio Function of Temperature Curve: Optional Object
  boost::optional<TrivariateFunctions> coolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve(
    coolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve());
  EXPECT_EQ(coolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve().get());

  // Cooling And Charge Mode Storage Energy Input Ratio Function of Flow Fraction Curve: Optional Object
  boost::optional<UnivariateFunctions> coolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve(
    coolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve());
  EXPECT_EQ(coolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve().get());

  // Cooling And Charge Mode Storage Energy Part Load Fraction Correlation Curve: Optional Object
  boost::optional<UnivariateFunctions> coolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve(
    coolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve());
  EXPECT_EQ(coolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve().get());

  // Cooling And Charge Mode Sensible Heat Ratio Function of Temperature Curve: Optional Object
  boost::optional<BivariateFunctions> coolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve(
    coolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve());
  EXPECT_EQ(coolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve().get());

  // Cooling And Charge Mode Sensible Heat Ratio Function of Flow Fraction Curve: Optional Object
  boost::optional<UnivariateFunctions> coolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve(
    coolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve());
  EXPECT_EQ(coolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve().get());

  // Cooling And Discharge Mode Available: Required Boolean
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeAvailable(true));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeAvailable());
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeAvailable(false));
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeAvailable());

  // Cooling And Discharge Mode Rated Total Evaporator Cooling Capacity: Optional Double
  // Autocalculate
  coilCoolingDXSingleSpeedThermalStorage.autocalculateCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.isCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacityAutocalculated());
  // Set
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity(4.9));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity());
  EXPECT_EQ(4.9, coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity().get());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity(-10.0));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity());
  EXPECT_EQ(4.9, coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity().get());
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.isCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacityAutocalculated());

  // Cooling And Discharge Mode Evaporator Capacity Sizing Factor: Required Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeEvaporatorCapacitySizingFactor(5.0));
  EXPECT_EQ(5.0, coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeEvaporatorCapacitySizingFactor());

  // Cooling And Discharge Mode Rated Storage Discharging Capacity: Optional Double
  // Autocalculate
  coilCoolingDXSingleSpeedThermalStorage.autocalculateCoolingAndDischargeModeRatedStorageDischargingCapacity();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.isCoolingAndDischargeModeRatedStorageDischargingCapacityAutocalculated());
  // Set
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeRatedStorageDischargingCapacity(5.1));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeRatedStorageDischargingCapacity());
  EXPECT_EQ(5.1, coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeRatedStorageDischargingCapacity().get());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeRatedStorageDischargingCapacity(-10.0));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeRatedStorageDischargingCapacity());
  EXPECT_EQ(5.1, coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeRatedStorageDischargingCapacity().get());
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.isCoolingAndDischargeModeRatedStorageDischargingCapacityAutocalculated());

  // Cooling And Discharge Mode Storage Discharge Capacity Sizing Factor: Required Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeStorageDischargeCapacitySizingFactor(5.2));
  EXPECT_EQ(5.2, coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeStorageDischargeCapacitySizingFactor());

  // Cooling And Discharge Mode Rated Sensible Heat Ratio: Required Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeRatedSensibleHeatRatio(0.981));
  EXPECT_EQ(0.981, coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeRatedSensibleHeatRatio());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeRatedSensibleHeatRatio(-10.0));
  EXPECT_EQ(0.981, coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeRatedSensibleHeatRatio());

  // Cooling And Discharge Mode Cooling Rated COP: Required Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeCoolingRatedCOP(5.4));
  EXPECT_EQ(5.4, coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeCoolingRatedCOP());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeCoolingRatedCOP(-10.0));
  EXPECT_EQ(5.4, coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeCoolingRatedCOP());

  // Cooling And Discharge Mode Discharging Rated COP: Required Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeDischargingRatedCOP(5.5));
  EXPECT_EQ(5.5, coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeDischargingRatedCOP());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeDischargingRatedCOP(-10.0));
  EXPECT_EQ(5.5, coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeDischargingRatedCOP());

  // Cooling And Discharge Mode Total Evaporator Cooling Capacity Function of Temperature Curve: Optional Object
  boost::optional<TrivariateFunctions> coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(
    coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve());
  EXPECT_EQ(coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve().get());

  // Cooling And Discharge Mode Total Evaporator Cooling Capacity Function of Flow Fraction Curve: Optional Object
  boost::optional<UnivariateFunctions> coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(
    coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve());
  EXPECT_EQ(coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve().get());

  // Cooling And Discharge Mode Evaporator Energy Input Ratio Function of Temperature Curve: Optional Object
  boost::optional<TrivariateFunctions> coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve(
    coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve());
  EXPECT_EQ(coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve().get());

  // Cooling And Discharge Mode Evaporator Energy Input Ratio Function of Flow Fraction Curve: Optional Object
  boost::optional<UnivariateFunctions> coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve(
    coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve());
  EXPECT_EQ(coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve().get());

  // Cooling And Discharge Mode Evaporator Part Load Fraction Correlation Curve: Optional Object
  boost::optional<UnivariateFunctions> coolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve(
    coolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve());
  EXPECT_EQ(coolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve().get());

  // Cooling And Discharge Mode Storage Discharge Capacity Function of Temperature Curve: Optional Object
  boost::optional<TrivariateFunctions> coolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve(
    coolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve());
  EXPECT_EQ(coolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve().get());

  // Cooling And Discharge Mode Storage Discharge Capacity Function of Flow Fraction Curve: Optional Object
  boost::optional<UnivariateFunctions> coolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve(
    coolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve());
  EXPECT_EQ(coolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve().get());

  // Cooling And Discharge Mode Storage Discharge Capacity Function of Total Evaporator PLR Curve: Optional Object
  boost::optional<UnivariateFunctions> coolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve(
    coolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve());
  EXPECT_EQ(coolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve().get());

  // Cooling And Discharge Mode Storage Energy Input Ratio Function of Temperature Curve: Optional Object
  boost::optional<TrivariateFunctions> coolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve(
    coolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve());
  EXPECT_EQ(coolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve().get());

  // Cooling And Discharge Mode Storage Energy Input Ratio Function of Flow Fraction Curve: Optional Object
  boost::optional<UnivariateFunctions> coolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve(
    coolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve());
  EXPECT_EQ(coolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve().get());

  // Cooling And Discharge Mode Storage Energy Part Load Fraction Correlation Curve: Optional Object
  boost::optional<UnivariateFunctions> coolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve(
    coolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve());
  EXPECT_EQ(coolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve().get());

  // Cooling And Discharge Mode Sensible Heat Ratio Function of Temperature Curve: Optional Object
  boost::optional<BivariateFunctions> coolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve(
    coolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve());
  EXPECT_EQ(coolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve().get());

  // Cooling And Discharge Mode Sensible Heat Ratio Function of Flow Fraction Curve: Optional Object
  boost::optional<UnivariateFunctions> coolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve(
    coolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve());
  EXPECT_EQ(coolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve().get());

  // Charge Only Mode Available: Required Boolean
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setChargeOnlyModeAvailable(true));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.chargeOnlyModeAvailable());
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setChargeOnlyModeAvailable(false));
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.chargeOnlyModeAvailable());

  // Charge Only Mode Rated Storage Charging Capacity: Optional Double
  // Autocalculate
  coilCoolingDXSingleSpeedThermalStorage.autocalculateChargeOnlyModeRatedStorageChargingCapacity();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.isChargeOnlyModeRatedStorageChargingCapacityAutocalculated());
  // Set
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setChargeOnlyModeRatedStorageChargingCapacity(7.0));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.chargeOnlyModeRatedStorageChargingCapacity());
  EXPECT_EQ(7.0, coilCoolingDXSingleSpeedThermalStorage.chargeOnlyModeRatedStorageChargingCapacity().get());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setChargeOnlyModeRatedStorageChargingCapacity(-10.0));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.chargeOnlyModeRatedStorageChargingCapacity());
  EXPECT_EQ(7.0, coilCoolingDXSingleSpeedThermalStorage.chargeOnlyModeRatedStorageChargingCapacity().get());
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.isChargeOnlyModeRatedStorageChargingCapacityAutocalculated());

  // Charge Only Mode Capacity Sizing Factor: Required Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setChargeOnlyModeCapacitySizingFactor(7.1));
  EXPECT_EQ(7.1, coilCoolingDXSingleSpeedThermalStorage.chargeOnlyModeCapacitySizingFactor());

  // Charge Only Mode Charging Rated COP: Required Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setChargeOnlyModeChargingRatedCOP(7.2));
  EXPECT_EQ(7.2, coilCoolingDXSingleSpeedThermalStorage.chargeOnlyModeChargingRatedCOP());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setChargeOnlyModeChargingRatedCOP(-10.0));
  EXPECT_EQ(7.2, coilCoolingDXSingleSpeedThermalStorage.chargeOnlyModeChargingRatedCOP());

  // Charge Only Mode Storage Charge Capacity Function of Temperature Curve: Optional Object
  boost::optional<BivariateFunctions> chargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setChargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve(
    chargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.chargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve());
  EXPECT_EQ(chargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve,
            coilCoolingDXSingleSpeedThermalStorage.chargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve().get());

  // Charge Only Mode Storage Energy Input Ratio Function of Temperature Curve: Optional Object
  boost::optional<BivariateFunctions> chargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setChargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve(
    chargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.chargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve());
  EXPECT_EQ(chargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve,
            coilCoolingDXSingleSpeedThermalStorage.chargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve().get());

  // Discharge Only Mode Available: Required Boolean
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeAvailable(true));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeAvailable());
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeAvailable(false));
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeAvailable());

  // Discharge Only Mode Rated Storage Discharging Capacity: Optional Double
  // Autocalculate
  coilCoolingDXSingleSpeedThermalStorage.autocalculateDischargeOnlyModeRatedStorageDischargingCapacity();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.isDischargeOnlyModeRatedStorageDischargingCapacityAutocalculated());
  // Set
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeRatedStorageDischargingCapacity(7.6));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeRatedStorageDischargingCapacity());
  EXPECT_EQ(7.6, coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeRatedStorageDischargingCapacity().get());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeRatedStorageDischargingCapacity(-10.0));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeRatedStorageDischargingCapacity());
  EXPECT_EQ(7.6, coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeRatedStorageDischargingCapacity().get());
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.isDischargeOnlyModeRatedStorageDischargingCapacityAutocalculated());

  // Discharge Only Mode Capacity Sizing Factor: Required Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeCapacitySizingFactor(7.7));
  EXPECT_EQ(7.7, coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeCapacitySizingFactor());

  // Discharge Only Mode Rated Sensible Heat Ratio: Optional Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeRatedSensibleHeatRatio(0.987));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeRatedSensibleHeatRatio());
  EXPECT_EQ(0.987, coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeRatedSensibleHeatRatio().get());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeRatedSensibleHeatRatio(-10.0));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeRatedSensibleHeatRatio());
  EXPECT_EQ(0.987, coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeRatedSensibleHeatRatio().get());

  // Discharge Only Mode Rated COP: Required Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeRatedCOP(7.9));
  EXPECT_EQ(7.9, coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeRatedCOP());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeRatedCOP(-10.0));
  EXPECT_EQ(7.9, coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeRatedCOP());

  // Discharge Only Mode Storage Discharge Capacity Function of Temperature Curve: Optional Object
  boost::optional<BivariateFunctions> dischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve(
    dischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve());
  EXPECT_EQ(dischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve,
            coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve().get());

  // Discharge Only Mode Storage Discharge Capacity Function of Flow Fraction Curve: Optional Object
  boost::optional<UnivariateFunctions> dischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve(
    dischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve());
  EXPECT_EQ(dischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve,
            coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve().get());

  // Discharge Only Mode Energy Input Ratio Function of Temperature Curve: Optional Object
  boost::optional<BivariateFunctions> dischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve(
    dischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve());
  EXPECT_EQ(dischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve,
            coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve().get());

  // Discharge Only Mode Energy Input Ratio Function of Flow Fraction Curve: Optional Object
  boost::optional<UnivariateFunctions> dischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve(
    dischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve());
  EXPECT_EQ(dischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve,
            coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve().get());

  // Discharge Only Mode Part Load Fraction Correlation Curve: Optional Object
  boost::optional<UnivariateFunctions> dischargeOnlyModePartLoadFractionCorrelationCurve(m);
  EXPECT_TRUE(
    coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModePartLoadFractionCorrelationCurve(dischargeOnlyModePartLoadFractionCorrelationCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModePartLoadFractionCorrelationCurve());
  EXPECT_EQ(dischargeOnlyModePartLoadFractionCorrelationCurve,
            coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModePartLoadFractionCorrelationCurve().get());

  // Discharge Only Mode Sensible Heat Ratio Function of Temperature Curve: Optional Object
  boost::optional<BivariateFunctions> dischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve(
    dischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve());
  EXPECT_EQ(dischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve,
            coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve().get());

  // Discharge Only Mode Sensible Heat Ratio Function of Flow Fraction Curve: Optional Object
  boost::optional<UnivariateFunctions> dischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve(
    dischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve());
  EXPECT_EQ(dischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve,
            coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve().get());

  // Ancillary Electric Power: Optional Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setAncillaryElectricPower(8.7));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.ancillaryElectricPower());
  EXPECT_EQ(8.7, coilCoolingDXSingleSpeedThermalStorage.ancillaryElectricPower().get());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setAncillaryElectricPower(-10.0));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.ancillaryElectricPower());
  EXPECT_EQ(8.7, coilCoolingDXSingleSpeedThermalStorage.ancillaryElectricPower().get());

  // Cold Weather Operation Minimum Outdoor Air Temperature: Optional Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setColdWeatherOperationMinimumOutdoorAirTemperature(8.8));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coldWeatherOperationMinimumOutdoorAirTemperature());
  EXPECT_EQ(8.8, coilCoolingDXSingleSpeedThermalStorage.coldWeatherOperationMinimumOutdoorAirTemperature().get());

  // Cold Weather Operation Ancillary Power: Optional Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setColdWeatherOperationAncillaryPower(8.9));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coldWeatherOperationAncillaryPower());
  EXPECT_EQ(8.9, coilCoolingDXSingleSpeedThermalStorage.coldWeatherOperationAncillaryPower().get());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setColdWeatherOperationAncillaryPower(-10.0));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coldWeatherOperationAncillaryPower());
  EXPECT_EQ(8.9, coilCoolingDXSingleSpeedThermalStorage.coldWeatherOperationAncillaryPower().get());

  // Condenser Air Inlet Node: Required Object
  Connection condenserAirInletNode(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCondenserAirInletNode(condenserAirInletNode));
  EXPECT_EQ(condenserAirInletNode, coilCoolingDXSingleSpeedThermalStorage.condenserAirInletNode());

  // Condenser Air Outlet Node: Required Object
  Connection condenserAirOutletNode(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCondenserAirOutletNode(condenserAirOutletNode));
  EXPECT_EQ(condenserAirOutletNode, coilCoolingDXSingleSpeedThermalStorage.condenserAirOutletNode());

  // Condenser Design Air Flow Rate: Required Double
  // Autocalculate
  coilCoolingDXSingleSpeedThermalStorage.autocalculateCondenserDesignAirFlowRate();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.isCondenserDesignAirFlowRateAutocalculated());
  // Set
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCondenserDesignAirFlowRate(9.2));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.condenserDesignAirFlowRate());
  EXPECT_EQ(9.2, coilCoolingDXSingleSpeedThermalStorage.condenserDesignAirFlowRate().get());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setCondenserDesignAirFlowRate(-10.0));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.condenserDesignAirFlowRate());
  EXPECT_EQ(9.2, coilCoolingDXSingleSpeedThermalStorage.condenserDesignAirFlowRate().get());
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.isCondenserDesignAirFlowRateAutocalculated());

  // Condenser Air Flow Sizing Factor: Required Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCondenserAirFlowSizingFactor(9.3));
  EXPECT_EQ(9.3, coilCoolingDXSingleSpeedThermalStorage.condenserAirFlowSizingFactor());

  // Condenser Type: Required String
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCondenserType("AirCooled"));
  EXPECT_EQ("AirCooled", coilCoolingDXSingleSpeedThermalStorage.condenserType());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setCondenserType("BADENUM"));
  EXPECT_EQ("AirCooled", coilCoolingDXSingleSpeedThermalStorage.condenserType());

  // Evaporative Condenser Effectiveness: Required Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setEvaporativeCondenserEffectiveness(0.99));
  EXPECT_EQ(0.99, coilCoolingDXSingleSpeedThermalStorage.evaporativeCondenserEffectiveness());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setEvaporativeCondenserEffectiveness(-10.0));
  EXPECT_EQ(0.99, coilCoolingDXSingleSpeedThermalStorage.evaporativeCondenserEffectiveness());

  // Evaporative Condenser Pump Rated Power Consumption: Required Double
  // Autosize
  coilCoolingDXSingleSpeedThermalStorage.autosizeEvaporativeCondenserPumpRatedPowerConsumption();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.isEvaporativeCondenserPumpRatedPowerConsumptionAutosized());
  // Set
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setEvaporativeCondenserPumpRatedPowerConsumption(9.6));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.evaporativeCondenserPumpRatedPowerConsumption());
  EXPECT_EQ(9.6, coilCoolingDXSingleSpeedThermalStorage.evaporativeCondenserPumpRatedPowerConsumption().get());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setEvaporativeCondenserPumpRatedPowerConsumption(-10.0));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.evaporativeCondenserPumpRatedPowerConsumption());
  EXPECT_EQ(9.6, coilCoolingDXSingleSpeedThermalStorage.evaporativeCondenserPumpRatedPowerConsumption().get());
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.isEvaporativeCondenserPumpRatedPowerConsumptionAutosized());

  // Basin Heater Capacity: Required Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setBasinHeaterCapacity(9.7));
  EXPECT_EQ(9.7, coilCoolingDXSingleSpeedThermalStorage.basinHeaterCapacity());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setBasinHeaterCapacity(-10.0));
  EXPECT_EQ(9.7, coilCoolingDXSingleSpeedThermalStorage.basinHeaterCapacity());

  // Basin Heater Setpoint Temperature: Required Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setBasinHeaterSetpointTemperature(11.8));
  EXPECT_EQ(11.8, coilCoolingDXSingleSpeedThermalStorage.basinHeaterSetpointTemperature());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setBasinHeaterSetpointTemperature(-8.0));
  EXPECT_EQ(11.8, coilCoolingDXSingleSpeedThermalStorage.basinHeaterSetpointTemperature());

  // Basin Heater Availability Schedule: Optional Object
  boost::optional<Schedule> basinHeaterAvailabilitySchedule(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setBasinHeaterAvailabilitySchedule(basinHeaterAvailabilitySchedule));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.basinHeaterAvailabilitySchedule());
  EXPECT_EQ(basinHeaterAvailabilitySchedule, coilCoolingDXSingleSpeedThermalStorage.basinHeaterAvailabilitySchedule().get());

  // Supply Water Storage Tank: Optional Object
  boost::optional<WaterStorageTank> supplyWaterStorageTank(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setSupplyWaterStorageTank(supplyWaterStorageTank));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.supplyWaterStorageTank());
  EXPECT_EQ(supplyWaterStorageTank, coilCoolingDXSingleSpeedThermalStorage.supplyWaterStorageTank().get());

  // Condensate Collection Water Storage Tank: Optional Object
  boost::optional<WaterStorageTank> condensateCollectionWaterStorageTank(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCondensateCollectionWaterStorageTank(condensateCollectionWaterStorageTank));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.condensateCollectionWaterStorageTank());
  EXPECT_EQ(condensateCollectionWaterStorageTank, coilCoolingDXSingleSpeedThermalStorage.condensateCollectionWaterStorageTank().get());

  // Storage Tank Plant Connection Inlet Node: Optional Object
  boost::optional<Connection> storageTankPlantConnectionInletNode(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setStorageTankPlantConnectionInletNode(storageTankPlantConnectionInletNode));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.storageTankPlantConnectionInletNode());
  EXPECT_EQ(storageTankPlantConnectionInletNode, coilCoolingDXSingleSpeedThermalStorage.storageTankPlantConnectionInletNode().get());

  // Storage Tank Plant Connection Outlet Node: Optional Object
  boost::optional<Connection> storageTankPlantConnectionOutletNode(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setStorageTankPlantConnectionOutletNode(storageTankPlantConnectionOutletNode));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.storageTankPlantConnectionOutletNode());
  EXPECT_EQ(storageTankPlantConnectionOutletNode, coilCoolingDXSingleSpeedThermalStorage.storageTankPlantConnectionOutletNode().get());

  // Storage Tank Plant Connection Design Flow Rate: Optional Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setStorageTankPlantConnectionDesignFlowRate(10.4));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.storageTankPlantConnectionDesignFlowRate());
  EXPECT_EQ(10.4, coilCoolingDXSingleSpeedThermalStorage.storageTankPlantConnectionDesignFlowRate().get());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setStorageTankPlantConnectionDesignFlowRate(-10.0));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.storageTankPlantConnectionDesignFlowRate());
  EXPECT_EQ(10.4, coilCoolingDXSingleSpeedThermalStorage.storageTankPlantConnectionDesignFlowRate().get());

  // Storage Tank Plant Connection Heat Transfer Effectiveness: Required Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setStorageTankPlantConnectionHeatTransferEffectiveness(0.991));
  EXPECT_EQ(0.991, coilCoolingDXSingleSpeedThermalStorage.storageTankPlantConnectionHeatTransferEffectiveness());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setStorageTankPlantConnectionHeatTransferEffectiveness(-10.0));
  EXPECT_EQ(0.991, coilCoolingDXSingleSpeedThermalStorage.storageTankPlantConnectionHeatTransferEffectiveness());

  // Storage Tank Minimum Operating Limit Fluid Temperature: Optional Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setStorageTankMinimumOperatingLimitFluidTemperature(10.6));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.storageTankMinimumOperatingLimitFluidTemperature());
  EXPECT_EQ(10.6, coilCoolingDXSingleSpeedThermalStorage.storageTankMinimumOperatingLimitFluidTemperature().get());

  // Storage Tank Maximum Operating Limit Fluid Temperature: Optional Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setStorageTankMaximumOperatingLimitFluidTemperature(10.7));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.storageTankMaximumOperatingLimitFluidTemperature());
  EXPECT_EQ(10.7, coilCoolingDXSingleSpeedThermalStorage.storageTankMaximumOperatingLimitFluidTemperature().get());
}
TEST_F(ModelFixture, CoilCoolingDXSingleSpeedThermalStorage_HeatCoolFuelTypes) {
  Model m;
  // TODO: Check regular Ctor arguments
  CoilCoolingDXSingleSpeedThermalStorage coilCoolingDXSingleSpeedThermalStorage(m);
  // TODO: Or if a UniqueModelObject (and make sure _Impl is included)
  // CoilCoolingDXSingleSpeedThermalStorage coilCoolingDXSingleSpeedThermalStorage = m.getUniqueModelObject<CoilCoolingDXSingleSpeedThermalStorage>();

  EXPECT_EQ(ComponentType(ComponentType::Both), coilCoolingDXSingleSpeedThermalStorage.componentType());
  testFuelTypeEquality({FuelType::Electricity}, coilCoolingDXSingleSpeedThermalStorage.coolingFuelTypes());
  testFuelTypeEquality({FuelType::Electricity, FuelType::Propane}, coilCoolingDXSingleSpeedThermalStorage.heatingFuelTypes());
  testAppGFuelTypeEquality({AppGFuelType::Fuel, AppGFuelType::HeatPump}, coilCoolingDXSingleSpeedThermalStorage.appGHeatingFuelTypes());
}