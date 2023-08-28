/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVACUnitarySystem.hpp"
#include "../../model/AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed.hpp"
#include "../../model/Node.hpp"
#include "../../model/CoilHeatingGasMultiStage.hpp"
#include "../../model/CoilHeatingGasMultiStage_Impl.hpp"
#include "../../model/CoilHeatingGasMultiStageStageData.hpp"
#include "../../model/CoilHeatingGasMultiStageStageData_Impl.hpp"
#include "../../model/CoilCoolingDXSingleSpeed.hpp"
#include "../../model/CoilCoolingDXMultiSpeed.hpp"
#include "../../model/CoilHeatingElectric.hpp"
#include "../../model/FanConstantVolume.hpp"
#include "../../model/CurveQuadratic.hpp"
#include "../../model/CurveQuadratic_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/Coil_Heating_Gas_MultiStage_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeed_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_UnitarySystem_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilHeatingGasMultiStage_HeatingCoil) {
  {  // AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed
    Model m;

    CoilCoolingDXMultiSpeed c(m);
    c.setName("HP CC");
    CoilHeatingGasMultiStage h(m);
    h.setName("HP HC");
    FanConstantVolume f(m);
    f.setName("HP FanConstantVol");
    CoilHeatingElectric s(m);
    s.setName("HP SupHC");

    auto alwaysOn = m.alwaysOnDiscreteSchedule();
    alwaysOn.setName("HP HC AvailSch");
    h.setAvailabilitySchedule(alwaysOn);
    CurveQuadratic curve(m);
    curve.setName("HP HC Curve");
    h.setPartLoadFractionCorrelationCurve(curve);
    h.setParasiticGasLoad(10.0);

    CoilHeatingGasMultiStageStageData stageData(m);
    stageData.setGasBurnerEfficiency(2.5);
    stageData.setNominalCapacity(2000.0);
    stageData.setParasiticElectricLoad(15.0);
    h.addStage(stageData);

    AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed unitary(m, f, h, c, s);
    unitary.setName("UnitarySys");

    AirLoopHVAC airLoop(m);
    Node supplyOutletNode = airLoop.supplyOutletNode();
    unitary.addToNode(supplyOutletNode);

    ForwardTranslator ft;
    Workspace workspace = ft.translateModel(m);

    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeed).size());
    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Coil_Heating_Gas_MultiStage).size());

    IdfObject idf_hp = workspace.getObjectsByType(IddObjectType::AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeed)[0];
    IdfObject idf_coil = workspace.getObjectsByType(IddObjectType::Coil_Heating_Gas_MultiStage)[0];

    EXPECT_EQ("HP FanConstantVol", idf_hp.getString(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::SupplyAirFanName, false).get());
    EXPECT_EQ("Coil:Heating:Gas:MultiStage",
              idf_hp.getString(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::HeatingCoilObjectType, false).get());
    EXPECT_EQ("HP HC", idf_hp.getString(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::HeatingCoilName, false).get());
    EXPECT_EQ("HP CC", idf_hp.getString(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::CoolingCoilName, false).get());
    EXPECT_EQ("HP SupHC", idf_hp.getString(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::SupplementalHeatingCoilName, false).get());

    EXPECT_EQ("HP HC AvailSch", idf_coil.getString(Coil_Heating_Gas_MultiStageFields::AvailabilityScheduleName, false).get());
    EXPECT_EQ("UnitarySys Cool Coil Outlet", idf_coil.getString(Coil_Heating_Gas_MultiStageFields::AirInletNodeName, false).get());
    EXPECT_EQ("UnitarySys Heat Coil Outlet", idf_coil.getString(Coil_Heating_Gas_MultiStageFields::AirOutletNodeName, false).get());
    EXPECT_TRUE(idf_coil.isEmpty(Coil_Heating_Gas_MultiStageFields::TemperatureSetpointNodeName));
    EXPECT_EQ("HP HC Curve", idf_coil.getString(Coil_Heating_Gas_MultiStageFields::PartLoadFractionCorrelationCurveName, false).get());
    EXPECT_EQ(10.0, idf_coil.getDouble(Coil_Heating_Gas_MultiStageFields::OffCycleParasiticGasLoad, false).get());
    EXPECT_EQ(1, idf_coil.getInt(Coil_Heating_Gas_MultiStageFields::NumberofStages, false).get());
    ASSERT_EQ(1, idf_coil.numExtensibleGroups());
    auto egs = idf_coil.extensibleGroups();
    auto stages = h.stages();
    for (size_t i = 0; i < idf_coil.extensibleGroups().size(); ++i) {
      const IdfExtensibleGroup& eg = egs[i];
      const auto stage = stages[i];

      EXPECT_EQ(2.5, eg.getDouble(Coil_Heating_Gas_MultiStageExtensibleFields::StageGasBurnerEfficiency).get());
      EXPECT_EQ(2000.0, eg.getDouble(Coil_Heating_Gas_MultiStageExtensibleFields::StageNominalCapacity).get());
      EXPECT_EQ(15.0, eg.getDouble(Coil_Heating_Gas_MultiStageExtensibleFields::StageOnCycleParasiticElectricLoad).get());
    }
  }

  {  // AirLoopHVACUnitarySystem
    Model m;

    CoilCoolingDXSingleSpeed c(m);
    c.setName("HP CC");
    CoilHeatingGasMultiStage h(m);
    h.setName("HP HC");
    FanConstantVolume f(m);
    f.setName("HP FanConstantVol");

    auto alwaysOn = m.alwaysOnDiscreteSchedule();
    alwaysOn.setName("HP HC AvailSch");
    h.setAvailabilitySchedule(alwaysOn);
    CurveQuadratic curve(m);
    curve.setName("HP HC Curve");
    h.setPartLoadFractionCorrelationCurve(curve);
    h.setParasiticGasLoad(10.0);

    CoilHeatingGasMultiStageStageData stageData(m);
    stageData.setGasBurnerEfficiency(2.5);
    stageData.setNominalCapacity(2000.0);
    stageData.setParasiticElectricLoad(15.0);
    h.addStage(stageData);

    AirLoopHVACUnitarySystem unitary(m);
    unitary.setName("UnitarySys");
    EXPECT_TRUE(unitary.setCoolingCoil(c));
    EXPECT_TRUE(unitary.setHeatingCoil(h));
    EXPECT_TRUE(unitary.setSupplyFan(f));

    AirLoopHVAC airLoop(m);
    Node supplyOutletNode = airLoop.supplyOutletNode();
    unitary.addToNode(supplyOutletNode);

    ForwardTranslator ft;
    Workspace workspace = ft.translateModel(m);

    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::AirLoopHVAC_UnitarySystem).size());
    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Coil_Heating_Gas_MultiStage).size());

    IdfObject idf_unitary = workspace.getObjectsByType(IddObjectType::AirLoopHVAC_UnitarySystem)[0];
    IdfObject idf_coil = workspace.getObjectsByType(IddObjectType::Coil_Heating_Gas_MultiStage)[0];

    EXPECT_EQ("HP FanConstantVol", idf_unitary.getString(AirLoopHVAC_UnitarySystemFields::SupplyFanName, false).get());
    EXPECT_EQ("Coil:Heating:Gas:MultiStage", idf_unitary.getString(AirLoopHVAC_UnitarySystemFields::HeatingCoilObjectType, false).get());
    EXPECT_EQ("HP HC", idf_unitary.getString(AirLoopHVAC_UnitarySystemFields::HeatingCoilName, false).get());
    EXPECT_EQ("HP CC", idf_unitary.getString(AirLoopHVAC_UnitarySystemFields::CoolingCoilName, false).get());
    EXPECT_TRUE(idf_unitary.isEmpty(AirLoopHVAC_UnitarySystemFields::SupplementalHeatingCoilName));

    EXPECT_EQ("HP HC AvailSch", idf_coil.getString(Coil_Heating_Gas_MultiStageFields::AvailabilityScheduleName, false).get());
    EXPECT_EQ("UnitarySys Cooling Coil - Heating Coil Node", idf_coil.getString(Coil_Heating_Gas_MultiStageFields::AirInletNodeName, false).get());
    EXPECT_EQ("UnitarySys Heating Coil - Fan Node", idf_coil.getString(Coil_Heating_Gas_MultiStageFields::AirOutletNodeName, false).get());
    EXPECT_TRUE(idf_coil.isEmpty(Coil_Heating_Gas_MultiStageFields::TemperatureSetpointNodeName));
    EXPECT_EQ("HP HC Curve", idf_coil.getString(Coil_Heating_Gas_MultiStageFields::PartLoadFractionCorrelationCurveName, false).get());
    EXPECT_EQ(10.0, idf_coil.getDouble(Coil_Heating_Gas_MultiStageFields::OffCycleParasiticGasLoad, false).get());
    EXPECT_EQ(1, idf_coil.getInt(Coil_Heating_Gas_MultiStageFields::NumberofStages, false).get());
    ASSERT_EQ(1, idf_coil.numExtensibleGroups());
    auto egs = idf_coil.extensibleGroups();
    auto stages = h.stages();
    for (size_t i = 0; i < idf_coil.extensibleGroups().size(); ++i) {
      const IdfExtensibleGroup& eg = egs[i];
      const auto stage = stages[i];

      EXPECT_EQ(2.5, eg.getDouble(Coil_Heating_Gas_MultiStageExtensibleFields::StageGasBurnerEfficiency).get());
      EXPECT_EQ(2000.0, eg.getDouble(Coil_Heating_Gas_MultiStageExtensibleFields::StageNominalCapacity).get());
      EXPECT_EQ(15.0, eg.getDouble(Coil_Heating_Gas_MultiStageExtensibleFields::StageOnCycleParasiticElectricLoad).get());
    }
  }
}
