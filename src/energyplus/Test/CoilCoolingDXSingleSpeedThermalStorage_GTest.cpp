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

#include "<gtest/gtest.h>"
#include "EnergyPlusFixture.hpp"

#include "ForwardTranslator.hpp"
#include "ReverseTranslator.hpp"

#include "../CoilCoolingDXSingleSpeedThermalStorage.hpp"
#include "../CoilCoolingDXSingleSpeedThermalStorage_Impl.hpp"
// TODO: Check the following class names against object getters and setters.
#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"
#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"
#include "../FluidAndGlycol.hpp"
#include "../FluidAndGlycol_Impl.hpp"
#include "../BivariateFunctions.hpp"
#include "../BivariateFunctions_Impl.hpp"
#include "../UnivariateFunctions.hpp"
#include "../UnivariateFunctions_Impl.hpp"
#include "../BivariateFunctions.hpp"
#include "../BivariateFunctions_Impl.hpp"
#include "../UnivariateFunctions.hpp"
#include "../UnivariateFunctions_Impl.hpp"
#include "../UnivariateFunctions.hpp"
#include "../UnivariateFunctions_Impl.hpp"
#include "../BivariateFunctions.hpp"
#include "../BivariateFunctions_Impl.hpp"
#include "../UnivariateFunctions.hpp"
#include "../UnivariateFunctions_Impl.hpp"
#include "../TrivariateFunctions.hpp"
#include "../TrivariateFunctions_Impl.hpp"
#include "../UnivariateFunctions.hpp"
#include "../UnivariateFunctions_Impl.hpp"
#include "../TrivariateFunctions.hpp"
#include "../TrivariateFunctions_Impl.hpp"
#include "../UnivariateFunctions.hpp"
#include "../UnivariateFunctions_Impl.hpp"
#include "../UnivariateFunctions.hpp"
#include "../UnivariateFunctions_Impl.hpp"
#include "../TrivariateFunctions.hpp"
#include "../TrivariateFunctions_Impl.hpp"
#include "../UnivariateFunctions.hpp"
#include "../UnivariateFunctions_Impl.hpp"
#include "../TrivariateFunctions.hpp"
#include "../TrivariateFunctions_Impl.hpp"
#include "../UnivariateFunctions.hpp"
#include "../UnivariateFunctions_Impl.hpp"
#include "../UnivariateFunctions.hpp"
#include "../UnivariateFunctions_Impl.hpp"
#include "../BivariateFunctions.hpp"
#include "../BivariateFunctions_Impl.hpp"
#include "../UnivariateFunctions.hpp"
#include "../UnivariateFunctions_Impl.hpp"
#include "../TrivariateFunctions.hpp"
#include "../TrivariateFunctions_Impl.hpp"
#include "../UnivariateFunctions.hpp"
#include "../UnivariateFunctions_Impl.hpp"
#include "../TrivariateFunctions.hpp"
#include "../TrivariateFunctions_Impl.hpp"
#include "../UnivariateFunctions.hpp"
#include "../UnivariateFunctions_Impl.hpp"
#include "../UnivariateFunctions.hpp"
#include "../UnivariateFunctions_Impl.hpp"
#include "../TrivariateFunctions.hpp"
#include "../TrivariateFunctions_Impl.hpp"
#include "../UnivariateFunctions.hpp"
#include "../UnivariateFunctions_Impl.hpp"
#include "../UnivariateFunctions.hpp"
#include "../UnivariateFunctions_Impl.hpp"
#include "../TrivariateFunctions.hpp"
#include "../TrivariateFunctions_Impl.hpp"
#include "../UnivariateFunctions.hpp"
#include "../UnivariateFunctions_Impl.hpp"
#include "../UnivariateFunctions.hpp"
#include "../UnivariateFunctions_Impl.hpp"
#include "../BivariateFunctions.hpp"
#include "../BivariateFunctions_Impl.hpp"
#include "../UnivariateFunctions.hpp"
#include "../UnivariateFunctions_Impl.hpp"
#include "../BivariateFunctions.hpp"
#include "../BivariateFunctions_Impl.hpp"
#include "../BivariateFunctions.hpp"
#include "../BivariateFunctions_Impl.hpp"
#include "../BivariateFunctions.hpp"
#include "../BivariateFunctions_Impl.hpp"
#include "../UnivariateFunctions.hpp"
#include "../UnivariateFunctions_Impl.hpp"
#include "../BivariateFunctions.hpp"
#include "../BivariateFunctions_Impl.hpp"
#include "../UnivariateFunctions.hpp"
#include "../UnivariateFunctions_Impl.hpp"
#include "../UnivariateFunctions.hpp"
#include "../UnivariateFunctions_Impl.hpp"
#include "../BivariateFunctions.hpp"
#include "../BivariateFunctions_Impl.hpp"
#include "../UnivariateFunctions.hpp"
#include "../UnivariateFunctions_Impl.hpp"
#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"
#include "../WaterStorageTank.hpp"
#include "../WaterStorageTank_Impl.hpp"
#include "../WaterStorageTank.hpp"
#include "../WaterStorageTank_Impl.hpp"

