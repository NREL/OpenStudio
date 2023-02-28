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
#include "../ForwardTranslator/ForwardTranslatePlantEquipmentOperationSchemes.hpp"

#include "../../model/Model.hpp"
#include "../../model/PlantLoop.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"

#include "../../model/ScheduleConstant.hpp"
#include "../../model/SetpointManagerScheduled.hpp"

#include "../../model/BoilerHotWater.hpp"
#include "../../model/CentralHeatPumpSystem.hpp"
#include "../../model/ChillerAbsorption.hpp"
#include "../../model/ChillerAbsorptionIndirect.hpp"
#include "../../model/ChillerElectricEIR.hpp"
#include "../../model/ChillerElectricReformulatedEIR.hpp"
#include "../../model/CoolingTowerSingleSpeed.hpp"
#include "../../model/CoolingTowerTwoSpeed.hpp"
#include "../../model/CoolingTowerVariableSpeed.hpp"
#include "../../model/DistrictCooling.hpp"
#include "../../model/DistrictHeating.hpp"
#include "../../model/EvaporativeFluidCoolerSingleSpeed.hpp"
#include "../../model/EvaporativeFluidCoolerTwoSpeed.hpp"
#include "../../model/FluidCoolerSingleSpeed.hpp"
#include "../../model/FluidCoolerTwoSpeed.hpp"
#include "../../model/GeneratorFuelCellExhaustGasToWaterHeatExchanger.hpp"
#include "../../model/GeneratorMicroTurbine.hpp"
#include "../../model/GeneratorMicroTurbineHeatRecovery.hpp"
#include "../../model/GroundHeatExchangerHorizontalTrench.hpp"
#include "../../model/GroundHeatExchangerVertical.hpp"
#include "../../model/HeatExchangerFluidToFluid.hpp"
#include "../../model/HeatPumpAirToWaterFuelFiredHeating.hpp"
#include "../../model/HeatPumpAirToWaterFuelFiredCooling.hpp"
#include "../../model/HeatPumpWaterToWaterEquationFitCooling.hpp"
#include "../../model/HeatPumpWaterToWaterEquationFitHeating.hpp"
#include "../../model/HeatPumpPlantLoopEIRCooling.hpp"
#include "../../model/HeatPumpPlantLoopEIRHeating.hpp"
#include "../../model/PlantComponentTemperatureSource.hpp"
#include "../../model/PlantComponentUserDefined.hpp"
#include "../../model/SolarCollectorFlatPlatePhotovoltaicThermal.hpp"
#include "../../model/SolarCollectorFlatPlateWater.hpp"
#include "../../model/SolarCollectorIntegralCollectorStorage.hpp"
#include "../../model/ThermalStorageChilledWaterStratified.hpp"
#include "../../model/ThermalStorageIceDetailed.hpp"
#include "../../model/WaterHeaterHeatPumpWrappedCondenser.hpp"
#include "../../model/WaterHeaterMixed.hpp"
#include "../../model/WaterHeaterStratified.hpp"

#include <utilities/idd/PlantLoop_FieldEnums.hxx>
#include <utilities/idd/PlantEquipmentOperationSchemes_FieldEnums.hxx>

#include <utilities/idd/PlantEquipmentOperation_HeatingLoad_FieldEnums.hxx>
#include <utilities/idd/PlantEquipmentOperation_CoolingLoad_FieldEnums.hxx>
#include <utilities/idd/PlantEquipmentOperation_Uncontrolled_FieldEnums.hxx>
#include <utilities/idd/PlantEquipmentOperation_ComponentSetpoint_FieldEnums.hxx>

#include <utilities/idd/PlantEquipmentList_FieldEnums.hxx>

#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/IdfObject_Impl.hpp"

#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/WorkspaceObject_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

/*
 * Tests the "smart" logic for guessing the operation type of a Heat Exchanger
 */
