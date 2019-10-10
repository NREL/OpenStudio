/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
  AirLoopHVAC example_loop = m.getModelObjects<AirLoopHVAC>()[0];
  example_loop.remove();

  // Add a System type 1 (PTAC) will create a PlantLoop for us
  openstudio::model::addSystemType1(m, m.getModelObjects<ThermalZone>());

  PlantLoop p = m.getModelObjects<PlantLoop>()[0];
  p.setName("Plant Loop");
  PumpVariableSpeed pump = m.getModelObjects<PumpVariableSpeed>()[0];
  pump.setName("Plant Loop Supply Pump");

  ThermalZone tz = m.getModelObjects<ThermalZone>()[0];

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
  for (const auto & actuator : actuators) {
    EXPECT_EQ("best plant component", actuator.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, false).get());
    EXPECT_EQ("Plant Connection 1", actuator.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, false).get());
  }

  ASSERT_TRUE(object.getString(PlantComponent_UserDefinedFields::Name, false));
  EXPECT_EQ("best plant component", object.getString(PlantComponent_UserDefinedFields::Name, false).get());
  ASSERT_TRUE(object.getString(PlantComponent_UserDefinedFields::MainModelProgramCallingManagerName, false));
  //EXPECT_EQ(mainPCM.nameString(), object.getString(PlantComponent_UserDefinedFields::MainModelProgramCallingManagerName, false).get());
  ASSERT_TRUE(object.getString(PlantComponent_UserDefinedFields::PlantConnection1InitializationProgramCallingManagerName, false));
  EXPECT_EQ(initPCM.get().nameString(), object.getString(PlantComponent_UserDefinedFields::PlantConnection1InitializationProgramCallingManagerName, false).get());
  ASSERT_TRUE(object.getString(PlantComponent_UserDefinedFields::PlantConnection1SimulationProgramCallingManagerName, false));
  EXPECT_EQ(simPCM.get().nameString(), object.getString(PlantComponent_UserDefinedFields::PlantConnection1SimulationProgramCallingManagerName, false).get());
  ASSERT_TRUE(object.getString(PlantComponent_UserDefinedFields::PlantConnection1InletNodeName, false));
  EXPECT_EQ(inname, object.getString(PlantComponent_UserDefinedFields::PlantConnection1InletNodeName, false).get());
  ASSERT_TRUE(object.getString(PlantComponent_UserDefinedFields::PlantConnection1OutletNodeName, false));
  EXPECT_EQ(outname, object.getString(PlantComponent_UserDefinedFields::PlantConnection1OutletNodeName, false).get());

  m.save(toPath("./PlantComponentUserDefined.osm"), true);
  w.save(toPath("./PlantComponentUserDefined.idf"), true);

}
