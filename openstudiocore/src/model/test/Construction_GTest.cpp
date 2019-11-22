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

#include "ModelFixture.hpp"
#include "../Model.hpp"
#include "../Model_Impl.hpp"
#include "../Building.hpp"
#include "../Building_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../Component.hpp"
#include "../CFactorUndergroundWallConstruction.hpp"
#include "../CFactorUndergroundWallConstruction_Impl.hpp"
#include "../Construction.hpp"
#include "../Construction_Impl.hpp"
#include "../DefaultConstructionSet.hpp"
#include "../DefaultConstructionSet_Impl.hpp"
#include "../DefaultSurfaceConstructions.hpp"
#include "../DefaultSurfaceConstructions_Impl.hpp"
#include "../DefaultSubSurfaceConstructions.hpp"
#include "../DefaultSubSurfaceConstructions_Impl.hpp"
#include "../ConstructionWithInternalSource.hpp"
#include "../ConstructionWithInternalSource_Impl.hpp"
#include "../FFactorGroundFloorConstruction.hpp"
#include "../FFactorGroundFloorConstruction_Impl.hpp"
#include "../WindowDataFile.hpp"
#include "../WindowDataFile_Impl.hpp"
#include "../StandardsInformationConstruction.hpp"
#include "../StandardsInformationConstruction_Impl.hpp"

#include "../Material.hpp"
#include "../Material_Impl.hpp"
#include "../AirGap.hpp"
#include "../AirWallMaterial.hpp"
#include "../StandardOpaqueMaterial.hpp"
#include "../StandardOpaqueMaterial_Impl.hpp"
#include "../StandardGlazing.hpp"
#include "../Space.hpp"
#include "../Space_Impl.hpp"
#include "../Surface.hpp"
#include "../Surface_Impl.hpp"
#include "../SubSurface.hpp"
#include "../SubSurface_Impl.hpp"
#include "../LifeCycleCost.hpp"

#include "../../utilities/geometry/Point3d.hpp"
#include "../../utilities/idf/IdfFile.hpp"
#include <utilities/idd/OS_Construction_FieldEnums.hxx>
#include <utilities/idd/OS_Material_FieldEnums.hxx>
#include <utilities/idd/OS_Material_AirGap_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, Construction_Layers)
{
  Model model;

  // Create some materials
  StandardOpaqueMaterial exterior(model);
  AirGap air(model);
  StandardOpaqueMaterial interior(model);

  OpaqueMaterialVector layers;
  layers.push_back(exterior);
  layers.push_back(air);
  layers.push_back(interior);

  Construction construction(layers);

  // Get layers
  MaterialVector testLayers = construction.layers();
  ASSERT_EQ(static_cast<unsigned>(3),testLayers.size());
  EXPECT_TRUE(testLayers[0] == exterior);
  EXPECT_TRUE(testLayers[1] == air);
  EXPECT_TRUE(testLayers[2] == interior);

  // construct by clone
  Model modelClone = model.clone().cast<Model>();
  EXPECT_EQ(static_cast<unsigned>(4),modelClone.objects().size());
  ConstructionVector constructions = modelClone.getModelObjects<Construction>();
  ASSERT_EQ(static_cast<unsigned>(1),constructions.size());
  construction = constructions[0];
  EXPECT_FALSE(construction.model() == model);
  EXPECT_TRUE(construction.iddObject().type() ==
              IddObjectType::OS_Construction);
}

TEST_F(ModelFixture, Construction_SetLayers)
{
  Model model;

  // Create some materials
  StandardOpaqueMaterial exterior(model);
  AirGap air(model);
  StandardOpaqueMaterial interior(model);

  MaterialVector layers;
  layers.push_back(exterior);
  layers.push_back(air);
  layers.push_back(interior);

  Construction construction(model);
  EXPECT_TRUE(construction.setLayers(layers));

  // Get layers
  MaterialVector testLayers = construction.layers();
  ASSERT_EQ(static_cast<unsigned>(3),testLayers.size());
  EXPECT_TRUE(testLayers[0] == exterior);
  EXPECT_TRUE(testLayers[1] == air);
  EXPECT_TRUE(testLayers[2] == interior);
}

TEST_F(ModelFixture, Construction_InsertLayers)
{
  Model model;

  // Create some materials
  StandardOpaqueMaterial exterior(model);
  AirGap air(model);
  StandardOpaqueMaterial interior(model);

  Construction construction(model);
  try{
    // these functions should not throw, just return false
    EXPECT_TRUE(construction.insertLayer(0, exterior));
    EXPECT_TRUE(construction.insertLayer(1, air));
    EXPECT_TRUE(construction.insertLayer(2, interior));
  }catch(const std::exception&){

  }

  // Get layers
  MaterialVector testLayers = construction.layers();
  ASSERT_EQ(static_cast<unsigned>(3),testLayers.size());
  EXPECT_TRUE(testLayers[0] == exterior);
  EXPECT_TRUE(testLayers[1] == air);
  EXPECT_TRUE(testLayers[2] == interior);
}


TEST_F(ModelFixture, ConstructionWithInternalSource_Layers)
{
  Model model;

  // Create some materials
  StandardOpaqueMaterial exterior(model);
  AirGap air(model);
  StandardOpaqueMaterial interior(model);

  MaterialVector layers;
  layers.push_back(exterior);
  layers.push_back(air);
  layers.push_back(interior);

  ConstructionWithInternalSource construction(model);
  EXPECT_TRUE(construction.setLayers(layers));

  // Get layers
  MaterialVector testLayers = construction.layers();
  ASSERT_EQ(static_cast<unsigned>(3),testLayers.size());
  EXPECT_TRUE(testLayers[0] == exterior);
  EXPECT_TRUE(testLayers[1] == air);
  EXPECT_TRUE(testLayers[2] == interior);
}