TEST_F(EnergyPlusFixture, ForwardTranslator_PlantEquipmentOperationSchemes_HeatExchanger_UncontrolledOn_Both) {

  boost::optional<WorkspaceObject> _wo;
  ForwardTranslator forwardTranslator;

  Model m;

  PlantLoop use_loop(m);
  use_loop.setName("Use Loop");

  BoilerHotWater b1(m);
  HeatExchangerFluidToFluid hx(m);

  use_loop.addSupplyBranchForComponent(b1);
  use_loop.addSupplyBranchForComponent(hx);

  // Create  a Source Loop with both cooling and heating component on the Supply Side (should be categorized as ComponentType::BOTH
  // and the HX on the demand side
  PlantLoop source_loop(m);
  source_loop.setName("Source Loop");
  BoilerHotWater b2(m);
  ChillerElectricEIR ch(m);
  source_loop.addSupplyBranchForComponent(b2);
  source_loop.addSupplyBranchForComponent(ch);
  source_loop.addDemandBranchForComponent(hx);

  ASSERT_TRUE(hx.setControlType("UncontrolledOn"));

  Workspace w = forwardTranslator.translateModel(m);

  // Get the Use Loop, and find its plant operation scheme
  _wo = w.getObjectByTypeAndName(IddObjectType::PlantLoop, use_loop.name().get());
  ASSERT_TRUE(_wo.is_initialized());
  WorkspaceObject idf_use_loop = _wo.get();
  WorkspaceObject idf_plant_op = idf_use_loop.getTarget(PlantLoopFields::PlantEquipmentOperationSchemeName).get();

  // Should have created a Heating Load one and an Uncontrolled one
  ASSERT_EQ(2u, idf_plant_op.extensibleGroups().size());
  auto w_eg = idf_plant_op.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
  ASSERT_EQ("PlantEquipmentOperation:HeatingLoad", w_eg.getString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeObjectType).get());

  w_eg = idf_plant_op.extensibleGroups()[1].cast<WorkspaceExtensibleGroup>();
  ASSERT_EQ("PlantEquipmentOperation:Uncontrolled", w_eg.getString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeObjectType).get());

  // Get the Operation Scheme
  _wo = w_eg.getTarget(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeName);
  ASSERT_TRUE(_wo.is_initialized());
  WorkspaceObject idf_op_scheme = _wo.get();

  // Get the Plant Equipment List of this Uncontrolled scheme
  _wo = idf_op_scheme.getTarget(PlantEquipmentOperation_UncontrolledFields::EquipmentListName);
  ASSERT_TRUE(_wo.is_initialized());
  WorkspaceObject idf_peq_list = _wo.get();

  // Should have one equipment on it
  ASSERT_EQ(1u, idf_peq_list.extensibleGroups().size());
  IdfExtensibleGroup idf_eg(idf_peq_list.extensibleGroups()[0]);
  // Which should be the HX
  ASSERT_EQ(hx.name().get(), idf_eg.getString(PlantEquipmentListExtensibleFields::EquipmentName).get());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_PlantEquipmentOperationSchemes_HeatExchanger_UncontrolledOn_Heating) {

  boost::optional<WorkspaceObject> _wo;
  ForwardTranslator forwardTranslator;

  Model m;

  PlantLoop use_loop(m);
  use_loop.setName("Use Loop");

  BoilerHotWater b1(m);
  HeatExchangerFluidToFluid hx(m);

  use_loop.addSupplyBranchForComponent(b1);
  use_loop.addSupplyBranchForComponent(hx);

  // Create  a Source Loop with only heating component on the Supply Side (should be categorized as ComponentType::HEATING
  // and the HX UncontrolledOn on the demand side (should inherit the source loop componentType)
  PlantLoop source_loop(m);
  source_loop.setName("Source Loop");
  BoilerHotWater b2(m);
  source_loop.addSupplyBranchForComponent(b2);
  source_loop.addDemandBranchForComponent(hx);

  ASSERT_TRUE(hx.setControlType("UncontrolledOn"));

  Workspace w = forwardTranslator.translateModel(m);

  // Get the Use Loop, and find its plant operation scheme
  _wo = w.getObjectByTypeAndName(IddObjectType::PlantLoop, use_loop.name().get());
  ASSERT_TRUE(_wo.is_initialized());
  WorkspaceObject idf_use_loop = _wo.get();
  WorkspaceObject idf_plant_op = idf_use_loop.getTarget(PlantLoopFields::PlantEquipmentOperationSchemeName).get();

  // Should have created a Heating Load one only
  ASSERT_EQ(1u, idf_plant_op.extensibleGroups().size());
  auto w_eg = idf_plant_op.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
  ASSERT_EQ("PlantEquipmentOperation:HeatingLoad", w_eg.getString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeObjectType).get());

  // Get the Operation Scheme
  _wo = w_eg.getTarget(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeName);
  ASSERT_TRUE(_wo.is_initialized());
  WorkspaceObject idf_op_scheme = _wo.get();

  // Get the Plant Equipment List of this HeatingLoad scheme
  // There should only be one Load Range
  ASSERT_EQ(1u, idf_op_scheme.extensibleGroups().size());
  // Load range 1
  w_eg = idf_op_scheme.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
  _wo = w_eg.getTarget(PlantEquipmentOperation_HeatingLoadExtensibleFields::RangeEquipmentListName);
  ASSERT_TRUE(_wo.is_initialized());
  WorkspaceObject idf_peq_list = _wo.get();

  // Should have two equipment on it: boiler, then HX
  ASSERT_EQ(2u, idf_peq_list.extensibleGroups().size());
  IdfExtensibleGroup idf_eg(idf_peq_list.extensibleGroups()[0]);
  ASSERT_EQ(b1.name().get(), idf_eg.getString(PlantEquipmentListExtensibleFields::EquipmentName).get());

  idf_eg = idf_peq_list.extensibleGroups()[1];
  ASSERT_EQ(hx.name().get(), idf_eg.getString(PlantEquipmentListExtensibleFields::EquipmentName).get());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_PlantEquipmentOperationSchemes_HeatExchanger_HeatingSetpointModulated) {

  boost::optional<WorkspaceObject> _wo;
  ForwardTranslator forwardTranslator;

  Model m;

  PlantLoop use_loop(m);
  use_loop.setName("Use Loop");

  BoilerHotWater b1(m);
  HeatExchangerFluidToFluid hx(m);

  use_loop.addSupplyBranchForComponent(b1);
  use_loop.addSupplyBranchForComponent(hx);

  // Create  a Source Loop with both cooling and heating component on the Supply Side (should be categorized as ComponentType::BOTH
  // and the HX on the demand side, but since the HX has a HeatingSetpointModulated Mode, it should be categorized as ComponentType:HEATING
  PlantLoop source_loop(m);
  source_loop.setName("Source Loop");
  BoilerHotWater b2(m);
  ChillerElectricEIR ch(m);
  source_loop.addSupplyBranchForComponent(b2);
  source_loop.addSupplyBranchForComponent(ch);
  source_loop.addDemandBranchForComponent(hx);

  ASSERT_TRUE(hx.setControlType("HeatingSetpointModulated"));

  Workspace w = forwardTranslator.translateModel(m);

  // Get the Use Loop, and find its plant operation scheme
  _wo = w.getObjectByTypeAndName(IddObjectType::PlantLoop, use_loop.name().get());
  ASSERT_TRUE(_wo.is_initialized());
  WorkspaceObject idf_use_loop = _wo.get();
  WorkspaceObject idf_plant_op = idf_use_loop.getTarget(PlantLoopFields::PlantEquipmentOperationSchemeName).get();

  // Should have created a Heating Load one only
  ASSERT_EQ(1u, idf_plant_op.extensibleGroups().size());
  auto w_eg = idf_plant_op.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
  ASSERT_EQ("PlantEquipmentOperation:HeatingLoad", w_eg.getString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeObjectType).get());

  // Get the Operation Scheme
  _wo = w_eg.getTarget(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeName);
  ASSERT_TRUE(_wo.is_initialized());
  WorkspaceObject idf_op_scheme = _wo.get();

  // Get the Plant Equipment List of this HeatingLoad scheme
  // There should only be one Load Range
  ASSERT_EQ(1u, idf_op_scheme.extensibleGroups().size());
  // Load range 1
  w_eg = idf_op_scheme.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
  _wo = w_eg.getTarget(PlantEquipmentOperation_HeatingLoadExtensibleFields::RangeEquipmentListName);
  ASSERT_TRUE(_wo.is_initialized());
  WorkspaceObject idf_peq_list = _wo.get();

  // Should have two equipment on it: boiler, then HX
  ASSERT_EQ(2u, idf_peq_list.extensibleGroups().size());
  IdfExtensibleGroup idf_eg(idf_peq_list.extensibleGroups()[0]);
  ASSERT_EQ(b1.name().get(), idf_eg.getString(PlantEquipmentListExtensibleFields::EquipmentName).get());

  idf_eg = idf_peq_list.extensibleGroups()[1];
  ASSERT_EQ(hx.name().get(), idf_eg.getString(PlantEquipmentListExtensibleFields::EquipmentName).get());
}

