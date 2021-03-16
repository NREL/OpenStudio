/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../ErrorFile.hpp"
#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/Model_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ScheduleTypeLimits.hpp"
#include "../../model/ScheduleTypeLimits_Impl.hpp"
#include "../../model/Building.hpp"
#include "../../model/Building_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/FanConstantVolume.hpp"
#include "../../model/FanConstantVolume_Impl.hpp"
#include "../../model/ElectricEquipment.hpp"
#include "../../model/ElectricEquipment_Impl.hpp"
#include "../../model/ElectricEquipmentDefinition.hpp"
#include "../../model/ElectricEquipmentDefinition_Impl.hpp"

#include "../../model/ExternalInterface.hpp"
#include "../../model/ExternalInterface_Impl.hpp"
#include "../../model/ExternalInterfaceActuator.hpp"
#include "../../model/ExternalInterfaceActuator_Impl.hpp"
#include "../../model/ExternalInterfaceFunctionalMockupUnitExportFromVariable.hpp"
#include "../../model/ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl.hpp"
#include "../../model/ExternalInterfaceFunctionalMockupUnitExportToActuator.hpp"
#include "../../model/ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl.hpp"
#include "../../model/ExternalInterfaceFunctionalMockupUnitExportToSchedule.hpp"
#include "../../model/ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl.hpp"
#include "../../model/ExternalInterfaceFunctionalMockupUnitExportToVariable.hpp"
#include "../../model/ExternalInterfaceFunctionalMockupUnitExportToVariable_Impl.hpp"
#include "../../model/ExternalInterfaceFunctionalMockupUnitImport.hpp"
#include "../../model/ExternalInterfaceFunctionalMockupUnitImport_Impl.hpp"
#include "../../model/ExternalInterfaceFunctionalMockupUnitImportFromVariable.hpp"
#include "../../model/ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl.hpp"
#include "../../model/ExternalInterfaceFunctionalMockupUnitImportToActuator.hpp"
#include "../../model/ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl.hpp"
#include "../../model/ExternalInterfaceFunctionalMockupUnitImportToSchedule.hpp"
#include "../../model/ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl.hpp"
#include "../../model/ExternalInterfaceFunctionalMockupUnitImportToVariable.hpp"
#include "../../model/ExternalInterfaceFunctionalMockupUnitImportToVariable_Impl.hpp"
#include "../../model/ExternalInterfaceSchedule.hpp"
#include "../../model/ExternalInterfaceSchedule_Impl.hpp"
#include "../../model/ExternalInterfaceVariable.hpp"
#include "../../model/ExternalInterfaceVariable_Impl.hpp"