TEST_F(ModelFixture,CFactorUndergroundWallConstruction)
{
  // construct from scratch
  Model model;
  CFactorUndergroundWallConstruction construction(model);
  EXPECT_EQ(static_cast<unsigned>(1),model.objects().size());
  CFactorUndergroundWallConstructionVector constructions =
      model.getModelObjects<CFactorUndergroundWallConstruction>();
  ASSERT_EQ(static_cast<unsigned>(1),constructions.size());
  EXPECT_TRUE(construction == constructions[0]);
  EXPECT_TRUE(construction.iddObject().type() ==
              IddObjectType::OS_Construction_CfactorUndergroundWall);

  // construct by clone
  Model modelClone = model.clone().cast<Model>();
  EXPECT_EQ(static_cast<unsigned>(1),modelClone.objects().size());
  constructions = modelClone.getModelObjects<CFactorUndergroundWallConstruction>();
  ASSERT_EQ(static_cast<unsigned>(1),constructions.size());
  construction = constructions[0];
  EXPECT_FALSE(construction.model() == model);
  EXPECT_TRUE(construction.iddObject().type() ==
              IddObjectType::OS_Construction_CfactorUndergroundWall);
}

TEST_F(ModelFixture,FFactorGroundFloorConstruction)
{
  // construct from scratch
  Model model;
  FFactorGroundFloorConstruction construction(model);
  EXPECT_EQ(static_cast<unsigned>(1),model.objects().size());
  FFactorGroundFloorConstructionVector constructions =
      model.getModelObjects<FFactorGroundFloorConstruction>();
  ASSERT_EQ(static_cast<unsigned>(1),constructions.size());
  EXPECT_TRUE(construction == constructions[0]);
  EXPECT_TRUE(construction.iddObject().type() ==
              IddObjectType::OS_Construction_FfactorGroundFloor);

  // construct by clone
  Model modelClone = model.clone().cast<Model>();
  EXPECT_EQ(static_cast<unsigned>(1),modelClone.objects().size());
  constructions = modelClone.getModelObjects<FFactorGroundFloorConstruction>();
  ASSERT_EQ(static_cast<unsigned>(1),constructions.size());
  construction = constructions[0];
  EXPECT_FALSE(construction.model() == model);
  EXPECT_TRUE(construction.iddObject().type() ==
              IddObjectType::OS_Construction_FfactorGroundFloor);
}

TEST_F(ModelFixture,WindowDataFile)
{
  // construct from scratch
  Model model;
  WindowDataFile construction(model);
  EXPECT_EQ(static_cast<unsigned>(1),model.objects().size());
  WindowDataFileVector constructions =
      model.getModelObjects<WindowDataFile>();
  ASSERT_EQ(static_cast<unsigned>(1),constructions.size());
  EXPECT_TRUE(construction == constructions[0]);
  EXPECT_TRUE(construction.iddObject().type() == IddObjectType::OS_Construction_WindowDataFile);

  // construct by clone
  Model modelClone = model.clone().cast<Model>();
  EXPECT_EQ(static_cast<unsigned>(1),modelClone.objects().size());
  constructions = modelClone.getModelObjects<WindowDataFile>();
  ASSERT_EQ(static_cast<unsigned>(1),constructions.size());
  construction = constructions[0];
  EXPECT_FALSE(construction.model() == model);
  EXPECT_TRUE(construction.iddObject().type() == IddObjectType::OS_Construction_WindowDataFile);
}

