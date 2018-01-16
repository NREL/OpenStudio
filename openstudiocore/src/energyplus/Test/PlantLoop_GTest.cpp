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

#include "../../model/PlantLoop.hpp"
#include "../../model/PlantLoop_Impl.hpp"

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
#include <utilities/idd/AvailabilityManagerAssignmentList_FieldEnums.hxx>
#include <utilities/idd/Pump_ConstantSpeed_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include "../../utilities/core/Logger.hpp"

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

Model createModelWithDummyPlantLoop(){

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

  return m;

}

TEST_F(EnergyPlusFixture, ForwardTranslator_PlantLoop_AvailabilityManagers_None) {

  Model m = createModelWithDummyPlantLoop();

  // Add new AVMs

  // Not here

  // ForwardTranslate
  ForwardTranslator forwardTranslator;
  Workspace w = forwardTranslator.translateModel(m);

  // Zero AVM List
  WorkspaceObjectVector idfObjs(w.getObjectsByType(IddObjectType::AvailabilityManagerAssignmentList));
  ASSERT_EQ(0u, idfObjs.size());

  m.save(toPath("./PlantLoop_AVM_None.osm"), true);
  w.save(toPath("./PlantLoop_AVM_None.idf"), true);

}

TEST_F(EnergyPlusFixture, ForwardTranslator_PlantLoop_AvailabilityManagers_ScheduledOn) {

  Model m = createModelWithDummyPlantLoop();

  // Add new AVMs
  PlantLoop p = m.getModelObjects<PlantLoop>()[0];

  AvailabilityManagerScheduledOn avm_schOn(m);
  p.addAvailabilityManager(avm_schOn);

  // ForwardTranslate
  ForwardTranslator forwardTranslator;
  Workspace w = forwardTranslator.translateModel(m);

  // One AVM List
  WorkspaceObjectVector idfObjs(w.getObjectsByType(IddObjectType::AvailabilityManagerAssignmentList));
  ASSERT_EQ(1u, idfObjs.size());
  WorkspaceObject idf_avm(idfObjs[0]);

  // One AVM, ScheduledOn
  ASSERT_EQ(1u, idf_avm.extensibleGroups().size());
  IdfExtensibleGroup eg = idf_avm.extensibleGroups()[0];
  ASSERT_EQ("AvailabilityManager:ScheduledOn", eg.getString(AvailabilityManagerAssignmentListExtensibleFields::AvailabilityManagerObjectType).get());

  m.save(toPath("./PlantLoop_AVM_ScheduledOn.osm"), true);
  w.save(toPath("./PlantLoop_AVM_ScheduledOn.idf"), true);

}


TEST_F(EnergyPlusFixture, ForwardTranslator_PlantLoop_AvailabilityManagers_ScheduledOnOff) {

  Model m = createModelWithDummyPlantLoop();

  // Add new AVMs
  PlantLoop p = m.getModelObjects<PlantLoop>()[0];

  // Add an AVM:ScheduledOn
  AvailabilityManagerScheduledOn avm_schOn(m);
  p.addAvailabilityManager(avm_schOn);

  AvailabilityManagerScheduledOff avm_schOff(m);
  p.addAvailabilityManager(avm_schOff);

  // ForwardTranslate
  ForwardTranslator forwardTranslator;
  Workspace w = forwardTranslator.translateModel(m);

  // One AVM List
  WorkspaceObjectVector idfObjs(w.getObjectsByType(IddObjectType::AvailabilityManagerAssignmentList));
  ASSERT_EQ(1u, idfObjs.size());
  WorkspaceObject idf_avm(idfObjs[0]);

  // Two AVMs, ScheduledOn, then ScheduledOff
  ASSERT_EQ(2u, idf_avm.extensibleGroups().size());

  IdfExtensibleGroup eg = idf_avm.extensibleGroups()[0];
  ASSERT_EQ("AvailabilityManager:ScheduledOn", eg.getString(AvailabilityManagerAssignmentListExtensibleFields::AvailabilityManagerObjectType).get());

  eg = idf_avm.extensibleGroups()[1];
  ASSERT_EQ("AvailabilityManager:ScheduledOff", eg.getString(AvailabilityManagerAssignmentListExtensibleFields::AvailabilityManagerObjectType).get());

  m.save(toPath("./PlantLoop_AVM_ScheduledOnOff.osm"), true);
  w.save(toPath("./PlantLoop_AVM_ScheduledOnOff.idf"), true);

}


