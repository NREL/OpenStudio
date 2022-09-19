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

#include "../ErrorFile.hpp"
#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/ChillerElectricASHRAE205.hpp"
#include "../../model/ChillerElectricASHRAE205_Impl.hpp"

#include "../../model/ExternalFile.hpp"
#include "../../model/ExternalFile_Impl.hpp"

#include "../../model/ThermalZone.hpp"
#include "../../model/Space.hpp"
#include "../../model/PlantLoop.hpp"
#include "../../model/PlantLoop_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/Mixer.hpp"
#include "../../model/Splitter.hpp"

#include "../../utilities/geometry/Point3d.hpp"

#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

// E+ FieldEnums
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/Chiller_Electric_ASHRAE205_FieldEnums.hxx>

#include <utilities/idd/PlantLoop_FieldEnums.hxx>
#include <utilities/idd/PlantEquipmentOperationSchemes_FieldEnums.hxx>

#include <utilities/idd/PlantEquipmentOperation_HeatingLoad_FieldEnums.hxx>
#include <utilities/idd/PlantEquipmentOperation_CoolingLoad_FieldEnums.hxx>
#include <utilities/idd/PlantEquipmentOperation_Uncontrolled_FieldEnums.hxx>

#include <utilities/idd/PlantEquipmentList_FieldEnums.hxx>

#include <utilities/idd/BranchList_FieldEnums.hxx>
#include <utilities/idd/Branch_FieldEnums.hxx>

#include "../../utilities/core/PathHelpers.hpp"
#include <resources.hxx>
#include <vector>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

/* Ensures that the nodes that translated correctly
 * that means correct node names in the ChillerElectricASHRAE205 but also
 * on the Branches
 */