TEST_F(ModelFixture, Construction_AddObjects) {
  IdfFile idfFile; // OpenStudio Idd is default

  // Can still use name references directly in IdfObjects. Will get turned into pointer/by handle
  // references on import into Workspace.
  IdfObject object(IddObjectType::OS_Construction);
  object.setName("Exterior Wall");
  unsigned index = OS_ConstructionFields::SurfaceRenderingName + 1;
  object.setString(index,"M01 100mm brick"); ++index;
  object.setString(index,"M15 200mm heavyweight concrete"); ++index;
  object.setString(index,"I02 50mm insulation board"); ++index;
  object.setString(index,"F04 Wall air space resistance"); ++index;
  object.setString(index,"G01a 19mm gypsum board"); ++index;
  idfFile.addObject(object);

  object = IdfObject(IddObjectType::OS_Material);
  object.setName("M01 100mm brick");
  object.setString(OS_MaterialFields::Roughness,"MediumRough");
  object.setDouble(OS_MaterialFields::Thickness,0.1016);
  object.setDouble(OS_MaterialFields::Conductivity,0.89);
  object.setDouble(OS_MaterialFields::Density,1920.0);
  object.setDouble(OS_MaterialFields::SpecificHeat,790.0);
  idfFile.addObject(object);

  object = IdfObject(IddObjectType::OS_Material);
  object.setName("M15 200mm heavyweight concrete");
  object.setString(OS_MaterialFields::Roughness,"MediumRough");
  object.setDouble(OS_MaterialFields::Thickness,0.2032);
  object.setDouble(OS_MaterialFields::Conductivity,1.95);
  object.setDouble(OS_MaterialFields::Density,2240.0);
  object.setDouble(OS_MaterialFields::SpecificHeat,900.0);
  idfFile.addObject(object);

  object = IdfObject(IddObjectType::OS_Material);
  object.setName("I02 50mm insulation board");
  object.setString(OS_MaterialFields::Roughness,"MediumRough");
  object.setDouble(OS_MaterialFields::Thickness,0.0508);
  object.setDouble(OS_MaterialFields::Conductivity,0.03);
  object.setDouble(OS_MaterialFields::Density,43.0);
  object.setDouble(OS_MaterialFields::SpecificHeat,1210.0);
  idfFile.addObject(object);

  object = IdfObject(IddObjectType::OS_Material_AirGap);
  object.setName("F04 Wall air space resistance");
  object.setDouble(OS_Material_AirGapFields::ThermalResistance,0.15);
  idfFile.addObject(object);

  object = IdfObject(IddObjectType::OS_Material);
  object.setName("G01a 19mm gypsum board");
  object.setString(OS_MaterialFields::Roughness,"MediumSmooth");
  object.setDouble(OS_MaterialFields::Thickness,0.019);
  object.setDouble(OS_MaterialFields::Conductivity,0.16);
  object.setDouble(OS_MaterialFields::Density,800.0);
  object.setDouble(OS_MaterialFields::SpecificHeat,1090.0);
  idfFile.addObject(object);

  // first add to a workspace
  Workspace workspace(StrictnessLevel::Draft, IddFileType::OpenStudio);
  workspace.addObjects(idfFile.objects());
  EXPECT_EQ(workspace.numObjects(), idfFile.numObjects());
  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::OS_Construction).size());
  WorkspaceObject workspaceObject = workspace.getObjectsByType(IddObjectType::OS_Construction)[0];
  ASSERT_EQ(8u, workspaceObject.numFields());
  for (int i = 3; i < 8; ++i){
    EXPECT_FALSE(workspaceObject.isEmpty(i)) << "Index " << i << " is empty for:" << std::endl << workspaceObject;
  }

  // now add to a model
  Model model;
  model.addObjects(idfFile.objects());
  EXPECT_EQ(model.numObjects(), idfFile.numObjects());
  ASSERT_EQ(1u, model.getModelObjects<Construction>().size());
  Construction construction = model.getModelObjects<Construction>()[0];
  ASSERT_EQ(8u, construction.numFields());
  for (int i = 3; i < 8; ++i){
    EXPECT_FALSE(construction.isEmpty(i)) << "Index " << i << " is empty for:" << std::endl << construction;
  }
  ASSERT_EQ(5u, construction.layers().size());
  std::vector<Material> layers = construction.layers();
  for (int i = 0; i < 5; ++i){
    EXPECT_TRUE(layers[i].name()) << "Layer " << i << " has no name:" << std::endl << layers[i];
  }

}


TEST_F(ModelFixture, Construction_Clone)
{
  Model library;

  // Create some materials
  StandardOpaqueMaterial exterior(library);
  AirGap air(library);
  StandardOpaqueMaterial interior(library);

  OpaqueMaterialVector layers;
  layers.push_back(exterior);
  layers.push_back(air);
  layers.push_back(interior);

  EXPECT_EQ(static_cast<unsigned>(3), library.getModelObjects<Material>().size());

  Construction construction(layers);
  ASSERT_EQ(static_cast<unsigned>(3), construction.layers().size());

  // Clone into same model
  ModelObject clone = construction.clone(library);

  // Material ResourceObject instances are shared resources so they have not been cloned
  EXPECT_EQ(static_cast<unsigned>(3), library.getModelObjects<Material>().size());

  // New handle for cloned construction
  EXPECT_FALSE(clone.handle() == construction.handle());
  ASSERT_TRUE(clone.optionalCast<Construction>());

  ASSERT_EQ(static_cast<unsigned>(3), clone.cast<Construction>().layers().size());

  // Clone into a differnt model
  Model model;

  auto clone2 = construction.clone(model).cast<Construction>();
  EXPECT_EQ(static_cast<unsigned>(3), model.getModelObjects<Material>().size());

  EXPECT_EQ(static_cast<unsigned>(1), model.getModelObjects<Construction>().size());

  // Make sure materials are still hooked up
  ASSERT_EQ(static_cast<unsigned>(3), clone2.cast<Construction>().layers().size());

  // Clone again
  auto clone3 = construction.clone(model).cast<Construction>();
  EXPECT_EQ(static_cast<unsigned>(3), model.getModelObjects<Material>().size());

  EXPECT_EQ(static_cast<unsigned>(2), model.getModelObjects<Construction>().size());

  // Make sure materials are still hooked up
  ASSERT_EQ(static_cast<unsigned>(3), clone3.cast<Construction>().layers().size());

  EXPECT_FALSE(clone2.handle() == clone3.handle());
}

TEST_F(ModelFixture, DuplicateMaterialName)
{
  Model model;

  StandardOpaqueMaterial material(model);
  EXPECT_TRUE(material.setName("OS:Material:AirWall 1"));

  AirWallMaterial airWall(model);
  EXPECT_NE(airWall.name().get(), material.name().get());
}

TEST_F(ModelFixture, Construction_SetUFactor) {
  Model model = exampleModel();

  OptionalConstruction tempC = model.getModelObjectByName<Construction>("Exterior Wall");
  ASSERT_TRUE(tempC);
  Construction construction(*tempC);
  OptionalOpaqueMaterial tempM = construction.insulation();
  ASSERT_TRUE(tempM);
  StandardOpaqueMaterial insulation = tempM->cast<StandardOpaqueMaterial>();
  // should not be able to make insulation thickness go < 0
  double originalResistance = 1.0 / construction.thermalConductance().get();
  double insulationResistance = insulation.thermalResistance();
  double newResistance = 0.8 * (originalResistance - insulationResistance);
  bool result = construction.setConductance(1.0 / newResistance);
  EXPECT_FALSE(result);
  if (result) {
    EXPECT_TRUE(insulation.thickness() < 0.0);
  }
  else {
    EXPECT_TRUE(insulation.thickness() > 0.0);
  }
  LOG(Trace,"Construction where tried to set thermal conductance too high:" << std::endl
      << construction.createComponent());
}