/*
 * WaterHeater:Mixed and WaterHeater:Strafied are Heating type if they have a capacity (autosized or not)
 */
TEST_F(EnergyPlusFixture, ForwardTranslator_PlantEquipmentOperationSchemes_WaterHeaters_Capacity_NoSource) {

  boost::optional<WorkspaceObject> _wo;

  Model m;

  PlantLoop use_loop(m);
  use_loop.setName("Use Loop");

  BoilerHotWater b1(m);
  WaterHeaterMixed wh(m);
  WaterHeaterStratified wh_s(m);

  use_loop.addSupplyBranchForComponent(b1);
  use_loop.addSupplyBranchForComponent(wh);
  use_loop.addSupplyBranchForComponent(wh_s);

  ASSERT_TRUE(wh.setHeaterMaximumCapacity(50000));
  wh_s.autosizeHeater1Capacity();
  ASSERT_TRUE(wh_s.setHeater2Capacity(0));

  ForwardTranslator forwardTranslator;
  Workspace w = forwardTranslator.translateModel(m);

  // Get the Use Loop, and find its plant operation scheme
  _wo = w.getObjectByTypeAndName(IddObjectType::PlantLoop, use_loop.name().get());
  ASSERT_TRUE(_wo.is_initialized());
  WorkspaceObject idf_use_loop = _wo.get();
  WorkspaceObject idf_plant_op = idf_use_loop.getTarget(PlantLoopFields::PlantEquipmentOperationSchemeName).get();

  // Should have created a Heating Load one only
  ASSERT_EQ(1u, idf_plant_op.extensibleGroups().size());
  auto w_eg = idf_plant_op.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
  ASSERT_EQ("PlantEquipmentOperation:HeatingLoad", w_eg.getString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeObjectType).get());

  // Get the Operation Scheme
  _wo = w_eg.getTarget(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeName);
  ASSERT_TRUE(_wo.is_initialized());
  WorkspaceObject idf_op_scheme = _wo.get();

  // Get the Plant Equipment List of this HeatingLoad scheme
  // There should only be one Load Range
  ASSERT_EQ(1u, idf_op_scheme.extensibleGroups().size());
  // Load range 1
  w_eg = idf_op_scheme.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
  _wo = w_eg.getTarget(PlantEquipmentOperation_HeatingLoadExtensibleFields::RangeEquipmentListName);
  ASSERT_TRUE(_wo.is_initialized());
  WorkspaceObject idf_peq_list = _wo.get();

  // Should have two equipment on it: boiler, then WaterHeater:Mixed, then WaterHeater:Stratified
  ASSERT_EQ(3u, idf_peq_list.extensibleGroups().size());
  IdfExtensibleGroup idf_eg(idf_peq_list.extensibleGroups()[0]);
  ASSERT_EQ(b1.name().get(), idf_eg.getString(PlantEquipmentListExtensibleFields::EquipmentName).get());

  idf_eg = idf_peq_list.extensibleGroups()[1];
  ASSERT_EQ(wh.name().get(), idf_eg.getString(PlantEquipmentListExtensibleFields::EquipmentName).get());

  idf_eg = idf_peq_list.extensibleGroups()[2];
  ASSERT_EQ(wh_s.name().get(), idf_eg.getString(PlantEquipmentListExtensibleFields::EquipmentName).get());
}

