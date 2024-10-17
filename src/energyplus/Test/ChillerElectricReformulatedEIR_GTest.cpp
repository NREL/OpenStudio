/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/ChillerElectricReformulatedEIR.hpp"
#include "../../model/ChillerElectricReformulatedEIR_Impl.hpp"

#include "../../model/CurveBiquadratic.hpp"
#include "../../model/CurveLinear.hpp"
#include "../../model/CurveQuadratic.hpp"
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

#include <utilities/idd/Chiller_Electric_ReformulatedEIR_FieldEnums.hxx>

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
TEST_F(EnergyPlusFixture, ForwardTranslator_ChillerElectricReformulatedEIR) {

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

  ChillerElectricReformulatedEIR ch(m);

  ch.setName("My ChillerElectricReformulatedEIR");

  // Reference Capacity: Required Double
  // Autosize
  EXPECT_TRUE(ch.setReferenceCapacity(30000.0));
  EXPECT_TRUE(ch.setReferenceCOP(4.5));
  EXPECT_TRUE(ch.setReferenceLeavingChilledWaterTemperature(7.0));
  EXPECT_TRUE(ch.setReferenceLeavingCondenserWaterTemperature(36.0));
  EXPECT_TRUE(ch.setReferenceChilledWaterFlowRate(3.0));
  EXPECT_TRUE(ch.setReferenceCondenserWaterFlowRate(4.0));

  // Cooling Capacity Function of Temperature Curve Name: Required Object
  ch.coolingCapacityFunctionOfTemperature().setName("capfT");

  // Electric Input to Cooling Output Ratio Function of Temperature Curve Name: Required Object
  ch.electricInputToCoolingOutputRatioFunctionOfTemperature().setName("EIRfT");

  EXPECT_TRUE(ch.setElectricInputToCoolingOutputRatioFunctionOfPLRType("Lift"));

  // Electric Input to Cooling Output Ratio Function of Part Load Ratio Curve Name: Required Object
  ch.electricInputToCoolingOutputRatioFunctionOfPLR().setName("EIRfPLR");

  EXPECT_TRUE(ch.setMinimumPartLoadRatio(0.1));
  EXPECT_TRUE(ch.setMaximumPartLoadRatio(1.3));
  EXPECT_TRUE(ch.setOptimumPartLoadRatio(0.8));
  EXPECT_TRUE(ch.setMinimumUnloadingRatio(0.2));

  // Chilled Water Inlet Node Name: Required Object
  // Chilled Water Outlet Node Name: Required Object
  auto chwLoop = createLoop("chwLoop");
  EXPECT_TRUE(chwLoop.addSupplyBranchForComponent(ch));
  ch.supplyInletModelObject()->setName("ChilledWater Inlet Node");
  ch.supplyOutletModelObject()->setName("ChilledWater Outlet Node");

  // Condenser Inlet Node Name: Optional Object
  // Condenser Outlet Node Name: Optional Object
  auto cndLoop = createLoop("cndLoop");
  EXPECT_TRUE(cndLoop.addDemandBranchForComponent(ch));
  ch.demandInletModelObject()->setName("Condenser Inlet Node");
  ch.demandOutletModelObject()->setName("Condenser Outlet Node");

  // Condenser Type

  EXPECT_TRUE(ch.setFractionofCompressorElectricConsumptionRejectedbyCondenser(0.957));
  EXPECT_TRUE(ch.setLeavingChilledWaterLowerTemperatureLimit(2.3));
  EXPECT_EQ(2.3, ch.leavingChilledWaterLowerTemperatureLimit());
  EXPECT_TRUE(ch.setChillerFlowMode("LeavingSetpointModulated"));
  EXPECT_TRUE(ch.setDesignHeatRecoveryWaterFlowRate(2.5));

  // Heat Recovery Inlet Node Name: Optional Object
  // Heat Recovery Outlet Node Name: Optional Object
  auto hwLoop = createLoop("HWLoop");
  EXPECT_TRUE(hwLoop.addDemandBranchForComponent(ch));

  ch.tertiaryInletModelObject()->setName("Heat Recovery Inlet Node");
  ch.tertiaryOutletModelObject()->setName("Heat Recovery Outlet Node");

  EXPECT_TRUE(ch.setSizingFactor(1.8));

  // Condenser Heat Recovery Relative Capacity Fraction: Required Double
  EXPECT_TRUE(ch.setCondenserHeatRecoveryRelativeCapacityFraction(0.97));

  // Heat Recovery Inlet High Temperature Limit Schedule Name: Optional Object
  ScheduleConstant heatRecoveryInletHighTemperatureLimitSchedule(m);
  heatRecoveryInletHighTemperatureLimitSchedule.setName("heatRecoveryInletHighTemperatureLimitSchedule");
  EXPECT_TRUE(ch.setHeatRecoveryInletHighTemperatureLimitSchedule(heatRecoveryInletHighTemperatureLimitSchedule));

  // Heat Recovery Leaving Temperature Setpoint Node Name: Optional Object
  EXPECT_TRUE(ch.setHeatRecoveryLeavingTemperatureSetpointNode(hwLoop.supplyOutletNode()));

  EXPECT_TRUE(ch.setEndUseSubcategory("Chillers"));

  // Condenser Flow Control: Required String
  EXPECT_TRUE(ch.setCondenserFlowControl("ConstantFlow"));

  // Condenser Loop Flow Rate Fraction Function of Loop Part Load Ratio Curve Name: Optional Object
  CurveLinear condenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurve(m);
  condenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurve.setName("condenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurve");
  EXPECT_TRUE(ch.setCondenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurve(condenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurve));

  // Temperature Difference Across Condenser Schedule Name: Optional Object
  ScheduleConstant temperatureDifferenceAcrossCondenserSchedule(m);
  temperatureDifferenceAcrossCondenserSchedule.setName("temperatureDifferenceAcrossCondenserSchedule");
  EXPECT_TRUE(ch.setTemperatureDifferenceAcrossCondenserSchedule(temperatureDifferenceAcrossCondenserSchedule));

  // Condenser Minimum Flow Fraction: Required Double
  EXPECT_TRUE(ch.setCondenserMinimumFlowFraction(0.9));
  EXPECT_EQ(0.9, ch.condenserMinimumFlowFraction());

  // Thermosiphon Capacity Fraction Curve Name: Optional Object
  CurveLinear thermosiphonCapacityFractionCurve(m);
  thermosiphonCapacityFractionCurve.setName("thermosiphonCapacityFractionCurve");
  EXPECT_TRUE(ch.setThermosiphonCapacityFractionCurve(thermosiphonCapacityFractionCurve));

  // Thermosiphon Minimum Temperature Difference: Required Double
  EXPECT_TRUE(ch.setThermosiphonMinimumTemperatureDifference(4.1));

  m.save("chiller.osm", true);

  {
    Workspace w = ft.translateModel(m);

    std::vector<WorkspaceObject> woChs = w.getObjectsByType(IddObjectType::Chiller_Electric_ReformulatedEIR);
    ASSERT_EQ(1, woChs.size());
    auto& woCh = woChs.front();

    EXPECT_EQ("My ChillerElectricReformulatedEIR", woCh.getString(Chiller_Electric_ReformulatedEIRFields::Name).get());
    EXPECT_EQ(30000.0, woCh.getDouble(Chiller_Electric_ReformulatedEIRFields::ReferenceCapacity).get());
    EXPECT_EQ(4.5, woCh.getDouble(Chiller_Electric_ReformulatedEIRFields::ReferenceCOP).get());
    EXPECT_EQ(7.0, woCh.getDouble(Chiller_Electric_ReformulatedEIRFields::ReferenceLeavingChilledWaterTemperature).get());
    EXPECT_EQ(36.0, woCh.getDouble(Chiller_Electric_ReformulatedEIRFields::ReferenceLeavingCondenserWaterTemperature).get());
    EXPECT_EQ(3.0, woCh.getDouble(Chiller_Electric_ReformulatedEIRFields::ReferenceChilledWaterFlowRate).get());
    EXPECT_EQ(4.0, woCh.getDouble(Chiller_Electric_ReformulatedEIRFields::ReferenceCondenserWaterFlowRate).get());
    EXPECT_EQ("capfT", woCh.getString(Chiller_Electric_ReformulatedEIRFields::CoolingCapacityFunctionofTemperatureCurveName).get());
    EXPECT_EQ("EIRfT", woCh.getString(Chiller_Electric_ReformulatedEIRFields::ElectricInputtoCoolingOutputRatioFunctionofTemperatureCurveName).get());
    EXPECT_EQ("Lift",
              woCh.getString(Chiller_Electric_ReformulatedEIRFields::ElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurveType).get());
    EXPECT_EQ("EIRfPLR",
              woCh.getString(Chiller_Electric_ReformulatedEIRFields::ElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurveName).get());
    EXPECT_EQ(0.1, woCh.getDouble(Chiller_Electric_ReformulatedEIRFields::MinimumPartLoadRatio).get());
    EXPECT_EQ(1.3, woCh.getDouble(Chiller_Electric_ReformulatedEIRFields::MaximumPartLoadRatio).get());
    EXPECT_EQ(0.8, woCh.getDouble(Chiller_Electric_ReformulatedEIRFields::OptimumPartLoadRatio).get());
    EXPECT_EQ(0.2, woCh.getDouble(Chiller_Electric_ReformulatedEIRFields::MinimumUnloadingRatio).get());
    EXPECT_EQ("ChilledWater Inlet Node", woCh.getString(Chiller_Electric_ReformulatedEIRFields::ChilledWaterInletNodeName).get());
    EXPECT_EQ("ChilledWater Outlet Node", woCh.getString(Chiller_Electric_ReformulatedEIRFields::ChilledWaterOutletNodeName).get());
    EXPECT_EQ("Condenser Inlet Node", woCh.getString(Chiller_Electric_ReformulatedEIRFields::CondenserInletNodeName).get());
    EXPECT_EQ("Condenser Outlet Node", woCh.getString(Chiller_Electric_ReformulatedEIRFields::CondenserOutletNodeName).get());
    EXPECT_EQ(0.957, woCh.getDouble(Chiller_Electric_ReformulatedEIRFields::FractionofCompressorElectricConsumptionRejectedbyCondenser).get());
    EXPECT_EQ(2.3, woCh.getDouble(Chiller_Electric_ReformulatedEIRFields::LeavingChilledWaterLowerTemperatureLimit).get());
    EXPECT_EQ("LeavingSetpointModulated", woCh.getString(Chiller_Electric_ReformulatedEIRFields::ChillerFlowModeType).get());
    EXPECT_EQ(2.5, woCh.getDouble(Chiller_Electric_ReformulatedEIRFields::DesignHeatRecoveryWaterFlowRate).get());
    EXPECT_EQ("Heat Recovery Inlet Node", woCh.getString(Chiller_Electric_ReformulatedEIRFields::HeatRecoveryInletNodeName).get());
    EXPECT_EQ("Heat Recovery Outlet Node", woCh.getString(Chiller_Electric_ReformulatedEIRFields::HeatRecoveryOutletNodeName).get());
    EXPECT_EQ(1.8, woCh.getDouble(Chiller_Electric_ReformulatedEIRFields::SizingFactor).get());
    EXPECT_EQ(0.97, woCh.getDouble(Chiller_Electric_ReformulatedEIRFields::CondenserHeatRecoveryRelativeCapacityFraction).get());
    EXPECT_EQ("heatRecoveryInletHighTemperatureLimitSchedule",
              woCh.getString(Chiller_Electric_ReformulatedEIRFields::HeatRecoveryInletHighTemperatureLimitScheduleName).get());
    EXPECT_EQ("HWLoop Supply Outlet", woCh.getString(Chiller_Electric_ReformulatedEIRFields::HeatRecoveryLeavingTemperatureSetpointNodeName).get());
    EXPECT_EQ("Chillers", woCh.getString(Chiller_Electric_ReformulatedEIRFields::EndUseSubcategory).get());
    EXPECT_EQ("ConstantFlow", woCh.getString(Chiller_Electric_ReformulatedEIRFields::CondenserFlowControl).get());
    EXPECT_EQ("condenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurve",
              woCh.getString(Chiller_Electric_ReformulatedEIRFields::CondenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurveName).get());
    EXPECT_EQ("temperatureDifferenceAcrossCondenserSchedule",
              woCh.getString(Chiller_Electric_ReformulatedEIRFields::TemperatureDifferenceAcrossCondenserScheduleName).get());
    EXPECT_EQ(0.9, woCh.getDouble(Chiller_Electric_ReformulatedEIRFields::CondenserMinimumFlowFraction).get());
    EXPECT_EQ("thermosiphonCapacityFractionCurve",
              woCh.getString(Chiller_Electric_ReformulatedEIRFields::ThermosiphonCapacityFractionCurveName).get());
    EXPECT_EQ(4.1, woCh.getDouble(Chiller_Electric_ReformulatedEIRFields::ThermosiphonMinimumTemperatureDifference).get());

    // Check node names on supply/demand branches
    // Checks that the special case implemented in ForwardTranslatePlantLoop::populateBranch does the right job

    struct Expected
    {
      Expected(bool t_isSupply, std::string t_plantName, std::string t_inletNodeName, std::string t_outletNodeName)
        : isSupply(t_isSupply),
          plantName(std::move(t_plantName)),
          inletNodeName(std::move(t_inletNodeName)),
          outletNodeName(std::move(t_outletNodeName)) {}
      bool isSupply = true;
      std::string plantName;
      std::string inletNodeName;
      std::string outletNodeName;
    };

    std::vector<Expected> expecteds = {
      {true, ch.chilledWaterLoop()->nameString(), ch.supplyInletModelObject()->nameString(), ch.supplyOutletModelObject()->nameString()},
      {false, ch.condenserWaterLoop()->nameString(), ch.demandInletModelObject()->nameString(), ch.demandOutletModelObject()->nameString()},
      {false, ch.heatRecoveryLoop()->nameString(), ch.tertiaryInletModelObject()->nameString(), ch.tertiaryOutletModelObject()->nameString()},
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
        ASSERT_EQ("Chiller:Electric:ReformulatedEIR", peqs.front().getString(PlantEquipmentListExtensibleFields::EquipmentObjectType).get());
        ASSERT_EQ(ch.nameString(), peqs.front().getString(PlantEquipmentListExtensibleFields::EquipmentName).get());

      } else {
        EXPECT_EQ(0, idf_plant_op.extensibleGroups().size());
      }
    }
  }
}