TEST_F(ModelFixture, Construction_NetArea) {
  Model model;

  Construction construction1(model);
  Construction construction2(model);
  EXPECT_DOUBLE_EQ(0.0, construction1.getNetArea());
  EXPECT_DOUBLE_EQ(0.0, construction2.getNetArea());

  // add costs
  boost::optional<LifeCycleCost> cost1 = LifeCycleCost::createLifeCycleCost("Brick", construction1, 3.0, "CostPerArea", "Construction");
  ASSERT_TRUE(cost1);
  boost::optional<LifeCycleCost> cost2 = LifeCycleCost::createLifeCycleCost("Glass", construction2, 5.0, "CostPerArea", "Construction");
  ASSERT_TRUE(cost2);
  EXPECT_DOUBLE_EQ(0, cost1->totalCost());
  EXPECT_DOUBLE_EQ(0, cost2->totalCost());

  Building building = model.getUniqueModelObject<Building>();
  DefaultSurfaceConstructions defaultExteriorSurfaceConstructions(model);
  DefaultSurfaceConstructions defaultInteriorSurfaceConstructions(model);
  DefaultSubSurfaceConstructions defaultExteriorSubSurfaceConstructions(model);
  DefaultSubSurfaceConstructions defaultInteriorSubSurfaceConstructions(model);
  DefaultConstructionSet defaultConstructionSet(model);
  defaultConstructionSet.setDefaultExteriorSurfaceConstructions(defaultExteriorSurfaceConstructions);
  defaultConstructionSet.setDefaultInteriorSurfaceConstructions(defaultInteriorSurfaceConstructions);
  defaultConstructionSet.setDefaultExteriorSubSurfaceConstructions(defaultExteriorSubSurfaceConstructions);
  defaultConstructionSet.setDefaultInteriorSubSurfaceConstructions(defaultInteriorSubSurfaceConstructions);
  building.setDefaultConstructionSet(defaultConstructionSet);

  Space space(model);

  Point3dVector points;
  points.push_back(Point3d(0, 0, 1));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(0, 1, 1));

  Surface surface1(points, model);
  surface1.setSpace(space);
  EXPECT_EQ("Wall", surface1.surfaceType());
  EXPECT_EQ("Outdoors", surface1.outsideBoundaryCondition());
  EXPECT_DOUBLE_EQ(1.0, surface1.netArea());

  points.clear();
  points.push_back(Point3d(0, 1, 1));
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(0, 0, 1));

  Surface surface2(points, model);
  surface2.setSpace(space);
  EXPECT_EQ("Wall", surface2.surfaceType());
  EXPECT_EQ("Outdoors", surface2.outsideBoundaryCondition());
  EXPECT_DOUBLE_EQ(1.0, surface2.netArea());

  EXPECT_DOUBLE_EQ(0.0, construction1.getNetArea());
  EXPECT_DOUBLE_EQ(0.0, construction2.getNetArea());
  EXPECT_DOUBLE_EQ(0, cost1->totalCost());
  EXPECT_DOUBLE_EQ(0, cost2->totalCost());

  defaultExteriorSurfaceConstructions.setWallConstruction(construction1);
  ASSERT_TRUE(surface1.construction());
  ASSERT_TRUE(surface2.construction());
  EXPECT_EQ(surface1.construction()->handle(), construction1.handle());
  EXPECT_EQ(surface2.construction()->handle(), construction1.handle());
  EXPECT_DOUBLE_EQ(2.0, construction1.getNetArea());
  EXPECT_DOUBLE_EQ(0.0, construction2.getNetArea());
  EXPECT_DOUBLE_EQ(6.0, cost1->totalCost());
  EXPECT_DOUBLE_EQ(0, cost2->totalCost());

  surface2.setConstruction(construction2);
  ASSERT_TRUE(surface1.construction());
  ASSERT_TRUE(surface2.construction());
  EXPECT_EQ(surface1.construction()->handle(), construction1.handle());
  EXPECT_EQ(surface2.construction()->handle(), construction2.handle());
  EXPECT_DOUBLE_EQ(1.0, construction1.getNetArea());
  EXPECT_DOUBLE_EQ(1.0, construction2.getNetArea());
  EXPECT_DOUBLE_EQ(3.0, cost1->totalCost());
  EXPECT_DOUBLE_EQ(5.0, cost2->totalCost());

  surface1.setAdjacentSurface(surface2);
  ASSERT_TRUE(surface1.construction());
  ASSERT_TRUE(surface2.construction());
  EXPECT_EQ(surface1.construction()->handle(), construction2.handle());
  EXPECT_EQ(surface2.construction()->handle(), construction2.handle());
  EXPECT_DOUBLE_EQ(0.0, construction1.getNetArea());
  EXPECT_DOUBLE_EQ(1.0, construction2.getNetArea());
  EXPECT_DOUBLE_EQ(0, cost1->totalCost());
  EXPECT_DOUBLE_EQ(5.0, cost2->totalCost());

  defaultInteriorSurfaceConstructions.setWallConstruction(construction1);
  ASSERT_TRUE(surface1.construction());
  ASSERT_TRUE(surface2.construction());
  EXPECT_EQ(surface1.construction()->handle(), construction2.handle());
  EXPECT_EQ(surface2.construction()->handle(), construction2.handle());
  EXPECT_DOUBLE_EQ(0.0, construction1.getNetArea());
  EXPECT_DOUBLE_EQ(1.0, construction2.getNetArea());
  EXPECT_DOUBLE_EQ(0, cost1->totalCost());
  EXPECT_DOUBLE_EQ(5.0, cost2->totalCost());

  surface2.resetConstruction();
  ASSERT_TRUE(surface1.construction());
  ASSERT_TRUE(surface2.construction());
  EXPECT_EQ(surface1.construction()->handle(), construction1.handle());
  EXPECT_EQ(surface2.construction()->handle(), construction1.handle());
  EXPECT_DOUBLE_EQ(1.0, construction1.getNetArea());
  EXPECT_DOUBLE_EQ(0.0, construction2.getNetArea());
  EXPECT_DOUBLE_EQ(3.0, cost1->totalCost());
  EXPECT_DOUBLE_EQ(0, cost2->totalCost());

  points.clear();
  points.push_back(Point3d(0, 0.75, 0.5));
  points.push_back(Point3d(0, 0.75, 0));
  points.push_back(Point3d(0, 0.25, 0));
  points.push_back(Point3d(0, 0.25, 0.5));

  SubSurface subSurface1(points, model);
  subSurface1.setSurface(surface1);
  EXPECT_FALSE(subSurface1.construction());
  EXPECT_EQ("Door", subSurface1.subSurfaceType());
  EXPECT_DOUBLE_EQ(0.25, subSurface1.netArea());
  EXPECT_DOUBLE_EQ(0.75, surface1.netArea());

  points.clear();
  points.push_back(Point3d(0, 0.25, 0.5));
  points.push_back(Point3d(0, 0.25, 0));
  points.push_back(Point3d(0, 0.75, 0));
  points.push_back(Point3d(0, 0.75, 0.5));

  SubSurface subSurface2(points, model);
  subSurface2.setSurface(surface2);
  EXPECT_FALSE(subSurface2.construction());
  EXPECT_EQ("Door", subSurface2.subSurfaceType());
  EXPECT_DOUBLE_EQ(0.25, subSurface2.netArea());
  EXPECT_DOUBLE_EQ(0.75, surface2.netArea());

  EXPECT_TRUE(subSurface2.setAdjacentSubSurface(subSurface1));

  EXPECT_DOUBLE_EQ(0.75, construction1.getNetArea());
  EXPECT_DOUBLE_EQ(0.0, construction2.getNetArea());
  EXPECT_DOUBLE_EQ(2.25, cost1->totalCost());
  EXPECT_DOUBLE_EQ(0.0, cost2->totalCost());

  subSurface1.setConstruction(construction2);
  ASSERT_TRUE(subSurface1.construction());
  ASSERT_TRUE(subSurface2.construction());
  EXPECT_EQ(subSurface1.construction()->handle(), construction2.handle());
  EXPECT_EQ(subSurface2.construction()->handle(), construction2.handle());

  EXPECT_DOUBLE_EQ(0.75, construction1.getNetArea());
  EXPECT_DOUBLE_EQ(0.25, construction2.getNetArea());
  EXPECT_DOUBLE_EQ(2.25, cost1->totalCost());
  EXPECT_DOUBLE_EQ(1.25, cost2->totalCost());

  subSurface1.resetConstruction();
  EXPECT_FALSE(subSurface1.construction());
  EXPECT_FALSE(subSurface2.construction());

  EXPECT_DOUBLE_EQ(0.75, construction1.getNetArea());
  EXPECT_DOUBLE_EQ(0.0, construction2.getNetArea());
  EXPECT_DOUBLE_EQ(2.25, cost1->totalCost());
  EXPECT_DOUBLE_EQ(0.0, cost2->totalCost());

  defaultInteriorSubSurfaceConstructions.setDoorConstruction(construction2);
  ASSERT_TRUE(subSurface1.construction());
  ASSERT_TRUE(subSurface2.construction());
  EXPECT_EQ(subSurface1.construction()->handle(), construction2.handle());
  EXPECT_EQ(subSurface2.construction()->handle(), construction2.handle());
  EXPECT_DOUBLE_EQ(0.75, construction1.getNetArea());
  EXPECT_DOUBLE_EQ(0.25, construction2.getNetArea());
  EXPECT_DOUBLE_EQ(2.25, cost1->totalCost());
  EXPECT_DOUBLE_EQ(1.25, cost2->totalCost());

  // now test multipliers
  ThermalZone thermalZone(model);
  thermalZone.setMultiplier(10.0);
  space.setThermalZone(thermalZone);
  EXPECT_DOUBLE_EQ(10.0, space.multiplier());
  EXPECT_DOUBLE_EQ(7.5, construction1.getNetArea());
  EXPECT_DOUBLE_EQ(2.5, construction2.getNetArea());
  EXPECT_DOUBLE_EQ(22.5, cost1->totalCost());
  EXPECT_DOUBLE_EQ(12.5, cost2->totalCost());
}