/*
* WaterHeater:Mixed and WaterHeater:Strafied used as storage tanks (no capacity and connected to source loop) inherit source loop type
*/
TEST_F(EnergyPlusFixture, ForwardTranslator_PlantEquipmentOperationSchemes_WaterHeaters_Tank_SourceHeating) {

  boost::optional<WorkspaceObject> _wo;

  Model m;

  PlantLoop use_loop(m);
  use_loop.setName("Use Loop");

  BoilerHotWater b1(m);

  WaterHeaterMixed wh(m);
  ASSERT_TRUE(wh.setHeaterMaximumCapacity(0));

  WaterHeaterStratified wh_s(m);
  ASSERT_TRUE(wh_s.setHeater1Capacity(0));
  ASSERT_TRUE(wh_s.setHeater1Capacity(0));

  use_loop.addSupplyBranchForComponent(b1);
  use_loop.addSupplyBranchForComponent(wh);
  use_loop.addSupplyBranchForComponent(wh_s);

  PlantLoop source_loop(m);
  source_loop.setName("Source Loop");
  BoilerHotWater b2(m);

  source_loop.addSupplyBranchForComponent(b2);
  source_loop.addDemandBranchForComponent(wh);

  ForwardTranslator forwardTranslator;
  Workspace w = forwardTranslator.translateModel(m);

  // Get the Use Loop, and find its plant operation scheme
  _wo = w.getObjectByTypeAndName(IddObjectType::PlantLoop, use_loop.name().get());
  ASSERT_TRUE(_wo.is_initialized());
  WorkspaceObject idf_use_loop = _wo.get();
  WorkspaceObject idf_plant_op = idf_use_loop.getTarget(PlantLoopFields::PlantEquipmentOperationSchemeName).get();

  // Should have created a Heating Load one only
  ASSERT_EQ(1u, idf_plant_op.extensibleGroups().size());
  auto w_eg = idf_plant_op.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
  ASSERT_EQ("PlantEquipmentOperation:HeatingLoad", w_eg.getString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeObjectType).get());

  // Get the Operation Scheme
  _wo = w_eg.getTarget(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeName);
  ASSERT_TRUE(_wo.is_initialized());
  WorkspaceObject idf_op_scheme = _wo.get();

  // Get the Plant Equipment List of this HeatingLoad scheme
  // There should only be one Load Range
  ASSERT_EQ(1u, idf_op_scheme.extensibleGroups().size());
  // Load range 1
  w_eg = idf_op_scheme.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
  _wo = w_eg.getTarget(PlantEquipmentOperation_HeatingLoadExtensibleFields::RangeEquipmentListName);
  ASSERT_TRUE(_wo.is_initialized());
  WorkspaceObject idf_peq_list = _wo.get();

  // Should have two equipment on it: boiler, then WaterHeater:Mixed, then WaterHeater:Stratified
  ASSERT_EQ(3u, idf_peq_list.extensibleGroups().size());
  IdfExtensibleGroup idf_eg(idf_peq_list.extensibleGroups()[0]);
  ASSERT_EQ(b1.name().get(), idf_eg.getString(PlantEquipmentListExtensibleFields::EquipmentName).get());

  idf_eg = idf_peq_list.extensibleGroups()[1];
  ASSERT_EQ(wh.name().get(), idf_eg.getString(PlantEquipmentListExtensibleFields::EquipmentName).get());

  idf_eg = idf_peq_list.extensibleGroups()[2];
  ASSERT_EQ(wh_s.name().get(), idf_eg.getString(PlantEquipmentListExtensibleFields::EquipmentName).get());
}

