/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/CoilCoolingDXSingleSpeedThermalStorage.hpp"
#include "../../model/CoilCoolingDXSingleSpeedThermalStorage_Impl.hpp"

#include "../../model/ScheduleConstant.hpp"
#include "../../model/ScheduleConstant_Impl.hpp"
#include "../../model/CurveBiquadratic.hpp"
#include "../../model/CurveBiquadratic_Impl.hpp"
#include "../../model/CurveQuadratic.hpp"
#include "../../model/CurveQuadratic_Impl.hpp"
#include "../../model/CurveTriquadratic.hpp"
#include "../../model/CurveTriquadratic_Impl.hpp"

#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVACUnitarySystem.hpp"
#include "../../model/Node.hpp"

#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/Coil_Cooling_DX_SingleSpeed_ThermalStorage_FieldEnums.hxx>
#include <utilities/idd/FluidProperties_Name_FieldEnums.hxx>
#include <utilities/idd/CoilSystem_Cooling_DX_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_FieldEnums.hxx>
#include <utilities/idd/BranchList_FieldEnums.hxx>
#include <utilities/idd/Branch_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_UnitarySystem_FieldEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilCoolingDXSingleSpeedThermalStorage_Unitary) {

  ForwardTranslator ft;

  Model m;

  CoilCoolingDXSingleSpeedThermalStorage coilCoolingDXSingleSpeedThermalStorage(m);

  AirLoopHVACUnitarySystem unitary(m);
  unitary.setCoolingCoil(coilCoolingDXSingleSpeedThermalStorage);
  AirLoopHVAC airLoop(m);
  Node supplyOutletNode = airLoop.supplyOutletNode();
  unitary.addToNode(supplyOutletNode);

  coilCoolingDXSingleSpeedThermalStorage.setName("My CoilCoolingDXSingleSpeedThermalStorage");
  ScheduleConstant availabilitySchedule(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setAvailabilitySchedule(availabilitySchedule));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setOperatingModeControlMethod("ScheduledModes"));
  ScheduleConstant operationModeControlSchedule(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setOperationModeControlSchedule(operationModeControlSchedule));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setStorageType("PropyleneGlycol"));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setGlycolConcentration(25));
  // Autosize
  // coilCoolingDXSingleSpeedThermalStorage.autosizeFluidStorageVolume();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setFluidStorageVolume(0.7));
  // Autosize
  // coilCoolingDXSingleSpeedThermalStorage.autosizeIceStorageCapacity();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setIceStorageCapacity(0.8));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setStorageCapacitySizingFactor(0.9));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setStorageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient(1.1));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setFluidStorageTankRatingTemperature(1.2));
  // Autosize
  // coilCoolingDXSingleSpeedThermalStorage.autosizeRatedEvaporatorAirFlowRate();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setRatedEvaporatorAirFlowRate(1.3));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeAvailable(true));
  // Autosize
  // coilCoolingDXSingleSpeedThermalStorage.autosizeCoolingOnlyModeRatedTotalEvaporatorCoolingCapacity();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeRatedTotalEvaporatorCoolingCapacity(1.7));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeRatedSensibleHeatRatio(0.947));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeRatedCOP(1.9));
  CurveBiquadratic coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(
    coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve));
  CurveQuadratic coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(
    coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve));
  CurveBiquadratic coolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve(
    coolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve));
  CurveQuadratic coolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve(
    coolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve));
  CurveQuadratic coolingOnlyModePartLoadFractionCorrelationCurve(m);
  EXPECT_TRUE(
    coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModePartLoadFractionCorrelationCurve(coolingOnlyModePartLoadFractionCorrelationCurve));
  CurveBiquadratic coolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve(
    coolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve));
  CurveQuadratic coolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve(
    coolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeAvailable(true));
  // Autosize
  // coilCoolingDXSingleSpeedThermalStorage.autosizeCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity(2.8));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeCapacitySizingFactor(2.9));
  // Autosize
  // coilCoolingDXSingleSpeedThermalStorage.autosizeCoolingAndChargeModeRatedStorageChargingCapacity();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeRatedStorageChargingCapacity(3.0));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeStorageCapacitySizingFactor(3.1));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeRatedSensibleHeatRatio(0.97));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeCoolingRatedCOP(3.3));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeChargingRatedCOP(3.4));
  CurveTriquadratic coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(
    coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve));
  CurveQuadratic coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(
    coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve));
  CurveTriquadratic coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve(
    coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve));
  CurveQuadratic coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve(
    coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve));
  CurveQuadratic coolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve(
    coolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve));
  CurveTriquadratic coolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve(
    coolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve));
  CurveQuadratic coolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve(
    coolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve));
  CurveTriquadratic coolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve(
    coolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve));
  CurveQuadratic coolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve(
    coolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve));
  CurveQuadratic coolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve(
    coolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve));
  CurveBiquadratic coolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve(
    coolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve));
  CurveQuadratic coolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve(
    coolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeAvailable(true));
  // Autosize
  // coilCoolingDXSingleSpeedThermalStorage.autosizeCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity(4.8));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeEvaporatorCapacitySizingFactor(4.9));
  // Autosize
  // coilCoolingDXSingleSpeedThermalStorage.autosizeCoolingAndDischargeModeRatedStorageDischargingCapacity();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeRatedStorageDischargingCapacity(5.0));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeStorageDischargeCapacitySizingFactor(5.1));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeRatedSensibleHeatRatio(0.981));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeCoolingRatedCOP(5.3));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeDischargingRatedCOP(5.4));
  CurveTriquadratic coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(
    coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve));
  CurveQuadratic coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(
    coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve));
  CurveTriquadratic coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve(
    coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve));
  CurveQuadratic coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve(
    coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve));
  CurveQuadratic coolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve(
    coolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve));
  CurveTriquadratic coolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve(
    coolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve));
  CurveQuadratic coolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve(
    coolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve));
  CurveQuadratic coolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve(
    coolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve));
  CurveTriquadratic coolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve(
    coolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve));
  CurveQuadratic coolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve(
    coolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve));
  CurveQuadratic coolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve(
    coolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve));
  CurveBiquadratic coolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve(
    coolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve));
  CurveQuadratic coolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve(
    coolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setChargeOnlyModeAvailable(true));
  // Autosize
  // coilCoolingDXSingleSpeedThermalStorage.autosizeChargeOnlyModeRatedStorageChargingCapacity();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setChargeOnlyModeRatedStorageChargingCapacity(6.9));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setChargeOnlyModeCapacitySizingFactor(7.0));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setChargeOnlyModeChargingRatedCOP(7.1));
  CurveBiquadratic chargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setChargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve(
    chargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve));
  CurveBiquadratic chargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setChargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve(
    chargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeAvailable(true));
  // Autosize
  // coilCoolingDXSingleSpeedThermalStorage.autosizeDischargeOnlyModeRatedStorageDischargingCapacity();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeRatedStorageDischargingCapacity(7.5));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeCapacitySizingFactor(7.6));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeRatedSensibleHeatRatio(0.987));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeRatedCOP(7.8));
  CurveBiquadratic dischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve(
    dischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve));
  CurveQuadratic dischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve(
    dischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve));
  CurveBiquadratic dischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve(
    dischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve));
  CurveQuadratic dischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve(
    dischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve));
  CurveQuadratic dischargeOnlyModePartLoadFractionCorrelationCurve(m);
  EXPECT_TRUE(
    coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModePartLoadFractionCorrelationCurve(dischargeOnlyModePartLoadFractionCorrelationCurve));
  CurveBiquadratic dischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve(
    dischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve));
  CurveQuadratic dischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve(
    dischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setAncillaryElectricPower(8.6));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setColdWeatherOperationMinimumOutdoorAirTemperature(8.7));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setColdWeatherOperationAncillaryPower(8.8));
  // Autosize
  // coilCoolingDXSingleSpeedThermalStorage.autosizeCondenserDesignAirFlowRate();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCondenserDesignAirFlowRate(9.1));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCondenserAirFlowSizingFactor(9.2));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCondenserType("AirCooled"));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setEvaporativeCondenserEffectiveness(0.989));
  // Autosize
  // coilCoolingDXSingleSpeedThermalStorage.autosizeEvaporativeCondenserPumpRatedPowerConsumption();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setEvaporativeCondenserPumpRatedPowerConsumption(9.5));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setBasinHeaterCapacity(9.6));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setBasinHeaterSetpointTemperature(11.7));
  ScheduleConstant basinHeaterAvailabilitySchedule(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setBasinHeaterAvailabilitySchedule(basinHeaterAvailabilitySchedule));
  // boost::optional<WaterStorageTank> supplyWaterStorageTank(m);
  // EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setSupplyWaterStorageTank(supplyWaterStorageTank));
  // boost::optional<WaterStorageTank> condensateCollectionWaterStorageTank(m);
  // EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCondensateCollectionWaterStorageTank(condensateCollectionWaterStorageTank));
  // Node storageTankPlantConnectionInletNodeName(m);
  // EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setStorageTankPlantConnectionInletNodeName(storageTankPlantConnectionInletNodeName));
  // Node storageTankPlantConnectionOutletNodeName(m);
  // EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setStorageTankPlantConnectionOutletNodeName(storageTankPlantConnectionOutletNodeName));
  // EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setStorageTankPlantConnectionDesignFlowRate(10.3));
  // EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setStorageTankPlantConnectionHeatTransferEffectiveness(0.99));
  // EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setStorageTankMinimumOperatingLimitFluidTemperature(10.5));
  // EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setStorageTankMaximumOperatingLimitFluidTemperature(10.6));

  const Workspace w = ft.translateModel(m);

  const auto idfObjs = w.getObjectsByType(IddObjectType::Coil_Cooling_DX_SingleSpeed_ThermalStorage);
  ASSERT_EQ(1u, idfObjs.size());
  const auto& idfObject = idfObjs.front();

  const auto idfUnitarys = w.getObjectsByType(IddObjectType::AirLoopHVAC_UnitarySystem);
  ASSERT_EQ(1u, idfUnitarys.size());
  const auto& idfUnitary = idfUnitarys.front();

  EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::FluidProperties_Name).size());

  EXPECT_EQ(availabilitySchedule.nameString(), idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::AvailabilityScheduleName).get());
  EXPECT_EQ("ScheduledModes", idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::OperatingModeControlMethod).get());
  EXPECT_EQ(operationModeControlSchedule.nameString(),
            idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::OperationModeControlScheduleName).get());
  EXPECT_EQ("UserDefinedFluidType", idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageType).get());
  boost::optional<WorkspaceObject> woUserDefinedFluidType(
    idfObject.getTarget(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::UserDefinedFluidType));
  ASSERT_TRUE(woUserDefinedFluidType);
  EXPECT_EQ(woUserDefinedFluidType->iddObject().type(), IddObjectType::FluidProperties_Name);
  EXPECT_EQ("PropyleneGlycol_25", woUserDefinedFluidType->getString(FluidProperties_NameFields::FluidName).get());
  EXPECT_EQ("Glycol", woUserDefinedFluidType->getString(FluidProperties_NameFields::FluidType).get());
  // EXPECT_EQ("Autosize", idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::FluidStorageVolume).get());
  EXPECT_EQ(0.7, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::FluidStorageVolume).get());
  // EXPECT_EQ("Autosize", idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::IceStorageCapacity).get());
  EXPECT_EQ(0.8, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::IceStorageCapacity).get());
  EXPECT_EQ(0.9, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageCapacitySizingFactor).get());
  EXPECT_EQ("My CoilCoolingDXSingleSpeedThermalStorage Storage Tank Ambient Temperature Node",
            idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankAmbientTemperatureNodeName).get());
  EXPECT_EQ(1.1,
            idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient).get());
  EXPECT_EQ(1.2, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::FluidStorageTankRatingTemperature).get());
  // EXPECT_EQ("Autosize", idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::RatedEvaporatorAirFlowRate).get());
  EXPECT_EQ(1.3, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::RatedEvaporatorAirFlowRate).get());

  // Check that Unitary ends up with CoilCoolingDXSingleSpeedThermalStorage
  EXPECT_EQ("Coil:Cooling:DX:SingleSpeed:ThermalStorage", idfUnitary.getString(AirLoopHVAC_UnitarySystemFields::CoolingCoilObjectType).get());
  EXPECT_EQ(idfObject.nameString(), idfUnitary.getString(AirLoopHVAC_UnitarySystemFields::CoolingCoilName).get());

  // Check Node connections
  EXPECT_NE("", idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::EvaporatorAirInletNodeName).get());
  EXPECT_NE("", idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::EvaporatorAirOutletNodeName).get());
  EXPECT_EQ(idfUnitary.getString(AirLoopHVAC_UnitarySystemFields::AirInletNodeName).get(),
            idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::EvaporatorAirInletNodeName).get());
  EXPECT_EQ(idfUnitary.getString(AirLoopHVAC_UnitarySystemFields::AirOutletNodeName).get(),
            idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::EvaporatorAirOutletNodeName).get());

  EXPECT_EQ("Yes", idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeAvailable).get());
  // EXPECT_EQ("Autosize", idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeRatedTotalEvaporatorCoolingCapacity).get());
  EXPECT_EQ(1.7, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeRatedTotalEvaporatorCoolingCapacity).get());
  EXPECT_EQ(0.947, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeRatedSensibleHeatRatio).get());
  EXPECT_EQ(1.9, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeRatedCOP).get());
  EXPECT_EQ(
    coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve.nameString(),
    idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurveName)
      .get());
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
  EXPECT_EQ("Yes", idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeAvailable).get());
  // EXPECT_EQ("Autosize", idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity).get());
  EXPECT_EQ(2.8,
            idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity).get());
  EXPECT_EQ(2.9, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeCapacitySizingFactor).get());
  // EXPECT_EQ("Autosize", idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeRatedStorageChargingCapacity).get());
  EXPECT_EQ(3.0, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeRatedStorageChargingCapacity).get());
  EXPECT_EQ(3.1, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageCapacitySizingFactor).get());
  EXPECT_EQ(0.97, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeRatedSensibleHeatRatio).get());
  EXPECT_EQ(3.3, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeCoolingRatedCOP).get());
  EXPECT_EQ(3.4, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeChargingRatedCOP).get());
  EXPECT_EQ(
    coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve.nameString(),
    idfObject
      .getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurveName)
      .get());
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
  EXPECT_EQ("Yes", idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeAvailable).get());
  // EXPECT_EQ("Autosize", idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity).get());
  EXPECT_EQ(4.8,
            idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity).get());
  EXPECT_EQ(4.9, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeEvaporatorCapacitySizingFactor).get());
  // EXPECT_EQ("Autosize", idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeRatedStorageDischargingCapacity).get());
  EXPECT_EQ(5.0, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeRatedStorageDischargingCapacity).get());
  EXPECT_EQ(5.1,
            idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageDischargeCapacitySizingFactor).get());
  EXPECT_EQ(0.981, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeRatedSensibleHeatRatio).get());
  EXPECT_EQ(5.3, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeCoolingRatedCOP).get());
  EXPECT_EQ(5.4, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeDischargingRatedCOP).get());
  EXPECT_EQ(coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve.nameString(),
            idfObject
              .getString(
                Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurveName)
              .get());
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
  EXPECT_EQ("Yes", idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeAvailable).get());
  // EXPECT_EQ("Autosize", idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeRatedStorageChargingCapacity).get());
  EXPECT_EQ(6.9, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeRatedStorageChargingCapacity).get());
  EXPECT_EQ(7.0, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeCapacitySizingFactor).get());
  EXPECT_EQ(7.1, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeChargingRatedCOP).get());
  EXPECT_EQ(
    chargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve.nameString(),
    idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurveName).get());
  EXPECT_EQ(
    chargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve.nameString(),
    idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurveName).get());
  EXPECT_EQ("Yes", idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeAvailable).get());
  // EXPECT_EQ("Autosize", idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeRatedStorageDischargingCapacity).get());
  EXPECT_EQ(7.5, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeRatedStorageDischargingCapacity).get());
  EXPECT_EQ(7.6, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeCapacitySizingFactor).get());
  EXPECT_EQ(0.987, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeRatedSensibleHeatRatio).get());
  EXPECT_EQ(7.8, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeRatedCOP).get());
  EXPECT_EQ(
    dischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve.nameString(),
    idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurveName)
      .get());
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
  EXPECT_EQ("My CoilCoolingDXSingleSpeedThermalStorage Condenser Air Inlet Node",
            idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondenserAirInletNodeName).get());
  EXPECT_EQ("My CoilCoolingDXSingleSpeedThermalStorage Condenser Air Outlet Node",
            idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondenserAirOutletNodeName).get());
  // EXPECT_EQ("Autosize", idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondenserDesignAirFlowRate).get());
  EXPECT_EQ(9.1, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondenserDesignAirFlowRate).get());
  EXPECT_EQ(9.2, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondenserAirFlowSizingFactor).get());
  EXPECT_EQ("AirCooled", idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondenserType).get());
  EXPECT_EQ(0.989, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::EvaporativeCondenserEffectiveness).get());
  // EXPECT_EQ("Autosize", idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::EvaporativeCondenserPumpRatedPowerConsumption).get());
  EXPECT_EQ(9.5, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::EvaporativeCondenserPumpRatedPowerConsumption).get());
  EXPECT_EQ(9.6, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::BasinHeaterCapacity).get());
  EXPECT_EQ(11.7, idfObject.getDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::BasinHeaterSetpointTemperature).get());
  EXPECT_EQ(basinHeaterAvailabilitySchedule.nameString(),
            idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::BasinHeaterAvailabilityScheduleName).get());
  EXPECT_TRUE(idfObject.isEmpty(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::SupplyWaterStorageTankName));
  EXPECT_TRUE(idfObject.isEmpty(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondensateCollectionWaterStorageTankName));
  EXPECT_TRUE(idfObject.isEmpty(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankPlantConnectionInletNodeName));
  EXPECT_TRUE(idfObject.isEmpty(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankPlantConnectionOutletNodeName));
  EXPECT_TRUE(idfObject.isEmpty(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankPlantConnectionDesignFlowRate));
  EXPECT_TRUE(idfObject.isEmpty(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankPlantConnectionHeatTransferEffectiveness));
  EXPECT_TRUE(idfObject.isEmpty(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankMinimumOperatingLimitFluidTemperature));
  EXPECT_TRUE(idfObject.isEmpty(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankMaximumOperatingLimitFluidTemperature));
}

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilCoolingDXSingleSpeedThermalStorage_AirLoopHVAC) {

  ForwardTranslator ft;

  Model m;

  CoilCoolingDXSingleSpeedThermalStorage coilCoolingDXSingleSpeedThermalStorage(m);
  coilCoolingDXSingleSpeedThermalStorage.setName("My CoilCoolingDXSingleSpeedThermalStorage");

  AirLoopHVAC airLoop(m);
  Node supplyOutletNode = airLoop.supplyOutletNode();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.addToNode(supplyOutletNode));

  const Workspace w = ft.translateModel(m);

  const auto idfObjs = w.getObjectsByType(IddObjectType::Coil_Cooling_DX_SingleSpeed_ThermalStorage);
  ASSERT_EQ(1u, idfObjs.size());
  const auto& idfObject = idfObjs.front();

  const auto idfCoilSyss = w.getObjectsByType(IddObjectType::CoilSystem_Cooling_DX);
  ASSERT_EQ(1u, idfCoilSyss.size());

  const auto idfAirLoops = w.getObjectsByType(IddObjectType::AirLoopHVAC);
  ASSERT_EQ(1u, idfAirLoops.size());
  const auto& idfAirLoop = idfAirLoops.front();

  EXPECT_EQ(idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::EvaporatorAirInletNodeName).get(),
            coilCoolingDXSingleSpeedThermalStorage.inletModelObject().get().nameString());
  EXPECT_EQ(idfObject.getString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::EvaporatorAirOutletNodeName).get(),
            coilCoolingDXSingleSpeedThermalStorage.outletModelObject().get().nameString());

  WorkspaceObject idf_brlist = idfAirLoop.getTarget(AirLoopHVACFields::BranchListName).get();

  // Should have one branch only
  ASSERT_EQ(1u, idf_brlist.extensibleGroups().size());
  auto w_eg = idf_brlist.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
  WorkspaceObject idf_branch = w_eg.getTarget(BranchListExtensibleFields::BranchName).get();

  // There should be only one equipment on the branch
  ASSERT_EQ(1u, idf_branch.extensibleGroups().size());
  auto w_eg2 = idf_branch.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();

  EXPECT_EQ(w_eg2.getString(BranchExtensibleFields::ComponentInletNodeName).get(),
            coilCoolingDXSingleSpeedThermalStorage.inletModelObject().get().nameString());
  EXPECT_EQ(w_eg2.getString(BranchExtensibleFields::ComponentOutletNodeName).get(),
            coilCoolingDXSingleSpeedThermalStorage.outletModelObject().get().nameString());

  EXPECT_EQ("CoilSystem:Cooling:DX", w_eg2.getString(BranchExtensibleFields::ComponentObjectType).get());
  auto idf_coilSystem = w_eg2.getTarget(BranchExtensibleFields::ComponentName).get();
  // CoilSystem:Cooling:DX wrapper needed, it's not inside a unitary
  EXPECT_EQ(1, w.getObjectsByType(IddObjectType::CoilSystem_Cooling_DX).size());

  EXPECT_EQ("Always On Discrete", idf_coilSystem.getString(CoilSystem_Cooling_DXFields::AvailabilityScheduleName).get());
  EXPECT_EQ(coilCoolingDXSingleSpeedThermalStorage.inletModelObject().get().nameString(),
            idf_coilSystem.getString(CoilSystem_Cooling_DXFields::DXCoolingCoilSystemInletNodeName).get());
  EXPECT_EQ(coilCoolingDXSingleSpeedThermalStorage.outletModelObject().get().nameString(),
            idf_coilSystem.getString(CoilSystem_Cooling_DXFields::DXCoolingCoilSystemOutletNodeName).get());
  EXPECT_EQ(coilCoolingDXSingleSpeedThermalStorage.outletModelObject().get().nameString(),
            idf_coilSystem.getString(CoilSystem_Cooling_DXFields::DXCoolingCoilSystemSensorNodeName).get());
  EXPECT_EQ("Coil:Cooling:DX:SingleSpeed:ThermalStorage", idf_coilSystem.getString(CoilSystem_Cooling_DXFields::CoolingCoilObjectType).get());
  EXPECT_EQ(coilCoolingDXSingleSpeedThermalStorage.nameString(), idf_coilSystem.getString(CoilSystem_Cooling_DXFields::CoolingCoilName).get());
  EXPECT_EQ(idfObject, idf_coilSystem.getTarget(CoilSystem_Cooling_DXFields::CoolingCoilName).get());
}