TEST_F(ModelFixture, Construction_NetArea_InteriorWall) {
  Model model;

  Construction construction1(model);
  Construction construction2(model);
  EXPECT_DOUBLE_EQ(0.0, construction1.getNetArea());
  EXPECT_DOUBLE_EQ(0.0, construction2.getNetArea());

  // add costs
  boost::optional<LifeCycleCost> cost1 = LifeCycleCost::createLifeCycleCost("Brick", construction1, 3.0, "CostPerArea", "Construction");
  ASSERT_TRUE(cost1);
  boost::optional<LifeCycleCost> cost2 = LifeCycleCost::createLifeCycleCost("Glass", construction2, 5.0, "CostPerArea", "Construction");
  ASSERT_TRUE(cost2);
  EXPECT_DOUBLE_EQ(0, cost1->totalCost());
  EXPECT_DOUBLE_EQ(0, cost2->totalCost());

  Building building = model.getUniqueModelObject<Building>();
  DefaultSurfaceConstructions defaultExteriorSurfaceConstructions(model);
  DefaultSurfaceConstructions defaultInteriorSurfaceConstructions(model);
  DefaultSubSurfaceConstructions defaultExteriorSubSurfaceConstructions(model);
  DefaultSubSurfaceConstructions defaultInteriorSubSurfaceConstructions(model);
  DefaultConstructionSet defaultConstructionSet(model);
  defaultConstructionSet.setDefaultExteriorSurfaceConstructions(defaultExteriorSurfaceConstructions);
  defaultConstructionSet.setDefaultInteriorSurfaceConstructions(defaultInteriorSurfaceConstructions);
  defaultConstructionSet.setDefaultExteriorSubSurfaceConstructions(defaultExteriorSubSurfaceConstructions);
  defaultConstructionSet.setDefaultInteriorSubSurfaceConstructions(defaultInteriorSubSurfaceConstructions);
  building.setDefaultConstructionSet(defaultConstructionSet);

  Space space(model);

  Point3dVector points;
  points.push_back(Point3d(0, 0, 1));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(0, 1, 1));

  Surface surface1(points, model);
  surface1.setSpace(space);
  EXPECT_EQ("Wall", surface1.surfaceType());
  EXPECT_EQ("Outdoors", surface1.outsideBoundaryCondition());
  EXPECT_DOUBLE_EQ(1.0, surface1.netArea());

  points.clear();
  points.push_back(Point3d(0, 1, 1));
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(0, 0, 1));

  Surface surface2(points, model);
  surface2.setSpace(space);
  EXPECT_EQ("Wall", surface2.surfaceType());
  EXPECT_EQ("Outdoors", surface2.outsideBoundaryCondition());
  EXPECT_DOUBLE_EQ(1.0, surface2.netArea());

  EXPECT_DOUBLE_EQ(0.0, construction1.getNetArea());
  EXPECT_DOUBLE_EQ(0.0, construction2.getNetArea());
  EXPECT_DOUBLE_EQ(0, cost1->totalCost());
  EXPECT_DOUBLE_EQ(0, cost2->totalCost());

  defaultExteriorSurfaceConstructions.setWallConstruction(construction1);
  ASSERT_TRUE(surface1.construction());
  ASSERT_TRUE(surface2.construction());
  EXPECT_EQ(surface1.construction()->handle(), construction1.handle());
  EXPECT_EQ(surface2.construction()->handle(), construction1.handle());
  EXPECT_DOUBLE_EQ(2.0, construction1.getNetArea());
  EXPECT_DOUBLE_EQ(0.0, construction2.getNetArea());
  EXPECT_DOUBLE_EQ(6.0, cost1->totalCost());
  EXPECT_DOUBLE_EQ(0, cost2->totalCost());

  surface1.setConstruction(construction1);
  surface2.setConstruction(construction2);
  ASSERT_TRUE(surface1.construction());
  ASSERT_TRUE(surface2.construction());
  EXPECT_EQ(surface1.construction()->handle(), construction1.handle());
  EXPECT_EQ(surface2.construction()->handle(), construction2.handle());
  EXPECT_DOUBLE_EQ(1.0, construction1.getNetArea());
  EXPECT_DOUBLE_EQ(1.0, construction2.getNetArea());
  EXPECT_DOUBLE_EQ(3.0, cost1->totalCost());
  EXPECT_DOUBLE_EQ(5.0, cost2->totalCost());

  surface1.setAdjacentSurface(surface2);
  ASSERT_TRUE(surface1.construction());
  ASSERT_TRUE(surface2.construction());
  EXPECT_EQ(surface1.construction()->handle(), construction1.handle());
  EXPECT_EQ(surface2.construction()->handle(), construction2.handle());
  EXPECT_DOUBLE_EQ(1.0, construction1.getNetArea());
  EXPECT_DOUBLE_EQ(1.0, construction2.getNetArea());
  EXPECT_DOUBLE_EQ(3.0, cost1->totalCost());
  EXPECT_DOUBLE_EQ(5.0, cost2->totalCost());

  surface2.setConstruction(construction1);
  ASSERT_TRUE(surface1.construction());
  ASSERT_TRUE(surface2.construction());
  EXPECT_EQ(surface1.construction()->handle(), construction1.handle());
  EXPECT_EQ(surface2.construction()->handle(), construction1.handle());
  EXPECT_DOUBLE_EQ(1.0, construction1.getNetArea());
  EXPECT_DOUBLE_EQ(0.0, construction2.getNetArea());
  EXPECT_DOUBLE_EQ(3.0, cost1->totalCost());
  EXPECT_DOUBLE_EQ(0.0, cost2->totalCost());
}

