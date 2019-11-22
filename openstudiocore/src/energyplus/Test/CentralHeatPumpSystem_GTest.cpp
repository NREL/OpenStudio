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

#include "../ErrorFile.hpp"
#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
//#include "../../model/Building.hpp"
//#include "../../model/Building_Impl.hpp"
//#include "../../model/Site.hpp"
//#include "../../model/Site_Impl.hpp"
//#include "../../model/WeatherFile.hpp"
//#include "../../model/WeatherFile_Impl.hpp"
//#include "../../model/ThermalZone.hpp"
//#include "../../model/Component.hpp"
//#include "../../model/Curve.hpp"
//#include "../../model/Curve_Impl.hpp"
//#include "../../model/CurveQuadratic.hpp"
//#include "../../model/CurveQuadratic_Impl.hpp"

#include "../../model/CentralHeatPumpSystem.hpp"
#include "../../model/CentralHeatPumpSystemModule.hpp"
#include "../../model/ChillerHeaterPerformanceElectricEIR.hpp"

#include "../../model/PlantLoop.hpp"
#include "../../model/Node.hpp"
// I think I'll need to cast to a node
#include "../../model/Node_Impl.hpp"

#include "../../model/BoilerHotWater.hpp"
#include "../../model/CoolingTowerSingleSpeed.hpp"

#include "../../model/Schedule.hpp"


#include "../../model/Version.hpp"
#include "../../model/Version_Impl.hpp"

#include "../../utilities/geometry/Point3d.hpp"

#include "../../utilities/core/Optional.hpp"
#include "../../utilities/core/Checksum.hpp"
#include "../../utilities/core/UUID.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/sql/SqlFile.hpp"
#include "../../utilities/idf/IdfFile.hpp"

// OS FieldEnums
// #include <utilities/idd/OS_CentralHeatPumpSystem_FieldEnums.hxx>
// #include <utilities/idd/OS_CentralHeatPumpSystem_Module_FieldEnums.hxx>
// #include <utilities/idd/OS_ChillerHeaterPerformance_Electric_EIR_FieldEnums.hxx>

// E+ FieldEnums
#include <utilities/idd/CentralHeatPumpSystem_FieldEnums.hxx>

#include <utilities/idd/PlantLoop_FieldEnums.hxx>
#include <utilities/idd/PlantEquipmentOperationSchemes_FieldEnums.hxx>

#include <utilities/idd/PlantEquipmentOperation_HeatingLoad_FieldEnums.hxx>
#include <utilities/idd/PlantEquipmentOperation_CoolingLoad_FieldEnums.hxx>
#include <utilities/idd/PlantEquipmentOperation_Uncontrolled_FieldEnums.hxx>

#include <utilities/idd/PlantEquipmentList_FieldEnums.hxx>

#include <utilities/idd/BranchList_FieldEnums.hxx>
#include <utilities/idd/Branch_FieldEnums.hxx>

#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/IdfObject_Impl.hpp"

#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/WorkspaceObject_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"


#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

#include <boost/algorithm/string/predicate.hpp>

#include <QThread>

#include <resources.hxx>

#include <sstream>

#include <vector>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;


/* Ensures that the nodes that translated correctly
 * that means correct node names in the CentralHeatPumpSystem but also
 * on the Branches
 */
