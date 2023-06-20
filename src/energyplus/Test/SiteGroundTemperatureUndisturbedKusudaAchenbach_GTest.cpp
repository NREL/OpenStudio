/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/GroundHeatExchangerHorizontalTrench.hpp"
#include "../../model/GroundHeatExchangerHorizontalTrench_Impl.hpp"
#include "../../model/SiteGroundTemperatureUndisturbedKusudaAchenbach.hpp"
#include "../../model/SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl.hpp"
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

TEST_F(EnergyPlusFixture, ForwardTranslator_SiteGroundTemperatureUndisturbedKusudaAchenbach) {

  ForwardTranslator ft;

  // Create a model
  Model m;

  SiteGroundTemperatureUndisturbedKusudaAchenbach uka(m);

  EXPECT_TRUE(uka.setSoilThermalConductivity(1.1));
  EXPECT_TRUE(uka.setSoilDensity(965));
  EXPECT_TRUE(uka.setSoilSpecificHeat(2600));
  EXPECT_TRUE(uka.setAverageSoilSurfaceTemperature(16.5));
  EXPECT_TRUE(uka.setAverageAmplitudeofSurfaceTemperature(13.8));
  EXPECT_TRUE(uka.setPhaseShiftofMinimumSurfaceTemperature(18.3));

  GroundHeatExchangerHorizontalTrench ghx(m, uka);

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

  EXPECT_EQ("Site:GroundTemperature:Undisturbed:KusudaAchenbach",
            idfObject.getString(GroundHeatExchanger_HorizontalTrenchFields::UndisturbedGroundTemperatureModelType).get());

  ASSERT_TRUE(idfObject.getTarget(GroundHeatExchanger_HorizontalTrenchFields::UndisturbedGroundTemperatureModelName));
  const WorkspaceObject kusuda = idfObject.getTarget(GroundHeatExchanger_HorizontalTrenchFields::UndisturbedGroundTemperatureModelName).get();
  EXPECT_EQ(IddObjectType{IddObjectType::Site_GroundTemperature_Undisturbed_KusudaAchenbach}, kusuda.iddObject().type());

  EXPECT_EQ(1.1, kusuda.getDouble(Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::SoilThermalConductivity).get());
  EXPECT_EQ(965.0, kusuda.getDouble(Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::SoilDensity).get());
  EXPECT_EQ(2600.0, kusuda.getDouble(Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::SoilSpecificHeat).get());
  EXPECT_EQ(16.5, kusuda.getDouble(Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::AverageSoilSurfaceTemperature).get());
  EXPECT_EQ(13.8, kusuda.getDouble(Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::AverageAmplitudeofSurfaceTemperature).get());
  EXPECT_EQ(18.3, kusuda.getDouble(Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::PhaseShiftofMinimumSurfaceTemperature).get());
}
