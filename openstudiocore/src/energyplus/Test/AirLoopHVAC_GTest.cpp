/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/AirLoopHVAC.hpp"

#include "../../model/Model.hpp"
#include "../../model/AvailabilityManagerAssignmentList.hpp"
#include "../../model/AvailabilityManagerAssignmentList_Impl.hpp"
#include "../../model/AvailabilityManagerScheduledOn.hpp"
#include "../../model/Node.hpp"
#include "../../model/ScheduleCompact.hpp"
#include "../../model/FanVariableVolume.hpp"
#include "../../model/FanConstantVolume.hpp"
#include "../../model/AirTerminalSingleDuctSeriesPIUReheat.hpp"
#include "../../model/CoilHeatingElectric.hpp"

#include "../../model/Space.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../utilities/geometry/Point3d.hpp"

#include "../../model/AirLoopHVAC_Impl.hpp"
#include "../../model/ThermalZone_Impl.hpp"

#include <utilities/idd/AirLoopHVAC_FieldEnums.hxx>
#include <utilities/idd/AvailabilityManagerAssignmentList_FieldEnums.hxx>
#include <utilities/idd/Fan_ConstantVolume_FieldEnums.hxx>
#include <utilities/idd/Fan_VariableVolume_FieldEnums.hxx>
#include <utilities/idd/AirTerminal_SingleDuct_SeriesPIU_Reheat_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include "../../utilities/core/Logger.hpp"

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

Model createModelWithDummyAirLoop(){

  // Generate the example Model
  Model m = openstudio::model::exampleModel();

  // Remove the existing loop
  AirLoopHVAC example_loop = m.getModelObjects<AirLoopHVAC>()[0];
  example_loop.remove();

  AirLoopHVAC a(m);

  FanVariableVolume fan(m);
  Node outletNode = a.supplyOutletNode();
  fan.addToNode(outletNode);
  fan.setName("AirLoopHVAC Supply Fan");

  ScheduleCompact hvac_op_sch(m);
  hvac_op_sch.setName("HVAC Operation Schedule");
  a.setAvailabilitySchedule(hvac_op_sch);

  FanConstantVolume piu_fan(m);
  piu_fan.setName("ATU PIU Fan");

  CoilHeatingElectric reheatC(m);
  reheatC.setName("ATU PIU Reheat Coil");

  AirTerminalSingleDuctSeriesPIUReheat piu(m, piu_fan, reheatC);

  // Get the single thermal Zone in the model
  ThermalZone z = m.getModelObjects<ThermalZone>()[0];

  a.addBranchForZone(z, piu);

  return m;

}

bool checkAvailabilityManagerList(Workspace w, std::vector<std::string> avm_types) {

  // Get AVM list (only one should be present)
  WorkspaceObjectVector idfObjs(w.getObjectsByType(IddObjectType::AvailabilityManagerAssignmentList));
  if (idfObjs.size() != 1u) {
    std::cout << "Expected one AVMList, got " << idfObjs.size() << std::endl;
    return false;
  }
  WorkspaceObject idf_avm(idfObjs[0]);

  // The avm_types should have the same size as the extensible groups to begin with
  if (avm_types.size() != idf_avm.extensibleGroups().size()) {
    return false;
  }

  for (std::size_t i = 0; i != avm_types.size(); ++i) {
    IdfExtensibleGroup eg = idf_avm.extensibleGroups()[i];
    std::string s = eg.getString(AvailabilityManagerAssignmentListExtensibleFields::AvailabilityManagerObjectType).get();
    if (avm_types[i] != s) {
      std::cout << "AVM type not matching: expected '" << avm_types[i] << "' and got '" << s << std::endl;
      return false;
    }
  }

  return true;

}

bool CheckFanSchedules(Workspace w) {

  boost::optional<WorkspaceObject> _wo;

  // Loop availabilitySchedule ends up on the Fan
  _wo = w.getObjectByTypeAndName(IddObjectType::Fan_VariableVolume, "AirLoopHVAC Supply Fan");
  if (!_wo.is_initialized()) {
    std::cout << "Cannot locate a Fan:VariableVolume named 'AirLoopHVAC Supply Fan'" << std::endl;
    return false;
  }

  WorkspaceObject idf_a_fan = _wo.get();
  std::string idf_a_fan_avail = idf_a_fan.getString(Fan_VariableVolumeFields::AvailabilityScheduleName).get();
  if ("HVAC Operation Schedule" != idf_a_fan_avail) {
    std::cout << "AirLoop Fan Availability Schedule Name was expected to be 'HVAC Operation Schedule', instead it is " << idf_a_fan_avail << std::endl;
    return false;
  }

  // And on the PIU Fan too
  _wo = w.getObjectByTypeAndName(IddObjectType::Fan_ConstantVolume, "ATU PIU Fan");
  std::string idf__fan_name = idf_a_fan.getString(Fan_ConstantVolumeFields::AvailabilityScheduleName).get();

  if (!_wo.is_initialized()) {
    std::cout << "Cannot locate a Fan:ConstantVolume named 'ATU PIU Fan'" << std::endl;
    return false;
  }

  WorkspaceObject idf_piu_fan = _wo.get();
  std::string idf_piu_fan_avail = idf_piu_fan.getString(Fan_ConstantVolumeFields::AvailabilityScheduleName).get();
  if ("HVAC Operation Schedule" != idf_piu_fan_avail) {
    std::cout << "PIU Fan Availability Schedule Name was expected to be 'HVAC Operation Schedule', instead it is " << idf_piu_fan_avail << std::endl;
    return false;
  }

  return true;

}

