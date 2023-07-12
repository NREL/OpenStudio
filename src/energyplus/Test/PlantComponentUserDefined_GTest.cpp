/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/PlantLoop.hpp"
#include "../../model/PlantLoop_Impl.hpp"
#include "../../model/PlantComponentUserDefined.hpp"
#include "../../model/PlantComponentUserDefined_Impl.hpp"
#include "../../model/EnergyManagementSystemProgramCallingManager.hpp"
#include "../../model/EnergyManagementSystemProgramCallingManager_Impl.hpp"
#include "../../model/EnergyManagementSystemProgram.hpp"
#include "../../model/EnergyManagementSystemProgram_Impl.hpp"
#include "../../model/Model.hpp"
#include "../../model/HVACTemplates.hpp"

#include "../../model/AvailabilityManagerAssignmentList.hpp"
#include "../../model/AvailabilityManagerAssignmentList_Impl.hpp"
#include "../../model/AvailabilityManagerScheduledOn.hpp"
#include "../../model/AvailabilityManagerScheduledOff.hpp"
#include "../../model/Node.hpp"
#include "../../model/ScheduleCompact.hpp"

#include "../../model/PumpVariableSpeed.hpp"
#include "../../model/PumpVariableSpeed_Impl.hpp"

#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"

#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVAC_Impl.hpp"

#include <utilities/idd/PlantLoop_FieldEnums.hxx>
#include <utilities/idd/PlantComponent_UserDefined_FieldEnums.hxx>
#include <utilities/idd/OS_PlantComponent_UserDefined_FieldEnums.hxx>
#include <utilities/idd/Pump_ConstantSpeed_FieldEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_ProgramCallingManager_FieldEnums.hxx>
#include <utilities/idd/EnergyManagementSystem_ProgramCallingManager_FieldEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_Actuator_FieldEnums.hxx>
#include <utilities/idd/EnergyManagementSystem_Actuator_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include "../../utilities/core/Logger.hpp"

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_PlantComponentUserDefined) {

  // Generate the example Model
  Model m = openstudio::model::exampleModel();

  // Remove the existing airLoop
  AirLoopHVAC example_loop = m.getConcreteModelObjects<AirLoopHVAC>()[0];
  example_loop.remove();

  // Add a System type 1 (PTAC) will create a PlantLoop for us
  openstudio::model::addSystemType1(m, m.getConcreteModelObjects<ThermalZone>());

  PlantLoop p = m.getConcreteModelObjects<PlantLoop>()[0];
  p.setName("Plant Loop");
  PumpVariableSpeed pump = m.getConcreteModelObjects<PumpVariableSpeed>()[0];
  pump.setName("Plant Loop Supply Pump");

  ThermalZone tz = m.getConcreteModelObjects<ThermalZone>()[0];

  PlantComponentUserDefined pcud(m);
  pcud.setName("best plant component");
  //EnergyManagementSystemProgramCallingManager mainPCM(m);
  //pcud.setMainModelProgramCallingManager(mainPCM);
  boost::optional<EnergyManagementSystemProgramCallingManager> initPCM = pcud.plantInitializationProgramCallingManager();
  boost::optional<EnergyManagementSystemProgramCallingManager> simPCM = pcud.plantSimulationProgramCallingManager();

  pcud.setAmbientZone(tz);

  EnergyManagementSystemActuator dvfrActuator = pcud.designVolumeFlowRateActuator().get();
  EnergyManagementSystemActuator mlcActuator = pcud.minimumLoadingCapacityActuator().get();
  EnergyManagementSystemActuator mxlcActuator = pcud.maximumLoadingCapacityActuator().get();
  EnergyManagementSystemActuator mmfrActuator = pcud.minimumMassFlowRateActuator().get();
  EnergyManagementSystemActuator mxfrActuator = pcud.maximumMassFlowRateActuator().get();
  EnergyManagementSystemActuator olcActuator = pcud.optimalLoadingCapacityActuator().get();
  EnergyManagementSystemActuator otActuator = pcud.outletTemperatureActuator().get();
  EnergyManagementSystemActuator mfrActuator = pcud.massFlowRateActuator().get();

  //Node supplyOutletNode = p.supplyOutletNode();
  //pcud.addToNode(supplyOutletNode);
  // Add a supply branch for the pcud
  ASSERT_TRUE(p.addSupplyBranchForComponent(pcud));

  std::string outname = pcud.outletModelObject().get().nameString();
  std::string inname = pcud.inletModelObject().get().nameString();
  // ForwardTranslate
  ForwardTranslator forwardTranslator;
  Workspace w = forwardTranslator.translateModel(m);

  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::PlantComponent_UserDefined).size());
  EXPECT_EQ(2u, w.getObjectsByType(IddObjectType::EnergyManagementSystem_ProgramCallingManager).size());
  EXPECT_EQ(8u, w.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator).size());
  WorkspaceObject object = w.getObjectsByType(IddObjectType::PlantComponent_UserDefined)[0];
  WorkspaceObjectVector actuators = w.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator);
  EXPECT_EQ(8u, actuators.size());
  for (const auto& actuator : actuators) {
    EXPECT_EQ("best plant component", actuator.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, false).get());
    EXPECT_EQ("Plant Connection 1", actuator.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, false).get());
  }

  ASSERT_TRUE(object.getString(PlantComponent_UserDefinedFields::Name, false));
  EXPECT_EQ("best plant component", object.getString(PlantComponent_UserDefinedFields::Name, false).get());
  ASSERT_TRUE(object.getString(PlantComponent_UserDefinedFields::MainModelProgramCallingManagerName, false));
  //EXPECT_EQ(mainPCM.nameString(), object.getString(PlantComponent_UserDefinedFields::MainModelProgramCallingManagerName, false).get());
  ASSERT_TRUE(object.getString(PlantComponent_UserDefinedFields::PlantConnection1InitializationProgramCallingManagerName, false));
  EXPECT_EQ(initPCM.get().nameString(),
            object.getString(PlantComponent_UserDefinedFields::PlantConnection1InitializationProgramCallingManagerName, false).get());
  ASSERT_TRUE(object.getString(PlantComponent_UserDefinedFields::PlantConnection1SimulationProgramCallingManagerName, false));
  EXPECT_EQ(simPCM.get().nameString(),
            object.getString(PlantComponent_UserDefinedFields::PlantConnection1SimulationProgramCallingManagerName, false).get());
  ASSERT_TRUE(object.getString(PlantComponent_UserDefinedFields::PlantConnection1InletNodeName, false));
  EXPECT_EQ(inname, object.getString(PlantComponent_UserDefinedFields::PlantConnection1InletNodeName, false).get());
  ASSERT_TRUE(object.getString(PlantComponent_UserDefinedFields::PlantConnection1OutletNodeName, false));
  EXPECT_EQ(outname, object.getString(PlantComponent_UserDefinedFields::PlantConnection1OutletNodeName, false).get());

  // m.save(toPath("./PlantComponentUserDefined.osm"), true);
  // w.save(toPath("./PlantComponentUserDefined.idf"), true);
}