TEST_F(EnergyPlusFixture,ForwardTranslatorCentralHeatPumpSystem_Nodes) {

  boost::optional<WorkspaceObject> _wo;
  ForwardTranslator ft;

  Model m;

  CentralHeatPumpSystem central_hp(m);
  ASSERT_TRUE(central_hp.setAncillaryPower(0.7));
  EXPECT_FALSE(central_hp.ancillaryOperationSchedule());
  Schedule schedule = m.alwaysOnDiscreteSchedule();

  // Return type: bool
  ASSERT_TRUE(central_hp.setAncillaryOperationSchedule(schedule));
  EXPECT_TRUE(central_hp.ancillaryOperationSchedule());

  // First module has one
  CentralHeatPumpSystemModule central_hp_module(m);
  central_hp.addModule(central_hp_module);
  EXPECT_EQ(1, central_hp_module.numberofChillerHeaterModules());

  // Second has 2
  CentralHeatPumpSystemModule central_hp_module2(m);
  central_hp.addModule(central_hp_module2);
  ASSERT_TRUE(central_hp_module2.setNumberofChillerHeaterModules(2));


  ASSERT_EQ( (unsigned)2, central_hp.modules().size() );


  // Connect the CentralHP to three plant loops
  // CoolingLoop: on the supply side
  PlantLoop coolingPlant(m);
  EXPECT_TRUE(coolingPlant.addSupplyBranchForComponent(central_hp));

  // SourceLoop: on the demand side
  PlantLoop sourcePlant(m);
  EXPECT_TRUE(sourcePlant.addDemandBranchForComponent(central_hp));
  // Also add a CT to the sourcePlant
  CoolingTowerSingleSpeed ct(m);
  sourcePlant.addSupplyBranchForComponent(ct);

  // HeatingLoop: on the supply side
  PlantLoop heatingPlant(m);
  // Workaround to be able to use addToTertiaryNode directly
  // (addSupplyBranchForComponent should work directly, but this is tested in model GTest, so here we just make sure we call
  // addToTertiaryNode directly)
  BoilerHotWater temp_b(m);
  EXPECT_TRUE(heatingPlant.addSupplyBranchForComponent(temp_b));

  ASSERT_TRUE(temp_b.inletModelObject());
  auto node = temp_b.inletModelObject().get().cast<Node>();
  EXPECT_TRUE(central_hp.addToTertiaryNode(node));
  temp_b.remove();


  // Translate
  Workspace w = ft.translateModel(m);

  EXPECT_EQ(0u, ft.errors().size());
  EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::CentralHeatPumpSystem).size());

  IdfObject i_central_hp = w.getObjectsByType(IddObjectType::CentralHeatPumpSystem)[0];

  // supply = Cooling
  ASSERT_EQ(i_central_hp.getString(CentralHeatPumpSystemFields::CoolingLoopInletNodeName).get(),
            central_hp.supplyInletModelObject().get().name());

  ASSERT_EQ(i_central_hp.getString(CentralHeatPumpSystemFields::CoolingLoopOutletNodeName).get(),
            central_hp.supplyOutletModelObject().get().name());

  // demand = Source
  ASSERT_EQ(i_central_hp.getString(CentralHeatPumpSystemFields::SourceLoopInletNodeName).get(),
            central_hp.demandInletModelObject().get().name());

  ASSERT_EQ(i_central_hp.getString(CentralHeatPumpSystemFields::SourceLoopOutletNodeName).get(),
            central_hp.demandOutletModelObject().get().name());

  // tertiary = Heating
  ASSERT_EQ(i_central_hp.getString(CentralHeatPumpSystemFields::HeatingLoopInletNodeName).get(),
            central_hp.tertiaryInletModelObject().get().name());

  ASSERT_EQ(i_central_hp.getString(CentralHeatPumpSystemFields::HeatingLoopOutletNodeName).get(),
            central_hp.tertiaryOutletModelObject().get().name());


  // Check node names on supply/demand branches
  // Checks that the special case implemented in ForwardTranslatePlantLoop::populateBranch does the right job

  // supply = Cooling (on supply)
  {
    _wo = w.getObjectByTypeAndName(IddObjectType::PlantLoop, coolingPlant.name().get());
    ASSERT_TRUE(_wo.is_initialized());
    WorkspaceObject idf_plant = _wo.get();
    WorkspaceObject idf_brlist = idf_plant.getTarget(PlantLoopFields::PlantSideBranchListName).get();

    // Should have three branches: supply inlet, the one with the centralHP, supply outlet
    ASSERT_EQ(3u, idf_brlist.extensibleGroups().size());
    // Get the Central HP one
    WorkspaceExtensibleGroup w_eg = idf_brlist.extensibleGroups()[1].cast<WorkspaceExtensibleGroup>();
    WorkspaceObject idf_branch = w_eg.getTarget(BranchListExtensibleFields::BranchName).get();

    // There should be only one equipment on the branch
    ASSERT_EQ(1u, idf_branch.extensibleGroups().size());
    WorkspaceExtensibleGroup w_eg2 = idf_branch.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();

    ASSERT_EQ(w_eg2.getString(BranchExtensibleFields::ComponentName).get(),
        central_hp.name());

    ASSERT_EQ(w_eg2.getString(BranchExtensibleFields::ComponentInletNodeName).get(),
        central_hp.supplyInletModelObject().get().name());

    ASSERT_EQ(w_eg2.getString(BranchExtensibleFields::ComponentOutletNodeName).get(),
        central_hp.supplyOutletModelObject().get().name());
  }

  // tertiary = Heating (on supply)
  {
    _wo = w.getObjectByTypeAndName(IddObjectType::PlantLoop, heatingPlant.name().get());
    ASSERT_TRUE(_wo.is_initialized());
    WorkspaceObject idf_plant = _wo.get();
    WorkspaceObject idf_brlist = idf_plant.getTarget(PlantLoopFields::PlantSideBranchListName).get();

    // Should have three branches: supply inlet, the one with the centralHP, supply outlet
    ASSERT_EQ(3u, idf_brlist.extensibleGroups().size());
    // Get the Central HP one
    WorkspaceExtensibleGroup w_eg = idf_brlist.extensibleGroups()[1].cast<WorkspaceExtensibleGroup>();
    WorkspaceObject idf_branch = w_eg.getTarget(BranchListExtensibleFields::BranchName).get();

    // There should be only one equipment on the branch
    ASSERT_EQ(1u, idf_branch.extensibleGroups().size());
    WorkspaceExtensibleGroup w_eg2 = idf_branch.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();

    ASSERT_EQ(w_eg2.getString(BranchExtensibleFields::ComponentName).get(),
        central_hp.name());

    ASSERT_EQ(w_eg2.getString(BranchExtensibleFields::ComponentInletNodeName).get(),
        central_hp.tertiaryInletModelObject().get().name());

    ASSERT_EQ(w_eg2.getString(BranchExtensibleFields::ComponentOutletNodeName).get(),
        central_hp.tertiaryOutletModelObject().get().name());
  }

  // demand = Source (on demand)
  {
    _wo = w.getObjectByTypeAndName(IddObjectType::PlantLoop, sourcePlant.name().get());
    ASSERT_TRUE(_wo.is_initialized());
    WorkspaceObject idf_plant = _wo.get();
    // Demand Side Branch List Name
    WorkspaceObject idf_brlist = idf_plant.getTarget(PlantLoopFields::DemandSideBranchListName).get();

    // Should have four branches: supply inlet, the one with the centralHP, a bypass, supply outlet
    ASSERT_EQ(4u, idf_brlist.extensibleGroups().size());
    // Get the Central HP one, which should be the second one
    WorkspaceExtensibleGroup w_eg = idf_brlist.extensibleGroups()[1].cast<WorkspaceExtensibleGroup>();
    WorkspaceObject idf_branch = w_eg.getTarget(BranchListExtensibleFields::BranchName).get();

    // There should be only one equipment on the branch
    ASSERT_EQ(1u, idf_branch.extensibleGroups().size());
    WorkspaceExtensibleGroup w_eg2 = idf_branch.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();

    ASSERT_EQ(w_eg2.getString(BranchExtensibleFields::ComponentName).get(),
        central_hp.name());

    ASSERT_EQ(w_eg2.getString(BranchExtensibleFields::ComponentInletNodeName).get(),
        central_hp.demandInletModelObject().get().name());

    ASSERT_EQ(w_eg2.getString(BranchExtensibleFields::ComponentOutletNodeName).get(),
        central_hp.demandOutletModelObject().get().name());
  }

  // m.save(toPath("./ft_central_hp.osm"), true);
  // w.save(toPath("./ft_central_hp.idf"), true);

}

