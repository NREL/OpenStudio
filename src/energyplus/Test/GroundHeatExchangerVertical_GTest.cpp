/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/GroundHeatExchangerVertical.hpp"
#include "../../model/GroundHeatExchangerVertical_Impl.hpp"
#include "../../model/PlantLoop.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"

#include <utilities/idd/GroundHeatExchanger_System_FieldEnums.hxx>
#include <utilities/idd/Site_GroundTemperature_Undisturbed_KusudaAchenbach_FieldEnums.hxx>
#include <utilities/idd/GroundHeatExchanger_ResponseFactors_FieldEnums.hxx>
#include <utilities/idd/GroundHeatExchanger_Vertical_Properties_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_GroundHeatExchangerVertical) {

  ForwardTranslator ft;

  // Create a model
  Model m;

  GroundHeatExchangerVertical ghx(m);

  EXPECT_TRUE(ghx.setDesignFlowRate(0.004));
  EXPECT_TRUE(ghx.setNumberofBoreHoles(100));
  EXPECT_TRUE(ghx.setBoreHoleTopDepth(1.35));
  EXPECT_TRUE(ghx.setBoreHoleLength(80.0));
  EXPECT_TRUE(ghx.setBoreHoleRadius(0.7E-01));
  EXPECT_TRUE(ghx.setGroundThermalConductivity(0.7));
  EXPECT_TRUE(ghx.setGroundThermalHeatCapacity(0.3E+07));
  EXPECT_TRUE(ghx.setGroundTemperature(14.0));
  EXPECT_TRUE(ghx.setGroutThermalConductivity(0.7));
  EXPECT_TRUE(ghx.setPipeThermalConductivity(0.4));
  EXPECT_TRUE(ghx.setPipeOutDiameter(2.7E-02));
  EXPECT_TRUE(ghx.setUTubeDistance(2.6E-02));
  EXPECT_TRUE(ghx.setPipeThickness(2.5E-03));
  EXPECT_TRUE(ghx.setMaximumLengthofSimulation(3));
  EXPECT_TRUE(ghx.setGFunctionReferenceRatio(0.001));

  PlantLoop p(m);
  EXPECT_TRUE(p.addSupplyBranchForComponent(ghx));

  ASSERT_TRUE(ghx.inletModelObject());
  ASSERT_TRUE(ghx.inletModelObject()->optionalCast<Node>());
  ghx.inletModelObject()->cast<Node>().setName("GHX Inlet Node");

  ASSERT_TRUE(ghx.outletModelObject());
  ASSERT_TRUE(ghx.outletModelObject()->optionalCast<Node>());
  ghx.outletModelObject()->cast<Node>().setName("GHX Outlet Node");

  const Workspace w = ft.translateModel(m);

  WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::GroundHeatExchanger_System);
  ASSERT_EQ(1u, idfObjs.size());
  auto idfObject = idfObjs.front();

  EXPECT_EQ("Ground Heat Exchanger Vertical 1", idfObject.getString(GroundHeatExchanger_SystemFields::Name).get());
  EXPECT_EQ("GHX Inlet Node", idfObject.getString(GroundHeatExchanger_SystemFields::InletNodeName).get());
  EXPECT_EQ("GHX Outlet Node", idfObject.getString(GroundHeatExchanger_SystemFields::OutletNodeName).get());
  EXPECT_EQ(0.004, idfObject.getDouble(GroundHeatExchanger_SystemFields::DesignFlowRate).get());

  EXPECT_EQ("Site:GroundTemperature:Undisturbed:KusudaAchenbach",
            idfObject.getString(GroundHeatExchanger_SystemFields::UndisturbedGroundTemperatureModelType).get());

  ASSERT_TRUE(idfObject.getTarget(GroundHeatExchanger_SystemFields::UndisturbedGroundTemperatureModelName));
  const WorkspaceObject kusuda = idfObject.getTarget(GroundHeatExchanger_SystemFields::UndisturbedGroundTemperatureModelName).get();
  EXPECT_EQ(IddObjectType{IddObjectType::Site_GroundTemperature_Undisturbed_KusudaAchenbach}, kusuda.iddObject().type());

  EXPECT_EQ(0.7, idfObject.getDouble(GroundHeatExchanger_SystemFields::GroundThermalConductivity).get());
  EXPECT_EQ(0.3E+07, idfObject.getDouble(GroundHeatExchanger_SystemFields::GroundThermalHeatCapacity).get());

  ASSERT_TRUE(idfObject.getTarget(GroundHeatExchanger_SystemFields::GHE_Vertical_ResponseFactorsObjectName));
  const WorkspaceObject response = idfObject.getTarget(GroundHeatExchanger_SystemFields::GHE_Vertical_ResponseFactorsObjectName).get();
  EXPECT_EQ(IddObjectType{IddObjectType::GroundHeatExchanger_ResponseFactors}, response.iddObject().type());

  EXPECT_TRUE(idfObject.isEmpty(GroundHeatExchanger_SystemFields::gFunctionCalculationMethod));
  ASSERT_FALSE(idfObject.getTarget(GroundHeatExchanger_SystemFields::GHE_Vertical_ArrayObjectName));

  EXPECT_EQ(0.692626, kusuda.getDouble(Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::SoilThermalConductivity).get());
  EXPECT_EQ(920.0, kusuda.getDouble(Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::SoilDensity).get());
  EXPECT_DOUBLE_EQ(0.234700E+07 / 920.0, kusuda.getDouble(Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::SoilSpecificHeat).get());
  EXPECT_EQ(13.375, kusuda.getDouble(Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::AverageSoilSurfaceTemperature).get());
  EXPECT_EQ(3.2, kusuda.getDouble(Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::AverageAmplitudeofSurfaceTemperature).get());
  EXPECT_EQ(8.0, kusuda.getDouble(Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::PhaseShiftofMinimumSurfaceTemperature).get());

  ASSERT_TRUE(response.getTarget(GroundHeatExchanger_ResponseFactorsFields::GHE_Vertical_PropertiesObjectName));
  const WorkspaceObject properties = response.getTarget(GroundHeatExchanger_ResponseFactorsFields::GHE_Vertical_PropertiesObjectName).get();
  EXPECT_EQ(IddObjectType{IddObjectType::GroundHeatExchanger_Vertical_Properties}, properties.iddObject().type());

  EXPECT_EQ(100, response.getDouble(GroundHeatExchanger_ResponseFactorsFields::NumberofBoreholes).get());
  EXPECT_EQ(0.001, response.getDouble(GroundHeatExchanger_ResponseFactorsFields::GFunctionReferenceRatio).get());

  EXPECT_EQ(1.35, properties.getDouble(GroundHeatExchanger_Vertical_PropertiesFields::DepthofTopofBorehole).get());
  EXPECT_EQ(80.0, properties.getDouble(GroundHeatExchanger_Vertical_PropertiesFields::BoreholeLength).get());
  EXPECT_EQ(0.7E-01 * 2, properties.getDouble(GroundHeatExchanger_Vertical_PropertiesFields::BoreholeDiameter).get());
  EXPECT_EQ(0.7, properties.getDouble(GroundHeatExchanger_Vertical_PropertiesFields::GroutThermalConductivity).get());
  EXPECT_EQ(3.90E+06, properties.getDouble(GroundHeatExchanger_Vertical_PropertiesFields::GroutThermalHeatCapacity).get());
  EXPECT_EQ(0.4, properties.getDouble(GroundHeatExchanger_Vertical_PropertiesFields::PipeThermalConductivity).get());
  EXPECT_EQ(1.77E+06, properties.getDouble(GroundHeatExchanger_Vertical_PropertiesFields::PipeThermalHeatCapacity).get());
  EXPECT_EQ(2.7E-02, properties.getDouble(GroundHeatExchanger_Vertical_PropertiesFields::PipeOuterDiameter).get());
  EXPECT_EQ(2.5E-03, properties.getDouble(GroundHeatExchanger_Vertical_PropertiesFields::PipeThickness).get());
  EXPECT_EQ(2.6E-02, properties.getDouble(GroundHeatExchanger_Vertical_PropertiesFields::UTubeDistance).get());
}