TEST_F(ModelFixture, Construction_NetArea_SubSurface) {
  Model model;

  ThermalZone thermalZone(model);

  Space space(model);
  space.setThermalZone(thermalZone);

  Construction construction1(model);
  Construction construction2(model);
  EXPECT_DOUBLE_EQ(0.0, construction1.getNetArea());
  EXPECT_DOUBLE_EQ(0.0, construction2.getNetArea());

  Point3dVector points;
  points.push_back(Point3d(0, 0, 10));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(0, 10, 0));
  points.push_back(Point3d(0, 10, 10));

  Surface surface(points, model);
  surface.setConstruction(construction1);
  surface.setSpace(space);
  EXPECT_EQ("Wall", surface.surfaceType());
  EXPECT_DOUBLE_EQ(100.0, surface.netArea());
  EXPECT_DOUBLE_EQ(100.0, construction1.getNetArea());
  EXPECT_DOUBLE_EQ(0.0, construction2.getNetArea());
  EXPECT_DOUBLE_EQ(0, surface.windowToWallRatio());

  points.clear();
  points.push_back(Point3d(0, 0, 2));
  points.push_back(Point3d(0, 0, 1));
  points.push_back(Point3d(0, 1, 1));
  points.push_back(Point3d(0, 1, 2));

  SubSurface subSurface(points, model);
  subSurface.setConstruction(construction2);
  subSurface.setSurface(surface);
  EXPECT_EQ("FixedWindow", subSurface.subSurfaceType());
  EXPECT_DOUBLE_EQ(1.0, subSurface.netArea());
  EXPECT_DOUBLE_EQ(99.0, construction1.getNetArea());
  EXPECT_DOUBLE_EQ(1.0, construction2.getNetArea());
  EXPECT_DOUBLE_EQ(1.0/100.0, surface.windowToWallRatio());

  EXPECT_TRUE(subSurface.setMultiplier(4));
  EXPECT_DOUBLE_EQ(1.0, subSurface.netArea());
  EXPECT_DOUBLE_EQ(96.0, construction1.getNetArea());
  EXPECT_DOUBLE_EQ(4.0, construction2.getNetArea());
  EXPECT_DOUBLE_EQ(4.0/100.0, surface.windowToWallRatio());

  EXPECT_TRUE(thermalZone.setMultiplier(4));
  EXPECT_DOUBLE_EQ(1.0, subSurface.netArea());
  EXPECT_DOUBLE_EQ(4*96.0, construction1.getNetArea());
  EXPECT_DOUBLE_EQ(4*4.0, construction2.getNetArea());
  EXPECT_DOUBLE_EQ(4.0/100.0, surface.windowToWallRatio());
}