/* This tests ensures that the CentralHeatPumpSystem ends up in a PlantEquipmentOperation:HeatingLoad for the heating loop
 * and PlantEquipmentOperation:CoolingLoad for the cooling loop. For the source loop, it's on the demand side so it shouldn't
 * be part of the plant equipment list used
 */
TEST_F(EnergyPlusFixture,ForwardTranslatorCentralHeatPumpSystem_PlantEquipmentOperation) {

  boost::optional<WorkspaceObject> _wo;
  ForwardTranslator ft;

  Model m;

  CentralHeatPumpSystem central_hp(m);

  // Add a Module
  CentralHeatPumpSystemModule central_hp_module(m);
  central_hp.addModule(central_hp_module);
  EXPECT_EQ(1, central_hp_module.numberofChillerHeaterModules());

  // Connect the CentralHP to three plant loops
  // CoolingLoop: on the supply side
  PlantLoop coolingPlant(m);
  EXPECT_TRUE(coolingPlant.addSupplyBranchForComponent(central_hp));

  // SourceLoop: on the demand side
  PlantLoop sourcePlant(m);
  EXPECT_TRUE(sourcePlant.addDemandBranchForComponent(central_hp));
  // Also add a CT to the sourcePlant
  CoolingTowerSingleSpeed ct(m);
  sourcePlant.addSupplyBranchForComponent(ct);

  // HeatingLoop: on the supply side
  PlantLoop heatingPlant(m);
  // Workaround to be able to use addToTertiaryNode directly
  // (addSupplyBranchForComponent should work directly, but this is tested in model GTest, so here we just make sure we call
  // addToTertiaryNode directly)
  BoilerHotWater temp_b(m);
  EXPECT_TRUE(heatingPlant.addSupplyBranchForComponent(temp_b));

  ASSERT_TRUE(temp_b.inletModelObject());
  auto node = temp_b.inletModelObject().get().cast<Node>();
  EXPECT_TRUE(central_hp.addToTertiaryNode(node));
  temp_b.remove();



  Workspace w = ft.translateModel(m);

  EXPECT_EQ(0u, ft.errors().size());
  EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::CentralHeatPumpSystem).size());

  IdfObject i_central_hp = w.getObjectsByType(IddObjectType::CentralHeatPumpSystem)[0];



  // Get the Loops, and find their plant operation scheme

  // supply = Cooling
  {
    _wo = w.getObjectByTypeAndName(IddObjectType::PlantLoop, coolingPlant.name().get());
    ASSERT_TRUE(_wo.is_initialized());
    WorkspaceObject idf_coolingPlant = _wo.get();
    WorkspaceObject idf_plant_op = idf_coolingPlant.getTarget(PlantLoopFields::PlantEquipmentOperationSchemeName).get();

    // Should have created a Cooling Load one only
    ASSERT_EQ(1u, idf_plant_op.extensibleGroups().size());
    WorkspaceExtensibleGroup w_eg = idf_plant_op.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
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

    // Should have one equipment on it: CentralHeatPumpSystem
    ASSERT_EQ(1u, idf_peq_list.extensibleGroups().size());
    IdfExtensibleGroup idf_eg(idf_peq_list.extensibleGroups()[0]);
    ASSERT_EQ(central_hp.name().get(), idf_eg.getString(PlantEquipmentListExtensibleFields::EquipmentName).get());

  }


  // tertiary = Heating
  {
    _wo = w.getObjectByTypeAndName(IddObjectType::PlantLoop, heatingPlant.name().get());
    ASSERT_TRUE(_wo.is_initialized());
    WorkspaceObject idf_heatingPlant = _wo.get();
    WorkspaceObject idf_plant_op = idf_heatingPlant.getTarget(PlantLoopFields::PlantEquipmentOperationSchemeName).get();

    // Should have created a Heating Load one only
    ASSERT_EQ(1u, idf_plant_op.extensibleGroups().size());
    WorkspaceExtensibleGroup w_eg = idf_plant_op.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
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

    // Should have one equipment on it: CentralHeatPumpSystem
    ASSERT_EQ(1u, idf_peq_list.extensibleGroups().size());
    IdfExtensibleGroup idf_eg(idf_peq_list.extensibleGroups()[0]);
    ASSERT_EQ(central_hp.name().get(), idf_eg.getString(PlantEquipmentListExtensibleFields::EquipmentName).get());

  }



  // SourceLoop: on the demand side. So it shouldn't be on it
  {
    _wo = w.getObjectByTypeAndName(IddObjectType::PlantLoop, sourcePlant.name().get());
    ASSERT_TRUE(_wo.is_initialized());
    WorkspaceObject idf_sourcePlant = _wo.get();
    WorkspaceObject idf_plant_op = idf_sourcePlant.getTarget(PlantLoopFields::PlantEquipmentOperationSchemeName).get();

    // Should have created a Cooling Load one only
    ASSERT_EQ(1u, idf_plant_op.extensibleGroups().size());
    WorkspaceExtensibleGroup w_eg = idf_plant_op.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
    ASSERT_EQ("PlantEquipmentOperation:CoolingLoad", w_eg.getString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeObjectType).get());

    // Get the Operation Scheme
    _wo = w_eg.getTarget(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeName);
    ASSERT_TRUE(_wo.is_initialized());
    WorkspaceObject idf_op_scheme = _wo.get();

    // Get the Plant Equipment List of this HeatingLoad scheme
    // There should only be one Load Range
    ASSERT_EQ(1u, idf_op_scheme.extensibleGroups().size());
    // Load range 1
    w_eg = idf_op_scheme.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
    _wo = w_eg.getTarget(PlantEquipmentOperation_CoolingLoadExtensibleFields::RangeEquipmentListName);
    ASSERT_TRUE(_wo.is_initialized());
    WorkspaceObject idf_peq_list = _wo.get();

    // Should have one equipment on it: Cooling Tower
    ASSERT_EQ(1u, idf_peq_list.extensibleGroups().size());
    IdfExtensibleGroup idf_eg(idf_peq_list.extensibleGroups()[0]);
    ASSERT_EQ(ct.name().get(), idf_eg.getString(PlantEquipmentListExtensibleFields::EquipmentName).get());

  }

  m.save(toPath("./ft_central_hp.osm"), true);
  w.save(toPath("./ft_central_hp.idf"), true);

}