TEST_F(EnergyPlusFixture, ForwardTranslator_ChillerElectricASHRAE205) {

  ForwardTranslator ft;

  Model m;

  auto createLoop = [&m](const std::string& prefix) {
    PlantLoop p(m);
    static constexpr std::array<std::string_view, 10> compNames = {
      "Supply Inlet", "Supply Splitter", "Supply Connection Node", "Supply Mixer", "Supply Outlet",
      "Demand Inlet", "Demand Splitter", "Demand Connection Node", "Demand Mixer", "Demand Outlet",
    };
    p.setName(prefix);
    for (size_t i = 0; auto& comp : p.components()) {
      comp.setName(prefix + " " + std::string{compNames[i++]});
    }
    return p;
  };

  openstudio::path p = resourcesPath() / toPath("model/A205ExampleChiller.RS0001.a205.cbor");
  EXPECT_TRUE(exists(p));

  boost::optional<ExternalFile> representationFile = ExternalFile::getExternalFile(m, openstudio::toString(p));
  ASSERT_TRUE(representationFile);

  ChillerElectricASHRAE205 ch(representationFile.get());

  EXPECT_TRUE(ch.setPerformanceInterpolationMethod("Cubic"));
  ch.autosizeRatedCapacity();
  EXPECT_TRUE(ch.setSizingFactor(1.1));

  ThermalZone z(m);
  z.setName("Basement");
  {
    std::vector<Point3d> sPoints{{0, 0, 0}, {0, 1, 0}, {1, 1, 0}, {1, 0, 0}};
    auto space_ = Space::fromFloorPrint(sPoints, 3.0, m);
    ASSERT_TRUE(space_);
    EXPECT_TRUE(space_->setThermalZone(z));
  }

  EXPECT_TRUE(ch.setAmbientTemperatureZone(z));

  EXPECT_TRUE(ch.setChilledWaterMaximumRequestedFlowRate(0.0428));

  auto chwLoop = createLoop("chwLoop");
  EXPECT_TRUE(chwLoop.addSupplyBranchForComponent(ch));

  auto cndLoop = createLoop("cndLoop");
  EXPECT_TRUE(cndLoop.addDemandBranchForComponent(ch));
  EXPECT_TRUE(ch.setCondenserMaximumRequestedFlowRate(0.0552));

  EXPECT_TRUE(ch.setChillerFlowMode("ConstantFlow"));

  auto ocLoop = createLoop("ocLoop");
  EXPECT_TRUE(ch.addDemandBranchOnOilCoolerLoop(ocLoop));
  EXPECT_TRUE(ch.setOilCoolerDesignFlowRate(0.001));

  auto auxLoop = createLoop("auxLoop");
  EXPECT_TRUE(ch.addDemandBranchOnAuxiliaryLoop(auxLoop));
  EXPECT_TRUE(ch.setAuxiliaryCoolingDesignFlowRate(0.002));

  auto hrLoop = createLoop("hrLoop");
  EXPECT_TRUE(hrLoop.addDemandBranchForComponent(ch, true));

  EXPECT_TRUE(ch.setEndUseSubcategory("Chiller"));

  ch.chilledWaterInletNode()->setName("ChilledWater Inlet Node");
  ch.chilledWaterOutletNode()->setName("ChilledWater Outlet Node");
  ch.condenserInletNode()->setName("Condenser Inlet Node");
  ch.condenserOutletNode()->setName("Condenser Outlet Node");
  ch.heatRecoveryInletNode()->setName("HeatRecovery Inlet Node");
  ch.heatRecoveryOutletNode()->setName("HeatRecovery Outlet Node");
  ch.oilCoolerInletNode()->setName("OilCooler Inlet Node");
  ch.oilCoolerOutletNode()->setName("OilCooler Outlet Node");
  ch.auxiliaryInletNode()->setName("Auxiliary Inlet Node");
  ch.auxiliaryOutletNode()->setName("Auxiliary Outlet Node");

  {
    Workspace w = ft.translateModel(m);

    std::vector<WorkspaceObject> woChs = w.getObjectsByType(IddObjectType::Chiller_Electric_ASHRAE205);
    ASSERT_EQ(1, woChs.size());
    auto& woCh = woChs.front();

    EXPECT_EQ(ch.nameString(), woCh.getString(Chiller_Electric_ASHRAE205Fields::Name).get());
    EXPECT_EQ("A205ExampleChiller.RS0001.a205.cbor", woCh.getString(Chiller_Electric_ASHRAE205Fields::RepresentationFileName).get());
    EXPECT_EQ("Cubic", woCh.getString(Chiller_Electric_ASHRAE205Fields::PerformanceInterpolationMethod).get());
    EXPECT_EQ("AutoSize", woCh.getString(Chiller_Electric_ASHRAE205Fields::RatedCapacity).get());
    EXPECT_EQ(1.1, woCh.getDouble(Chiller_Electric_ASHRAE205Fields::SizingFactor).get());

    EXPECT_EQ("Zone", woCh.getString(Chiller_Electric_ASHRAE205Fields::AmbientTemperatureIndicator).get());
    EXPECT_TRUE(woCh.isEmpty(Chiller_Electric_ASHRAE205Fields::AmbientTemperatureScheduleName));
    EXPECT_EQ("Basement", woCh.getString(Chiller_Electric_ASHRAE205Fields::AmbientTemperatureZoneName).get());
    EXPECT_TRUE(woCh.isEmpty(Chiller_Electric_ASHRAE205Fields::AmbientTemperatureOutdoorAirNodeName));

    EXPECT_EQ(0.0428, woCh.getDouble(Chiller_Electric_ASHRAE205Fields::ChilledWaterMaximumRequestedFlowRate).get());

    EXPECT_EQ(0.0552, woCh.getDouble(Chiller_Electric_ASHRAE205Fields::CondenserMaximumRequestedFlowRate).get());

    EXPECT_EQ("ConstantFlow", woCh.getString(Chiller_Electric_ASHRAE205Fields::ChillerFlowMode).get());

    EXPECT_EQ(0.001, woCh.getDouble(Chiller_Electric_ASHRAE205Fields::OilCoolerDesignFlowRate).get());

    EXPECT_EQ(0.002, woCh.getDouble(Chiller_Electric_ASHRAE205Fields::AuxiliaryCoolingDesignFlowRate).get());

    EXPECT_EQ("Chiller", woCh.getString(Chiller_Electric_ASHRAE205Fields::EndUseSubcategory).get());

    EXPECT_EQ("ChilledWater Inlet Node", woCh.getString(Chiller_Electric_ASHRAE205Fields::ChilledWaterInletNodeName).get());
    EXPECT_EQ("ChilledWater Outlet Node", woCh.getString(Chiller_Electric_ASHRAE205Fields::ChilledWaterOutletNodeName).get());
    EXPECT_EQ("Condenser Inlet Node", woCh.getString(Chiller_Electric_ASHRAE205Fields::CondenserInletNodeName).get());
    EXPECT_EQ("Condenser Outlet Node", woCh.getString(Chiller_Electric_ASHRAE205Fields::CondenserOutletNodeName).get());
    EXPECT_EQ("HeatRecovery Inlet Node", woCh.getString(Chiller_Electric_ASHRAE205Fields::HeatRecoveryInletNodeName).get());
    EXPECT_EQ("HeatRecovery Outlet Node", woCh.getString(Chiller_Electric_ASHRAE205Fields::HeatRecoveryOutletNodeName).get());
    EXPECT_EQ("OilCooler Inlet Node", woCh.getString(Chiller_Electric_ASHRAE205Fields::OilCoolerInletNodeName).get());
    EXPECT_EQ("OilCooler Outlet Node", woCh.getString(Chiller_Electric_ASHRAE205Fields::OilCoolerOutletNodeName).get());
    EXPECT_EQ("Auxiliary Inlet Node", woCh.getString(Chiller_Electric_ASHRAE205Fields::AuxiliaryInletNodeName).get());
    EXPECT_EQ("Auxiliary Outlet Node", woCh.getString(Chiller_Electric_ASHRAE205Fields::AuxiliaryOutletNodeName).get());

    EXPECT_EQ(ch.chilledWaterInletNode()->nameString(), woCh.getString(Chiller_Electric_ASHRAE205Fields::ChilledWaterInletNodeName).get());
    EXPECT_EQ(ch.chilledWaterOutletNode()->nameString(), woCh.getString(Chiller_Electric_ASHRAE205Fields::ChilledWaterOutletNodeName).get());
    EXPECT_EQ(ch.condenserInletNode()->nameString(), woCh.getString(Chiller_Electric_ASHRAE205Fields::CondenserInletNodeName).get());
    EXPECT_EQ(ch.condenserOutletNode()->nameString(), woCh.getString(Chiller_Electric_ASHRAE205Fields::CondenserOutletNodeName).get());
    EXPECT_EQ(ch.heatRecoveryInletNode()->nameString(), woCh.getString(Chiller_Electric_ASHRAE205Fields::HeatRecoveryInletNodeName).get());
    EXPECT_EQ(ch.heatRecoveryOutletNode()->nameString(), woCh.getString(Chiller_Electric_ASHRAE205Fields::HeatRecoveryOutletNodeName).get());
    EXPECT_EQ(ch.oilCoolerInletNode()->nameString(), woCh.getString(Chiller_Electric_ASHRAE205Fields::OilCoolerInletNodeName).get());
    EXPECT_EQ(ch.oilCoolerOutletNode()->nameString(), woCh.getString(Chiller_Electric_ASHRAE205Fields::OilCoolerOutletNodeName).get());
    EXPECT_EQ(ch.auxiliaryInletNode()->nameString(), woCh.getString(Chiller_Electric_ASHRAE205Fields::AuxiliaryInletNodeName).get());
    EXPECT_EQ(ch.auxiliaryOutletNode()->nameString(), woCh.getString(Chiller_Electric_ASHRAE205Fields::AuxiliaryOutletNodeName).get());

    // Check node names on supply/demand branches
    // Checks that the special case implemented in ForwardTranslatePlantLoop::populateBranch does the right job

    struct Expected
    {
      bool isSupply = true;
      std::string plantName;
      std::string inletNodeName;
      std::string outletNodeName;
    };

    std::vector<Expected> expecteds = {
      {true, ch.chilledWaterLoop()->nameString(), ch.chilledWaterInletNode()->nameString(), ch.chilledWaterOutletNode()->nameString()},
      {false, ch.condenserWaterLoop()->nameString(), ch.condenserInletNode()->nameString(), ch.condenserOutletNode()->nameString()},
      {false, ch.heatRecoveryLoop()->nameString(), ch.heatRecoveryInletNode()->nameString(), ch.heatRecoveryOutletNode()->nameString()},
      {false, ch.oilCoolerLoop()->nameString(), ch.oilCoolerInletNode()->nameString(), ch.oilCoolerOutletNode()->nameString()},
      {false, ch.auxiliaryLoop()->nameString(), ch.auxiliaryInletNode()->nameString(), ch.auxiliaryOutletNode()->nameString()},
    };
    for (const auto& e : expecteds) {
      auto p_ = w.getObjectByTypeAndName(IddObjectType::PlantLoop, e.plantName);
      ASSERT_TRUE(p_.is_initialized()) << "Cannot find PlantLoop named " << e.plantName;
      WorkspaceObject idf_plant = p_.get();
      unsigned index = e.isSupply ? PlantLoopFields::PlantSideBranchListName : PlantLoopFields::DemandSideBranchListName;
      WorkspaceObject idf_brlist = idf_plant.getTarget(index).get();

      // Should have at least three branches: supply inlet, the one with the Chiller, supply outlet.
      // On the demand side, there's also a bypass branch that is added by the FT by default
      ASSERT_EQ(e.isSupply ? 3 : 4, idf_brlist.extensibleGroups().size()) << "Failed for " << e.plantName;
      // Get the Chiller one
      auto w_eg = idf_brlist.extensibleGroups()[1].cast<WorkspaceExtensibleGroup>();
      WorkspaceObject idf_branch = w_eg.getTarget(BranchListExtensibleFields::BranchName).get();

      // There should be only one equipment on the branch
      ASSERT_EQ(1, idf_branch.extensibleGroups().size());
      auto w_eg2 = idf_branch.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();

      ASSERT_EQ(w_eg2.getString(BranchExtensibleFields::ComponentName).get(), ch.nameString());
      ASSERT_EQ(w_eg2.getString(BranchExtensibleFields::ComponentInletNodeName).get(), e.inletNodeName);
      ASSERT_EQ(w_eg2.getString(BranchExtensibleFields::ComponentOutletNodeName).get(), e.outletNodeName);

      WorkspaceObject idf_plant_op = p_->getTarget(PlantLoopFields::PlantEquipmentOperationSchemeName).get();
      if (e.isSupply) {
        // Should have created a Cooling Load one only
        ASSERT_EQ(1, idf_plant_op.extensibleGroups().size());
        auto w_eg = idf_plant_op.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
        ASSERT_EQ("PlantEquipmentOperation:CoolingLoad",
                  w_eg.getString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeObjectType).get());

        // Get the Operation Scheme
        auto op_scheme_ = w_eg.getTarget(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeName);
        ASSERT_TRUE(op_scheme_);

        // Get the Plant Equipment List of this CoolingLoad scheme
        // There should only be one Load Range
        ASSERT_EQ(1u, op_scheme_->extensibleGroups().size());

        // Load range 1
        w_eg = op_scheme_->extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
        auto peq_list_ = w_eg.getTarget(PlantEquipmentOperation_HeatingLoadExtensibleFields::RangeEquipmentListName);
        ASSERT_TRUE(peq_list_);

        // Should have one equipment on it: CentralHeatPumpSystem
        auto peqs = peq_list_->extensibleGroups();
        ASSERT_EQ(1, peqs.size());
        ASSERT_EQ("Chiller:Electric:ASHRAE205", peqs.front().getString(PlantEquipmentListExtensibleFields::EquipmentObjectType).get());
        ASSERT_EQ(ch.nameString(), peqs.front().getString(PlantEquipmentListExtensibleFields::EquipmentName).get());

      } else {
        EXPECT_EQ(0, idf_plant_op.extensibleGroups().size());
      }
    }
  }

  // Not assigned to a Chilled Water Loop? not translated, it's required
  ch.removeFromPlantLoop();
  {
    // Assigned to a Surface
    Workspace w = ft.translateModel(m);

    EXPECT_EQ(0, w.getObjectsByType(IddObjectType::Chiller_Electric_ASHRAE205).size());
  }
}