TEST_F(ModelFixture, Construction_EnsureUniqueLayers)
{
  Model model;

  // Create some materials
  StandardOpaqueMaterial exterior(model);
  AirGap air(model);
  StandardOpaqueMaterial interior1(model);
  StandardOpaqueMaterial interior2(model);

  OpaqueMaterialVector layers1;
  layers1.push_back(exterior);
  layers1.push_back(air);
  layers1.push_back(interior1);

  OpaqueMaterialVector layers2;
  layers2.push_back(exterior);
  layers2.push_back(air);
  layers2.push_back(interior2);

  EXPECT_EQ(4u, model.getModelObjects<Material>().size());

  Construction construction1(layers1);
  ASSERT_EQ(3u, construction1.layers().size());

  Construction construction2(layers2);
  ASSERT_EQ(3u, construction2.layers().size());

  EXPECT_EQ(construction1.layers()[0].handle(), construction2.layers()[0].handle());
  EXPECT_EQ(construction1.layers()[1].handle(), construction2.layers()[1].handle());
  EXPECT_NE(construction1.layers()[2].handle(), construction2.layers()[2].handle());

  construction1.ensureUniqueLayers();

  EXPECT_EQ(6u, model.getModelObjects<Material>().size());

  ASSERT_EQ(3u, construction1.layers().size());
  ASSERT_EQ(3u, construction2.layers().size());

  EXPECT_NE(construction1.layers()[0].handle(), construction2.layers()[0].handle());
  EXPECT_NE(construction1.layers()[1].handle(), construction2.layers()[1].handle());
  EXPECT_NE(construction1.layers()[2].handle(), construction2.layers()[2].handle());

}

TEST_F(ModelFixture, Construction_StandardsInformationConstruction)
{

  EXPECT_FALSE(StandardsInformationConstruction::standardPerturbableLayerTypeValues().empty());
  EXPECT_FALSE(StandardsInformationConstruction::fenestrationTypeValues().empty());
  EXPECT_FALSE(StandardsInformationConstruction::fenestrationAssemblyContextValues().empty());
  EXPECT_FALSE(StandardsInformationConstruction::fenestrationNumberOfPanesValues().empty());
  EXPECT_FALSE(StandardsInformationConstruction::fenestrationFrameTypeValues().empty());
  EXPECT_FALSE(StandardsInformationConstruction::fenestrationDividerTypeValues().empty());
  EXPECT_FALSE(StandardsInformationConstruction::fenestrationTintValues().empty());
  EXPECT_FALSE(StandardsInformationConstruction::fenestrationGasFillValues().empty());
  EXPECT_FALSE(StandardsInformationConstruction::intendedSurfaceTypeValues().empty());

  Model model;

  Construction construction(model);
  EXPECT_EQ(0, model.getModelObjects<StandardsInformationConstruction>().size());

  StandardsInformationConstruction tmp = construction.standardsInformation();
  EXPECT_EQ(1u, model.getModelObjects<StandardsInformationConstruction>().size());

  StandardsInformationConstruction info = construction.standardsInformation();
  EXPECT_EQ(1u, model.getModelObjects<StandardsInformationConstruction>().size());
  EXPECT_EQ(toString(info.handle()), toString(tmp.handle()));
  EXPECT_EQ(toString(construction.handle()), toString(info.construction().handle()));

  EXPECT_FALSE(info.intendedSurfaceType());
  EXPECT_TRUE(info.suggestedStandardsConstructionTypes().empty());

  EXPECT_TRUE(info.setIntendedSurfaceType("ExteriorWall"));
  ASSERT_TRUE(info.intendedSurfaceType());
  EXPECT_EQ("ExteriorWall", info.intendedSurfaceType().get());
  EXPECT_FALSE(info.suggestedStandardsConstructionTypes().empty());

  EXPECT_FALSE(info.constructionStandard());
  EXPECT_TRUE(info.suggestedConstructionStandardSources().empty());

  info.setConstructionStandard("CEC Title24-2013");
  ASSERT_TRUE(info.constructionStandard());
  EXPECT_EQ("CEC Title24-2013", info.constructionStandard().get());
 // EXPECT_FALSE(info.suggestedConstructionStandardSources().empty());

  construction.remove();
  EXPECT_EQ(0, model.getModelObjects<StandardsInformationConstruction>().size());

}

