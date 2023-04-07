/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../../model/Model.hpp"
#include "../../model/HeatPumpPlantLoopEIRCooling.hpp"
#include "../../model/HeatPumpPlantLoopEIRHeating.hpp"
#include "../../model/Node.hpp"
#include "../../model/PlantLoop.hpp"
#include "../../model/CurveBiquadratic.hpp"
#include "../../model/CurveQuadratic.hpp"

#include <utilities/idd/HeatPump_PlantLoop_EIR_Cooling_FieldEnums.hxx>
#include <utilities/idd/HeatPump_PlantLoop_EIR_Heating_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/IdfObject_Impl.hpp"

#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/WorkspaceObject_Impl.hpp"

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_HeatPumpPlantLoopEIR_AirSource) {

  Model m;

  PlantLoop plant_loop_plhp_clg(m);
  PlantLoop plant_loop_plhp_htg(m);

  HeatPumpPlantLoopEIRCooling plhp_clg(m);
  EXPECT_TRUE(plhp_clg.setLoadSideReferenceFlowRate(1.0));
  EXPECT_TRUE(plhp_clg.setSourceSideReferenceFlowRate(2.0));
  EXPECT_TRUE(plhp_clg.setReferenceCapacity(3.0));
  EXPECT_TRUE(plhp_clg.setReferenceCoefficientofPerformance(4.0));
  EXPECT_TRUE(plhp_clg.setSizingFactor(5.0));
  CurveBiquadratic curve1(m);
  EXPECT_TRUE(plhp_clg.setCapacityModifierFunctionofTemperatureCurve(curve1));
  CurveBiquadratic curve2(m);
  EXPECT_TRUE(plhp_clg.setElectricInputtoOutputRatioModifierFunctionofTemperatureCurve(curve2));
  CurveQuadratic curve3(m);
  EXPECT_TRUE(plhp_clg.setElectricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve(curve3));

  HeatPumpPlantLoopEIRHeating plhp_htg(m);
  EXPECT_TRUE(plhp_htg.setLoadSideReferenceFlowRate(1.0));
  EXPECT_TRUE(plhp_htg.setSourceSideReferenceFlowRate(2.0));
  EXPECT_TRUE(plhp_htg.setReferenceCapacity(3.0));
  EXPECT_TRUE(plhp_htg.setReferenceCoefficientofPerformance(4.0));
  EXPECT_TRUE(plhp_htg.setSizingFactor(5.0));
  CurveBiquadratic curve4(m);
  EXPECT_TRUE(plhp_htg.setCapacityModifierFunctionofTemperatureCurve(curve4));
  CurveBiquadratic curve5(m);
  EXPECT_TRUE(plhp_htg.setElectricInputtoOutputRatioModifierFunctionofTemperatureCurve(curve5));
  CurveQuadratic curve6(m);
  EXPECT_TRUE(plhp_htg.setElectricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve(curve6));

  EXPECT_TRUE(plhp_clg.setCompanionHeatingHeatPump(plhp_htg));
  EXPECT_TRUE(plhp_htg.setCompanionCoolingHeatPump(plhp_clg));

  EXPECT_TRUE(plant_loop_plhp_clg.addSupplyBranchForComponent(plhp_clg));
  EXPECT_TRUE(plant_loop_plhp_htg.addSupplyBranchForComponent(plhp_htg));

  // Didn't connect them to the demand side of a loop, so should be AirSource
  EXPECT_EQ("AirSource", plhp_clg.condenserType());
  EXPECT_EQ("AirSource", plhp_htg.condenserType());

  openstudio::energyplus::ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  EXPECT_EQ(0u, ft.errors().size());

  {
    WorkspaceObjectVector idf_ccs(w.getObjectsByType(IddObjectType::HeatPump_PlantLoop_EIR_Cooling));
    EXPECT_EQ(1u, idf_ccs.size());
    WorkspaceObject idf_cc(idf_ccs[0]);

    EXPECT_NE("", idf_cc.getString(HeatPump_PlantLoop_EIR_CoolingFields::LoadSideInletNodeName, false).get());
    EXPECT_NE("", idf_cc.getString(HeatPump_PlantLoop_EIR_CoolingFields::LoadSideOutletNodeName, false).get());
    EXPECT_EQ("Heat Pump Plant Loop EIR Cooling 1 Inlet Node For Source Side",
              idf_cc.getString(HeatPump_PlantLoop_EIR_CoolingFields::SourceSideInletNodeName, false).get());
    EXPECT_EQ("Heat Pump Plant Loop EIR Cooling 1 Outlet Node For Source Side",
              idf_cc.getString(HeatPump_PlantLoop_EIR_CoolingFields::SourceSideOutletNodeName, false).get());
    EXPECT_EQ("AirSource", idf_cc.getString(HeatPump_PlantLoop_EIR_CoolingFields::CondenserType, false).get());
    EXPECT_EQ(plhp_htg.nameString(), idf_cc.getString(HeatPump_PlantLoop_EIR_CoolingFields::CompanionHeatPumpName).get());
    EXPECT_EQ(1.0, idf_cc.getDouble(HeatPump_PlantLoop_EIR_CoolingFields::LoadSideReferenceFlowRate, false).get());
    EXPECT_EQ(2.0, idf_cc.getDouble(HeatPump_PlantLoop_EIR_CoolingFields::SourceSideReferenceFlowRate, false).get());
    EXPECT_EQ(3.0, idf_cc.getDouble(HeatPump_PlantLoop_EIR_CoolingFields::ReferenceCapacity, false).get());
    EXPECT_EQ(4.0, idf_cc.getDouble(HeatPump_PlantLoop_EIR_CoolingFields::ReferenceCoefficientofPerformance, false).get());
    EXPECT_EQ(5.0, idf_cc.getDouble(HeatPump_PlantLoop_EIR_CoolingFields::SizingFactor, false).get());
    boost::optional<WorkspaceObject> woCurve1(idf_cc.getTarget(HeatPump_PlantLoop_EIR_CoolingFields::CapacityModifierFunctionofTemperatureCurveName));
    EXPECT_TRUE(woCurve1);
    EXPECT_EQ(woCurve1->iddObject().type(), IddObjectType::Curve_Biquadratic);
    boost::optional<WorkspaceObject> woCurve2(
      idf_cc.getTarget(HeatPump_PlantLoop_EIR_CoolingFields::ElectricInputtoOutputRatioModifierFunctionofTemperatureCurveName));
    EXPECT_TRUE(woCurve2);
    EXPECT_EQ(woCurve2->iddObject().type(), IddObjectType::Curve_Biquadratic);
    boost::optional<WorkspaceObject> woCurve3(
      idf_cc.getTarget(HeatPump_PlantLoop_EIR_CoolingFields::ElectricInputtoOutputRatioModifierFunctionofPartLoadRatioCurveName));
    EXPECT_TRUE(woCurve3);
    EXPECT_EQ(woCurve3->iddObject().type(), IddObjectType::Curve_Quadratic);
  }

  {
    WorkspaceObjectVector idf_hcs(w.getObjectsByType(IddObjectType::HeatPump_PlantLoop_EIR_Heating));
    EXPECT_EQ(1u, idf_hcs.size());
    WorkspaceObject idf_hc(idf_hcs[0]);

    EXPECT_NE("", idf_hc.getString(HeatPump_PlantLoop_EIR_HeatingFields::LoadSideInletNodeName, false).get());
    EXPECT_NE("", idf_hc.getString(HeatPump_PlantLoop_EIR_HeatingFields::LoadSideOutletNodeName, false).get());
    EXPECT_EQ("Heat Pump Plant Loop EIR Heating 1 Inlet Node For Source Side",
              idf_hc.getString(HeatPump_PlantLoop_EIR_HeatingFields::SourceSideInletNodeName, false).get());
    EXPECT_EQ("Heat Pump Plant Loop EIR Heating 1 Outlet Node For Source Side",
              idf_hc.getString(HeatPump_PlantLoop_EIR_HeatingFields::SourceSideOutletNodeName, false).get());
    EXPECT_EQ("AirSource", idf_hc.getString(HeatPump_PlantLoop_EIR_HeatingFields::CondenserType, false).get());
    EXPECT_EQ(plhp_clg.nameString(), idf_hc.getString(HeatPump_PlantLoop_EIR_HeatingFields::CompanionHeatPumpName).get());
    EXPECT_EQ(1.0, idf_hc.getDouble(HeatPump_PlantLoop_EIR_HeatingFields::LoadSideReferenceFlowRate, false).get());
    EXPECT_EQ(2.0, idf_hc.getDouble(HeatPump_PlantLoop_EIR_HeatingFields::SourceSideReferenceFlowRate, false).get());
    EXPECT_EQ(3.0, idf_hc.getDouble(HeatPump_PlantLoop_EIR_HeatingFields::ReferenceCapacity, false).get());
    EXPECT_EQ(4.0, idf_hc.getDouble(HeatPump_PlantLoop_EIR_HeatingFields::ReferenceCoefficientofPerformance, false).get());
    EXPECT_EQ(5.0, idf_hc.getDouble(HeatPump_PlantLoop_EIR_HeatingFields::SizingFactor, false).get());
    boost::optional<WorkspaceObject> woCurve1(idf_hc.getTarget(HeatPump_PlantLoop_EIR_HeatingFields::CapacityModifierFunctionofTemperatureCurveName));
    EXPECT_TRUE(woCurve1);
    EXPECT_EQ(woCurve1->iddObject().type(), IddObjectType::Curve_Biquadratic);
    boost::optional<WorkspaceObject> woCurve2(
      idf_hc.getTarget(HeatPump_PlantLoop_EIR_HeatingFields::ElectricInputtoOutputRatioModifierFunctionofTemperatureCurveName));
    EXPECT_TRUE(woCurve2);
    EXPECT_EQ(woCurve2->iddObject().type(), IddObjectType::Curve_Biquadratic);
    boost::optional<WorkspaceObject> woCurve3(
      idf_hc.getTarget(HeatPump_PlantLoop_EIR_HeatingFields::ElectricInputtoOutputRatioModifierFunctionofPartLoadRatioCurveName));
    EXPECT_TRUE(woCurve3);
    EXPECT_EQ(woCurve3->iddObject().type(), IddObjectType::Curve_Quadratic);
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_HeatPumpPlantLoopEIR_WaterSource) {

  Model m;

  PlantLoop plant_loop_cup_clg(m);
  PlantLoop plant_loop_cup_htg(m);
  PlantLoop plant_loop_plhp_clg(m);
  PlantLoop plant_loop_plhp_htg(m);

  HeatPumpPlantLoopEIRCooling plhp_clg(m);

  EXPECT_TRUE(plhp_clg.setLoadSideReferenceFlowRate(1.0));
  EXPECT_TRUE(plhp_clg.setSourceSideReferenceFlowRate(2.0));
  EXPECT_TRUE(plhp_clg.setReferenceCapacity(3.0));
  EXPECT_TRUE(plhp_clg.setReferenceCoefficientofPerformance(4.0));
  EXPECT_TRUE(plhp_clg.setSizingFactor(5.0));
  CurveBiquadratic curve1(m);
  EXPECT_TRUE(plhp_clg.setCapacityModifierFunctionofTemperatureCurve(curve1));
  CurveBiquadratic curve2(m);
  EXPECT_TRUE(plhp_clg.setElectricInputtoOutputRatioModifierFunctionofTemperatureCurve(curve2));
  CurveQuadratic curve3(m);
  EXPECT_TRUE(plhp_clg.setElectricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve(curve3));

  HeatPumpPlantLoopEIRHeating plhp_htg(m);

  EXPECT_TRUE(plhp_htg.setLoadSideReferenceFlowRate(1.0));
  EXPECT_TRUE(plhp_htg.setSourceSideReferenceFlowRate(2.0));
  EXPECT_TRUE(plhp_htg.setReferenceCapacity(3.0));
  EXPECT_TRUE(plhp_htg.setReferenceCoefficientofPerformance(4.0));
  EXPECT_TRUE(plhp_htg.setSizingFactor(5.0));
  CurveBiquadratic curve4(m);
  EXPECT_TRUE(plhp_htg.setCapacityModifierFunctionofTemperatureCurve(curve4));
  CurveBiquadratic curve5(m);
  EXPECT_TRUE(plhp_htg.setElectricInputtoOutputRatioModifierFunctionofTemperatureCurve(curve5));
  CurveQuadratic curve6(m);
  EXPECT_TRUE(plhp_htg.setElectricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve(curve6));

  EXPECT_TRUE(plhp_clg.setCompanionHeatingHeatPump(plhp_htg));
  EXPECT_TRUE(plhp_htg.setCompanionCoolingHeatPump(plhp_clg));

  EXPECT_TRUE(plant_loop_cup_clg.addDemandBranchForComponent(plhp_clg));
  EXPECT_TRUE(plant_loop_plhp_clg.addSupplyBranchForComponent(plhp_clg));
  EXPECT_TRUE(plant_loop_cup_htg.addDemandBranchForComponent(plhp_htg));
  EXPECT_TRUE(plant_loop_plhp_htg.addSupplyBranchForComponent(plhp_htg));

  // Connected them to the demand side of a loop, so should be WaterSource
  EXPECT_EQ("WaterSource", plhp_htg.condenserType());
  EXPECT_EQ("WaterSource", plhp_clg.condenserType());

  openstudio::energyplus::ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  EXPECT_EQ(0u, ft.errors().size());

  {
    WorkspaceObjectVector idf_ccs(w.getObjectsByType(IddObjectType::HeatPump_PlantLoop_EIR_Cooling));
    EXPECT_EQ(1u, idf_ccs.size());
    WorkspaceObject idf_cc(idf_ccs[0]);

    EXPECT_NE("", idf_cc.getString(HeatPump_PlantLoop_EIR_CoolingFields::LoadSideInletNodeName, false).get());
    EXPECT_NE("", idf_cc.getString(HeatPump_PlantLoop_EIR_CoolingFields::LoadSideOutletNodeName, false).get());
    EXPECT_NE("", idf_cc.getString(HeatPump_PlantLoop_EIR_CoolingFields::SourceSideInletNodeName, false).get());
    EXPECT_NE("", idf_cc.getString(HeatPump_PlantLoop_EIR_CoolingFields::SourceSideOutletNodeName, false).get());
    EXPECT_EQ("WaterSource", idf_cc.getString(HeatPump_PlantLoop_EIR_CoolingFields::CondenserType, false).get());
    EXPECT_EQ(plhp_htg.nameString(), idf_cc.getString(HeatPump_PlantLoop_EIR_CoolingFields::CompanionHeatPumpName).get());
    EXPECT_EQ(1.0, idf_cc.getDouble(HeatPump_PlantLoop_EIR_CoolingFields::LoadSideReferenceFlowRate, false).get());
    EXPECT_EQ(2.0, idf_cc.getDouble(HeatPump_PlantLoop_EIR_CoolingFields::SourceSideReferenceFlowRate, false).get());
    EXPECT_EQ(3.0, idf_cc.getDouble(HeatPump_PlantLoop_EIR_CoolingFields::ReferenceCapacity, false).get());
    EXPECT_EQ(4.0, idf_cc.getDouble(HeatPump_PlantLoop_EIR_CoolingFields::ReferenceCoefficientofPerformance, false).get());
    EXPECT_EQ(5.0, idf_cc.getDouble(HeatPump_PlantLoop_EIR_CoolingFields::SizingFactor, false).get());
    boost::optional<WorkspaceObject> woCurve1(idf_cc.getTarget(HeatPump_PlantLoop_EIR_CoolingFields::CapacityModifierFunctionofTemperatureCurveName));
    EXPECT_TRUE(woCurve1);
    EXPECT_EQ(woCurve1->iddObject().type(), IddObjectType::Curve_Biquadratic);
    boost::optional<WorkspaceObject> woCurve2(
      idf_cc.getTarget(HeatPump_PlantLoop_EIR_CoolingFields::ElectricInputtoOutputRatioModifierFunctionofTemperatureCurveName));
    EXPECT_TRUE(woCurve2);
    EXPECT_EQ(woCurve2->iddObject().type(), IddObjectType::Curve_Biquadratic);
    boost::optional<WorkspaceObject> woCurve3(
      idf_cc.getTarget(HeatPump_PlantLoop_EIR_CoolingFields::ElectricInputtoOutputRatioModifierFunctionofPartLoadRatioCurveName));
    EXPECT_TRUE(woCurve3);
    EXPECT_EQ(woCurve3->iddObject().type(), IddObjectType::Curve_Quadratic);
  }

  {
    WorkspaceObjectVector idf_hcs(w.getObjectsByType(IddObjectType::HeatPump_PlantLoop_EIR_Heating));
    EXPECT_EQ(1u, idf_hcs.size());
    WorkspaceObject idf_hc(idf_hcs[0]);

    EXPECT_NE("", idf_hc.getString(HeatPump_PlantLoop_EIR_HeatingFields::LoadSideInletNodeName, false).get());
    EXPECT_NE("", idf_hc.getString(HeatPump_PlantLoop_EIR_HeatingFields::LoadSideOutletNodeName, false).get());
    EXPECT_NE("", idf_hc.getString(HeatPump_PlantLoop_EIR_HeatingFields::SourceSideInletNodeName, false).get());
    EXPECT_NE("", idf_hc.getString(HeatPump_PlantLoop_EIR_HeatingFields::SourceSideOutletNodeName, false).get());
    EXPECT_EQ("WaterSource", idf_hc.getString(HeatPump_PlantLoop_EIR_HeatingFields::CondenserType, false).get());
    EXPECT_EQ(plhp_clg.nameString(), idf_hc.getString(HeatPump_PlantLoop_EIR_HeatingFields::CompanionHeatPumpName).get());
    EXPECT_EQ(1.0, idf_hc.getDouble(HeatPump_PlantLoop_EIR_HeatingFields::LoadSideReferenceFlowRate, false).get());
    EXPECT_EQ(2.0, idf_hc.getDouble(HeatPump_PlantLoop_EIR_HeatingFields::SourceSideReferenceFlowRate, false).get());
    EXPECT_EQ(3.0, idf_hc.getDouble(HeatPump_PlantLoop_EIR_HeatingFields::ReferenceCapacity, false).get());
    EXPECT_EQ(4.0, idf_hc.getDouble(HeatPump_PlantLoop_EIR_HeatingFields::ReferenceCoefficientofPerformance, false).get());
    EXPECT_EQ(5.0, idf_hc.getDouble(HeatPump_PlantLoop_EIR_HeatingFields::SizingFactor, false).get());
    boost::optional<WorkspaceObject> woCurve1(idf_hc.getTarget(HeatPump_PlantLoop_EIR_HeatingFields::CapacityModifierFunctionofTemperatureCurveName));
    EXPECT_TRUE(woCurve1);
    EXPECT_EQ(woCurve1->iddObject().type(), IddObjectType::Curve_Biquadratic);
    boost::optional<WorkspaceObject> woCurve2(
      idf_hc.getTarget(HeatPump_PlantLoop_EIR_HeatingFields::ElectricInputtoOutputRatioModifierFunctionofTemperatureCurveName));
    EXPECT_TRUE(woCurve2);
    EXPECT_EQ(woCurve2->iddObject().type(), IddObjectType::Curve_Biquadratic);
    boost::optional<WorkspaceObject> woCurve3(
      idf_hc.getTarget(HeatPump_PlantLoop_EIR_HeatingFields::ElectricInputtoOutputRatioModifierFunctionofPartLoadRatioCurveName));
    EXPECT_TRUE(woCurve3);
    EXPECT_EQ(woCurve3->iddObject().type(), IddObjectType::Curve_Quadratic);
  }
}
