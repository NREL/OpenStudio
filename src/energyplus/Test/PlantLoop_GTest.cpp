/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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

Model createModelWithDummyPlantLoop() {

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

  // m.save(toPath("./PlantLoop_AVM_None.osm"), true);
  // w.save(toPath("./PlantLoop_AVM_None.idf"), true);
}

TEST_F(EnergyPlusFixture, ForwardTranslator_PlantLoop_AvailabilityManagers_ScheduledOn) {

  Model m = createModelWithDummyPlantLoop();

  // Add new AVMs
  PlantLoop p = m.getConcreteModelObjects<PlantLoop>()[0];

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

  // m.save(toPath("./PlantLoop_AVM_ScheduledOn.osm"), true);
  // w.save(toPath("./PlantLoop_AVM_ScheduledOn.idf"), true);
}

TEST_F(EnergyPlusFixture, ForwardTranslator_PlantLoop_AvailabilityManagers_ScheduledOnOff) {

  Model m = createModelWithDummyPlantLoop();

  // Add new AVMs
  PlantLoop p = m.getConcreteModelObjects<PlantLoop>()[0];

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

  // m.save(toPath("./PlantLoop_AVM_ScheduledOnOff.osm"), true);
  // w.save(toPath("./PlantLoop_AVM_ScheduledOnOff.idf"), true);
}

TEST_F(EnergyPlusFixture, ForwardTranslator_PlantLoop_createFluidProperties) {
  // test for #4482 - It crashes with 5 plantloops

  for (int n = 1; n <= 10; ++n) {
    Model m;

    for (int i = 0; i < n; ++i) {
      PlantLoop p(m);
      EXPECT_TRUE(p.setFluidType("PropyleneGlycol"));
      EXPECT_TRUE(p.setGlycolConcentration(50 + i));
    }
    ForwardTranslator ft;
    // EXPECT_TRUE(false) << "Failed for " << n << " PlantLoops";
    ASSERT_NO_THROW(ft.translateModel(m)) << "Failed for " << n << " PlantLoops";
  }
}
