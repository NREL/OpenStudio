/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/ConstructionWithInternalSource.hpp"
#include "../../model/ConstructionWithInternalSource_Impl.hpp"
#include "../../model/StandardOpaqueMaterial.hpp"
#include "../../model/StandardOpaqueMaterial_Impl.hpp"
#include "../../model/Construction.hpp"
#include "../../model/Construction_Impl.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/Construction_FieldEnums.hxx>
#include <utilities/idd/ConstructionProperty_InternalHeatSource_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_ConstructionWithInternalSource) {

  ForwardTranslator ft;

  Model model;

  ConstructionWithInternalSource construction(model);
  construction.setName("Construction Internal Source 1");
  construction.setTubeSpacing(0.5);
  construction.setTwoDimensionalTemperatureCalculationPosition(0.75);

  StandardOpaqueMaterial exterior(model);
  StandardOpaqueMaterial interior(model);
  EXPECT_TRUE(construction.setLayers({interior, exterior}));
  EXPECT_EQ(2u, construction.numLayers());

  ForwardTranslator trans;
  Workspace workspace = trans.translateModel(model);

  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Construction).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ConstructionProperty_InternalHeatSource).size());

  std::vector<WorkspaceObject> cs(workspace.getObjectsByType(IddObjectType::Construction));
  ASSERT_EQ(1u, cs.size());
  WorkspaceObject c(cs.at(0));
  EXPECT_EQ(2u, c.numExtensibleGroups());

  EXPECT_EQ("Construction Internal Source 1", c.getString(ConstructionFields::Name).get());

  std::vector<WorkspaceObject> cprops(workspace.getObjectsByType(IddObjectType::ConstructionProperty_InternalHeatSource));
  ASSERT_EQ(1u, cprops.size());
  WorkspaceObject cprop(cprops.at(0));

  EXPECT_EQ("Construction Internal Source 1 InternalHeatSource Property", cprop.nameString());
  ASSERT_TRUE(cprop.getTarget(ConstructionProperty_InternalHeatSourceFields::ConstructionName));
  EXPECT_EQ(c, cprop.getTarget(ConstructionProperty_InternalHeatSourceFields::ConstructionName).get());
  EXPECT_EQ(1, cprop.getInt(ConstructionProperty_InternalHeatSourceFields::ThermalSourcePresentAfterLayerNumber).get());
  EXPECT_EQ(1, cprop.getInt(ConstructionProperty_InternalHeatSourceFields::TemperatureCalculationRequestedAfterLayerNumber).get());
  EXPECT_EQ(1, cprop.getInt(ConstructionProperty_InternalHeatSourceFields::DimensionsfortheCTFCalculation).get());
  EXPECT_EQ(0.5, cprop.getDouble(ConstructionProperty_InternalHeatSourceFields::TubeSpacing).get());
  EXPECT_EQ(0.75, cprop.getDouble(ConstructionProperty_InternalHeatSourceFields::TwoDimensionalTemperatureCalculationPosition).get());
}

TEST_F(EnergyPlusFixture, ReverseTranslator_ConstructionWithInternalSource) {

  Workspace workspace(StrictnessLevel::Minimal, IddFileType::EnergyPlus);

  openstudio::IdfObject idfMaterial(openstudio::IddObjectType::Material);
  idfMaterial.setString(0, "Material 1");  // Name
  idfMaterial.setString(1, "Smooth");
  idfMaterial.setString(2, "0.012");
  idfMaterial.setString(3, "3.2");
  idfMaterial.setString(4, "2.5");
  idfMaterial.setString(5, "2.04");
  idfMaterial.setString(6, "0.8");
  idfMaterial.setString(7, "0.6");
  idfMaterial.setString(8, "0.6");
  workspace.addObject(idfMaterial);

  IdfObject idfConstruction(IddObjectType::Construction);
  idfConstruction.setString(ConstructionFields::Name, "Construction Internal Source 1");
  EXPECT_FALSE(idfConstruction.pushExtensibleGroup(StringVector(1u, idfMaterial.name().get())).empty());
  workspace.addObject(idfConstruction);

  IdfObject idfConstructionProp(IddObjectType::ConstructionProperty_InternalHeatSource);
  idfConstructionProp.setString(ConstructionProperty_InternalHeatSourceFields::Name, "Construction Internal Source 1 Property");
  idfConstructionProp.setString(ConstructionProperty_InternalHeatSourceFields::ConstructionName, "Construction Internal Source 1");
  idfConstructionProp.setDouble(ConstructionProperty_InternalHeatSourceFields::TubeSpacing, 0.5);
  idfConstructionProp.setDouble(ConstructionProperty_InternalHeatSourceFields::TwoDimensionalTemperatureCalculationPosition, 0.75);
  workspace.addObject(idfConstructionProp);

  ReverseTranslator trans;
  ASSERT_NO_THROW(trans.translateWorkspace(workspace));
  Model model = trans.translateWorkspace(workspace);

  ASSERT_EQ(0u, model.getConcreteModelObjects<Construction>().size());
  std::vector<ConstructionWithInternalSource> constructions = model.getConcreteModelObjects<ConstructionWithInternalSource>();
  ASSERT_EQ(1u, constructions.size());
  ConstructionWithInternalSource construction = constructions[0];
  EXPECT_EQ(1, construction.numLayers());
  EXPECT_EQ(1, construction.layers().size());
  EXPECT_EQ(1, construction.sourcePresentAfterLayerNumber());
  EXPECT_EQ(1, construction.temperatureCalculationRequestedAfterLayerNumber());
  EXPECT_EQ(1, construction.dimensionsForTheCTFCalculation());
  EXPECT_EQ(0.5, construction.tubeSpacing());
  EXPECT_EQ(0.75, construction.twoDimensionalTemperatureCalculationPosition());
}
