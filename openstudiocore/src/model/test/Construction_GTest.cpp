/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#include <gtest/gtest.h>

#include <model/test/ModelFixture.hpp>
#include <model/Model.hpp>
#include <model/Model_Impl.hpp>
#include <model/Building.hpp>
#include <model/Building_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/Component.hpp>
#include <model/CFactorUndergroundWallConstruction.hpp>
#include <model/CFactorUndergroundWallConstruction_Impl.hpp>
#include <model/Construction.hpp>
#include <model/Construction_Impl.hpp>
#include <model/DefaultConstructionSet.hpp>
#include <model/DefaultConstructionSet_Impl.hpp>
#include <model/DefaultSurfaceConstructions.hpp>
#include <model/DefaultSurfaceConstructions_Impl.hpp>
#include <model/DefaultSubSurfaceConstructions.hpp>
#include <model/DefaultSubSurfaceConstructions_Impl.hpp>
#include <model/ConstructionWithInternalSource.hpp>
#include <model/ConstructionWithInternalSource_Impl.hpp>
#include <model/FFactorGroundFloorConstruction.hpp>
#include <model/FFactorGroundFloorConstruction_Impl.hpp>
#include <model/WindowDataFile.hpp>
#include <model/WindowDataFile_Impl.hpp>

#include <model/Material.hpp>
#include <model/Material_Impl.hpp>
#include <model/StandardOpaqueMaterial.hpp>
#include <model/AirGap.hpp>
#include <model/AirWallMaterial.hpp>
#include <model/StandardOpaqueMaterial.hpp>
#include <model/StandardOpaqueMaterial_Impl.hpp>
#include <model/Space.hpp>
#include <model/Space_Impl.hpp>
#include <model/Surface.hpp>
#include <model/Surface_Impl.hpp>
#include <model/SubSurface.hpp>
#include <model/SubSurface_Impl.hpp>
#include <model/LifeCycleCost.hpp>

#include <utilities/geometry/Point3d.hpp>
#include <utilities/idf/IdfFile.hpp>
#include <utilities/idd/OS_Construction_FieldEnums.hxx>
#include <utilities/idd/OS_Material_FieldEnums.hxx>
#include <utilities/idd/OS_Material_AirGap_FieldEnums.hxx>

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
  Model model;

  // Create some materials
  StandardOpaqueMaterial exterior(model);
  AirGap air(model);
  StandardOpaqueMaterial interior(model);

  OpaqueMaterialVector layers;
  layers.push_back(exterior);
  layers.push_back(air);
  layers.push_back(interior);

  EXPECT_EQ(static_cast<unsigned>(3), model.getModelObjects<Material>().size());

  Construction construction(layers);
  ASSERT_EQ(static_cast<unsigned>(3), construction.layers().size());

  ModelObject clone = construction.clone(model);

  EXPECT_EQ(static_cast<unsigned>(3), model.getModelObjects<Material>().size());

  EXPECT_FALSE(clone.handle() == construction.handle());
  ASSERT_TRUE(clone.optionalCast<Construction>());
  ASSERT_EQ(static_cast<unsigned>(3), clone.cast<Construction>().layers().size());
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
