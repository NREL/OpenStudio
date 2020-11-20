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

// Objects of interest
#include "../../model/GeneratorMicroTurbine.hpp"
#include "../../model/GeneratorMicroTurbine_Impl.hpp"
#include "../../model/GeneratorMicroTurbineHeatRecovery.hpp"
#include "../../model/GeneratorMicroTurbineHeatRecovery_Impl.hpp"

// Needed resources
#include "../../model/PlantLoop.hpp"
#include "../../model/PlantLoop_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"
#include "../../model/CurveBiquadratic.hpp"
#include "../../model/CurveBiquadratic_Impl.hpp"
#include "../../model/CurveCubic.hpp"
#include "../../model/CurveCubic_Impl.hpp"
#include "../../model/CurveQuadratic.hpp"
#include "../../model/CurveQuadratic_Impl.hpp"
#include "../../model/StraightComponent.hpp"
#include "../../model/StraightComponent_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
// For testing PlantEquipOperation
#include "../../model/WaterHeaterMixed.hpp"
#include "../../model/WaterHeaterMixed_Impl.hpp"
#include "../../model/PlantEquipmentOperationHeatingLoad.hpp"
#include "../../model/PlantEquipmentOperationHeatingLoad_Impl.hpp"
#include "../../model/ElectricLoadCenterDistribution.hpp"
#include "../../model/ElectricLoadCenterDistribution_Impl.hpp"

// IDF FieldEnums
#include <utilities/idd/Generator_MicroTurbine_FieldEnums.hxx>
// #include <utilities/idd/OS_Generator_MicroTurbine_HeatRecovery_FieldEnums.hxx>
#include <utilities/idd/PlantEquipmentList_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

// Misc
#include "../../model/Version.hpp"
#include "../../model/Version_Impl.hpp"

#include "../../utilities/core/Optional.hpp"
#include "../../utilities/core/Checksum.hpp"
#include "../../utilities/core/UUID.hpp"
#include "../../utilities/sql/SqlFile.hpp"
#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/IdfObject.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <boost/algorithm/string/predicate.hpp>

#include <resources.hxx>

#include <sstream>

#include <vector>

// Debug
#include "../../utilities/core/Logger.hpp"

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

/**
 * Tests whether the ForwarTranslator will handle the name of the GeneratorMicroTurbine correctly in the PlantEquipmentOperationHeatingLoad
 **/
