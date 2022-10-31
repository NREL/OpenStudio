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
#include "../../model/SiteGroundTemperatureUndisturbedXing.hpp"
#include "../../model/SiteGroundTemperatureUndisturbedXing_Impl.hpp"
#include "../../model/PlantLoop.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"

#include <utilities/idd/GroundHeatExchanger_HorizontalTrench_FieldEnums.hxx>
#include <utilities/idd/Site_GroundTemperature_Undisturbed_Xing_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_SiteGroundTemperatureUndisturbedXing) {

  ForwardTranslator ft;

  // Create a model
  Model m;

  SiteGroundTemperatureUndisturbedXing ux(m);

  EXPECT_TRUE(ux.setSoilThermalConductivity(1.1));
  EXPECT_TRUE(ux.setSoilDensity(965));
  EXPECT_TRUE(ux.setSoilSpecificHeat(2600));
  EXPECT_TRUE(ux.setAverageSurfaceTemperature(16.5));
  EXPECT_TRUE(ux.setSoilSurfaceTemperatureAmplitude1(13.8));
  EXPECT_TRUE(ux.setSoilSurfaceTemperatureAmplitude2(0.8));
  EXPECT_TRUE(ux.setPhaseShiftofTemperatureAmplitude1(26));
  EXPECT_TRUE(ux.setPhaseShiftofTemperatureAmplitude2(30));

  GroundHeatExchangerHorizontalTrench ghx(m, ux);

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

  EXPECT_EQ("Site:GroundTemperature:Undisturbed:Xing",
            idfObject.getString(GroundHeatExchanger_HorizontalTrenchFields::UndisturbedGroundTemperatureModelType).get());

  ASSERT_TRUE(idfObject.getTarget(GroundHeatExchanger_HorizontalTrenchFields::UndisturbedGroundTemperatureModelName));
  const WorkspaceObject xing = idfObject.getTarget(GroundHeatExchanger_HorizontalTrenchFields::UndisturbedGroundTemperatureModelName).get();
  EXPECT_EQ(IddObjectType{IddObjectType::Site_GroundTemperature_Undisturbed_Xing}, xing.iddObject().type());
  EXPECT_EQ(0.358, idfObject.getDouble(GroundHeatExchanger_HorizontalTrenchFields::EvapotranspirationGroundCoverParameter).get());

  EXPECT_EQ(1.1, xing.getDouble(Site_GroundTemperature_Undisturbed_XingFields::SoilThermalConductivity).get());
  EXPECT_EQ(965.0, xing.getDouble(Site_GroundTemperature_Undisturbed_XingFields::SoilDensity).get());
  EXPECT_EQ(2600.0, xing.getDouble(Site_GroundTemperature_Undisturbed_XingFields::SoilSpecificHeat).get());
  EXPECT_EQ(16.5, xing.getDouble(Site_GroundTemperature_Undisturbed_XingFields::AverageSoilSurfaceTemperature).get());
  EXPECT_EQ(13.8, xing.getDouble(Site_GroundTemperature_Undisturbed_XingFields::SoilSurfaceTemperatureAmplitude1).get());
  EXPECT_EQ(0.8, xing.getDouble(Site_GroundTemperature_Undisturbed_XingFields::SoilSurfaceTemperatureAmplitude2).get());
  EXPECT_EQ(26, xing.getDouble(Site_GroundTemperature_Undisturbed_XingFields::PhaseShiftofTemperatureAmplitude1).get());
  EXPECT_EQ(30, xing.getDouble(Site_GroundTemperature_Undisturbed_XingFields::PhaseShiftofTemperatureAmplitude2).get());
}
