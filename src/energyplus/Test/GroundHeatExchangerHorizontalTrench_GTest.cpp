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

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/GroundHeatExchangerHorizontalTrench.hpp"
#include "../../model/GroundHeatExchangerHorizontalTrench_Impl.hpp"
#include "../../model/PlantLoop.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"

#include <utilities/idd/GroundHeatExchanger_HorizontalTrench_FieldEnums.hxx>
#include <utilities/idd/Site_GroundTemperature_Undisturbed_KusudaAchenbach_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_GroundHeatExchangerHorizontalTrench) {

  ForwardTranslator ft;

  // Create a model
  Model m;

  GroundHeatExchangerHorizontalTrench ghx(m);

  EXPECT_TRUE(ghx.setDesignFlowRate(0.05));
  EXPECT_TRUE(ghx.setTrenchLengthinPipeAxialDirection(72.0));
  EXPECT_TRUE(ghx.setNumberofTrenches(3));
  EXPECT_TRUE(ghx.setHorizontalSpacingBetweenPipes(2.1));
  EXPECT_TRUE(ghx.setPipeInnerDiameter(0.019));
  EXPECT_TRUE(ghx.setPipeOuterDiameter(0.029));
  EXPECT_TRUE(ghx.setBurialDepth(1.35));
  EXPECT_TRUE(ghx.setSoilThermalConductivity(1.02));
  EXPECT_TRUE(ghx.setSoilDensity(970.0));
  EXPECT_TRUE(ghx.setSoilSpecificHeat(2430.0));
  EXPECT_TRUE(ghx.setPipeThermalConductivity(0.32));
  EXPECT_TRUE(ghx.setPipeDensity(620.0));
  EXPECT_TRUE(ghx.setPipeSpecificHeat(2200.0));
  EXPECT_TRUE(ghx.setSoilMoistureContentPercent(32.0));
  EXPECT_TRUE(ghx.setSoilMoistureContentPercentatSaturation(53.0));
  EXPECT_TRUE(ghx.setGroundTemperatureModel("KusudaAchenbach"));
  EXPECT_TRUE(ghx.setKusudaAchenbachAverageSurfaceTemperature(16.5));
  EXPECT_TRUE(ghx.setKusudaAchenbachAverageAmplitudeofSurfaceTemperature(13.8));
  EXPECT_TRUE(ghx.setKusudaAchenbachPhaseShiftofMinimumSurfaceTemperature(18.3));
  EXPECT_TRUE(ghx.setEvapotranspirationGroundCoverParameter(0.358));

  PlantLoop p(m);
  EXPECT_TRUE(p.addSupplyBranchForComponent(ghx));

  ASSERT_TRUE(ghx.inletModelObject());
  ASSERT_TRUE(ghx.inletModelObject()->optionalCast<Node>());
  ghx.inletModelObject()->cast<Node>().setName("GHX Inlet Node");

  ASSERT_TRUE(ghx.outletModelObject());
  ASSERT_TRUE(ghx.outletModelObject()->optionalCast<Node>());
  ghx.outletModelObject()->cast<Node>().setName("GHX Outlet Node");

  const Workspace w = ft.translateModel(m);

  WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::GroundHeatExchanger_HorizontalTrench);
  ASSERT_EQ(1u, idfObjs.size());
  auto idfObject = idfObjs.front();

  EXPECT_EQ("Ground Heat Exchanger Horizontal Trench 1", idfObject.getString(GroundHeatExchanger_HorizontalTrenchFields::Name).get());
  EXPECT_EQ("GHX Inlet Node", idfObject.getString(GroundHeatExchanger_HorizontalTrenchFields::InletNodeName).get());
  EXPECT_EQ("GHX Outlet Node", idfObject.getString(GroundHeatExchanger_HorizontalTrenchFields::OutletNodeName).get());
  EXPECT_EQ(0.05, idfObject.getDouble(GroundHeatExchanger_HorizontalTrenchFields::DesignFlowRate).get());
  EXPECT_EQ(72.0, idfObject.getDouble(GroundHeatExchanger_HorizontalTrenchFields::TrenchLengthinPipeAxialDirection).get());
  EXPECT_EQ(3, idfObject.getInt(GroundHeatExchanger_HorizontalTrenchFields::NumberofTrenches).get());
  EXPECT_EQ(2.1, idfObject.getDouble(GroundHeatExchanger_HorizontalTrenchFields::HorizontalSpacingBetweenPipes).get());
  EXPECT_EQ(0.019, idfObject.getDouble(GroundHeatExchanger_HorizontalTrenchFields::PipeInnerDiameter).get());
  EXPECT_EQ(0.029, idfObject.getDouble(GroundHeatExchanger_HorizontalTrenchFields::PipeOuterDiameter).get());
  EXPECT_EQ(1.35, idfObject.getDouble(GroundHeatExchanger_HorizontalTrenchFields::BurialDepth).get());
  EXPECT_EQ(1.02, idfObject.getDouble(GroundHeatExchanger_HorizontalTrenchFields::SoilThermalConductivity).get());
  EXPECT_EQ(970.0, idfObject.getDouble(GroundHeatExchanger_HorizontalTrenchFields::SoilDensity).get());
  EXPECT_EQ(2430.0, idfObject.getDouble(GroundHeatExchanger_HorizontalTrenchFields::SoilSpecificHeat).get());
  EXPECT_EQ(0.32, idfObject.getDouble(GroundHeatExchanger_HorizontalTrenchFields::PipeThermalConductivity).get());
  EXPECT_EQ(620.0, idfObject.getDouble(GroundHeatExchanger_HorizontalTrenchFields::PipeDensity).get());
  EXPECT_EQ(2200.0, idfObject.getDouble(GroundHeatExchanger_HorizontalTrenchFields::PipeSpecificHeat).get());
  EXPECT_EQ(32.0, idfObject.getDouble(GroundHeatExchanger_HorizontalTrenchFields::SoilMoistureContentPercent).get());
  EXPECT_EQ(53.0, idfObject.getDouble(GroundHeatExchanger_HorizontalTrenchFields::SoilMoistureContentPercentatSaturation).get());

  EXPECT_EQ("Site:GroundTemperature:Undisturbed:KusudaAchenbach",
            idfObject.getString(GroundHeatExchanger_HorizontalTrenchFields::UndisturbedGroundTemperatureModelType).get());

  ASSERT_TRUE(idfObject.getTarget(GroundHeatExchanger_HorizontalTrenchFields::UndisturbedGroundTemperatureModelName));
  const WorkspaceObject kusuda = idfObject.getTarget(GroundHeatExchanger_HorizontalTrenchFields::UndisturbedGroundTemperatureModelName).get();
  EXPECT_EQ(IddObjectType{IddObjectType::Site_GroundTemperature_Undisturbed_KusudaAchenbach}, kusuda.iddObject().type());
  EXPECT_EQ(0.358, idfObject.getDouble(GroundHeatExchanger_HorizontalTrenchFields::EvapotranspirationGroundCoverParameter).get());

  EXPECT_EQ(1.02, kusuda.getDouble(Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::SoilThermalConductivity).get());
  EXPECT_EQ(970.0, kusuda.getDouble(Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::SoilDensity).get());
  EXPECT_EQ(2430.0, kusuda.getDouble(Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::SoilSpecificHeat).get());
  EXPECT_EQ(16.5, kusuda.getDouble(Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::AverageSoilSurfaceTemperature).get());
  EXPECT_EQ(13.8, kusuda.getDouble(Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::AverageAmplitudeofSurfaceTemperature).get());
  EXPECT_EQ(18.3, kusuda.getDouble(Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::PhaseShiftofMinimumSurfaceTemperature).get());
}