#include <utilities/idd/OS_ExternalInterface_FieldEnums.hxx>
#include <utilities/idd/ExternalInterface_FieldEnums.hxx>
#include <utilities/idd/OS_ExternalInterface_Actuator_FieldEnums.hxx>
#include <utilities/idd/ExternalInterface_Actuator_FieldEnums.hxx>
#include <utilities/idd/OS_ExternalInterface_Variable_FieldEnums.hxx>
#include <utilities/idd/ExternalInterface_Variable_FieldEnums.hxx>
#include <utilities/idd/OS_ExternalInterface_Schedule_FieldEnums.hxx>
#include <utilities/idd/ExternalInterface_Schedule_FieldEnums.hxx>
#include <utilities/idd/OS_ExternalInterface_FunctionalMockupUnitImport_To_Variable_FieldEnums.hxx>
#include <utilities/idd/ExternalInterface_FunctionalMockupUnitImport_To_Variable_FieldEnums.hxx>
#include <utilities/idd/OS_ExternalInterface_FunctionalMockupUnitImport_To_Schedule_FieldEnums.hxx>
#include <utilities/idd/ExternalInterface_FunctionalMockupUnitImport_To_Schedule_FieldEnums.hxx>
#include <utilities/idd/OS_ExternalInterface_FunctionalMockupUnitImport_To_Actuator_FieldEnums.hxx>
#include <utilities/idd/ExternalInterface_FunctionalMockupUnitImport_To_Actuator_FieldEnums.hxx>
#include <utilities/idd/OS_ExternalInterface_FunctionalMockupUnitImport_From_Variable_FieldEnums.hxx>
#include <utilities/idd/ExternalInterface_FunctionalMockupUnitImport_From_Variable_FieldEnums.hxx>
#include <utilities/idd/OS_ExternalInterface_FunctionalMockupUnitImport_FieldEnums.hxx>
#include <utilities/idd/ExternalInterface_FunctionalMockupUnitImport_FieldEnums.hxx>
#include <utilities/idd/OS_ExternalInterface_FunctionalMockupUnitExport_To_Variable_FieldEnums.hxx>
#include <utilities/idd/ExternalInterface_FunctionalMockupUnitExport_To_Variable_FieldEnums.hxx>
#include <utilities/idd/OS_ExternalInterface_FunctionalMockupUnitExport_To_Schedule_FieldEnums.hxx>
#include <utilities/idd/ExternalInterface_FunctionalMockupUnitExport_To_Schedule_FieldEnums.hxx>
#include <utilities/idd/OS_ExternalInterface_FunctionalMockupUnitExport_To_Actuator_FieldEnums.hxx>
#include <utilities/idd/ExternalInterface_FunctionalMockupUnitExport_To_Actuator_FieldEnums.hxx>
#include <utilities/idd/OS_ExternalInterface_FunctionalMockupUnitExport_From_Variable_FieldEnums.hxx>
#include <utilities/idd/ExternalInterface_FunctionalMockupUnitExport_From_Variable_FieldEnums.hxx>

#include "../../model/Version.hpp"
#include "../../model/Version_Impl.hpp"

#include "../../utilities/geometry/Point3d.hpp"

#include "../../utilities/core/Optional.hpp"
#include "../../utilities/core/Checksum.hpp"
#include "../../utilities/core/UUID.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/sql/SqlFile.hpp"
#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/IdfObject.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

#include <boost/algorithm/string/predicate.hpp>

#include <resources.hxx>

#include <sstream>