/*
 * #3761: assert than all solar collectors can properly end up on a PlantEquipmentList
 */
TEST_F(EnergyPlusFixture, ForwardTranslator_PlantEquipmentOperationSchemes_SolarCollectors) {

  boost::optional<WorkspaceObject> _wo;

  Model m;

  PlantLoop use_loop(m);
  use_loop.setName("Use Loop");

  SolarCollectorFlatPlateWater collector_flatplate_water(m);
  SolarCollectorFlatPlatePhotovoltaicThermal collector_flatplate_pvt(m);
  SolarCollectorIntegralCollectorStorage collector_integralcollectorstorage(m);

  use_loop.addSupplyBranchForComponent(collector_flatplate_water);
  use_loop.addSupplyBranchForComponent(collector_flatplate_pvt);
  use_loop.addSupplyBranchForComponent(collector_integralcollectorstorage);

  ForwardTranslator forwardTranslator;
  Workspace w = forwardTranslator.translateModel(m);

  // Get the Use Loop, and find its plant operation scheme
  _wo = w.getObjectByTypeAndName(IddObjectType::PlantLoop, use_loop.name().get());
  ASSERT_TRUE(_wo.is_initialized());
  WorkspaceObject idf_use_loop = _wo.get();
  WorkspaceObject idf_plant_op = idf_use_loop.getTarget(PlantLoopFields::PlantEquipmentOperationSchemeName).get();

  // Should have created a Heating Load one only
  ASSERT_EQ(1u, idf_plant_op.extensibleGroups().size());
  auto w_eg = idf_plant_op.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
  ASSERT_EQ("PlantEquipmentOperation:HeatingLoad", w_eg.getString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeObjectType).get());

  // Get the Operation Scheme
  _wo = w_eg.getTarget(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeName);
  ASSERT_TRUE(_wo.is_initialized());
  WorkspaceObject idf_op_scheme = _wo.get();

  // Get the Plant Equipment List of this HeatingLoad scheme
  // There should only be one Load Range
  ASSERT_EQ(1u, idf_op_scheme.extensibleGroups().size());
  // Load range 1
  w_eg = idf_op_scheme.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
  _wo = w_eg.getTarget(PlantEquipmentOperation_HeatingLoadExtensibleFields::RangeEquipmentListName);
  ASSERT_TRUE(_wo.is_initialized());
  WorkspaceObject idf_peq_list = _wo.get();

  // Should have the three solar collectors on it, with the name properly filled out
  ASSERT_EQ(3u, idf_peq_list.extensibleGroups().size());
  IdfExtensibleGroup idf_eg(idf_peq_list.extensibleGroups()[0]);
  EXPECT_EQ("SolarCollector:FlatPlate:Water", idf_eg.getString(PlantEquipmentListExtensibleFields::EquipmentObjectType).get());
  EXPECT_EQ(collector_flatplate_water.name().get(), idf_eg.getString(PlantEquipmentListExtensibleFields::EquipmentName).get());

  idf_eg = idf_peq_list.extensibleGroups()[1];
  EXPECT_EQ("SolarCollector:FlatPlate:PhotovoltaicThermal", idf_eg.getString(PlantEquipmentListExtensibleFields::EquipmentObjectType).get());
  EXPECT_EQ(collector_flatplate_pvt.name().get(), idf_eg.getString(PlantEquipmentListExtensibleFields::EquipmentName).get());

  idf_eg = idf_peq_list.extensibleGroups()[2];
  EXPECT_EQ("SolarCollector:IntegralCollectorStorage", idf_eg.getString(PlantEquipmentListExtensibleFields::EquipmentObjectType).get());
  EXPECT_EQ(collector_integralcollectorstorage.name().get(), idf_eg.getString(PlantEquipmentListExtensibleFields::EquipmentName).get());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_PlantEquipmentOperationSchemes_Both_ComponentSetpoint) {

  // Test for #4217 - Having a SetpointManager on the outlet of a Supply Component
  // such as ThermalStorage:ice:Detailed creates two PlantEquipmentOperationSchemes
  // We should only create the PlantEquipmentOperation:Uncontrolled if we haven't created a PlantEquipmentOperation:ComponentSetpoint already

  Model m;
  PlantLoop p(m);

  ScheduleConstant sch(m);

  SetpointManagerScheduled spm(m, sch);

  ThermalStorageIceDetailed ts(m);

  EXPECT_TRUE(p.addSupplyBranchForComponent(ts));

  ASSERT_TRUE(ts.outletModelObject());
  Node ts_outletNode = ts.outletModelObject()->cast<Node>();

  spm.addToNode(ts_outletNode);

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  // Get the Use Loop, and find its plant operation scheme
  boost::optional<WorkspaceObject> _wo = w.getObjectByTypeAndName(IddObjectType::PlantLoop, p.name().get());
  ASSERT_TRUE(_wo.is_initialized());
  WorkspaceObject idf_loop = _wo.get();
  WorkspaceObject idf_plant_op = idf_loop.getTarget(PlantLoopFields::PlantEquipmentOperationSchemeName).get();

  // Should have created a ComponentSetpoint Load one only
  ASSERT_EQ(1u, idf_plant_op.extensibleGroups().size());
  auto w_eg = idf_plant_op.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
  ASSERT_EQ("PlantEquipmentOperation:ComponentSetpoint",
            w_eg.getString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeObjectType).get());

  // Get the Operation Scheme
  _wo = w_eg.getTarget(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeName);
  ASSERT_TRUE(_wo.is_initialized());
  WorkspaceObject idf_op_scheme = _wo.get();

  // Get the Plant Equipment List of this HeatingLoad scheme
  // There should only be one component on that scheme
  ASSERT_EQ(1u, idf_op_scheme.extensibleGroups().size());
  w_eg = idf_op_scheme.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
  EXPECT_EQ("ThermalStorage:Ice:Detailed", w_eg.getString(PlantEquipmentOperation_ComponentSetpointExtensibleFields::EquipmentObjectType).get());
  EXPECT_EQ(ts.nameString(), w_eg.getString(PlantEquipmentOperation_ComponentSetpointExtensibleFields::EquipmentName).get());
  EXPECT_EQ("Dual", w_eg.getString(PlantEquipmentOperation_ComponentSetpointExtensibleFields::OperationType).get());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_PlantEquipmentOperationSchemes_HeatPumpPlantLoopEIR) {
  Model m;

  PlantLoop plant_loop_cup_clg(m);
  PlantLoop plant_loop_cup_htg(m);
  PlantLoop plant_loop_plhp_clg(m);
  PlantLoop plant_loop_plhp_htg(m);

  HeatPumpPlantLoopEIRCooling plhp_clg(m);
  HeatPumpPlantLoopEIRHeating plhp_htg(m);
  EXPECT_TRUE(plhp_clg.setCompanionHeatingHeatPump(plhp_htg));
  EXPECT_TRUE(plhp_htg.setCompanionCoolingHeatPump(plhp_clg));

  EXPECT_TRUE(plant_loop_cup_clg.addDemandBranchForComponent(plhp_clg));
  EXPECT_TRUE(plant_loop_plhp_clg.addSupplyBranchForComponent(plhp_clg));
  EXPECT_TRUE(plant_loop_cup_htg.addDemandBranchForComponent(plhp_htg));
  EXPECT_TRUE(plant_loop_plhp_htg.addSupplyBranchForComponent(plhp_htg));

  openstudio::energyplus::ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  {
    boost::optional<WorkspaceObject> _wo;
    _wo = w.getObjectByTypeAndName(IddObjectType::PlantLoop, plant_loop_plhp_htg.name().get());
    ASSERT_TRUE(_wo.is_initialized());
    WorkspaceObject idf_use_loop = _wo.get();
    WorkspaceObject idf_plant_op = idf_use_loop.getTarget(PlantLoopFields::PlantEquipmentOperationSchemeName).get();

    // Should have created a Heating Load one only
    ASSERT_EQ(1u, idf_plant_op.extensibleGroups().size());
    auto w_eg = idf_plant_op.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
    ASSERT_EQ("PlantEquipmentOperation:HeatingLoad", w_eg.getString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeObjectType).get());

    // Get the Operation Scheme
    _wo = w_eg.getTarget(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeName);
    ASSERT_TRUE(_wo.is_initialized());
    WorkspaceObject idf_op_scheme = _wo.get();

    // Get the Plant Equipment List of this HeatingLoad scheme
    // There should only be one Load Range
    ASSERT_EQ(1u, idf_op_scheme.extensibleGroups().size());
    // Load range 1
    w_eg = idf_op_scheme.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
    _wo = w_eg.getTarget(PlantEquipmentOperation_HeatingLoadExtensibleFields::RangeEquipmentListName);
    ASSERT_TRUE(_wo.is_initialized());
    WorkspaceObject idf_peq_list = _wo.get();

    // Should have one equipment on it: HeatPumpPlantLoopEIRHeating
    ASSERT_EQ(1u, idf_peq_list.extensibleGroups().size());
    IdfExtensibleGroup idf_eg(idf_peq_list.extensibleGroups()[0]);
    ASSERT_EQ(plhp_htg.name().get(), idf_eg.getString(PlantEquipmentListExtensibleFields::EquipmentName).get());
  }

  {
    boost::optional<WorkspaceObject> _wo;
    _wo = w.getObjectByTypeAndName(IddObjectType::PlantLoop, plant_loop_plhp_clg.name().get());
    ASSERT_TRUE(_wo.is_initialized());
    WorkspaceObject idf_use_loop = _wo.get();
    WorkspaceObject idf_plant_op = idf_use_loop.getTarget(PlantLoopFields::PlantEquipmentOperationSchemeName).get();

    // Should have created a Cooling Load one only
    ASSERT_EQ(1u, idf_plant_op.extensibleGroups().size());
    auto w_eg = idf_plant_op.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
    ASSERT_EQ("PlantEquipmentOperation:CoolingLoad", w_eg.getString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeObjectType).get());

    // Get the Operation Scheme
    _wo = w_eg.getTarget(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeName);
    ASSERT_TRUE(_wo.is_initialized());
    WorkspaceObject idf_op_scheme = _wo.get();

    // Get the Plant Equipment List of this CoolingLoad scheme
    // There should only be one Load Range
    ASSERT_EQ(1u, idf_op_scheme.extensibleGroups().size());
    // Load range 1
    w_eg = idf_op_scheme.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
    _wo = w_eg.getTarget(PlantEquipmentOperation_CoolingLoadExtensibleFields::RangeEquipmentListName);
    ASSERT_TRUE(_wo.is_initialized());
    WorkspaceObject idf_peq_list = _wo.get();

    // Should have one equipment on it: HeatPumpPlantLoopEIRCooling
    ASSERT_EQ(1u, idf_peq_list.extensibleGroups().size());
    IdfExtensibleGroup idf_eg(idf_peq_list.extensibleGroups()[0]);
    ASSERT_EQ(plhp_clg.name().get(), idf_eg.getString(PlantEquipmentListExtensibleFields::EquipmentName).get());
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_PlantEquipmentOperationSchemes_componentType) {

  Model m;

  {
    EvaporativeFluidCoolerTwoSpeed obj(m);
    EXPECT_EQ(ComponentType::COOLING, openstudio::energyplus::componentType(obj));
  }

  {
    GeneratorFuelCellExhaustGasToWaterHeatExchanger obj(m);
    EXPECT_EQ(ComponentType::HEATING, openstudio::energyplus::componentType(obj));
  }

  {
    BoilerHotWater obj(m);
    EXPECT_EQ(ComponentType::HEATING, openstudio::energyplus::componentType(obj));
  }

  {
    DistrictHeating obj(m);
    EXPECT_EQ(ComponentType::HEATING, openstudio::energyplus::componentType(obj));
  }

  {
    ChillerElectricEIR obj(m);
    EXPECT_EQ(ComponentType::COOLING, openstudio::energyplus::componentType(obj));
  }

  {
    ChillerElectricReformulatedEIR obj(m);
    EXPECT_EQ(ComponentType::COOLING, openstudio::energyplus::componentType(obj));
  }

  {
    ChillerAbsorptionIndirect obj(m);
    EXPECT_EQ(ComponentType::COOLING, openstudio::energyplus::componentType(obj));
  }

  {
    ChillerAbsorption obj(m);
    EXPECT_EQ(ComponentType::COOLING, openstudio::energyplus::componentType(obj));
  }

  {
    ThermalStorageChilledWaterStratified obj(m);
    EXPECT_EQ(ComponentType::COOLING, openstudio::energyplus::componentType(obj));
  }

  {
    DistrictCooling obj(m);
    EXPECT_EQ(ComponentType::COOLING, openstudio::energyplus::componentType(obj));
  }

  {
    CoolingTowerSingleSpeed obj(m);
    EXPECT_EQ(ComponentType::COOLING, openstudio::energyplus::componentType(obj));
  }

  {
    CoolingTowerVariableSpeed obj(m);
    EXPECT_EQ(ComponentType::COOLING, openstudio::energyplus::componentType(obj));
  }

  {
    CoolingTowerTwoSpeed obj(m);
    EXPECT_EQ(ComponentType::COOLING, openstudio::energyplus::componentType(obj));
  }

  {
    EvaporativeFluidCoolerSingleSpeed obj(m);
    EXPECT_EQ(ComponentType::COOLING, openstudio::energyplus::componentType(obj));
  }

  {
    EvaporativeFluidCoolerTwoSpeed obj(m);
    EXPECT_EQ(ComponentType::COOLING, openstudio::energyplus::componentType(obj));
  }

  {
    FluidCoolerSingleSpeed obj(m);
    EXPECT_EQ(ComponentType::COOLING, openstudio::energyplus::componentType(obj));
  }

  {
    FluidCoolerTwoSpeed obj(m);
    EXPECT_EQ(ComponentType::COOLING, openstudio::energyplus::componentType(obj));
  }

  {
    GroundHeatExchangerVertical obj(m);
    EXPECT_EQ(ComponentType::BOTH, openstudio::energyplus::componentType(obj));
  }

  {
    GroundHeatExchangerHorizontalTrench obj(m);
    EXPECT_EQ(ComponentType::BOTH, openstudio::energyplus::componentType(obj));
  }

  {
    SolarCollectorFlatPlatePhotovoltaicThermal obj(m);
    EXPECT_EQ(ComponentType::HEATING, openstudio::energyplus::componentType(obj));
  }

  {
    SolarCollectorFlatPlateWater obj(m);
    EXPECT_EQ(ComponentType::HEATING, openstudio::energyplus::componentType(obj));
  }

  {
    SolarCollectorIntegralCollectorStorage obj(m);
    EXPECT_EQ(ComponentType::HEATING, openstudio::energyplus::componentType(obj));
  }

  {
    PlantComponentUserDefined obj(m);
    EXPECT_EQ(ComponentType::BOTH, openstudio::energyplus::componentType(obj));
  }

  {
    HeatPumpWaterToWaterEquationFitHeating obj(m);
    EXPECT_EQ(ComponentType::HEATING, openstudio::energyplus::componentType(obj));
  }

  {
    HeatPumpWaterToWaterEquationFitCooling obj(m);
    EXPECT_EQ(ComponentType::COOLING, openstudio::energyplus::componentType(obj));
  }

  {
    HeatPumpPlantLoopEIRHeating obj(m);
    EXPECT_EQ(ComponentType::HEATING, openstudio::energyplus::componentType(obj));
  }

  {
    HeatPumpPlantLoopEIRCooling obj(m);
    EXPECT_EQ(ComponentType::COOLING, openstudio::energyplus::componentType(obj));
  }

  {
    HeatPumpAirToWaterFuelFiredHeating obj(m);
    EXPECT_EQ(ComponentType::HEATING, openstudio::energyplus::componentType(obj));
  }

  {
    HeatPumpAirToWaterFuelFiredCooling obj(m);
    EXPECT_EQ(ComponentType::COOLING, openstudio::energyplus::componentType(obj));
  }

  {
    GeneratorMicroTurbine chp(m);
    GeneratorMicroTurbineHeatRecovery obj(m, chp);
    EXPECT_EQ(ComponentType::HEATING, openstudio::energyplus::componentType(obj));
  }

  {
    PlantComponentTemperatureSource obj(m);
    EXPECT_EQ(ComponentType::BOTH, openstudio::energyplus::componentType(obj));
  }

  /***********************************************
  *          C O M P L E X    T Y P E S          *
  ***********************************************/
  {
    WaterHeaterMixed obj(m);

    // No Capacity, not on a loop, not in a HPWH => NONE
    obj.setHeaterMaximumCapacity(0.0);
    EXPECT_EQ(ComponentType::NONE, openstudio::energyplus::componentType(obj));

    // If it has a capacity, it's heating
    obj.setHeaterMaximumCapacity(1000);
    EXPECT_EQ(ComponentType::HEATING, openstudio::energyplus::componentType(obj));
  }

  {
    WaterHeaterStratified obj(m);
    // If it has a capacity, it's heating
    obj.setHeater1Capacity(0.0);
    obj.setHeater2Capacity(0.0);
    EXPECT_EQ(ComponentType::NONE, openstudio::energyplus::componentType(obj));

    // Any of the two heaters has a capacity? => HEATING
    obj.setHeater1Capacity(100.0);
    EXPECT_EQ(ComponentType::HEATING, openstudio::energyplus::componentType(obj));
    obj.setHeater1Capacity(0.0);
    EXPECT_EQ(ComponentType::NONE, openstudio::energyplus::componentType(obj));

    obj.setHeater2Capacity(100.0);
    EXPECT_EQ(ComponentType::HEATING, openstudio::energyplus::componentType(obj));
  }

  {
    // This is a special case for CentralHeatPumpSystem, it's handled later
    CentralHeatPumpSystem obj(m);
    EXPECT_EQ(ComponentType::NONE, openstudio::energyplus::componentType(obj));
  }

  {
    ThermalStorageIceDetailed obj(m);
    // TODO
  }

  {
    HeatExchangerFluidToFluid obj(m);

    std::vector<std::pair<std::string, ComponentType>> expectedResults{
      {"UncontrolledOn", ComponentType::NONE},  // Not on a plant loop

      {"HeatingSetpointModulated", ComponentType::HEATING},
      {"HeatingSetpointOnOff", ComponentType::HEATING},

      {"CoolingSetpointModulated", ComponentType::COOLING},
      {"CoolingSetpointOnOff", ComponentType::COOLING},
      {"CoolingDifferentialOnOff", ComponentType::COOLING},
      {"CoolingSetpointOnOffWithComponentOverride", ComponentType::COOLING},

      {"OperationSchemeModulated", ComponentType::BOTH},
      {"OperationSchemeOnOff", ComponentType::BOTH},
      {"DualDeadbandSetpointModulated", ComponentType::BOTH},
      {"DualDeadbandSetpointOnOff", ComponentType::BOTH},
    };

    for (const auto& [controlType, compType] : expectedResults) {
      obj.setControlType(controlType);
      EXPECT_EQ(compType, openstudio::energyplus::componentType(obj));
    }
  }
}