TEST_F(EnergyPlusFixture, ForwardTranslatorGeneratorMicroTurbine_ELCD_PlantLoop) {

  // TODO: Temporarily output the Log in the console with the Trace (-3) level
  // for debug
  // openstudio::Logger::instance().standardOutLogger().enable();
  // openstudio::Logger::instance().standardOutLogger().setLogLevel(Trace);

  // Create a model, a mchp, a mchpHR, a plantLoop and an electricalLoadCenter
  Model model;

  GeneratorMicroTurbine mchp = GeneratorMicroTurbine(model);
  GeneratorMicroTurbineHeatRecovery mchpHR = GeneratorMicroTurbineHeatRecovery(model, mchp);
  ASSERT_EQ(mchpHR, mchp.generatorMicroTurbineHeatRecovery().get());

  PlantLoop plantLoop(model);
  // Add a supply branch for the mchpHR
  ASSERT_TRUE(plantLoop.addSupplyBranchForComponent(mchpHR));

  // Create a WaterHeater:Mixed
  WaterHeaterMixed waterHeater(model);
  // Add it on the same branch as the chpHR, right after it
  Node mchpHROutletNode = mchpHR.outletModelObject()->cast<Node>();
  ASSERT_TRUE(waterHeater.addToNode(mchpHROutletNode));

  // Create a plantEquipmentOperationHeatingLoad
  PlantEquipmentOperationHeatingLoad operation(model);
  operation.setName(plantLoop.name().get() + " PlantEquipmentOperationHeatingLoad");
  ASSERT_TRUE(plantLoop.setPlantEquipmentOperationHeatingLoad(operation));
  ASSERT_TRUE(operation.addEquipment(mchpHR));
  ASSERT_TRUE(operation.addEquipment(waterHeater));

  // Create an ELCD
  ElectricLoadCenterDistribution elcd = ElectricLoadCenterDistribution(model);
  elcd.setName("Capstone C65 ELCD");
  elcd.setElectricalBussType("AlternatingCurrent");
  elcd.addGenerator(mchp);

  // Forward Translates
  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  EXPECT_EQ(0u, forwardTranslator.errors().size());
  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::WaterHeater_Mixed).size());
  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::ElectricLoadCenter_Distribution).size());
  // The MicroTurbine should have been forward translated since there is an ELCD

  WorkspaceObjectVector microTurbineObjects(workspace.getObjectsByType(IddObjectType::Generator_MicroTurbine));
  EXPECT_EQ(1u, microTurbineObjects.size());
  // Check that the HR nodes have been set
  WorkspaceObject idf_mchp(microTurbineObjects[0]);
  EXPECT_EQ(mchpHR.inletModelObject()->name().get(), idf_mchp.getString(Generator_MicroTurbineFields::HeatRecoveryWaterInletNodeName).get());
  EXPECT_EQ(mchpHR.outletModelObject()->name().get(), idf_mchp.getString(Generator_MicroTurbineFields::HeatRecoveryWaterOutletNodeName).get());

  OptionalWorkspaceObject idf_operation(workspace.getObjectByTypeAndName(IddObjectType::PlantEquipmentOperation_HeatingLoad, *(operation.name())));
  ASSERT_TRUE(idf_operation);
  // Get the extensible
  ASSERT_EQ(1u, idf_operation->numExtensibleGroups());
  // IdfExtensibleGroup eg = idf_operation.getExtensibleGroup(0);
  // idf_operation.targets[0]
  ASSERT_EQ(1u, idf_operation->targets().size());
  WorkspaceObject plantEquipmentList(idf_operation->targets()[0]);
  ASSERT_EQ(2u, plantEquipmentList.extensibleGroups().size());

  IdfExtensibleGroup eg(plantEquipmentList.extensibleGroups()[0]);
  ASSERT_EQ("Generator:MicroTurbine", eg.getString(PlantEquipmentListExtensibleFields::EquipmentObjectType).get());
  // This fails
  EXPECT_EQ(mchp.name().get(), eg.getString(PlantEquipmentListExtensibleFields::EquipmentName).get());

  IdfExtensibleGroup eg2(plantEquipmentList.extensibleGroups()[1]);
  ASSERT_EQ("WaterHeater:Mixed", eg2.getString(PlantEquipmentListExtensibleFields::EquipmentObjectType).get());
  EXPECT_EQ(waterHeater.name().get(), eg2.getString(PlantEquipmentListExtensibleFields::EquipmentName).get());

  // model.save(toPath("./ForwardTranslatorGeneratorMicroTurbine_ELCD_PlantLoop.osm"), true);
  // workspace.save(toPath("./ForwardTranslatorGeneratorMicroTurbine_ELCD_PlantLoop.idf"), true);
}
//test orphaning the generator before FT
TEST_F(EnergyPlusFixture, ForwardTranslatorGeneratorMicroTurbine_ELCD_Orphan) {
  // Create a model, a mchp, a mchpHR, a plantLoop and an electricalLoadCenter
  Model model;

  GeneratorMicroTurbine mchp = GeneratorMicroTurbine(model);
  GeneratorMicroTurbineHeatRecovery mchpHR = GeneratorMicroTurbineHeatRecovery(model, mchp);
  ASSERT_EQ(mchpHR, mchp.generatorMicroTurbineHeatRecovery().get());

  PlantLoop plantLoop(model);
  // Add a supply branch for the mchpHR
  ASSERT_TRUE(plantLoop.addSupplyBranchForComponent(mchpHR));

  // Create a WaterHeater:Mixed
  WaterHeaterMixed waterHeater(model);
  // Add it on the same branch as the chpHR, right after it
  Node mchpHROutletNode = mchpHR.outletModelObject()->cast<Node>();
  ASSERT_TRUE(waterHeater.addToNode(mchpHROutletNode));
  EXPECT_TRUE(waterHeater.plantLoop());

  // Create a plantEquipmentOperationHeatingLoad
  PlantEquipmentOperationHeatingLoad operation(model);
  operation.setName(plantLoop.name().get() + " PlantEquipmentOperationHeatingLoad");
  ASSERT_TRUE(plantLoop.setPlantEquipmentOperationHeatingLoad(operation));
  ASSERT_TRUE(operation.addEquipment(mchpHR));
  ASSERT_TRUE(operation.addEquipment(waterHeater));

  // Create an ELCD
  ElectricLoadCenterDistribution elcd = ElectricLoadCenterDistribution(model);
  elcd.setName("Capstone C65 ELCD");
  elcd.setElectricalBussType("AlternatingCurrent");
  elcd.addGenerator(mchp);

  // orphan the generator from the ELCD
  boost::optional<ElectricLoadCenterDistribution> elcd2 = mchp.electricLoadCenterDistribution();
  elcd2.get().remove();
  EXPECT_FALSE(mchp.electricLoadCenterDistribution());

  // Forward Translates
  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  EXPECT_EQ(0u, forwardTranslator.errors().size());
  //ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::WaterHeater_Mixed).size());
  ASSERT_EQ(0u, workspace.getObjectsByType(IddObjectType::ElectricLoadCenter_Distribution).size());

  // model.save(toPath("./ForwardTranslatorGeneratorMicroTurbine_ELCD_orhpan.osm"), true);
  // workspace.save(toPath("./ForwardTranslatorGeneratorMicroTurbine_ELCD_orphan.idf"), true);
}