#include <vector>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_ExternalInterface) {
  Model model;

  ExternalInterface externalinterface = model.getUniqueModelObject<ExternalInterface>();
  EXPECT_EQ("PtolemyServer", externalinterface.nameofExternalInterface());
  EXPECT_TRUE(externalinterface.setNameofExternalInterface("FunctionalMockupUnitImport"));

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ExternalInterface).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::ExternalInterface)[0];

  ASSERT_TRUE(object.getString(ExternalInterfaceFields::NameofExternalInterface, false));
  EXPECT_EQ("FunctionalMockupUnitImport", object.getString(ExternalInterfaceFields::NameofExternalInterface, false).get());

  // model.save(toPath("./ExternalInterface.osm"), true);
  // workspace.save(toPath("./ExternalInterface.idf"), true);
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ExternalInterfaceActuator) {
  Model model;

  Building building = model.getUniqueModelObject<Building>();

  // add fan
  Schedule s = model.alwaysOnDiscreteSchedule();
  FanConstantVolume fan(model, s);

  // add actuator
  std::string fanControlType = "Fan Pressure Rise";
  std::string ComponentType = "Fan";
  ExternalInterfaceActuator fanActuator(fan, ComponentType, fanControlType);

  std::string fanName = fan.name().get() + "Press Actuator";
  fanActuator.setName(fanName);
  fanActuator.setOptionalInitialValue(1);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ExternalInterface_Actuator).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::ExternalInterface_Actuator)[0];

  ASSERT_TRUE(object.getString(ExternalInterface_ActuatorFields::ActuatedComponentUniqueName, false));
  EXPECT_EQ(fan.nameString(), object.getString(ExternalInterface_ActuatorFields::ActuatedComponentUniqueName, false).get());
  ASSERT_TRUE(object.getString(ExternalInterface_ActuatorFields::ActuatedComponentType, false));
  EXPECT_EQ(ComponentType, object.getString(ExternalInterface_ActuatorFields::ActuatedComponentType, false).get());
  ASSERT_TRUE(object.getString(ExternalInterface_ActuatorFields::ActuatedComponentControlType, false));
  EXPECT_EQ(fanControlType, object.getString(ExternalInterface_ActuatorFields::ActuatedComponentControlType, false).get());
  ASSERT_TRUE(object.getDouble(ExternalInterface_ActuatorFields::OptionalInitialValue, false));
  EXPECT_EQ(1.0, object.getDouble(ExternalInterface_ActuatorFields::OptionalInitialValue, false).get());

  // model.save(toPath("./ExternalInterfaceActuator.osm"), true);
  // workspace.save(toPath("./ExternalInterfaceActuator.idf"), true);
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ExternalInterfaceVariable) {

  Model model;
  ExternalInterfaceVariable variable(model, "test name", 10);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ExternalInterface_Variable).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::ExternalInterface_Variable)[0];

  ASSERT_TRUE(object.getString(ExternalInterface_VariableFields::Name, false));
  EXPECT_EQ("test name", object.getString(ExternalInterface_VariableFields::Name, false).get());
  ASSERT_TRUE(object.getDouble(ExternalInterface_VariableFields::InitialValue, false));
  EXPECT_EQ(10.0, object.getDouble(ExternalInterface_VariableFields::InitialValue, false).get());

  // model.save(toPath("./ExternalInterfaceVariable.osm"), true);
  // workspace.save(toPath("./ExternalInterfaceVariable.idf"), true);
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ExternalInterfaceSchedule) {

  Model model;
  ExternalInterfaceSchedule schedule(model, 10);
  EXPECT_EQ(10.0, schedule.initialValue());
  EXPECT_EQ("External Interface Schedule 1", schedule.nameString());

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ExternalInterface_Schedule).size());
  //EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ScheduleTypeLimits).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::ExternalInterface_Schedule)[0];
  //WorkspaceObject object2 = workspace.getObjectsByType(IddObjectType::ScheduleTypeLimits)[0];

  ASSERT_TRUE(object.getString(ExternalInterface_ScheduleFields::Name, false));
  EXPECT_EQ("External Interface Schedule 1", object.getString(ExternalInterface_ScheduleFields::Name, false).get());
  ASSERT_TRUE(object.getString(ExternalInterface_ScheduleFields::ScheduleTypeLimitsName, false));
  //EXPECT_EQ(object2.nameString(), object.getString(ExternalInterface_ScheduleFields::ScheduleTypeLimitsName, false).get());
  ASSERT_TRUE(object.getDouble(ExternalInterface_ScheduleFields::InitialValue, false));
  EXPECT_EQ(10.0, object.getDouble(ExternalInterface_ScheduleFields::InitialValue, false).get());

  // model.save(toPath("./ExternalInterfaceSchedule.osm"), true);
  // workspace.save(toPath("./ExternalInterfaceSchedule.idf"), true);
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ExternalInterfaceFunctionalMockupUnitImport) {

  Model model;

  ExternalInterfaceFunctionalMockupUnitImport eifmui(model, "c:\\Program Files\\Test\\blah.fmu");

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ExternalInterface_FunctionalMockupUnitImport).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::ExternalInterface_FunctionalMockupUnitImport)[0];

  ASSERT_TRUE(object.getString(ExternalInterface_FunctionalMockupUnitImportFields::FMUFileName, false));
  EXPECT_EQ("c:\\Program Files\\Test\\blah.fmu", object.getString(ExternalInterface_FunctionalMockupUnitImportFields::FMUFileName, false).get());
  ASSERT_TRUE(object.getDouble(ExternalInterface_FunctionalMockupUnitImportFields::FMUTimeout, false));
  EXPECT_EQ(0, object.getDouble(ExternalInterface_FunctionalMockupUnitImportFields::FMUTimeout, false).get());
  ASSERT_TRUE(object.getDouble(ExternalInterface_FunctionalMockupUnitImportFields::FMULoggingOn, false));
  EXPECT_EQ(0, object.getDouble(ExternalInterface_FunctionalMockupUnitImportFields::FMULoggingOn, false).get());

  // model.save(toPath("./ExternalInterfaceFunctionalMockupUnitImport.osm"), true);
  // workspace.save(toPath("./ExternalInterfaceFunctionalMockupUnitImport.idf"), true);
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ExternalInterfaceFunctionalMockupUnitExportFromVariable) {
  Model model;

  std::string outputVariableIndexKeyName = "outputVariableIndexKeyName";
  std::string outputVariableName = "outputVariableName";
  std::string fMUVariableName = "fMUVariableName";

  ExternalInterfaceFunctionalMockupUnitExportFromVariable variable(model, outputVariableIndexKeyName, outputVariableName, fMUVariableName);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ExternalInterface_FunctionalMockupUnitExport_From_Variable).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::ExternalInterface_FunctionalMockupUnitExport_From_Variable)[0];

  ASSERT_TRUE(object.getString(ExternalInterface_FunctionalMockupUnitExport_From_VariableFields::Output_VariableIndexKeyName, false));
  EXPECT_EQ(outputVariableIndexKeyName,
            object.getString(ExternalInterface_FunctionalMockupUnitExport_From_VariableFields::Output_VariableIndexKeyName, false).get());
  ASSERT_TRUE(object.getString(ExternalInterface_FunctionalMockupUnitExport_From_VariableFields::Output_VariableName, false));
  EXPECT_EQ(outputVariableName, object.getString(ExternalInterface_FunctionalMockupUnitExport_From_VariableFields::Output_VariableName, false).get());
  ASSERT_TRUE(object.getString(ExternalInterface_FunctionalMockupUnitExport_From_VariableFields::FMUVariableName, false));
  EXPECT_EQ(fMUVariableName, object.getString(ExternalInterface_FunctionalMockupUnitExport_From_VariableFields::FMUVariableName, false).get());

  // model.save(toPath("./ExternalInterfaceFunctionalMockupUnitExportFromVariable.osm"), true);
  // workspace.save(toPath("./ExternalInterfaceFunctionalMockupUnitExportFromVariable.idf"), true);
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ExternalInterfaceFunctionalMockupUnitExportToActuator) {
  Model model;

  Building building = model.getUniqueModelObject<Building>();

  // add fan
  Schedule s = model.alwaysOnDiscreteSchedule();
  FanConstantVolume fan(model, s);

  // add actuator
  std::string fanControlType = "Fan Pressure Rise";
  std::string ComponentType = "Fan";
  ExternalInterfaceFunctionalMockupUnitExportToActuator fanActuator(fan, ComponentType, fanControlType, "Fan FMU name", 10);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ExternalInterface_FunctionalMockupUnitExport_To_Actuator).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::ExternalInterface_FunctionalMockupUnitExport_To_Actuator)[0];

  ASSERT_TRUE(object.getString(ExternalInterface_FunctionalMockupUnitExport_To_ActuatorFields::Name, false));
  EXPECT_EQ("External Interface Functional Mockup Unit Export To Actuator 1",
            object.getString(ExternalInterface_FunctionalMockupUnitExport_To_ActuatorFields::Name, false).get());
  ASSERT_TRUE(object.getString(ExternalInterface_FunctionalMockupUnitExport_To_ActuatorFields::ActuatedComponentUniqueName, false));
  EXPECT_EQ(fan.nameString(),
            object.getString(ExternalInterface_FunctionalMockupUnitExport_To_ActuatorFields::ActuatedComponentUniqueName, false).get());
  ASSERT_TRUE(object.getString(ExternalInterface_FunctionalMockupUnitExport_To_ActuatorFields::ActuatedComponentType, false));
  EXPECT_EQ(ComponentType, object.getString(ExternalInterface_FunctionalMockupUnitExport_To_ActuatorFields::ActuatedComponentType, false).get());
  ASSERT_TRUE(object.getString(ExternalInterface_FunctionalMockupUnitExport_To_ActuatorFields::ActuatedComponentControlType, false));
  EXPECT_EQ(fanControlType,
            object.getString(ExternalInterface_FunctionalMockupUnitExport_To_ActuatorFields::ActuatedComponentControlType, false).get());
  ASSERT_TRUE(object.getString(ExternalInterface_FunctionalMockupUnitExport_To_ActuatorFields::FMUVariableName, false));
  EXPECT_EQ("Fan FMU name", object.getString(ExternalInterface_FunctionalMockupUnitExport_To_ActuatorFields::FMUVariableName, false).get());
  ASSERT_TRUE(object.getDouble(ExternalInterface_FunctionalMockupUnitExport_To_ActuatorFields::InitialValue, false));
  EXPECT_EQ(10.0, object.getDouble(ExternalInterface_FunctionalMockupUnitExport_To_ActuatorFields::InitialValue, false).get());

  // model.save(toPath("./ExternalInterfaceFunctionalMockupUnitExportToActuator.osm"), true);
  // workspace.save(toPath("./ExternalInterfaceFunctionalMockupUnitExportToActuator.idf"), true);
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ExternalInterfaceFunctionalMockupUnitExportToVariable) {
  Model model;

  ExternalInterfaceFunctionalMockupUnitExportToVariable variable(model, "FMU name", 10);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ExternalInterface_FunctionalMockupUnitExport_To_Variable).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::ExternalInterface_FunctionalMockupUnitExport_To_Variable)[0];

  ASSERT_TRUE(object.getString(ExternalInterface_FunctionalMockupUnitExport_To_VariableFields::Name, false));
  EXPECT_EQ("External Interface Functional Mockup Unit Export To Variable 1",
            object.getString(ExternalInterface_FunctionalMockupUnitExport_To_VariableFields::Name, false).get());
  ASSERT_TRUE(object.getString(ExternalInterface_FunctionalMockupUnitExport_To_VariableFields::FMUVariableName, false));
  EXPECT_EQ("FMU name", object.getString(ExternalInterface_FunctionalMockupUnitExport_To_VariableFields::FMUVariableName, false).get());
  ASSERT_TRUE(object.getDouble(ExternalInterface_FunctionalMockupUnitExport_To_VariableFields::InitialValue, false));
  EXPECT_EQ(10.0, object.getDouble(ExternalInterface_FunctionalMockupUnitExport_To_VariableFields::InitialValue, false).get());

  // model.save(toPath("./ExternalInterfaceFunctionalMockupUnitExportToVariable.osm"), true);
  // workspace.save(toPath("./ExternalInterfaceFunctionalMockupUnitExportToVariable.idf"), true);
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ExternalInterfaceFunctionalMockupUnitExportToSchedule) {

  Model model;
  ExternalInterfaceFunctionalMockupUnitExportToSchedule schedule(model, "FMU name", 10);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ExternalInterface_FunctionalMockupUnitExport_To_Schedule).size());
  //EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ScheduleTypeLimits).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::ExternalInterface_FunctionalMockupUnitExport_To_Schedule)[0];
  //WorkspaceObject object2 = workspace.getObjectsByType(IddObjectType::ScheduleTypeLimits)[0];

  ASSERT_TRUE(object.getString(ExternalInterface_FunctionalMockupUnitExport_To_ScheduleFields::ScheduleName, false));
  EXPECT_EQ("External Interface Functional Mockup Unit Export To Schedule 1",
            object.getString(ExternalInterface_FunctionalMockupUnitExport_To_ScheduleFields::ScheduleName, false).get());
  ASSERT_TRUE(object.getString(ExternalInterface_FunctionalMockupUnitExport_To_ScheduleFields::ScheduleTypeLimitsNames, false));
  //EXPECT_EQ(object2.nameString(), object.getString(ExternalInterface_FunctionalMockupUnitExport_To_ScheduleFields::ScheduleTypeLimitsNames, false).get());
  ASSERT_TRUE(object.getString(ExternalInterface_FunctionalMockupUnitExport_To_ScheduleFields::FMUVariableName, false));
  EXPECT_EQ("FMU name", object.getString(ExternalInterface_FunctionalMockupUnitExport_To_ScheduleFields::FMUVariableName, false).get());
  ASSERT_TRUE(object.getDouble(ExternalInterface_FunctionalMockupUnitExport_To_ScheduleFields::InitialValue, false));
  EXPECT_EQ(10.0, object.getDouble(ExternalInterface_FunctionalMockupUnitExport_To_ScheduleFields::InitialValue, false).get());

  // model.save(toPath("./ExternalInterfaceFunctionalMockupUnitExportToSchedule.osm"), true);
  // workspace.save(toPath("./ExternalInterfaceFunctionalMockupUnitExportToSchedule.idf"), true);
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ExternalInterfaceFunctionalMockupUnitImportFromVariable) {
  Model model;
  // add FMU
  ExternalInterfaceFunctionalMockupUnitImport eifmui(model, "test name");

  std::string outputVariableIndexKeyName = "outputVariableIndexKeyName";
  std::string outputVariableName = "outputVariableName";
  std::string fMUVariableName = "fMUVariableName";
  std::string fMUInstanceName = "fMUInstanceName";

  ExternalInterfaceFunctionalMockupUnitImportFromVariable variable(model, outputVariableIndexKeyName, outputVariableName, eifmui, fMUInstanceName,
                                                                   fMUVariableName);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ExternalInterface_FunctionalMockupUnitImport_From_Variable).size());
  //EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ScheduleTypeLimits).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::ExternalInterface_FunctionalMockupUnitImport_From_Variable)[0];
  //WorkspaceObject object2 = workspace.getObjectsByType(IddObjectType::ScheduleTypeLimits)[0];

  ASSERT_TRUE(object.getString(ExternalInterface_FunctionalMockupUnitImport_From_VariableFields::Output_VariableIndexKeyName, false));
  EXPECT_EQ(outputVariableIndexKeyName,
            object.getString(ExternalInterface_FunctionalMockupUnitImport_From_VariableFields::Output_VariableIndexKeyName, false).get());
  ASSERT_TRUE(object.getString(ExternalInterface_FunctionalMockupUnitImport_From_VariableFields::Output_VariableName, false));
  EXPECT_EQ(outputVariableName, object.getString(ExternalInterface_FunctionalMockupUnitImport_From_VariableFields::Output_VariableName, false).get());
  ASSERT_TRUE(object.getString(ExternalInterface_FunctionalMockupUnitImport_From_VariableFields::FMUFileName, false));
  EXPECT_EQ(eifmui.fMUFileName(), object.getString(ExternalInterface_FunctionalMockupUnitImport_From_VariableFields::FMUFileName, false).get());
  ASSERT_TRUE(object.getString(ExternalInterface_FunctionalMockupUnitImport_From_VariableFields::FMUInstanceName, false));
  EXPECT_EQ(fMUInstanceName, object.getString(ExternalInterface_FunctionalMockupUnitImport_From_VariableFields::FMUInstanceName, false).get());
  ASSERT_TRUE(object.getString(ExternalInterface_FunctionalMockupUnitImport_From_VariableFields::FMUVariableName, false));
  EXPECT_EQ(fMUVariableName, object.getString(ExternalInterface_FunctionalMockupUnitImport_From_VariableFields::FMUVariableName, false).get());

  // model.save(toPath("./ExternalInterfaceFunctionalMockupUnitImportFromVariable.osm"), true);
  // workspace.save(toPath("./ExternalInterfaceFunctionalMockupUnitImportFromVariable.idf"), true);
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ExternalInterfaceFunctionalMockupUnitImportToVariable) {
  Model model;
  // add FMU
  ExternalInterfaceFunctionalMockupUnitImport eifmui(model, "test name");

  std::string fMUVariableName = "fMUVariableName";
  std::string fMUInstanceName = "fMUInstanceName";

  ExternalInterfaceFunctionalMockupUnitImportToVariable variable(model, eifmui, fMUInstanceName, fMUVariableName, 10);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ExternalInterface_FunctionalMockupUnitImport_To_Variable).size());
  //EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ScheduleTypeLimits).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::ExternalInterface_FunctionalMockupUnitImport_To_Variable)[0];
  //WorkspaceObject object2 = workspace.getObjectsByType(IddObjectType::ScheduleTypeLimits)[0];

  ASSERT_TRUE(object.getString(ExternalInterface_FunctionalMockupUnitImport_To_VariableFields::Name, false));
  EXPECT_EQ("External Interface Functional Mockup Unit Import To Variable 1",
            object.getString(ExternalInterface_FunctionalMockupUnitImport_To_VariableFields::Name, false).get());
  ASSERT_TRUE(object.getString(ExternalInterface_FunctionalMockupUnitImport_To_VariableFields::FMUFileName, false));
  EXPECT_EQ(eifmui.fMUFileName(), object.getString(ExternalInterface_FunctionalMockupUnitImport_To_VariableFields::FMUFileName, false).get());
  ASSERT_TRUE(object.getString(ExternalInterface_FunctionalMockupUnitImport_To_VariableFields::FMUInstanceName, false));
  EXPECT_EQ(fMUInstanceName, object.getString(ExternalInterface_FunctionalMockupUnitImport_To_VariableFields::FMUInstanceName, false).get());
  ASSERT_TRUE(object.getString(ExternalInterface_FunctionalMockupUnitImport_To_VariableFields::FMUVariableName, false));
  EXPECT_EQ(fMUVariableName, object.getString(ExternalInterface_FunctionalMockupUnitImport_To_VariableFields::FMUVariableName, false).get());
  ASSERT_TRUE(object.getDouble(ExternalInterface_FunctionalMockupUnitImport_To_VariableFields::InitialValue, false));
  EXPECT_EQ(10.0, object.getDouble(ExternalInterface_FunctionalMockupUnitImport_To_VariableFields::InitialValue, false).get());

  // model.save(toPath("./ExternalInterfaceFunctionalMockupUnitImportToVariable.osm"), true);
  // workspace.save(toPath("./ExternalInterfaceFunctionalMockupUnitImportToVariable.idf"), true);
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ExternalInterfaceFunctionalMockupUnitImportToSchedule) {

  Model model;

  ExternalInterfaceFunctionalMockupUnitImport eifmui(model, "test name");

  ExternalInterfaceFunctionalMockupUnitImportToSchedule schedule(model, eifmui, "FMU", "FMU name", 10);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ExternalInterface_FunctionalMockupUnitImport_To_Schedule).size());
  //EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ScheduleTypeLimits).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::ExternalInterface_FunctionalMockupUnitImport_To_Schedule)[0];
  //WorkspaceObject object2 = workspace.getObjectsByType(IddObjectType::ScheduleTypeLimits)[0];

  ASSERT_TRUE(object.getString(ExternalInterface_FunctionalMockupUnitImport_To_ScheduleFields::Name, false));
  EXPECT_EQ("External Interface Functional Mockup Unit Import To Schedule 1",
            object.getString(ExternalInterface_FunctionalMockupUnitImport_To_ScheduleFields::Name, false).get());
  ASSERT_TRUE(object.getString(ExternalInterface_FunctionalMockupUnitImport_To_ScheduleFields::ScheduleTypeLimitsNames, false));
  //EXPECT_EQ(object2.nameString(), object.getString(ExternalInterface_FunctionalMockupUnitImport_To_ScheduleFields::ScheduleTypeLimitsNames, false).get());
  ASSERT_TRUE(object.getString(ExternalInterface_FunctionalMockupUnitImport_To_ScheduleFields::FMUFileName, false));
  EXPECT_EQ(eifmui.fMUFileName(), object.getString(ExternalInterface_FunctionalMockupUnitImport_To_ScheduleFields::FMUFileName, false).get());
  ASSERT_TRUE(object.getString(ExternalInterface_FunctionalMockupUnitImport_To_ScheduleFields::FMUInstanceName, false));
  EXPECT_EQ("FMU", object.getString(ExternalInterface_FunctionalMockupUnitImport_To_ScheduleFields::FMUInstanceName, false).get());
  ASSERT_TRUE(object.getString(ExternalInterface_FunctionalMockupUnitImport_To_ScheduleFields::FMUVariableName, false));
  EXPECT_EQ("FMU name", object.getString(ExternalInterface_FunctionalMockupUnitImport_To_ScheduleFields::FMUVariableName, false).get());
  ASSERT_TRUE(object.getDouble(ExternalInterface_FunctionalMockupUnitImport_To_ScheduleFields::InitialValue, false));
  EXPECT_EQ(10.0, object.getDouble(ExternalInterface_FunctionalMockupUnitImport_To_ScheduleFields::InitialValue, false).get());

  // model.save(toPath("./ExternalInterfaceFunctionalMockupUnitImportToSchedule.osm"), true);
  // workspace.save(toPath("./ExternalInterfaceFunctionalMockupUnitImportToSchedule.idf"), true);
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ExternalInterfaceFunctionalMockupUnitImportToActuator) {
  Model model;

  Building building = model.getUniqueModelObject<Building>();

  // add fan
  Schedule s = model.alwaysOnDiscreteSchedule();
  FanConstantVolume fan(model, s);

  // add FMU
  ExternalInterfaceFunctionalMockupUnitImport eifmui(model, "test name");

  // add actuator
  std::string fanControlType = "Fan Pressure Rise";
  std::string ComponentType = "Fan";
  ExternalInterfaceFunctionalMockupUnitImportToActuator fanActuator(fan, ComponentType, fanControlType, eifmui, "FMU", "Fan FMU name", 10);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ExternalInterface_FunctionalMockupUnitImport_To_Actuator).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::ExternalInterface_FunctionalMockupUnitImport_To_Actuator)[0];

  ASSERT_TRUE(object.getString(ExternalInterface_FunctionalMockupUnitImport_To_ActuatorFields::Name, false));
  EXPECT_EQ("External Interface Functional Mockup Unit Import To Actuator 1",
            object.getString(ExternalInterface_FunctionalMockupUnitImport_To_ActuatorFields::Name, false).get());
  ASSERT_TRUE(object.getString(ExternalInterface_FunctionalMockupUnitImport_To_ActuatorFields::ActuatedComponentUniqueName, false));
  EXPECT_EQ(fan.nameString(),
            object.getString(ExternalInterface_FunctionalMockupUnitImport_To_ActuatorFields::ActuatedComponentUniqueName, false).get());
  ASSERT_TRUE(object.getString(ExternalInterface_FunctionalMockupUnitImport_To_ActuatorFields::ActuatedComponentType, false));
  EXPECT_EQ(ComponentType, object.getString(ExternalInterface_FunctionalMockupUnitImport_To_ActuatorFields::ActuatedComponentType, false).get());
  ASSERT_TRUE(object.getString(ExternalInterface_FunctionalMockupUnitImport_To_ActuatorFields::ActuatedComponentControlType, false));
  EXPECT_EQ(fanControlType,
            object.getString(ExternalInterface_FunctionalMockupUnitImport_To_ActuatorFields::ActuatedComponentControlType, false).get());
  ASSERT_TRUE(object.getString(ExternalInterface_FunctionalMockupUnitImport_To_ActuatorFields::FMUFileName, false));
  EXPECT_EQ(eifmui.fMUFileName(), object.getString(ExternalInterface_FunctionalMockupUnitImport_To_ActuatorFields::FMUFileName, false).get());
  ASSERT_TRUE(object.getString(ExternalInterface_FunctionalMockupUnitImport_To_ActuatorFields::FMUVariableName, false));
  EXPECT_EQ("Fan FMU name", object.getString(ExternalInterface_FunctionalMockupUnitImport_To_ActuatorFields::FMUVariableName, false).get());
  ASSERT_TRUE(object.getDouble(ExternalInterface_FunctionalMockupUnitImport_To_ActuatorFields::InitialValue, false));
  EXPECT_EQ(10.0, object.getDouble(ExternalInterface_FunctionalMockupUnitImport_To_ActuatorFields::InitialValue, false).get());

  // model.save(toPath("./ExternalInterfaceFunctionalMockupUnitImportToActuator.osm"), true);
  // workspace.save(toPath("./ExternalInterfaceFunctionalMockupUnitImportToActuator.idf"), true);
}