#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
// E+ FieldEnums
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/Coil_Cooling_DX_SingleSpeed_ThermalStorage_FieldEnums.hxx>
using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilCoolingDXSingleSpeedThermalStorage) {

  ForwardTranslator ft;

  Model m;
  // TODO: Check regular Ctor arguments
  CoilCoolingDXSingleSpeedThermalStorage coilCoolingDXSingleSpeedThermalStorage(m);
  // TODO: Or if a UniqueModelObject (and make sure _Impl is included)
  // CoilCoolingDXSingleSpeedThermalStorage coilCoolingDXSingleSpeedThermalStorage = m.getUniqueModelObject<CoilCoolingDXSingleSpeedThermalStorage>();

  coilCoolingDXSingleSpeedThermalStorage.setName("My CoilCoolingDXSingleSpeedThermalStorage");
  boost::optional<Schedule> availabilitySchedule(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setAvailabilitySchedule(availabilitySchedule));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setOperatingModeControlMethod("ScheduledModes"));
  boost::optional<Schedule> operationModeControlSchedule(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setOperationModeControlSchedule(operationModeControlSchedule));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setStorageType("Water"));
  boost::optional<FluidAndGlycol> userDefinedFluidType(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setUserDefinedFluidType(userDefinedFluidType));
  // Autocalculate
  // coilCoolingDXSingleSpeedThermalStorage.autocalculateFluidStorageVolume();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setFluidStorageVolume(0.7));
  // Autocalculate
  // coilCoolingDXSingleSpeedThermalStorage.autocalculateIceStorageCapacity();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setIceStorageCapacity(0.8));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setStorageCapacitySizingFactor(0.9));
  Node storageTankAmbientTemperatureNodeName(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setStorageTankAmbientTemperatureNodeName(storageTankAmbientTemperatureNodeName));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setStorageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient(1.1));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setFluidStorageTankRatingTemperature(1.2));
  // Autosize
  // coilCoolingDXSingleSpeedThermalStorage.autosizeRatedEvaporatorAirFlowRate();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setRatedEvaporatorAirFlowRate(1.3));
  Node evaporatorAirInletNodeName(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setEvaporatorAirInletNodeName(evaporatorAirInletNodeName));
  Node evaporatorAirOutletNodeName(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setEvaporatorAirOutletNodeName(evaporatorAirOutletNodeName));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeAvailable(true));  // TODO: check this isnt the same as the Ctor
  // Autosize
  // coilCoolingDXSingleSpeedThermalStorage.autosizeCoolingOnlyModeRatedTotalEvaporatorCoolingCapacity();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeRatedTotalEvaporatorCoolingCapacity(1.7));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeRatedSensibleHeatRatio(0.947));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeRatedCOP(1.9));
  boost::optional<BivariateFunctions> coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(
    coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve));
  boost::optional<UnivariateFunctions> coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(
    coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve));
  boost::optional<BivariateFunctions> coolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve(
    coolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve));
  boost::optional<UnivariateFunctions> coolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve(
    coolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve));
  boost::optional<UnivariateFunctions> coolingOnlyModePartLoadFractionCorrelationCurve(m);
  EXPECT_TRUE(
    coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModePartLoadFractionCorrelationCurve(coolingOnlyModePartLoadFractionCorrelationCurve));
  boost::optional<BivariateFunctions> coolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve(
    coolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve));
  boost::optional<UnivariateFunctions> coolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve(
    coolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeAvailable(true));  // TODO: check this isnt the same as the Ctor
  // Autocalculate
  // coilCoolingDXSingleSpeedThermalStorage.autocalculateCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity(2.8));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeCapacitySizingFactor(2.9));
  // Autocalculate
  // coilCoolingDXSingleSpeedThermalStorage.autocalculateCoolingAndChargeModeRatedStorageChargingCapacity();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeRatedStorageChargingCapacity(3.0));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeStorageCapacitySizingFactor(3.1));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeRatedSensibleHeatRatio(0.97));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeCoolingRatedCOP(3.3));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeChargingRatedCOP(3.4));
  boost::optional<TrivariateFunctions> coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(
    coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve));
  boost::optional<UnivariateFunctions> coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(
    coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve));
  boost::optional<TrivariateFunctions> coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve(
    coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve));
  boost::optional<UnivariateFunctions> coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve(
    coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve));
  boost::optional<UnivariateFunctions> coolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve(
    coolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve));
  boost::optional<TrivariateFunctions> coolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve(
    coolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve));
  boost::optional<UnivariateFunctions> coolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve(
    coolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve));
  boost::optional<TrivariateFunctions> coolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve(
    coolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve));
  boost::optional<UnivariateFunctions> coolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve(
    coolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve));
  boost::optional<UnivariateFunctions> coolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve(
    coolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve));
  boost::optional<BivariateFunctions> coolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve(
    coolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve));
  boost::optional<UnivariateFunctions> coolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve(
    coolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeAvailable(true));  // TODO: check this isnt the same as the Ctor
  // Autocalculate
  // coilCoolingDXSingleSpeedThermalStorage.autocalculateCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity(4.8));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeEvaporatorCapacitySizingFactor(4.9));
  // Autocalculate
  // coilCoolingDXSingleSpeedThermalStorage.autocalculateCoolingAndDischargeModeRatedStorageDischargingCapacity();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeRatedStorageDischargingCapacity(5.0));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeStorageDischargeCapacitySizingFactor(5.1));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeRatedSensibleHeatRatio(0.981));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeCoolingRatedCOP(5.3));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeDischargingRatedCOP(5.4));
  boost::optional<TrivariateFunctions> coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(
    coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve));
  boost::optional<UnivariateFunctions> coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(
    coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve));
  boost::optional<TrivariateFunctions> coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve(
    coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve));
  boost::optional<UnivariateFunctions> coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve(
    coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve));
  boost::optional<UnivariateFunctions> coolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve(
    coolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve));
  boost::optional<TrivariateFunctions> coolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve(
    coolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve));
  boost::optional<UnivariateFunctions> coolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve(
    coolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve));
  boost::optional<UnivariateFunctions> coolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve(
    coolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve));
  boost::optional<TrivariateFunctions> coolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve(
    coolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve));
  boost::optional<UnivariateFunctions> coolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve(
    coolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve));
  boost::optional<UnivariateFunctions> coolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve(
    coolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve));
  boost::optional<BivariateFunctions> coolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve(
    coolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve));
  boost::optional<UnivariateFunctions> coolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve(
    coolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setChargeOnlyModeAvailable(true));  // TODO: check this isnt the same as the Ctor
  // Autocalculate
  // coilCoolingDXSingleSpeedThermalStorage.autocalculateChargeOnlyModeRatedStorageChargingCapacity();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setChargeOnlyModeRatedStorageChargingCapacity(6.9));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setChargeOnlyModeCapacitySizingFactor(7.0));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setChargeOnlyModeChargingRatedCOP(7.1));
  boost::optional<BivariateFunctions> chargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setChargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve(
    chargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve));
  boost::optional<BivariateFunctions> chargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setChargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve(
    chargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeAvailable(true));  // TODO: check this isnt the same as the Ctor
  // Autocalculate
  // coilCoolingDXSingleSpeedThermalStorage.autocalculateDischargeOnlyModeRatedStorageDischargingCapacity();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeRatedStorageDischargingCapacity(7.5));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeCapacitySizingFactor(7.6));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeRatedSensibleHeatRatio(0.987));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeRatedCOP(7.8));
  boost::optional<BivariateFunctions> dischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve(
    dischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve));
  boost::optional<UnivariateFunctions> dischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve(
    dischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve));
  boost::optional<BivariateFunctions> dischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve(
    dischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve));
  boost::optional<UnivariateFunctions> dischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve(
    dischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve));
  boost::optional<UnivariateFunctions> dischargeOnlyModePartLoadFractionCorrelationCurve(m);
  EXPECT_TRUE(
    coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModePartLoadFractionCorrelationCurve(dischargeOnlyModePartLoadFractionCorrelationCurve));
  boost::optional<BivariateFunctions> dischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve(
    dischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve));
  boost::optional<UnivariateFunctions> dischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve(
    dischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setAncillaryElectricPower(8.6));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setColdWeatherOperationMinimumOutdoorAirTemperature(8.7));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setColdWeatherOperationAncillaryPower(8.8));
  Node condenserAirInletNodeName(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCondenserAirInletNodeName(condenserAirInletNodeName));
  Node condenserAirOutletNodeName(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCondenserAirOutletNodeName(condenserAirOutletNodeName));
  // Autocalculate
  // coilCoolingDXSingleSpeedThermalStorage.autocalculateCondenserDesignAirFlowRate();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCondenserDesignAirFlowRate(9.1));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCondenserAirFlowSizingFactor(9.2));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCondenserType("AirCooled"));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setEvaporativeCondenserEffectiveness(0.989));
  // Autosize
  // coilCoolingDXSingleSpeedThermalStorage.autosizeEvaporativeCondenserPumpRatedPowerConsumption();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setEvaporativeCondenserPumpRatedPowerConsumption(9.5));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setBasinHeaterCapacity(9.6));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setBasinHeaterSetpointTemperature(11.7));
  boost::optional<Schedule> basinHeaterAvailabilitySchedule(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setBasinHeaterAvailabilitySchedule(basinHeaterAvailabilitySchedule));
  boost::optional<WaterStorageTank> supplyWaterStorageTank(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setSupplyWaterStorageTank(supplyWaterStorageTank));
  boost::optional<WaterStorageTank> condensateCollectionWaterStorageTank(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCondensateCollectionWaterStorageTank(condensateCollectionWaterStorageTank));
  Node storageTankPlantConnectionInletNodeName(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setStorageTankPlantConnectionInletNodeName(storageTankPlantConnectionInletNodeName));
  Node storageTankPlantConnectionOutletNodeName(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setStorageTankPlantConnectionOutletNodeName(storageTankPlantConnectionOutletNodeName));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setStorageTankPlantConnectionDesignFlowRate(10.3));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setStorageTankPlantConnectionHeatTransferEffectiveness(0.99));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setStorageTankMinimumOperatingLimitFluidTemperature(10.5));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setStorageTankMaximumOperatingLimitFluidTemperature(10.6));

  // TODO: you're responsible for creating all other objects needed so this object actually gets ForwardTranslated

  const Workspace w = ft.translateModel(m);
  const auto idfObjs = w.getObjectsByType(IddObjectType::Coil_Cooling_DX_SingleSpeed_ThermalStorage);
  ASSERT_EQ(1u, idfObjs.size());

  const auto& idfObject = idfObjs.front();
  EXPECT_EQ(availabilitySchedule.nameString(), idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::AvailabilityScheduleName).get());
  EXPECT_EQ("ScheduledModes", idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::OperatingModeControlMethod).get());
  EXPECT_EQ(operationModeControlSchedule.nameString(),
            idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::OperationModeControlScheduleName).get());
  EXPECT_EQ("Water", idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageType).get());
  EXPECT_EQ(userDefinedFluidType.nameString(), idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::UserDefinedFluidType).get());
  // EXPECT_EQ("Autocalculate", idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::FluidStorageVolume).get());  EXPECT_EQ(0.7, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::FluidStorageVolume).get());  // EXPECT_EQ("Autocalculate", idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::IceStorageCapacity).get());  EXPECT_EQ(0.8, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::IceStorageCapacity).get());  EXPECT_EQ(0.9, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageCapacitySizingFactor).get()); EXPECT_EQ(storageTankAmbientTemperatureNodeName.nameString(), idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankAmbientTemperatureNodeName).get());
  EXPECT_EQ(1.1,
            idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient).get());
  EXPECT_EQ(
    1.2,
    idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::FluidStorageTankRatingTemperature)
      .get());  // EXPECT_EQ("Autosize", idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::RatedEvaporatorAirFlowRate).get());  EXPECT_EQ(1.3, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::RatedEvaporatorAirFlowRate).get()); EXPECT_EQ(evaporatorAirInletNodeName.nameString(), idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::EvaporatorAirInletNodeName).get());
  EXPECT_EQ(evaporatorAirOutletNodeName.nameString(),
            idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::EvaporatorAirOutletNodeName).get());
  EXPECT_EQ(
    "Yes",
    idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeAvailable)
      .get());  // EXPECT_EQ("Autosize", idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeRatedTotalEvaporatorCoolingCapacity).get());  EXPECT_EQ(1.7, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeRatedTotalEvaporatorCoolingCapacity).get());  EXPECT_EQ(0.947, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeRatedSensibleHeatRatio).get());  EXPECT_EQ(1.9, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeRatedCOP).get()); EXPECT_EQ(coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve.nameString(), idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurveName).get());
  EXPECT_EQ(
    coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve.nameString(),
    idfObject
      .getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurveName)
      .get());
  EXPECT_EQ(
    coolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve.nameString(),
    idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeEnergyInputRatioFunctionofTemperatureCurveName).get());
  EXPECT_EQ(
    coolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve.nameString(),
    idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurveName).get());
  EXPECT_EQ(coolingOnlyModePartLoadFractionCorrelationCurve.nameString(),
            idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModePartLoadFractionCorrelationCurveName).get());
  EXPECT_EQ(
    coolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve.nameString(),
    idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurveName).get());
  EXPECT_EQ(
    coolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve.nameString(),
    idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurveName).get());
  EXPECT_EQ(
    "Yes",
    idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeAvailable)
      .get());  // EXPECT_EQ("Autocalculate", idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity).get());  EXPECT_EQ(2.8, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity).get());  EXPECT_EQ(2.9, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeCapacitySizingFactor).get());  // EXPECT_EQ("Autocalculate", idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeRatedStorageChargingCapacity).get());  EXPECT_EQ(3.0, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeRatedStorageChargingCapacity).get());  EXPECT_EQ(3.1, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageCapacitySizingFactor).get());  EXPECT_EQ(0.97, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeRatedSensibleHeatRatio).get());  EXPECT_EQ(3.3, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeCoolingRatedCOP).get());  EXPECT_EQ(3.4, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeChargingRatedCOP).get()); EXPECT_EQ(coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve.nameString(), idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurveName).get());
  EXPECT_EQ(
    coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve.nameString(),
    idfObject
      .getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurveName)
      .get());
  EXPECT_EQ(
    coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve.nameString(),
    idfObject
      .getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurveName)
      .get());
  EXPECT_EQ(
    coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve.nameString(),
    idfObject
      .getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurveName)
      .get());
  EXPECT_EQ(
    coolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve.nameString(),
    idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurveName).get());
  EXPECT_EQ(
    coolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve.nameString(),
    idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurveName)
      .get());
  EXPECT_EQ(
    coolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve.nameString(),
    idfObject
      .getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurveName)
      .get());
  EXPECT_EQ(
    coolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve.nameString(),
    idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurveName)
      .get());
  EXPECT_EQ(
    coolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve.nameString(),
    idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurveName)
      .get());
  EXPECT_EQ(
    coolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve.nameString(),
    idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurveName)
      .get());
  EXPECT_EQ(
    coolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve.nameString(),
    idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurveName).get());
  EXPECT_EQ(
    coolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve.nameString(),
    idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurveName)
      .get());
  EXPECT_EQ(
    "Yes",
    idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeAvailable)
      .get());  // EXPECT_EQ("Autocalculate", idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity).get());  EXPECT_EQ(4.8, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity).get());  EXPECT_EQ(4.9, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeEvaporatorCapacitySizingFactor).get());  // EXPECT_EQ("Autocalculate", idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeRatedStorageDischargingCapacity).get());  EXPECT_EQ(5.0, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeRatedStorageDischargingCapacity).get());  EXPECT_EQ(5.1, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageDischargeCapacitySizingFactor).get());  EXPECT_EQ(0.981, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeRatedSensibleHeatRatio).get());  EXPECT_EQ(5.3, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeCoolingRatedCOP).get());  EXPECT_EQ(5.4, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeDischargingRatedCOP).get()); EXPECT_EQ(coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve.nameString(), idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurveName).get());
  EXPECT_EQ(
    coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve.nameString(),
    idfObject
      .getString(
        Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurveName)
      .get());
  EXPECT_EQ(
    coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve.nameString(),
    idfObject
      .getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurveName)
      .get());
  EXPECT_EQ(
    coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve.nameString(),
    idfObject
      .getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurveName)
      .get());
  EXPECT_EQ(
    coolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve.nameString(),
    idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurveName)
      .get());
  EXPECT_EQ(
    coolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve.nameString(),
    idfObject
      .getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurveName)
      .get());
  EXPECT_EQ(
    coolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve.nameString(),
    idfObject
      .getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurveName)
      .get());
  EXPECT_EQ(
    coolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve.nameString(),
    idfObject
      .getString(
        Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurveName)
      .get());
  EXPECT_EQ(
    coolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve.nameString(),
    idfObject
      .getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurveName)
      .get());
  EXPECT_EQ(
    coolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve.nameString(),
    idfObject
      .getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurveName)
      .get());
  EXPECT_EQ(
    coolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve.nameString(),
    idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurveName)
      .get());
  EXPECT_EQ(
    coolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve.nameString(),
    idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurveName)
      .get());
  EXPECT_EQ(
    coolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve.nameString(),
    idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurveName)
      .get());
  EXPECT_EQ(
    "Yes",
    idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeAvailable)
      .get());  // EXPECT_EQ("Autocalculate", idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeRatedStorageChargingCapacity).get());  EXPECT_EQ(6.9, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeRatedStorageChargingCapacity).get());  EXPECT_EQ(7.0, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeCapacitySizingFactor).get());  EXPECT_EQ(7.1, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeChargingRatedCOP).get()); EXPECT_EQ(chargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve.nameString(), idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurveName).get());
  EXPECT_EQ(
    chargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve.nameString(),
    idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurveName).get());
  EXPECT_EQ(
    "Yes",
    idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeAvailable)
      .get());  // EXPECT_EQ("Autocalculate", idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeRatedStorageDischargingCapacity).get());  EXPECT_EQ(7.5, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeRatedStorageDischargingCapacity).get());  EXPECT_EQ(7.6, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeCapacitySizingFactor).get());  EXPECT_EQ(0.987, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeRatedSensibleHeatRatio).get());  EXPECT_EQ(7.8, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeRatedCOP).get()); EXPECT_EQ(dischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve.nameString(), idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurveName).get());
  EXPECT_EQ(
    dischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve.nameString(),
    idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurveName)
      .get());
  EXPECT_EQ(
    dischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve.nameString(),
    idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurveName).get());
  EXPECT_EQ(
    dischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve.nameString(),
    idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurveName).get());
  EXPECT_EQ(dischargeOnlyModePartLoadFractionCorrelationCurve.nameString(),
            idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModePartLoadFractionCorrelationCurveName).get());
  EXPECT_EQ(
    dischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve.nameString(),
    idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurveName).get());
  EXPECT_EQ(
    dischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve.nameString(),
    idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurveName).get());
  EXPECT_EQ(8.6, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::AncillaryElectricPower).get());
  EXPECT_EQ(8.7, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ColdWeatherOperationMinimumOutdoorAirTemperature).get());
  EXPECT_EQ(8.8, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ColdWeatherOperationAncillaryPower).get());
  EXPECT_EQ(condenserAirInletNodeName.nameString(),
            idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondenserAirInletNodeName).get());
  EXPECT_EQ(condenserAirOutletNodeName.nameString(),
            idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondenserAirOutletNodeName).get());
  // EXPECT_EQ("Autocalculate", idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondenserDesignAirFlowRate).get());  EXPECT_EQ(9.1, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondenserDesignAirFlowRate).get());  EXPECT_EQ(9.2, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondenserAirFlowSizingFactor).get());  EXPECT_EQ("AirCooled", idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondenserType).get());  EXPECT_EQ(0.989, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::EvaporativeCondenserEffectiveness).get());  // EXPECT_EQ("Autosize", idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::EvaporativeCondenserPumpRatedPowerConsumption).get());  EXPECT_EQ(9.5, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::EvaporativeCondenserPumpRatedPowerConsumption).get());  EXPECT_EQ(9.6, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::BasinHeaterCapacity).get());  EXPECT_EQ(11.7, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::BasinHeaterSetpointTemperature).get()); EXPECT_EQ(basinHeaterAvailabilitySchedule.nameString(), idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::BasinHeaterAvailabilityScheduleName).get());
  EXPECT_EQ(supplyWaterStorageTank.nameString(),
            idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::SupplyWaterStorageTankName).get());
  EXPECT_EQ(condensateCollectionWaterStorageTank.nameString(),
            idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondensateCollectionWaterStorageTankName).get());
  EXPECT_EQ(storageTankPlantConnectionInletNodeName.nameString(),
            idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankPlantConnectionInletNodeName).get());
  EXPECT_EQ(storageTankPlantConnectionOutletNodeName.nameString(),
            idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankPlantConnectionOutletNodeName).get());
  EXPECT_EQ(10.3, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankPlantConnectionDesignFlowRate).get());
  EXPECT_EQ(0.99, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankPlantConnectionHeatTransferEffectiveness).get());
  EXPECT_EQ(10.5, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankMinimumOperatingLimitFluidTemperature).get());
  EXPECT_EQ(10.6, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankMaximumOperatingLimitFluidTemperature).get());
}