TEST_F(EnergyPlusFixture,ForwardTranslatorCentralHeatPumpSystem_NoModules) {

  boost::optional<WorkspaceObject> _wo;
  ForwardTranslator ft;

  Model m;

  CentralHeatPumpSystem central_hp(m);

  // DO NOT Add a Module
  EXPECT_EQ(0u, central_hp.modules().size());

  // Connect the CentralHP to three plant loops
  // CoolingLoop: on the supply side
  PlantLoop coolingPlant(m);
  EXPECT_TRUE(coolingPlant.addSupplyBranchForComponent(central_hp));

  // SourceLoop: on the demand side
  PlantLoop sourcePlant(m);
  EXPECT_TRUE(sourcePlant.addDemandBranchForComponent(central_hp));
  // Also add a CT to the sourcePlant
  CoolingTowerSingleSpeed ct(m);
  sourcePlant.addSupplyBranchForComponent(ct);

   // HeatingLoop: on the supply side
  PlantLoop heatingPlant(m);
  // Workaround to be able to use addToTertiaryNode directly
  // (addSupplyBranchForComponent should work directly, but this is tested in model GTest, so here we just make sure we call
  // addToTertiaryNode directly)
  BoilerHotWater temp_b(m);
  EXPECT_TRUE(heatingPlant.addSupplyBranchForComponent(temp_b));

  ASSERT_TRUE(temp_b.inletModelObject());
  auto node = temp_b.inletModelObject().get().cast<Node>();
  EXPECT_TRUE(central_hp.addToTertiaryNode(node));
  temp_b.remove();
  EXPECT_TRUE(central_hp.coolingPlantLoop());
  EXPECT_TRUE(central_hp.sourcePlantLoop());
  EXPECT_TRUE(central_hp.heatingPlantLoop());

  Workspace w = ft.translateModel(m);

  EXPECT_EQ(0u, ft.errors().size());
  EXPECT_EQ(0u, w.getObjectsByType(IddObjectType::CentralHeatPumpSystem).size());

}