TEST_F(ModelFixture, Construction_NumLayers)
{
  // from E+ constructions can have up to 10 layers total, 8 for windows
  Model model;

  // Create some materials
  StandardOpaqueMaterial exterior(model);
  AirGap air(model);
  StandardOpaqueMaterial interior(model);
  StandardGlazing glazing(model);

  {
    OpaqueMaterialVector layers;

    EXPECT_THROW({ Construction construction(layers); }, openstudio::Exception);
  }

  {
    OpaqueMaterialVector layers;
    layers.push_back(exterior); // 1
    layers.push_back(exterior); // 2
    layers.push_back(exterior); // 3
    layers.push_back(exterior); // 4
    layers.push_back(exterior); // 5
    layers.push_back(air);      // 6
    layers.push_back(interior); // 7
    layers.push_back(interior); // 8
    layers.push_back(interior); // 9
    layers.push_back(interior); // 10

    EXPECT_NO_THROW({ Construction construction(layers); });

    Construction construction2(model);
    EXPECT_TRUE(construction2.setLayers(castVector<Material>(layers)));
    EXPECT_EQ(10, construction2.layers().size());
  }

  {
    OpaqueMaterialVector layers;
    layers.push_back(exterior); // 1
    layers.push_back(exterior); // 2
    layers.push_back(exterior); // 3
    layers.push_back(exterior); // 4
    layers.push_back(exterior); // 5
    layers.push_back(air);      // 6
    layers.push_back(interior); // 7
    layers.push_back(interior); // 8
    layers.push_back(interior); // 9
    layers.push_back(interior); // 10
    layers.push_back(interior); // 11

    EXPECT_THROW({ Construction construction(layers); }, openstudio::Exception);

    Construction construction2(model);
    EXPECT_FALSE(construction2.setLayers(castVector<Material>(layers)));
    EXPECT_EQ(0, construction2.layers().size());
  }

  {
    Construction construction(model);

    OpaqueMaterialVector layers;
    EXPECT_TRUE(construction.setLayers(castVector<Material>(layers)));
    EXPECT_EQ(0, construction.layers().size());

    for (unsigned i = 0; i < 12; ++i){
      bool test = construction.insertLayer(i, exterior);
      if (i < 10){
        EXPECT_TRUE(test);
        EXPECT_EQ(i + 1, construction.layers().size());
      } else{
        EXPECT_FALSE(test);
        EXPECT_EQ(10u, construction.layers().size());
      }
    }
  }

  {
   FenestrationMaterialVector layers;

    EXPECT_THROW({ Construction construction(layers); }, openstudio::Exception);
  }

  {
    FenestrationMaterialVector layers;
    layers.push_back(glazing); // 1
    layers.push_back(glazing); // 2
    layers.push_back(glazing); // 3
    layers.push_back(glazing); // 4
    layers.push_back(glazing); // 5
    layers.push_back(glazing); // 6
    layers.push_back(glazing); // 7
    layers.push_back(glazing); // 8

    EXPECT_NO_THROW({ Construction construction(layers); });

    Construction construction2(model);
    EXPECT_TRUE(construction2.setLayers(castVector<Material>(layers)));
    EXPECT_EQ(8, construction2.layers().size());
  }

  {
    FenestrationMaterialVector layers;
    layers.push_back(glazing); // 1
    layers.push_back(glazing); // 2
    layers.push_back(glazing); // 3
    layers.push_back(glazing); // 4
    layers.push_back(glazing); // 5
    layers.push_back(glazing); // 6
    layers.push_back(glazing); // 7
    layers.push_back(glazing); // 8
    layers.push_back(glazing); // 9

    EXPECT_THROW({ Construction construction(layers); }, openstudio::Exception);

    Construction construction2(model);
    EXPECT_FALSE(construction2.setLayers(castVector<Material>(layers)));
    EXPECT_EQ(0, construction2.layers().size());
  }

  {
    Construction construction(model);

    FenestrationMaterialVector layers;
    EXPECT_TRUE(construction.setLayers(castVector<Material>(layers)));
    EXPECT_EQ(0, construction.layers().size());

    for (unsigned i = 0; i < 10; ++i){
      bool test = construction.insertLayer(i, glazing);
      if (i < 8){
        EXPECT_TRUE(test);
        EXPECT_EQ(i+1, construction.layers().size());
      } else{
        EXPECT_FALSE(test);
        EXPECT_EQ(8u, construction.layers().size());
      }
    }
  }
}