TEST_F(EnergyPlusFixture, ForwardTranslator_AirLoopHVAC_AvailabilityManagers_None) {



  Model m = createModelWithDummyAirLoop();

  // Add new AVMs

  // Not for this case

  // ForwardTranslate
  ForwardTranslator forwardTranslator;
  Workspace w = forwardTranslator.translateModel(m);

  // There should only be an AVM:Scheduled (auto created)
  std::vector<std::string> avm_types;
  avm_types.push_back("AvailabilityManager:Scheduled");

  ASSERT_TRUE(checkAvailabilityManagerList(w, avm_types));

  // Check that the loop availability schedule ended up in the Fan Schedules
  ASSERT_TRUE(CheckFanSchedules(w));

  m.save(toPath("./AirLoopHVAC_AVM_None.osm"), true);
  w.save(toPath("./AirLoopHVAC_AVM_None.idf"), true);

}

TEST_F(EnergyPlusFixture, ForwardTranslator_AirLoopHVAC_AvailabilityManagers_NightCycle) {

  Model m = createModelWithDummyAirLoop();

  // Add new AVMs
  AirLoopHVAC a = m.getModelObjects<AirLoopHVAC>()[0];
  // This triggers creation of an AVM:NightCycle
  a.setNightCycleControlType("CycleOnAny");

  // ForwardTranslate
  ForwardTranslator forwardTranslator;
  Workspace w = forwardTranslator.translateModel(m);

  // There should only be an AVM:Scheduled (auto created)
  std::vector<std::string> avm_types;
  avm_types.push_back("AvailabilityManager:NightCycle");

  ASSERT_TRUE(checkAvailabilityManagerList(w, avm_types));

  // Check that the loop availability schedule ended up in the Fan Schedules
  ASSERT_TRUE(CheckFanSchedules(w));

  m.save(toPath("./AirLoopHVAC_AVM_NightCycle.osm"), true);
  w.save(toPath("./AirLoopHVAC_AVM_NightCycle.idf"), true);

}

TEST_F(EnergyPlusFixture, ForwardTranslator_AirLoopHVAC_AvailabilityManagers_ScheduledOn) {

  Model m = createModelWithDummyAirLoop();

  // Add new AVMs
  AirLoopHVAC a = m.getModelObjects<AirLoopHVAC>()[0];

  AvailabilityManagerScheduledOn avm_schOn(m);
  a.addAvailabilityManager(avm_schOn);

  // ForwardTranslate
  ForwardTranslator forwardTranslator;
  Workspace w = forwardTranslator.translateModel(m);

  // There should only be an AVM:Scheduled (auto created)
  std::vector<std::string> avm_types;
  avm_types.push_back("AvailabilityManager:ScheduledOn");

  ASSERT_TRUE(checkAvailabilityManagerList(w, avm_types));

  // Check that the loop availability schedule ended up in the Fan Schedules
  ASSERT_TRUE(CheckFanSchedules(w));

  m.save(toPath("./AirLoopHVAC_AVM_ScheduledOn.osm"), true);
  w.save(toPath("./AirLoopHVAC_AVM_ScheduledOn.idf"), true);

}


TEST_F(EnergyPlusFixture, ForwardTranslator_AirLoopHVAC_AvailabilityManagers_NightCycle_ScheduledOn) {

  Model m = createModelWithDummyAirLoop();

  // Add new AVMs
  AirLoopHVAC a = m.getModelObjects<AirLoopHVAC>()[0];
  // This triggers creation of an AVM:NightCycle
  a.setNightCycleControlType("CycleOnAny");

  // Add an AVM:ScheduledOn
  AvailabilityManagerScheduledOn avm_schOn(m);
  a.addAvailabilityManager(avm_schOn);


  // Not for this case

  // ForwardTranslate
  ForwardTranslator forwardTranslator;
  Workspace w = forwardTranslator.translateModel(m);

  // There should only be an AVM:Scheduled (auto created)
  std::vector<std::string> avm_types;
  avm_types.push_back("AvailabilityManager:NightCycle");
  avm_types.push_back("AvailabilityManager:ScheduledOn");


  ASSERT_TRUE(checkAvailabilityManagerList(w, avm_types));

  // Check that the loop availability schedule ended up in the Fan Schedules
  ASSERT_TRUE(CheckFanSchedules(w));

  m.save(toPath("./AirLoopHVAC_AVM_NightCycle_ScheduledOn.osm"), true);
  w.save(toPath("./AirLoopHVAC_AVM_NightCycle_ScheduledOn.idf"), true);

}


