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
#include <model/BuildingStory.hpp>
#include <model/BuildingStory_Impl.hpp>
#include <model/Space.hpp>
#include <model/Space_Impl.hpp>
#include <model/Surface.hpp>
#include <model/Surface_Impl.hpp>
#include <model/SubSurface.hpp>
#include <model/SubSurface_Impl.hpp>
#include <model/ResourceObject.hpp>
#include <model/ResourceObject_Impl.hpp>
#include <model/ComponentData.hpp>
#include <model/ComponentData_Impl.hpp>
#include <model/ConstructionBase.hpp>
#include <model/ConstructionBase_Impl.hpp>
#include <model/LayeredConstruction.hpp>
#include <model/LayeredConstruction_Impl.hpp>
#include <model/Construction.hpp>
#include <model/Construction_Impl.hpp>
#include <model/Material.hpp>
#include <model/Material_Impl.hpp>
#include <model/AirGap.hpp>
#include <model/AirGap_Impl.hpp>
#include <model/AirWallMaterial.hpp>
#include <model/AirWallMaterial_Impl.hpp>
#include <model/StandardOpaqueMaterial.hpp>
#include <model/StandardOpaqueMaterial_Impl.hpp>
#include <model/DefaultConstructionSet.hpp>
#include <model/DefaultConstructionSet_Impl.hpp>
#include <model/DefaultSurfaceConstructions.hpp>
#include <model/DefaultSurfaceConstructions_Impl.hpp>
#include <model/SpaceType.hpp>
#include <model/SpaceType_Impl.hpp>
#include <model/Relationship.hpp>

#include <utilities/data/Attribute.hpp>
#include <utilities/idf/IdfObject.hpp>
#include <utilities/idf/WorkspaceWatcher.hpp>
#include <utilities/idf/WorkspaceExtensibleGroup.hpp>

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Surface_FieldEnums.hxx>

#include <utilities/geometry/Geometry.hpp>
#include <utilities/geometry/Point3d.hpp>
#include <utilities/geometry/Vector3d.hpp>

#include <utilities/math/FloatCompare.hpp>

#include <utilities/core/Finder.hpp>

#include <utilities/core/Assert.hpp>

#include <boost/foreach.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/math/constants/constants.hpp>

using namespace openstudio;
using namespace openstudio::model;
using std::string;
using boost::to_upper_copy;

TEST_F(ModelFixture, Surface_SetVertices)
{
  Model model;

  Point3dVector points;
  Point3dVector testPoints;
  
  // triangle with unit area
  points.clear();
  points.push_back(Point3d(0, 2, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(1, 0, 0));
  Surface surface(points, model);
  EXPECT_EQ("RoofCeiling", surface.surfaceType());
  EXPECT_EQ("Outdoors", surface.outsideBoundaryCondition());

  testPoints = surface.vertices();
  ASSERT_EQ(points.size(), testPoints.size());
  for (unsigned i = 0; i < points.size(); ++i){
    EXPECT_EQ(points[i], testPoints[i]);
  }

  // set will fail, object is unchanged
  EXPECT_FALSE(surface.setVertices(Point3dVector()));
  EXPECT_EQ("RoofCeiling", surface.surfaceType());
  EXPECT_EQ("Outdoors", surface.outsideBoundaryCondition());

  testPoints = surface.vertices();
  ASSERT_EQ(points.size(), testPoints.size());
  for (unsigned i = 0; i < points.size(); ++i){
    EXPECT_EQ(points[i], testPoints[i]);
  }

  // square with unit area
  points.clear();
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(1, 0, 0));
  points.push_back(Point3d(1, 1, 0));
  EXPECT_TRUE(surface.setVertices(points));
  EXPECT_EQ("RoofCeiling", surface.surfaceType());
  EXPECT_EQ("Outdoors", surface.outsideBoundaryCondition());

  testPoints = surface.vertices();
  ASSERT_EQ(points.size(), testPoints.size());
  for (unsigned i = 0; i < points.size(); ++i){
    EXPECT_EQ(points[i], testPoints[i]);
  }

  // u shape (concave) with 5 unit area, includes colinear points
  points.clear();
  points.push_back(Point3d(0, 0, 2));
  points.push_back(Point3d(0, 0, 1));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(1, 0, 0));
  points.push_back(Point3d(2, 0, 0));
  points.push_back(Point3d(3, 0, 0));
  points.push_back(Point3d(3, 0, 1));
  points.push_back(Point3d(3, 0, 2));
  points.push_back(Point3d(2, 0, 2));
  points.push_back(Point3d(2, 0, 1));
  points.push_back(Point3d(1, 0, 1));
  points.push_back(Point3d(1, 0, 2));
  EXPECT_TRUE(surface.setVertices(points));
  // set vertices does not change surfaceType or outsideBoundaryCondition
  EXPECT_EQ("RoofCeiling", surface.surfaceType());
  EXPECT_EQ("Outdoors", surface.outsideBoundaryCondition());

  testPoints = surface.vertices();
  ASSERT_EQ(points.size(), testPoints.size());
  for (unsigned i = 0; i < points.size(); ++i){
    EXPECT_EQ(points[i], testPoints[i]);
  }
}

TEST_F(ModelFixture, Surface_Initialization)
{
  Model model;

  Point3dVector points;
  Point3dVector testPoints;
  
  points.clear();
  points.push_back(Point3d(0, 2, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(1, 0, 0));
  Surface roof(points, model);
  EXPECT_EQ("RoofCeiling", roof.surfaceType());
  EXPECT_EQ("Outdoors", roof.outsideBoundaryCondition());
  EXPECT_EQ(0, roof.outwardNormal().x());
  EXPECT_EQ(0, roof.outwardNormal().y());
  EXPECT_EQ(1, roof.outwardNormal().z());

  points.clear();
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(0, 2, 0));
  points.push_back(Point3d(1, 0, 0));
  Surface floor(points, model);
  EXPECT_EQ("Floor", floor.surfaceType());
  EXPECT_EQ("Ground", floor.outsideBoundaryCondition());
  EXPECT_EQ(0, floor.outwardNormal().x());
  EXPECT_EQ(0, floor.outwardNormal().y());
  EXPECT_EQ(-1, floor.outwardNormal().z());

  points.clear();
  points.push_back(Point3d(0, 0, 2));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(0, -1, 0));
  Surface wall1(points, model);
  EXPECT_EQ("Wall", wall1.surfaceType());
  EXPECT_EQ("Outdoors", wall1.outsideBoundaryCondition());
  EXPECT_EQ(-1, wall1.outwardNormal().x());
  EXPECT_EQ(0, wall1.outwardNormal().y());
  EXPECT_EQ(0, wall1.outwardNormal().z());

  points.clear();
  points.push_back(Point3d(0, 0, 2));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(0, 1, 0));
  Surface wall2(points, model);
  EXPECT_EQ("Wall", wall2.surfaceType());
  EXPECT_EQ("Outdoors", wall2.outsideBoundaryCondition());
  EXPECT_EQ(1, wall2.outwardNormal().x());
  EXPECT_EQ(0, wall2.outwardNormal().y());
  EXPECT_EQ(0, wall2.outwardNormal().z());
}

TEST_F(ModelFixture, Surface_Area)
{
  Model model;

  Point3dVector points;

  // triangle with unit area
  points.clear();
  points.push_back(Point3d(0, 2, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(1, 0, 0));

  Surface surface(points, model);
  EXPECT_EQ(0, surface.outwardNormal().x());
  EXPECT_EQ(0, surface.outwardNormal().y());
  EXPECT_EQ(1, surface.outwardNormal().z());
  EXPECT_EQ(1, surface.outwardNormal().length());
  EXPECT_EQ(1.0, surface.grossArea());
  EXPECT_EQ(1.0, surface.netArea());

  // square with unit area
  points.clear();
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(1, 0, 0));
  points.push_back(Point3d(1, 1, 0));
  surface.setVertices(points);

  EXPECT_EQ(0, surface.outwardNormal().x());
  EXPECT_EQ(0, surface.outwardNormal().y());
  EXPECT_EQ(1, surface.outwardNormal().z());
  EXPECT_EQ(1, surface.outwardNormal().length());
  EXPECT_EQ(1.0, surface.grossArea());
  EXPECT_EQ(1.0, surface.netArea());

  // u shape (concave) with 5 unit area, includes colinear points
  points.clear();
  points.push_back(Point3d(0, 0, 2));
  points.push_back(Point3d(0, 0, 1));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(1, 0, 0));
  points.push_back(Point3d(2, 0, 0));
  points.push_back(Point3d(3, 0, 0));
  points.push_back(Point3d(3, 0, 1));
  points.push_back(Point3d(3, 0, 2));
  points.push_back(Point3d(2, 0, 2));
  points.push_back(Point3d(2, 0, 1));
  points.push_back(Point3d(1, 0, 1));
  points.push_back(Point3d(1, 0, 2));
  surface.setVertices(points);

  EXPECT_EQ(0, surface.outwardNormal().x());
  EXPECT_EQ(-1, surface.outwardNormal().y());
  EXPECT_EQ(0, surface.outwardNormal().z());
  EXPECT_EQ(1, surface.outwardNormal().length());
  EXPECT_EQ(5.0, surface.grossArea());
  EXPECT_EQ(5.0, surface.netArea());
}

/* HAS TO WAIT UNTIL WE GET A GOOD OSM EXAMPLE
TEST_F(ModelFixture, Surface_Area_In_File)
{
  openstudio::path modelDir = resourcesPath() / toPath("model/Daylighting_Office/");
  openstudio::path modelPath = modelDir / toPath("in.osm");
  openstudio::path sqlPath = modelDir / toPath("eplusout.sql");

  OptionalModel optionalModel = Model::load(modelPath);
  ASSERT_TRUE(optionalModel);
  Model model(*optionalModel);

  SqlFile sqlFile(sqlPath);

  model.setSqlFile(sqlFile);
  ASSERT_TRUE(model.sqlFile());

  SurfaceVector surfaces = model.getModelObjects<Surface>();
  EXPECT_TRUE(surfaces.size() > 0);

  BOOST_FOREACH(const Surface& surface, surfaces){
    OptionalString name = surface.name();
    ASSERT_TRUE(name);

    // compute metrics from input object
    double netArea = surface.netArea();
    double grossArea = surface.grossArea();
    EXPECT_TRUE(netArea > 0);
    EXPECT_TRUE(grossArea > 0);
    EXPECT_TRUE(grossArea >= netArea);
    double tilt = surface.tilt();
    EXPECT_TRUE(greaterThanOrEqual(tilt,0.0));
    EXPECT_TRUE(lessThanOrEqual(tilt,boost::math::constants::pi<double>()));
    double azimuth = surface.azimuth();
    EXPECT_TRUE(greaterThanOrEqual(azimuth,0.0));
    EXPECT_TRUE(lessThanOrEqual(azimuth,2.0*boost::math::constants::pi<double>()));

    // TODO: check tilt and azimuth too

    // query to get area from sql file
    std::string query = "SELECT Area FROM surfaces WHERE SurfaceName='" + to_upper_copy(*name) + "'";
    OptionalDouble sqlArea = model.sqlFile()->execAndReturnFirstDouble(query);
    ASSERT_TRUE(sqlArea);
    EXPECT_TRUE(*sqlArea > 0);
    EXPECT_NEAR(*sqlArea, netArea, 0.000001);

    // query to get gross area from sql file
    query = "SELECT GrossArea FROM surfaces WHERE SurfaceName='" + to_upper_copy(*name) + "'";
    OptionalDouble sqlGrossArea = model.sqlFile()->execAndReturnFirstDouble(query);
    ASSERT_TRUE(sqlGrossArea);
    EXPECT_TRUE(*sqlGrossArea > 0);
    EXPECT_NEAR(*sqlGrossArea, grossArea, 0.000001);

    // check that we can get u-factors and thermal conductances
    OptionalDouble ufactor = surface.uFactor();
    ASSERT_TRUE(ufactor);
    EXPECT_TRUE(*ufactor > 0);

    OptionalDouble thermalConductance = surface.thermalConductance();
    ASSERT_TRUE(thermalConductance);
    EXPECT_TRUE(*thermalConductance > 0);

    // check that we can get interior and exterior visible absorbtances
    OptionalDouble interiorVisibleAbsorbtance = surface.interiorVisibleAbsorbtance();
    ASSERT_TRUE(interiorVisibleAbsorbtance);
    EXPECT_TRUE(*interiorVisibleAbsorbtance > 0);

    OptionalDouble exteriorVisibleAbsorbtance = surface.exteriorVisibleAbsorbtance();
    ASSERT_TRUE(exteriorVisibleAbsorbtance);
    EXPECT_TRUE(*exteriorVisibleAbsorbtance > 0);

    // check that construction is returned as resource used by surface
    ResourceObjectVector resources = surface.resources();
    ASSERT_FALSE(resources.empty());
    EXPECT_EQ(static_cast<unsigned>(1),resources.size());
    EXPECT_TRUE(resources[0].optionalCast<ConstructionBase>());

    if (resources[0].optionalCast<LayeredConstruction>()) {
      // check that materials can be retrieved
      MaterialVector materials = getSubsetOfRecursiveResources<Material>(surface);
      EXPECT_FALSE(materials.empty());
    }
  }
}
*/

/*
TEST_F(ModelFixture, Surface_BuildingComponentLibraryRoofConstruction)
{
  Workspace workspace(idfFile);

  // translate energyplus simulation to building model
  energyplus::ReverseTranslator reverseTranslator(workspace);
  OptionalModel optionalModel = reverseTranslator.convert();
  ASSERT_TRUE(optionalModel);
  Model model(*optionalModel);

  // translate energyplus roof component to Component
  openstudio::path componentDirectory = resourcesPath()/toPath("energyplus/Components/idf_roof_1");
  energyplus::OptionalComponentTranslator oComponentTranslator = energyplus::ComponentTranslator::load(componentDirectory);
  ASSERT_TRUE(oComponentTranslator);
  energyplus::ComponentTranslator componentTranslator = *oComponentTranslator;
  ASSERT_TRUE(componentTranslator.convert());
  Component roofComponent = componentTranslator.component().get();

  // replace roof construction in model
  SurfaceVector surfaces = model.getModelObjects<Surface>();
  OptionalComponentData ocd = model.insertComponent(roofComponent);
  ASSERT_TRUE(ocd);
  OptionalConstructionBase ocb = ocd->primaryComponentObject().optionalCast<ConstructionBase>();
  ASSERT_TRUE(ocb);
  ConstructionBase roofComponentPrimaryObject = *ocb;
  unsigned numReplaced = 0;
  BOOST_FOREACH(Surface& surface,surfaces) {
    OptionalSurfaceType oType = surface.surfaceType();
    if (oType && (*oType == SurfaceType::Roof)) {
      EXPECT_TRUE(surface.setConstruction(roofComponentPrimaryObject));
      ++numReplaced;
    }
  }
  EXPECT_TRUE(numReplaced > 0);
  LOG(Debug,"Replaced the construction of " << numReplaced  << " surface objects identified as "
      << "roofs.");
}
*/
TEST_F(ModelFixture, AirWall)
{
  Model model;
    
  // triangle with unit area
  Point3dVector points;
  points.push_back(Point3d(0, 2, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(1, 0, 0));

  AirGap materialAirGap(model);
  StandardOpaqueMaterial material(model);
  AirWallMaterial materialAirWall(model);
  Construction construction(model);
  std::vector<Material> layers;

  // no construction
  Surface surface(points, model);
  EXPECT_FALSE(surface.isAirWall());

  // empty construction
  EXPECT_TRUE(surface.setConstruction(construction));
  EXPECT_FALSE(surface.isAirWall());

  // material
  layers.clear();
  layers.push_back(material);
  EXPECT_TRUE(construction.setLayers(layers));
  EXPECT_FALSE(surface.isAirWall());

  // material, materialAirGap
  layers.clear();
  layers.push_back(material);
  layers.push_back(materialAirGap);
  EXPECT_TRUE(construction.setLayers(layers));
  EXPECT_FALSE(surface.isAirWall());

  // material, materialAirGap, material
  layers.clear();
  layers.push_back(material);
  layers.push_back(materialAirGap);
  layers.push_back(material);
  EXPECT_TRUE(construction.setLayers(layers));
  EXPECT_FALSE(surface.isAirWall());

  // materialAirGap
  layers.clear();
  layers.push_back(materialAirGap);
  EXPECT_TRUE(construction.setLayers(layers));
  EXPECT_FALSE(surface.isAirWall());

  // materialAirWall
  EXPECT_TRUE(construction.setLayer(materialAirWall));
  EXPECT_TRUE(surface.isAirWall());

}

TEST_F(ModelFixture, 0_Vertex_Surface)
{
  Model model;

  std::vector<Point3d> vertices;

  EXPECT_THROW(Surface(vertices, model), openstudio::Exception);
}

TEST_F(ModelFixture, 1_Vertex_Surface)
{
  Model model;

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0,0,1));

  EXPECT_THROW(Surface(vertices, model), openstudio::Exception);
}

TEST_F(ModelFixture, 2_Vertex_Surface)
{
  Model model;

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0,0,1));
  vertices.push_back(Point3d(0,0,0));

  EXPECT_THROW(Surface(vertices, model), openstudio::Exception);
}

TEST_F(ModelFixture, 3_Vertex_Surface)
{
  Model model;

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0,0,1));
  vertices.push_back(Point3d(0,0,0));
  vertices.push_back(Point3d(1,0,0));

  EXPECT_NO_THROW(Surface(vertices, model));
}

TEST_F(ModelFixture, 4_Vertex_Surface)
{
  Model model;

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0,0,1));
  vertices.push_back(Point3d(0,0,0));
  vertices.push_back(Point3d(1,0,0));
  vertices.push_back(Point3d(1,0,1));

  EXPECT_NO_THROW(Surface(vertices, model));
}

TEST_F(ModelFixture, 5_Vertex_Surface)
{
  Model model;

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0,0,1));
  vertices.push_back(Point3d(0,0,0));
  vertices.push_back(Point3d(1,0,0));
  vertices.push_back(Point3d(1,0,1));
  vertices.push_back(Point3d(0.5,0,1.5));

  EXPECT_NO_THROW(Surface(vertices, model));
}

TEST_F(ModelFixture, Surface_Clone) {
  Model model;

  // construct Construction
  StandardOpaqueMaterial material(model);
  Construction construction(model);
  StringVector values;
  values.push_back(material.name().get());
  IdfExtensibleGroup eg = construction.pushExtensibleGroup(values);
  EXPECT_FALSE(eg.empty());
  ASSERT_TRUE(construction.getModelObjectTargets<StandardOpaqueMaterial>().size() == 1u);
  EXPECT_TRUE(construction.getModelObjectTargets<StandardOpaqueMaterial>()[0] == material);

  // construct Surface
  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0,0,1));
  vertices.push_back(Point3d(0,0,0));
  vertices.push_back(Point3d(1,0,0));
  vertices.push_back(Point3d(1,0,1));
  Surface surface(vertices, model);
  EXPECT_TRUE(surface.setConstruction(construction));
  ASSERT_TRUE(surface.construction());
  EXPECT_TRUE(surface.construction().get() == construction);

  // clone should maintain connection to Construction
  Surface clone1 = surface.clone().cast<Surface>();
  ASSERT_TRUE(clone1.model() == surface.model());
  ASSERT_TRUE(clone1.construction());
  EXPECT_TRUE(clone1.construction().get() == construction);

  // even if through ModelObject
  Surface clone2 = surface.cast<ModelObject>().clone().cast<Surface>();
  ASSERT_TRUE(clone2.model() == surface.model());
  ASSERT_TRUE(clone2.construction());
  EXPECT_TRUE(clone2.construction().get() == construction);
}

TEST_F(ModelFixture, Surface_Construction_Relationship)
{
  Model model;
  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0,0,1));
  vertices.push_back(Point3d(0,0,0));
  vertices.push_back(Point3d(1,0,0));
  vertices.push_back(Point3d(1,0,1));
  Surface surface(vertices, model);

  // can't get relationship as attribute
  boost::optional<Attribute> attribute = surface.getAttribute("construction");
  EXPECT_FALSE(attribute);

  // can't get attributes as relationship
  EXPECT_TRUE(surface.getAttribute("name"));
  EXPECT_FALSE(surface.getRelationship("name"));
  EXPECT_TRUE(surface.getAttribute("grossArea"));
  EXPECT_FALSE(surface.getRelationship("grossArea"));

  // no construction yet
  StringVector relationshipNames = surface.relationshipNames();
  EXPECT_FALSE(std::find(relationshipNames.begin(),
                         relationshipNames.end(),
                         "construction") == relationshipNames.end());
  RelationshipVector relationships = surface.relationships();
  NameFinder<Relationship> finder("construction");
  EXPECT_FALSE(std::find_if(relationships.begin(),
                            relationships.end(),
                            finder) == relationships.end());
  boost::optional<Relationship> relationship = surface.getRelationship("construction");
  ASSERT_TRUE(relationship);
  EXPECT_EQ(surface.handle(), relationship->modelObject().handle());
  ASSERT_TRUE(relationship->isSingular());
  ASSERT_TRUE(relationship->isSettable());
  EXPECT_FALSE(relationship->relatedModelObject());
  EXPECT_TRUE(relationship->resetRelatedModelObject());
  EXPECT_FALSE(relationship->relatedModelObject());

  // make material
  StandardOpaqueMaterial material(model);

  EXPECT_FALSE(relationship->setRelatedModelObject(material));
  EXPECT_FALSE(relationship->relatedModelObject());

  // make construction
  Construction construction(model);

  EXPECT_TRUE(relationship->setRelatedModelObject(construction));
  ASSERT_TRUE(relationship->relatedModelObject());
  EXPECT_EQ(construction.handle(), relationship->relatedModelObject()->handle());
  ASSERT_TRUE(surface.construction());
  EXPECT_EQ(construction.handle(), surface.construction()->handle());

  // not yet implemented
  //EXPECT_TRUE(relationship->resetRelatedModelObject());
  //EXPECT_FALSE(relationship->relatedModelObject());
}

TEST_F(ModelFixture, AdjacentSurface)
{
  Model model;

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0,0,3));
  vertices.push_back(Point3d(0,0,0));
  vertices.push_back(Point3d(3,0,0));
  vertices.push_back(Point3d(3,0,3));

  Space space1(model);
  Surface wall1(vertices, model);
  wall1.setSpace(space1);
  EXPECT_FALSE(wall1.adjacentSurface());
  EXPECT_EQ("Outdoors", wall1.outsideBoundaryCondition());
  EXPECT_EQ("SunExposed", wall1.sunExposure());
  EXPECT_EQ("WindExposed", wall1.windExposure());

  std::reverse(vertices.begin(), vertices.end());

  Space space2(model);
  Surface wall2(vertices, model);
  wall2.setSpace(space2);
  EXPECT_FALSE(wall2.adjacentSurface());
  EXPECT_EQ("Outdoors", wall2.outsideBoundaryCondition());
  EXPECT_EQ("SunExposed", wall2.sunExposure());
  EXPECT_EQ("WindExposed", wall2.windExposure());

  EXPECT_TRUE(wall1.setAdjacentSurface(wall2));
  ASSERT_TRUE(wall1.adjacentSurface());
  EXPECT_EQ(wall2.handle(), wall1.adjacentSurface()->handle());
  ASSERT_TRUE(wall2.adjacentSurface());
  EXPECT_EQ(wall1.handle(), wall2.adjacentSurface()->handle());
  EXPECT_EQ("Surface", wall1.outsideBoundaryCondition());
  EXPECT_EQ("NoSun", wall1.sunExposure());
  EXPECT_EQ("NoWind", wall1.windExposure());
  EXPECT_EQ("Surface", wall2.outsideBoundaryCondition());
  EXPECT_EQ("NoSun", wall2.sunExposure());
  EXPECT_EQ("NoWind", wall2.windExposure());

  wall2.resetAdjacentSurface();
  EXPECT_FALSE(wall1.adjacentSurface());
  EXPECT_FALSE(wall2.adjacentSurface());
  EXPECT_EQ("Outdoors", wall1.outsideBoundaryCondition());
  EXPECT_EQ("SunExposed", wall1.sunExposure());
  EXPECT_EQ("WindExposed", wall1.windExposure());
  EXPECT_EQ("Outdoors", wall2.outsideBoundaryCondition());
  EXPECT_EQ("SunExposed", wall2.sunExposure());
  EXPECT_EQ("WindExposed", wall2.windExposure());

  EXPECT_TRUE(wall1.setAdjacentSurface(wall2));
  ASSERT_TRUE(wall1.adjacentSurface());
  EXPECT_EQ(wall2.handle(), wall1.adjacentSurface()->handle());
  ASSERT_TRUE(wall2.adjacentSurface());
  EXPECT_EQ(wall1.handle(), wall2.adjacentSurface()->handle());
  EXPECT_EQ("Surface", wall1.outsideBoundaryCondition());
  EXPECT_EQ("NoSun", wall1.sunExposure());
  EXPECT_EQ("NoWind", wall1.windExposure());
  EXPECT_EQ("Surface", wall2.outsideBoundaryCondition());
  EXPECT_EQ("NoSun", wall2.sunExposure());
  EXPECT_EQ("NoWind", wall2.windExposure());

  EXPECT_TRUE(wall1.setAdjacentSurface(wall1));
  ASSERT_TRUE(wall1.adjacentSurface());
  EXPECT_EQ(wall1.handle(), wall1.adjacentSurface()->handle());
  EXPECT_FALSE(wall2.adjacentSurface());
  EXPECT_EQ("Surface", wall1.outsideBoundaryCondition());
  EXPECT_EQ("NoSun", wall1.sunExposure());
  EXPECT_EQ("NoWind", wall1.windExposure());
  EXPECT_EQ("Outdoors", wall2.outsideBoundaryCondition());
  EXPECT_EQ("SunExposed", wall2.sunExposure());
  EXPECT_EQ("WindExposed", wall2.windExposure());
}

TEST_F(ModelFixture, Surface_BadSurfaceType)
{
  Model model;

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0,1,0));
  vertices.push_back(Point3d(0,0,0));
  vertices.push_back(Point3d(1,0,0));
  vertices.push_back(Point3d(1,1,0));

  Surface roof(vertices, model);
  EXPECT_FALSE(roof.setSurfaceType("Roof")); // should be RoofCeiling

  vertices.clear();
  vertices.push_back(Point3d(1,1,0));
  vertices.push_back(Point3d(1,0,0));
  vertices.push_back(Point3d(0,0,0));
  vertices.push_back(Point3d(0,1,0));

  EXPECT_NO_THROW(Surface floor(vertices, model));

}

TEST_F(ModelFixture,Surface_IddAssumptions) {

  // Choice Fields

  // outside boundary condition is hand-mapped to similar enums in standardsinterface.
  // if this test starts failing, please re-map the standardsinterface
  StringVector keys = Surface::validOutsideBoundaryConditionValues();
  EXPECT_EQ(14u, keys.size());
  EXPECT_FALSE(std::find_if(keys.begin(),keys.end(),boost::bind(istringEqual,"Adiabatic",_1)) == keys.end());
  EXPECT_FALSE(std::find_if(keys.begin(),keys.end(),boost::bind(istringEqual,"Surface",_1)) == keys.end());
  EXPECT_FALSE(std::find_if(keys.begin(),keys.end(),boost::bind(istringEqual,"Outdoors",_1)) == keys.end());
  EXPECT_FALSE(std::find_if(keys.begin(),keys.end(),boost::bind(istringEqual,"Ground",_1)) == keys.end());
  EXPECT_FALSE(std::find_if(keys.begin(),keys.end(),boost::bind(istringEqual,"GroundFCfactorMethod",_1)) == keys.end());
  EXPECT_FALSE(std::find_if(keys.begin(),keys.end(),boost::bind(istringEqual,"OtherSideCoefficients",_1)) == keys.end());
  EXPECT_FALSE(std::find_if(keys.begin(),keys.end(),boost::bind(istringEqual,"OtherSideConditionsModel",_1)) == keys.end());
  EXPECT_FALSE(std::find_if(keys.begin(),keys.end(),boost::bind(istringEqual,"GroundSlabPreprocessorAverage",_1)) == keys.end());
  EXPECT_FALSE(std::find_if(keys.begin(),keys.end(),boost::bind(istringEqual,"GroundSlabPreprocessorCore",_1)) == keys.end());
  EXPECT_FALSE(std::find_if(keys.begin(),keys.end(),boost::bind(istringEqual,"GroundSlabPreprocessorPerimeter",_1)) == keys.end());
  EXPECT_FALSE(std::find_if(keys.begin(),keys.end(),boost::bind(istringEqual,"GroundBasementPreprocessorAverageWall",_1)) == keys.end());
  EXPECT_FALSE(std::find_if(keys.begin(),keys.end(),boost::bind(istringEqual,"GroundBasementPreprocessorAverageFloor",_1)) == keys.end());
  EXPECT_FALSE(std::find_if(keys.begin(),keys.end(),boost::bind(istringEqual,"GroundBasementPreprocessorUpperWall",_1)) == keys.end());
  EXPECT_FALSE(std::find_if(keys.begin(),keys.end(),boost::bind(istringEqual,"GroundBasementPreprocessorLowerWall",_1)) == keys.end());
}

void expect_point_eq(const Point3d& p1, const Point3d& p2)
{
  EXPECT_NEAR(p1.x(), p2.x(), 0.001);
  EXPECT_NEAR(p1.y(), p2.y(), 0.001);
  EXPECT_NEAR(p1.z(), p2.z(), 0.001);
}


TEST_F(ModelFixture, Surface_SouthWall_WWR)
{
  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0,0,3));
  vertices.push_back(Point3d(0,0,0));
  vertices.push_back(Point3d(10,0,0));
  vertices.push_back(Point3d(10,0,3));

  double W = 10.0;
  double H = 3.0;

  {
    // regular case
    Model model;
    Surface wall(vertices, model);
    EXPECT_EQ("Wall", wall.surfaceType());
    EXPECT_EQ(0.0, wall.windowToWallRatio());
    double x = 0.0254;
    double y = 1.0;
    double offset = 1.0;
    double wwr = (W-2.0*x)*(H-y-offset)/(W*H);
    boost::optional<SubSurface> window = wall.setWindowToWallRatio(wwr, 1.0, true);
    ASSERT_TRUE(window);
    ASSERT_EQ(4u, window->vertices().size());
    expect_point_eq(Point3d(x, 0, H-y), window->vertices()[0]);
    expect_point_eq(Point3d(x, 0, offset), window->vertices()[1]);
    expect_point_eq(Point3d(W-x, 0, offset), window->vertices()[2]);
    expect_point_eq(Point3d(W-x, 0, H-y), window->vertices()[3]);
    EXPECT_NEAR(wwr, wall.windowToWallRatio(), 0.001);
  }

  {
    // daylighting window
    Model model;
    Surface wall(vertices, model);
    EXPECT_EQ("Wall", wall.surfaceType());
    EXPECT_EQ(0.0, wall.windowToWallRatio());
    double x = 0.0254;
    double y = 1.0;
    double offset = 1.0;
    double wwr = (W-2.0*x)*(H-y-offset)/(W*H);
    boost::optional<SubSurface> window = wall.setWindowToWallRatio(wwr, 1.0, false);
    ASSERT_TRUE(window);
    ASSERT_EQ(4u, window->vertices().size());
    expect_point_eq(Point3d(x, 0, H-offset), window->vertices()[0]);
    expect_point_eq(Point3d(x, 0, y), window->vertices()[1]);
    expect_point_eq(Point3d(W-x, 0, y), window->vertices()[2]);
    expect_point_eq(Point3d(W-x, 0, H-offset), window->vertices()[3]);
    EXPECT_NEAR(wwr, wall.windowToWallRatio(), 0.001);
  }

  {
    // window goes all the way up to min limit at ceiling
    Model model;
    Surface wall(vertices, model);
    EXPECT_EQ("Wall", wall.surfaceType());
    EXPECT_EQ(0.0, wall.windowToWallRatio());
    double x = 0.0254;
    double y = 0.0254;
    double offset = 1.0;
    double wwr = (W-2.0*x)*(H-y-offset)/(W*H);
    boost::optional<SubSurface> window = wall.setWindowToWallRatio(wwr, 1.0, true);
    ASSERT_TRUE(window);
    ASSERT_EQ(4u, window->vertices().size());
    expect_point_eq(Point3d(x, 0, H-y), window->vertices()[0]);
    expect_point_eq(Point3d(x, 0, offset), window->vertices()[1]);
    expect_point_eq(Point3d(W-x, 0, offset), window->vertices()[2]);
    expect_point_eq(Point3d(W-x, 0, H-y), window->vertices()[3]);
    EXPECT_NEAR(wwr, wall.windowToWallRatio(), 0.001);
  }

  {
    // window extends below desired sill height
    Model model;
    Surface wall(vertices, model);
    EXPECT_EQ("Wall", wall.surfaceType());
    EXPECT_EQ(0.0, wall.windowToWallRatio());
    double x = 0.0254;
    double y = 0.0254;
    double offset = 0.5;
    double wwr = (W-2.0*x)*(H-y-offset)/(W*H);
    boost::optional<SubSurface> window = wall.setWindowToWallRatio(wwr, 1.0, true);
    ASSERT_TRUE(window);
    ASSERT_EQ(4u, window->vertices().size());
    expect_point_eq(Point3d(x, 0, H-y), window->vertices()[0]);
    expect_point_eq(Point3d(x, 0, offset), window->vertices()[1]);
    expect_point_eq(Point3d(W-x, 0, offset), window->vertices()[2]);
    expect_point_eq(Point3d(W-x, 0, H-y), window->vertices()[3]);
    EXPECT_NEAR(wwr, wall.windowToWallRatio(), 0.001);
  }

  {
    // window is too small, shrinks in x
    Model model;
    Surface wall(vertices, model);
    EXPECT_EQ("Wall", wall.surfaceType());
    EXPECT_EQ(0.0, wall.windowToWallRatio());
    double x = 4.0;
    double y = H-0.3048-1.0;
    double offset = 1.0;
    double wwr = (W-2.0*x)*(H-y-offset)/(W*H);
    boost::optional<SubSurface> window = wall.setWindowToWallRatio(wwr, 1.0, true);
    ASSERT_TRUE(window);
    ASSERT_EQ(4u, window->vertices().size());
    expect_point_eq(Point3d(x, 0, H-y), window->vertices()[0]);
    expect_point_eq(Point3d(x, 0, offset), window->vertices()[1]);
    expect_point_eq(Point3d(W-x, 0, offset), window->vertices()[2]);
    expect_point_eq(Point3d(W-x, 0, H-y), window->vertices()[3]);
    EXPECT_NEAR(wwr, wall.windowToWallRatio(), 0.001);
  }

  {
    // existing window is no problem
    Model model;
    Surface wall(vertices, model);
    EXPECT_EQ("Wall", wall.surfaceType());
    EXPECT_EQ(0.0, wall.windowToWallRatio());

    std::vector<Point3d> windowVertices;
    windowVertices.push_back(Point3d(1,0,2));
    windowVertices.push_back(Point3d(1,0,1));
    windowVertices.push_back(Point3d(9,0,1));
    windowVertices.push_back(Point3d(9,0,2));
    SubSurface window(windowVertices, model);
    window.setSurface(wall);
    EXPECT_EQ("FixedWindow", window.subSurfaceType());

    double wwr = 8.0/30.0; // 0.26666
    EXPECT_EQ(1u, wall.subSurfaces().size());
    EXPECT_NEAR(wwr, wall.windowToWallRatio(), 0.001);

    wwr = 10.0/30.0; // 0.3333
    boost::optional<SubSurface> newWindow = wall.setWindowToWallRatio(wwr);
    ASSERT_TRUE(newWindow);
    EXPECT_EQ(1u, wall.subSurfaces().size());
    EXPECT_NEAR(wwr, wall.windowToWallRatio(), 0.001);
  }

  {
    // existing door is no problem if it does not intersect new window
    Model model;
    Surface wall(vertices, model);
    EXPECT_EQ("Wall", wall.surfaceType());
    EXPECT_EQ(0.0, wall.windowToWallRatio());

    std::vector<Point3d> doorVertices;
    doorVertices.push_back(Point3d(4,0,0.5));
    doorVertices.push_back(Point3d(4,0,0));
    doorVertices.push_back(Point3d(6,0,0));
    doorVertices.push_back(Point3d(6,0,0.5));
    SubSurface door(doorVertices, model);
    door.setSurface(wall);
    EXPECT_EQ("Door", door.subSurfaceType());

    EXPECT_EQ(1u, wall.subSurfaces().size());
    EXPECT_NEAR(0.0, wall.windowToWallRatio(), 0.001);

    double wwr = 10.0/30.0; // 0.3333
    boost::optional<SubSurface> newWindow = wall.setWindowToWallRatio(wwr, 1.0, true);
    ASSERT_TRUE(newWindow);
    EXPECT_EQ(2u, wall.subSurfaces().size());
    EXPECT_NEAR(wwr, wall.windowToWallRatio(), 0.001);
  }
}

TEST_F(ModelFixture, Surface_EastWall_WWR)
{
  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0,0,3));
  vertices.push_back(Point3d(0,0,0));
  vertices.push_back(Point3d(0,10,0));
  vertices.push_back(Point3d(0,10,3));

  double W = 10.0;
  double H = 3.0;

  {
    // regular case
    Model model;
    Surface wall(vertices, model);
    EXPECT_EQ("Wall", wall.surfaceType());
    EXPECT_EQ(0.0, wall.windowToWallRatio());
    double x = 0.0254;
    double y = 1.0;
    double offset = 1.0;
    double wwr = (W-2.0*x)*(H-y-offset)/(W*H);
    boost::optional<SubSurface> window = wall.setWindowToWallRatio(wwr, 1.0, true);
    ASSERT_TRUE(window);
    ASSERT_EQ(4u, window->vertices().size());
    expect_point_eq(Point3d(0, x, H-y), window->vertices()[0]);
    expect_point_eq(Point3d(0, x, offset), window->vertices()[1]);
    expect_point_eq(Point3d(0, W-x, offset), window->vertices()[2]);
    expect_point_eq(Point3d(0, W-x, H-y), window->vertices()[3]);
    EXPECT_NEAR(wwr, wall.windowToWallRatio(), 0.001);
  }
}

TEST_F(ModelFixture, Surface_WWR_Fail)
{
  {
    // triangle
    std::vector<Point3d> vertices;
    vertices.push_back(Point3d(0,0,1));
    vertices.push_back(Point3d(0,0,0));
    vertices.push_back(Point3d(1,0,0));

    Model model;
    Surface wall(vertices, model);
    EXPECT_EQ("Wall", wall.surfaceType());
    EXPECT_EQ(0.0, wall.windowToWallRatio());

    EXPECT_FALSE(wall.setWindowToWallRatio(0.2));
  }

  {
    // T-shape
    std::vector<Point3d> vertices;
    vertices.push_back(Point3d(0,0,3));
    vertices.push_back(Point3d(0,0,2));
    vertices.push_back(Point3d(3,0,2));
    vertices.push_back(Point3d(3,0,0));
    vertices.push_back(Point3d(4,0,0));
    vertices.push_back(Point3d(4,0,2));
    vertices.push_back(Point3d(7,0,2));
    vertices.push_back(Point3d(7,0,3));

    Model model;
    Surface wall(vertices, model);
    EXPECT_EQ("Wall", wall.surfaceType());
    EXPECT_EQ(0.0, wall.windowToWallRatio());

    EXPECT_FALSE(wall.setWindowToWallRatio(0.2));
  }

  {
    // out of limits
    std::vector<Point3d> vertices;
    vertices.push_back(Point3d(0,0,3));
    vertices.push_back(Point3d(0,0,0));
    vertices.push_back(Point3d(10,0,0));
    vertices.push_back(Point3d(10,0,3));

    Model model;
    Surface wall(vertices, model);
    EXPECT_EQ("Wall", wall.surfaceType());
    EXPECT_FALSE(wall.setWindowToWallRatio(-1));
    EXPECT_FALSE(wall.setWindowToWallRatio(0));
    EXPECT_FALSE(wall.setWindowToWallRatio(1));
    EXPECT_FALSE(wall.setWindowToWallRatio(2));
  }

  {
    // bad surface type
    std::vector<Point3d> vertices;
    vertices.push_back(Point3d(0,3,0));
    vertices.push_back(Point3d(0,0,0));
    vertices.push_back(Point3d(10,0,0));
    vertices.push_back(Point3d(10,3,0));

    Model model;
    Surface roof(vertices, model);
    EXPECT_EQ("RoofCeiling", roof.surfaceType());
    EXPECT_FALSE(roof.setWindowToWallRatio(0.4));
  }

  {
    // bad surface type
    std::vector<Point3d> vertices;
    vertices.push_back(Point3d(0,0,0));
    vertices.push_back(Point3d(0,3,0));
    vertices.push_back(Point3d(10,3,0));
    vertices.push_back(Point3d(10,0,0));

    Model model;
    Surface roof(vertices, model);
    EXPECT_EQ("Floor", roof.surfaceType());
    EXPECT_FALSE(roof.setWindowToWallRatio(0.4));
  }

  {
    // door prevents window from being added
    std::vector<Point3d> vertices;
    vertices.push_back(Point3d(0,0,3));
    vertices.push_back(Point3d(0,0,0));
    vertices.push_back(Point3d(10,0,0));
    vertices.push_back(Point3d(10,0,3));

    Model model;
    Surface wall(vertices, model);
    EXPECT_EQ("Wall", wall.surfaceType());
    EXPECT_EQ(0.0, wall.windowToWallRatio());

    std::vector<Point3d> doorVertices;
    doorVertices.push_back(Point3d(4,0,2));
    doorVertices.push_back(Point3d(4,0,0));
    doorVertices.push_back(Point3d(6,0,0));
    doorVertices.push_back(Point3d(6,0,2));
    SubSurface door(doorVertices, model);
    door.setSurface(wall);
    EXPECT_EQ("Door", door.subSurfaceType());
    EXPECT_EQ(1u, wall.subSurfaces().size());

    EXPECT_NEAR(0.0, wall.windowToWallRatio(), 0.001);

    EXPECT_FALSE(wall.setWindowToWallRatio(0.3));
  }
}


// the following tests demonstrate issues with initialization

class SurfaceWorkspaceWatcher : public openstudio::WorkspaceWatcher {

 public:

  SurfaceWorkspaceWatcher(const Workspace& workspace)
    : WorkspaceWatcher(workspace)
  {}

  virtual void onObjectAdd(const WorkspaceObject& addedObject)
  {
    WorkspaceWatcher::onObjectAdd(addedObject);

    EXPECT_EQ(IddObjectType::OS_Surface, addedObject.iddObject().type().value());
    boost::optional<Surface> surface = addedObject.optionalCast<Surface>();
    ASSERT_TRUE(surface);
    ASSERT_NO_THROW(surface->vertices());
  }
};

TEST_F(ModelFixture, Surface_Initializer1)
{
  Model model;
  SurfaceWorkspaceWatcher watcher(model);
  EXPECT_FALSE(watcher.objectAdded());

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0,0,1));
  vertices.push_back(Point3d(0,0,0));
  vertices.push_back(Point3d(1,0,0));
  vertices.push_back(Point3d(1,0,1));
  Surface surface(vertices, model);
  EXPECT_TRUE(watcher.objectAdded());

  EXPECT_EQ(4u, surface.vertices().size());
}

TEST_F(ModelFixture, Surface_Initializer2)
{
  Model model;
  SurfaceWorkspaceWatcher watcher(model);
  EXPECT_FALSE(watcher.objectAdded());
  
  IdfObject idfObject(IddObjectType::OS_Surface);
  model.addObject(idfObject);
  EXPECT_TRUE(watcher.objectAdded());
  
  std::vector<Surface> surfaces = model.getModelObjects<Surface>();
  ASSERT_EQ(1u, surfaces.size());
  
  Surface surface = surfaces[0];
  EXPECT_EQ(0u, surface.vertices().size());

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0,0,1));
  vertices.push_back(Point3d(0,0,0));
  vertices.push_back(Point3d(1,0,0));
  vertices.push_back(Point3d(1,0,1));
  EXPECT_TRUE(surface.setVertices(vertices));
  EXPECT_EQ(4u, surface.vertices().size());
}


TEST_F(ModelFixture, Surface_SearchDistance)
{
  Model model;

  Construction construction(model);

  DefaultSurfaceConstructions defaultSurfaceConstructions(model);
  defaultSurfaceConstructions.setWallConstruction(construction);

  DefaultConstructionSet defaultConstructionSet(model);
  defaultConstructionSet.setDefaultExteriorSurfaceConstructions(defaultSurfaceConstructions);

  SpaceType spaceType(model);
  spaceType.setDefaultConstructionSet(defaultConstructionSet);

  Building building = model.getUniqueModelObject<Building>();
  building.setSpaceType(spaceType);
  building.setDefaultConstructionSet(defaultConstructionSet);

  BuildingStory buildingStory(model);
  buildingStory.setDefaultConstructionSet(defaultConstructionSet);

  Space space(model);
  space.setBuildingStory(buildingStory);
  space.setSpaceType(spaceType);
  space.setDefaultConstructionSet(defaultConstructionSet);

  boost::optional<std::pair<ConstructionBase, int> > test;

  Point3dVector points;
  points.push_back(Point3d(0, 0, 1));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(1, 0, 0));
  points.push_back(Point3d(1, 0, 1));
  Surface surface(points, model);
  EXPECT_EQ("Wall", surface.surfaceType());
  EXPECT_FALSE(surface.construction());
  test = surface.constructionWithSearchDistance();
  EXPECT_FALSE(test);

  surface.setConstruction(construction);
  EXPECT_TRUE(surface.construction());
  test = surface.constructionWithSearchDistance();
  ASSERT_TRUE(test);
  EXPECT_EQ(construction.handle(), test->first.handle());
  EXPECT_EQ(0, test->second);

  EXPECT_TRUE(surface.setSpace(space));

  EXPECT_TRUE(surface.construction());
  test = surface.constructionWithSearchDistance();
  ASSERT_TRUE(test);
  EXPECT_EQ(construction.handle(), test->first.handle());
  EXPECT_EQ(0, test->second);

  surface.resetConstruction();

  /// This space's default construction set, search distance = 1
  EXPECT_TRUE(surface.construction());
  test = surface.constructionWithSearchDistance();
  ASSERT_TRUE(test);
  EXPECT_EQ(construction.handle(), test->first.handle());
  EXPECT_EQ(1, test->second);

  space.resetDefaultConstructionSet();

  /// This space's space type's default construction set, search distance = 2
  EXPECT_TRUE(surface.construction());
  test = surface.constructionWithSearchDistance();
  ASSERT_TRUE(test);
  EXPECT_EQ(construction.handle(), test->first.handle());
  EXPECT_EQ(2, test->second);

  space.resetSpaceType();

  /// This space's building story's default construction set, search distance = 3
  EXPECT_TRUE(surface.construction());
  test = surface.constructionWithSearchDistance();
  ASSERT_TRUE(test);
  EXPECT_EQ(construction.handle(), test->first.handle());
  EXPECT_EQ(3, test->second);

  buildingStory.resetDefaultConstructionSet();

  /// This space's building's default construction set, search distance = 4
  EXPECT_TRUE(surface.construction());
  test = surface.constructionWithSearchDistance();
  ASSERT_TRUE(test);
  EXPECT_EQ(construction.handle(), test->first.handle());
  EXPECT_EQ(4, test->second);

  building.resetDefaultConstructionSet();

  /// This space's building's space type's default construction set, search distance = 5
  EXPECT_TRUE(surface.construction());
  test = surface.constructionWithSearchDistance();
  ASSERT_TRUE(test);
  EXPECT_EQ(construction.handle(), test->first.handle());
  EXPECT_EQ(5, test->second);

  building.resetSpaceType();

  // no more options
  EXPECT_FALSE(surface.construction());
  test = surface.constructionWithSearchDistance();
  EXPECT_FALSE(test);
}

TEST_F(ModelFixture, Surface_createAdjacentSurface){
  Model model;
  Space space1(model);

  Point3dVector points;
  points.push_back(Point3d(0, 10, 3));
  points.push_back(Point3d(0, 10, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(0, 0, 3));
  Surface surface1(points, model);
  surface1.setSpace(space1);

  points.clear();
  points.push_back(Point3d(0, 9, 2));
  points.push_back(Point3d(0, 9, 1));
  points.push_back(Point3d(0, 1, 1));
  points.push_back(Point3d(0, 1, 2));
  SubSurface subSurface1(points, model);
  subSurface1.setSurface(surface1);

  Space space2(model);
  boost::optional<Surface> surface2 = surface1.createAdjacentSurface(space2);

  ASSERT_TRUE(surface2);
  EXPECT_EQ(surface1.grossArea(), surface2->grossArea());
  EXPECT_EQ(surface1.netArea(), surface2->netArea());

  ASSERT_EQ(1u, surface2->subSurfaces().size());
  SubSurface subSurface2 = surface2->subSurfaces()[0];
  EXPECT_EQ(subSurface1.grossArea(), subSurface2.grossArea());
  EXPECT_EQ(subSurface1.netArea(), subSurface2.netArea());
}

TEST_F(ModelFixture, Surface_DeleteAdjacentSurface){
  Model model;
  Space space1(model);

  Point3dVector points;
  points.push_back(Point3d(0, 10, 3));
  points.push_back(Point3d(0, 10, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(0, 0, 3));
  Surface surface1(points, model);
  surface1.setSpace(space1);
  EXPECT_EQ("Outdoors", surface1.outsideBoundaryCondition());

  points.clear();
  points.push_back(Point3d(0, 9, 2));
  points.push_back(Point3d(0, 9, 1));
  points.push_back(Point3d(0, 1, 1));
  points.push_back(Point3d(0, 1, 2));
  SubSurface subSurface1(points, model);
  subSurface1.setSurface(surface1);

  Space space2(model);
  boost::optional<Surface> surface2 = surface1.createAdjacentSurface(space2);
  ASSERT_TRUE(surface2);
  ASSERT_EQ(1u, surface2->subSurfaces().size());
  SubSurface subSurface2 = surface2->subSurfaces()[0];

  EXPECT_EQ("Surface", surface1.outsideBoundaryCondition());
  ASSERT_TRUE(surface1.adjacentSurface());
  EXPECT_EQ(surface2->handle(), surface1.adjacentSurface()->handle());
  ASSERT_TRUE(subSurface1.adjacentSubSurface());
  EXPECT_EQ(subSurface2.handle(), subSurface1.adjacentSubSurface()->handle());

  std::vector<IdfObject> removed = surface2->remove();
  EXPECT_EQ(2u, removed.size());

  EXPECT_EQ("Outdoors", surface1.outsideBoundaryCondition());
  EXPECT_FALSE(surface1.adjacentSurface());
  EXPECT_FALSE(subSurface1.adjacentSubSurface());
}

TEST_F(ModelFixture, Surface_ChangeAdjacentSurface_SameSpace){
  Model model;
  Space space(model);

  Point3dVector points;
  points.push_back(Point3d(0, 10, 3));
  points.push_back(Point3d(0, 10, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(0, 0, 3));
  Surface surface1(points, model);
  surface1.setSpace(space);
  Surface surface2(points, model);
  surface2.setSpace(space);
  Surface surface3(points, model);
  surface3.setSpace(space);

  points.clear();
  points.push_back(Point3d(0, 9, 2));
  points.push_back(Point3d(0, 9, 1));
  points.push_back(Point3d(0, 1, 1));
  points.push_back(Point3d(0, 1, 2));
  SubSurface subSurface1(points, model);
  subSurface1.setSurface(surface1);
  SubSurface subSurface2(points, model);
  subSurface2.setSurface(surface2);
  SubSurface subSurface3(points, model);
  subSurface3.setSurface(surface3);

  EXPECT_EQ("Outdoors", surface1.outsideBoundaryCondition());
  EXPECT_FALSE(surface1.adjacentSurface());
  EXPECT_FALSE(subSurface1.adjacentSubSurface());
  EXPECT_EQ("Outdoors", surface2.outsideBoundaryCondition());
  EXPECT_FALSE(surface2.adjacentSurface());
  EXPECT_FALSE(subSurface2.adjacentSubSurface());
  EXPECT_EQ("Outdoors", surface3.outsideBoundaryCondition());
  EXPECT_FALSE(surface3.adjacentSurface());
  EXPECT_FALSE(subSurface3.adjacentSubSurface());

  // can set surface or sub surface adjacent to self
  EXPECT_TRUE(surface1.setAdjacentSurface(surface1));
  EXPECT_TRUE(subSurface1.setAdjacentSubSurface(subSurface1));
  EXPECT_EQ("Surface", surface1.outsideBoundaryCondition());
  ASSERT_TRUE(surface1.adjacentSurface());
  EXPECT_EQ(surface1.handle(), surface1.adjacentSurface()->handle());
  ASSERT_TRUE(subSurface1.adjacentSubSurface());
  EXPECT_EQ(subSurface1.handle(), subSurface1.adjacentSubSurface()->handle());
  EXPECT_EQ("Outdoors", surface2.outsideBoundaryCondition());
  EXPECT_FALSE(surface2.adjacentSurface());
  EXPECT_FALSE(subSurface2.adjacentSubSurface());
  EXPECT_EQ("Outdoors", surface3.outsideBoundaryCondition());
  EXPECT_FALSE(surface3.adjacentSurface());
  EXPECT_FALSE(subSurface3.adjacentSubSurface());

  // setting surface 1 adjacent to surface 2, does not set sub surfaces adjacent
  EXPECT_TRUE(surface1.setAdjacentSurface(surface2));
  EXPECT_EQ("Surface", surface1.outsideBoundaryCondition());
  ASSERT_TRUE(surface1.adjacentSurface());
  EXPECT_EQ(surface2.handle(), surface1.adjacentSurface()->handle());
  EXPECT_FALSE(subSurface1.adjacentSubSurface());
  EXPECT_EQ("Surface", surface2.outsideBoundaryCondition());
  ASSERT_TRUE(surface2.adjacentSurface());
  EXPECT_EQ(surface1.handle(), surface2.adjacentSurface()->handle());
  EXPECT_FALSE(subSurface2.adjacentSubSurface());
  EXPECT_EQ("Outdoors", surface3.outsideBoundaryCondition());
  EXPECT_FALSE(surface3.adjacentSurface());
  EXPECT_FALSE(subSurface3.adjacentSubSurface());

  // now can set sub surfaces adjacent
  EXPECT_TRUE(subSurface1.setAdjacentSubSurface(subSurface2));
  EXPECT_EQ("Surface", surface1.outsideBoundaryCondition());
  ASSERT_TRUE(surface1.adjacentSurface());
  EXPECT_EQ(surface2.handle(), surface1.adjacentSurface()->handle());
  ASSERT_TRUE(subSurface1.adjacentSubSurface());
  EXPECT_EQ(subSurface2.handle(), subSurface1.adjacentSubSurface()->handle());
  EXPECT_EQ("Surface", surface2.outsideBoundaryCondition());
  ASSERT_TRUE(surface2.adjacentSurface());
  EXPECT_EQ(surface1.handle(), surface2.adjacentSurface()->handle());
  ASSERT_TRUE(subSurface2.adjacentSubSurface());
  EXPECT_EQ(subSurface1.handle(), subSurface2.adjacentSubSurface()->handle());
  EXPECT_EQ("Outdoors", surface3.outsideBoundaryCondition());
  EXPECT_FALSE(surface3.adjacentSurface());
  EXPECT_FALSE(subSurface3.adjacentSubSurface());

  // setting surface 1 adjacent to surface 2, does not reset sub surfaces adjacent
  EXPECT_TRUE(surface1.setAdjacentSurface(surface2));
  EXPECT_EQ("Surface", surface1.outsideBoundaryCondition());
  ASSERT_TRUE(surface1.adjacentSurface());
  EXPECT_EQ(surface2.handle(), surface1.adjacentSurface()->handle());
  ASSERT_TRUE(subSurface1.adjacentSubSurface());
  EXPECT_EQ(subSurface2.handle(), subSurface1.adjacentSubSurface()->handle());
  EXPECT_EQ("Surface", surface2.outsideBoundaryCondition());
  ASSERT_TRUE(surface2.adjacentSurface());
  EXPECT_EQ(surface1.handle(), surface2.adjacentSurface()->handle());
  ASSERT_TRUE(subSurface2.adjacentSubSurface());
  EXPECT_EQ(subSurface1.handle(), subSurface2.adjacentSubSurface()->handle());
  EXPECT_EQ("Outdoors", surface3.outsideBoundaryCondition());
  EXPECT_FALSE(surface3.adjacentSurface());
  EXPECT_FALSE(subSurface3.adjacentSubSurface());

  // setting surface 1 adjacent to surface 3, undoes adjacencies with surface 2
  EXPECT_TRUE(surface1.setAdjacentSurface(surface3));
  EXPECT_EQ("Surface", surface1.outsideBoundaryCondition());
  ASSERT_TRUE(surface1.adjacentSurface());
  EXPECT_EQ(surface3.handle(), surface1.adjacentSurface()->handle());
  EXPECT_FALSE(subSurface1.adjacentSubSurface());
  EXPECT_EQ("Outdoors", surface2.outsideBoundaryCondition());
  EXPECT_FALSE(surface2.adjacentSurface());
  EXPECT_FALSE(subSurface2.adjacentSubSurface());
  EXPECT_EQ("Surface", surface3.outsideBoundaryCondition());  
  ASSERT_TRUE(surface3.adjacentSurface());
  EXPECT_EQ(surface1.handle(), surface3.adjacentSurface()->handle());
  EXPECT_FALSE(subSurface3.adjacentSubSurface());

  // setting surface 2 adjacent to surface 3, undoes adjacencies with surface 1
  EXPECT_TRUE(surface2.setAdjacentSurface(surface3));
  EXPECT_EQ("Outdoors", surface1.outsideBoundaryCondition());
  EXPECT_FALSE(surface1.adjacentSurface());
  EXPECT_EQ("Surface", surface2.outsideBoundaryCondition());
  ASSERT_TRUE(surface2.adjacentSurface());
  EXPECT_EQ(surface3.handle(), surface2.adjacentSurface()->handle());
  EXPECT_EQ("Surface", surface3.outsideBoundaryCondition());  
  ASSERT_TRUE(surface3.adjacentSurface());
  EXPECT_EQ(surface2.handle(), surface3.adjacentSurface()->handle());
}

TEST_F(ModelFixture, Surface_ChangeAdjacentSurface_DiffSpaces){
  Model model;
  Space space1(model);
  Space space2(model);
  Space space3(model);

  Point3dVector points;
  points.push_back(Point3d(0, 10, 3));
  points.push_back(Point3d(0, 10, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(0, 0, 3));
  Surface surface1(points, model);
  surface1.setSpace(space1);
  Surface surface2(points, model);
  surface2.setSpace(space2);
  Surface surface3(points, model);
  surface3.setSpace(space3);

  points.clear();
  points.push_back(Point3d(0, 9, 2));
  points.push_back(Point3d(0, 9, 1));
  points.push_back(Point3d(0, 1, 1));
  points.push_back(Point3d(0, 1, 2));
  SubSurface subSurface1(points, model);
  subSurface1.setSurface(surface1);
  SubSurface subSurface2(points, model);
  subSurface2.setSurface(surface2);
  SubSurface subSurface3(points, model);
  subSurface3.setSurface(surface3);

  EXPECT_EQ("Outdoors", surface1.outsideBoundaryCondition());
  EXPECT_FALSE(surface1.adjacentSurface());
  EXPECT_FALSE(subSurface1.adjacentSubSurface());
  EXPECT_EQ("Outdoors", surface2.outsideBoundaryCondition());
  EXPECT_FALSE(surface2.adjacentSurface());
  EXPECT_FALSE(subSurface2.adjacentSubSurface());
  EXPECT_EQ("Outdoors", surface3.outsideBoundaryCondition());
  EXPECT_FALSE(surface3.adjacentSurface());
  EXPECT_FALSE(subSurface3.adjacentSubSurface());

  // setting surface 1 adjacent to surface 2, does not set sub surfaces adjacent
  EXPECT_TRUE(surface1.setAdjacentSurface(surface2));
  EXPECT_EQ("Surface", surface1.outsideBoundaryCondition());
  ASSERT_TRUE(surface1.adjacentSurface());
  EXPECT_EQ(surface2.handle(), surface1.adjacentSurface()->handle());
  EXPECT_FALSE(subSurface1.adjacentSubSurface());
  EXPECT_EQ("Surface", surface2.outsideBoundaryCondition());
  ASSERT_TRUE(surface2.adjacentSurface());
  EXPECT_EQ(surface1.handle(), surface2.adjacentSurface()->handle());
  EXPECT_FALSE(subSurface2.adjacentSubSurface());
  EXPECT_EQ("Outdoors", surface3.outsideBoundaryCondition());
  EXPECT_FALSE(surface3.adjacentSurface());
  EXPECT_FALSE(subSurface3.adjacentSubSurface());

  // now can set sub surfaces adjacent
  EXPECT_TRUE(subSurface1.setAdjacentSubSurface(subSurface2));
  EXPECT_EQ("Surface", surface1.outsideBoundaryCondition());
  ASSERT_TRUE(surface1.adjacentSurface());
  EXPECT_EQ(surface2.handle(), surface1.adjacentSurface()->handle());
  ASSERT_TRUE(subSurface1.adjacentSubSurface());
  EXPECT_EQ(subSurface2.handle(), subSurface1.adjacentSubSurface()->handle());
  EXPECT_EQ("Surface", surface2.outsideBoundaryCondition());
  ASSERT_TRUE(surface2.adjacentSurface());
  EXPECT_EQ(surface1.handle(), surface2.adjacentSurface()->handle());
  ASSERT_TRUE(subSurface2.adjacentSubSurface());
  EXPECT_EQ(subSurface1.handle(), subSurface2.adjacentSubSurface()->handle());
  EXPECT_EQ("Outdoors", surface3.outsideBoundaryCondition());
  EXPECT_FALSE(surface3.adjacentSurface());
  EXPECT_FALSE(subSurface3.adjacentSubSurface());

  // setting surface 1 adjacent to surface 2, does not reset sub surfaces adjacent
  EXPECT_TRUE(surface1.setAdjacentSurface(surface2));
  EXPECT_EQ("Surface", surface1.outsideBoundaryCondition());
  ASSERT_TRUE(surface1.adjacentSurface());
  EXPECT_EQ(surface2.handle(), surface1.adjacentSurface()->handle());
  ASSERT_TRUE(subSurface1.adjacentSubSurface());
  EXPECT_EQ(subSurface2.handle(), subSurface1.adjacentSubSurface()->handle());
  EXPECT_EQ("Surface", surface2.outsideBoundaryCondition());
  ASSERT_TRUE(surface2.adjacentSurface());
  EXPECT_EQ(surface1.handle(), surface2.adjacentSurface()->handle());
  ASSERT_TRUE(subSurface2.adjacentSubSurface());
  EXPECT_EQ(subSurface1.handle(), subSurface2.adjacentSubSurface()->handle());
  EXPECT_EQ("Outdoors", surface3.outsideBoundaryCondition());
  EXPECT_FALSE(surface3.adjacentSurface());
  EXPECT_FALSE(subSurface3.adjacentSubSurface());

  // setting surface 1 adjacent to surface 3, undoes adjacencies with surface 2
  EXPECT_TRUE(surface1.setAdjacentSurface(surface3));
  EXPECT_EQ("Surface", surface1.outsideBoundaryCondition());
  ASSERT_TRUE(surface1.adjacentSurface());
  EXPECT_EQ(surface3.handle(), surface1.adjacentSurface()->handle());
  EXPECT_FALSE(subSurface1.adjacentSubSurface());
  EXPECT_EQ("Outdoors", surface2.outsideBoundaryCondition());
  EXPECT_FALSE(surface2.adjacentSurface());
  EXPECT_FALSE(subSurface2.adjacentSubSurface());
  EXPECT_EQ("Surface", surface3.outsideBoundaryCondition());  
  ASSERT_TRUE(surface3.adjacentSurface());
  EXPECT_EQ(surface1.handle(), surface3.adjacentSurface()->handle());
  EXPECT_FALSE(subSurface3.adjacentSubSurface());

  // setting surface 2 adjacent to surface 3, undoes adjacencies with surface 1
  EXPECT_TRUE(surface2.setAdjacentSurface(surface3));
  EXPECT_EQ("Outdoors", surface1.outsideBoundaryCondition());
  EXPECT_FALSE(surface1.adjacentSurface());
  EXPECT_EQ("Surface", surface2.outsideBoundaryCondition());
  ASSERT_TRUE(surface2.adjacentSurface());
  EXPECT_EQ(surface3.handle(), surface2.adjacentSurface()->handle());
  EXPECT_EQ("Surface", surface3.outsideBoundaryCondition());  
  ASSERT_TRUE(surface3.adjacentSurface());
  EXPECT_EQ(surface2.handle(), surface3.adjacentSurface()->handle());
}

TEST_F(ModelFixture, Surface_Intersect_CompletelyContained){
  Model model;
  Space space1(model);
  Space space2(model);

  Point3dVector points1;
  points1.push_back(Point3d(0,  0,  0));
  points1.push_back(Point3d(10, 0,  0));
  points1.push_back(Point3d(10, 10, 0));
  points1.push_back(Point3d(0,  10, 0));
  Surface surface1(points1, model);
  surface1.setSpace(space1);
  double surface1Area = surface1.grossArea();

  Point3dVector points2;
  points2.push_back(Point3d(4, 6, 0));
  points2.push_back(Point3d(6, 6, 0));
  points2.push_back(Point3d(6, 4, 0));
  points2.push_back(Point3d(4, 4, 0));
  Surface surface2(points2, model);
  surface2.setSpace(space2);
  double surface2Area = surface2.grossArea();

  Plane plane1(points1);
  Plane plane2(points2);
  EXPECT_TRUE(plane1.reverseEqual(plane2));
  EXPECT_TRUE(plane2.reverseEqual(plane1));
  EXPECT_FALSE(plane1.equal(plane2));

  EXPECT_EQ(1u, space1.surfaces().size());
  EXPECT_EQ(4u, surface1.vertices().size());
  EXPECT_EQ(1u, space2.surfaces().size());
  EXPECT_EQ(4u, surface2.vertices().size());

  EXPECT_TRUE(surface1.intersect(surface2));

  EXPECT_EQ(2u, space1.surfaces().size());
  EXPECT_EQ(4u, surface1.vertices().size());
  EXPECT_EQ(1u, space2.surfaces().size());
  EXPECT_EQ(4u, surface2.vertices().size());

  Point3dVector points;
  points.push_back(Point3d(4, 4, 0));
  points.push_back(Point3d(6, 4, 0));
  points.push_back(Point3d(6, 6, 0));
  points.push_back(Point3d(4, 6, 0));
  EXPECT_TRUE(circularEqual(surface1.vertices(), points));
  EXPECT_DOUBLE_EQ(surface2Area, surface1.grossArea());
  
  BOOST_FOREACH(const Surface& surface, space1.surfaces()){
    if (surface.handle() != surface1.handle()){
      points.clear();
      points.push_back(Point3d(0,  0, 0));
      points.push_back(Point3d(4,  6, 0));
      points.push_back(Point3d(6,  6, 0));
      points.push_back(Point3d(6,  4, 0));
      points.push_back(Point3d(4,  4, 0));
      points.push_back(Point3d(4,  6, 0));
      points.push_back(Point3d(0,  0, 0));
      points.push_back(Point3d(10, 0, 0));
      points.push_back(Point3d(10, 10, 0));
      points.push_back(Point3d(0,  10, 0));
      EXPECT_TRUE(circularEqual(surface.vertices(), points));
      EXPECT_DOUBLE_EQ(surface1Area, surface1.grossArea() + surface.grossArea());
      break;
    }
  }

  points.clear();
  points.push_back(Point3d(4, 6, 0));
  points.push_back(Point3d(6, 6, 0));
  points.push_back(Point3d(6, 4, 0));
  points.push_back(Point3d(4, 4, 0));
  EXPECT_TRUE(circularEqual(surface2.vertices(), points));
  EXPECT_DOUBLE_EQ(surface2Area, surface2.grossArea());
}

TEST_F(ModelFixture, Surface_Intersect_SameHeight_PartialOverlap){
  Model model;
  Space space1(model);
  Space space2(model);

  Point3dVector points1;
  points1.push_back(Point3d(0,  0,  0));
  points1.push_back(Point3d(10, 0,  0));
  points1.push_back(Point3d(10, 10, 0));
  points1.push_back(Point3d(0,  10, 0));
  Surface surface1(points1, model);
  surface1.setSpace(space1);
  double surface1Area = surface1.grossArea();

  Point3dVector points2;
  points2.push_back(Point3d(7,  10, 0));
  points2.push_back(Point3d(13, 10, 0));
  points2.push_back(Point3d(13,  0, 0));
  points2.push_back(Point3d(7,   0, 0));
  Surface surface2(points2, model);
  surface2.setSpace(space2);
  double surface2Area = surface2.grossArea();

  Plane plane1(points1);
  Plane plane2(points2);
  EXPECT_TRUE(plane1.reverseEqual(plane2));
  EXPECT_TRUE(plane2.reverseEqual(plane1));
  EXPECT_FALSE(plane1.equal(plane2));

  EXPECT_EQ(1u, space1.surfaces().size());
  EXPECT_EQ(4u, surface1.vertices().size());
  EXPECT_EQ(1u, space2.surfaces().size());
  EXPECT_EQ(4u, surface2.vertices().size());

  EXPECT_TRUE(surface1.intersect(surface2));

  EXPECT_EQ(2u, space1.surfaces().size());
  EXPECT_EQ(4u, surface1.vertices().size());
  EXPECT_EQ(2u, space2.surfaces().size());
  EXPECT_EQ(4u, surface2.vertices().size());

  Point3dVector points;
  points.push_back(Point3d(7,  0,  0));
  points.push_back(Point3d(10, 0,  0));
  points.push_back(Point3d(10, 10, 0));
  points.push_back(Point3d(7,  10, 0));
  EXPECT_TRUE(circularEqual(surface1.vertices(), points));
  
  BOOST_FOREACH(const Surface& surface, space1.surfaces()){
    if (surface.handle() != surface1.handle()){
      points.clear();
      points.push_back(Point3d(0, 0,  0));
      points.push_back(Point3d(7, 0,  0));
      points.push_back(Point3d(7, 10, 0));
      points.push_back(Point3d(0, 10, 0));
      EXPECT_TRUE(circularEqual(surface.vertices(), points));
      EXPECT_DOUBLE_EQ(surface1Area, surface1.grossArea() + surface.grossArea());
      break;
    }
  }

  points.clear();
  points.push_back(Point3d(7,  10, 0));
  points.push_back(Point3d(10, 10, 0));
  points.push_back(Point3d(10,  0, 0));
  points.push_back(Point3d(7,  0, 0));
  EXPECT_TRUE(circularEqual(surface2.vertices(), points));

  BOOST_FOREACH(const Surface& surface, space2.surfaces()){
    if (surface.handle() != surface2.handle()){
      points.clear();
      points.push_back(Point3d(10, 10, 0));
      points.push_back(Point3d(13, 10, 0));
      points.push_back(Point3d(13,  0, 0));
      points.push_back(Point3d(10,  0, 0));
      EXPECT_TRUE(circularEqual(surface.vertices(), points));
      EXPECT_DOUBLE_EQ(surface2Area, surface2.grossArea() + surface.grossArea());
      break;
    }
  }
}

TEST_F(ModelFixture, Surface_Intersect_DifferentHeight_PartialOverlap){
  Model model;
  Space space1(model);
  Space space2(model);

  Point3dVector points1;
  points1.push_back(Point3d(0,  0,  0));
  points1.push_back(Point3d(10, 0,  0));
  points1.push_back(Point3d(10, 10, 0));
  points1.push_back(Point3d(0,  10, 0));
  Surface surface1(points1, model);
  surface1.setSpace(space1);
  double surface1Area = surface1.grossArea();

  Point3dVector points2;
  points2.push_back(Point3d(7,  12, 0));
  points2.push_back(Point3d(13, 12, 0));
  points2.push_back(Point3d(13, -2, 0));
  points2.push_back(Point3d(7,  -2, 0));
  Surface surface2(points2, model);
  surface2.setSpace(space2);
  double surface2Area = surface2.grossArea();

  Plane plane1(points1);
  Plane plane2(points2);
  EXPECT_TRUE(plane1.reverseEqual(plane2));
  EXPECT_TRUE(plane2.reverseEqual(plane1));
  EXPECT_FALSE(plane1.equal(plane2));

  EXPECT_EQ(1u, space1.surfaces().size());
  EXPECT_EQ(4u, surface1.vertices().size());
  EXPECT_EQ(1u, space2.surfaces().size());
  EXPECT_EQ(4u, surface2.vertices().size());

  EXPECT_TRUE(surface1.intersect(surface2));

  EXPECT_EQ(2u, space1.surfaces().size());
  EXPECT_EQ(4u, surface1.vertices().size());
  EXPECT_EQ(2u, space2.surfaces().size());
  EXPECT_EQ(4u, surface2.vertices().size());

  Point3dVector points;
  points.push_back(Point3d(7,   0, 0));
  points.push_back(Point3d(10,  0, 0));
  points.push_back(Point3d(10, 10, 0));
  points.push_back(Point3d(7,  10, 0));
  EXPECT_TRUE(circularEqual(surface1.vertices(), points));
  
  BOOST_FOREACH(const Surface& surface, space1.surfaces()){
    if (surface.handle() != surface1.handle()){
      points.clear();
      points.push_back(Point3d(0, 0,  0));
      points.push_back(Point3d(7, 0,  0));
      points.push_back(Point3d(7, 10, 0));
      points.push_back(Point3d(0, 10, 0));
      EXPECT_TRUE(circularEqual(surface.vertices(), points));
      EXPECT_DOUBLE_EQ(surface1Area, surface1.grossArea() + surface.grossArea());
      break;
    }
  }

  points.clear();
  points.push_back(Point3d(7,  10, 0));
  points.push_back(Point3d(10, 10, 0));
  points.push_back(Point3d(10,  0, 0));
  points.push_back(Point3d(7,  0, 0));
  EXPECT_TRUE(circularEqual(surface2.vertices(), points));

  BOOST_FOREACH(const Surface& surface, space2.surfaces()){
    if (surface.handle() != surface2.handle()){
      points.clear();
      points.push_back(Point3d(7,  12, 0));
      points.push_back(Point3d(13, 12, 0));
      points.push_back(Point3d(13, -2, 0));
      points.push_back(Point3d(7,  -2, 0));
      points.push_back(Point3d(7,   0, 0));
      points.push_back(Point3d(10,  0, 0));
      points.push_back(Point3d(10, 10, 0));
      points.push_back(Point3d(7,  10, 0));
      EXPECT_TRUE(circularEqual(surface.vertices(), points));
      EXPECT_DOUBLE_EQ(surface2Area, surface2.grossArea() + surface.grossArea());
      break;
    }
  }
}

TEST_F(ModelFixture, Surface_Intersect_DifferentHeight_ShareOneEdge_PartialOverlap){
  Model model;
  Space space1(model);
  Space space2(model);

  Point3dVector points1;
  points1.push_back(Point3d(0,  0,  0));
  points1.push_back(Point3d(10, 0,  0));
  points1.push_back(Point3d(10, 10, 0));
  points1.push_back(Point3d(0,  10, 0));
  Surface surface1(points1, model);
  surface1.setSpace(space1);
  double surface1Area = surface1.grossArea();

  Point3dVector points2;
  points2.push_back(Point3d(7,  12, 0));
  points2.push_back(Point3d(13, 12, 0));
  points2.push_back(Point3d(13,  0, 0));
  points2.push_back(Point3d(7,   0, 0));
  Surface surface2(points2, model);
  surface2.setSpace(space2);
  double surface2Area = surface2.grossArea();

  Plane plane1(points1);
  Plane plane2(points2);
  EXPECT_TRUE(plane1.reverseEqual(plane2));
  EXPECT_TRUE(plane2.reverseEqual(plane1));
  EXPECT_FALSE(plane1.equal(plane2));

  EXPECT_EQ(1u, space1.surfaces().size());
  EXPECT_EQ(4u, surface1.vertices().size());
  EXPECT_EQ(1u, space2.surfaces().size());
  EXPECT_EQ(4u, surface2.vertices().size());

  EXPECT_TRUE(surface1.intersect(surface2));

  EXPECT_EQ(2u, space1.surfaces().size());
  EXPECT_EQ(4u, surface1.vertices().size());
  EXPECT_EQ(2u, space2.surfaces().size());
  EXPECT_EQ(4u, surface2.vertices().size());

  Point3dVector points;
  points.push_back(Point3d(7,   0, 0));
  points.push_back(Point3d(10,  0, 0));
  points.push_back(Point3d(10, 10, 0));
  points.push_back(Point3d(7,  10, 0));
  EXPECT_TRUE(circularEqual(surface1.vertices(), points));
  
  BOOST_FOREACH(const Surface& surface, space1.surfaces()){
    if (surface.handle() != surface1.handle()){
      points.clear();
      points.push_back(Point3d(0, 0,  0));
      points.push_back(Point3d(7, 0,  0));
      points.push_back(Point3d(7, 10, 0));
      points.push_back(Point3d(0, 10, 0));
      EXPECT_TRUE(circularEqual(surface.vertices(), points));
      EXPECT_DOUBLE_EQ(surface1Area, surface1.grossArea() + surface.grossArea());
      break;
    }
  }

  points.clear();
  points.push_back(Point3d(7,  10, 0));
  points.push_back(Point3d(10, 10, 0));
  points.push_back(Point3d(10,  0, 0));
  points.push_back(Point3d(7,  0, 0));
  EXPECT_TRUE(circularEqual(surface2.vertices(), points));

  BOOST_FOREACH(const Surface& surface, space2.surfaces()){
    if (surface.handle() != surface2.handle()){
      points.clear();
      points.push_back(Point3d(7,  12, 0));
      points.push_back(Point3d(13, 12, 0));
      points.push_back(Point3d(13,  0, 0));
      points.push_back(Point3d(10,  0, 0));
      points.push_back(Point3d(10, 10, 0));
      points.push_back(Point3d(7,  10, 0));
      EXPECT_TRUE(circularEqual(surface.vertices(), points));
      EXPECT_DOUBLE_EQ(surface2Area, surface2.grossArea() + surface.grossArea());
      break;
    }
  }
}

TEST_F(ModelFixture, Surface_Intersect_AdjacentEdges){
  Model model;
  Space space1(model);
  Space space2(model);

  Point3dVector points1;
  points1.push_back(Point3d(0, 0,  0));
  points1.push_back(Point3d(5, 0,  0));
  points1.push_back(Point3d(5, 10, 0));
  points1.push_back(Point3d(0, 10, 0));
  Surface surface1(points1, model);
  surface1.setSpace(space1);
  double surface1Area = surface1.grossArea();

  Point3dVector points2;
  points2.push_back(Point3d(5,  10, 0));
  points2.push_back(Point3d(10, 10, 0));
  points2.push_back(Point3d(10,  0, 0));
  points2.push_back(Point3d(5,   0, 0));
  Surface surface2(points2, model);
  surface2.setSpace(space2);
  double surface2Area = surface2.grossArea();

  Plane plane1(points1);
  Plane plane2(points2);
  EXPECT_TRUE(plane1.reverseEqual(plane2));
  EXPECT_TRUE(plane2.reverseEqual(plane1));
  EXPECT_FALSE(plane1.equal(plane2));

  EXPECT_EQ(1u, space1.surfaces().size());
  EXPECT_EQ(4u, surface1.vertices().size());
  EXPECT_EQ(1u, space2.surfaces().size());
  EXPECT_EQ(4u, surface2.vertices().size());

  EXPECT_FALSE(surface1.intersect(surface2));

  EXPECT_EQ(1u, space1.surfaces().size());
  EXPECT_EQ(4u, surface1.vertices().size());
  EXPECT_EQ(1u, space2.surfaces().size());
  EXPECT_EQ(4u, surface2.vertices().size());

  Point3dVector points;

  points.push_back(Point3d(0, 0,  0));
  points.push_back(Point3d(5, 0,  0));
  points.push_back(Point3d(5, 10, 0));
  points.push_back(Point3d(0, 10, 0));
  EXPECT_TRUE(circularEqual(surface1.vertices(), points));
  EXPECT_DOUBLE_EQ(surface1Area, surface1.grossArea());

  points.clear();
  points.push_back(Point3d(5,  10, 0));
  points.push_back(Point3d(10, 10, 0));
  points.push_back(Point3d(10,  0, 0));
  points.push_back(Point3d(5,   0, 0));
  EXPECT_TRUE(circularEqual(surface2.vertices(), points));
  EXPECT_DOUBLE_EQ(surface2Area, surface2.grossArea());
}

TEST_F(ModelFixture, Surface_Intersect_SameSize){
  Model model;
  Space space1(model);
  Space space2(model);

  Point3dVector points1;
  points1.push_back(Point3d(0,  0,  0));
  points1.push_back(Point3d(10, 0,  0));
  points1.push_back(Point3d(10, 10, 0));
  points1.push_back(Point3d(0,  10, 0));
  Surface surface1(points1, model);
  surface1.setSpace(space1);
  double surface1Area = surface1.grossArea();

  Point3dVector points2;
  points2.push_back(Point3d(0,  10, 0));
  points2.push_back(Point3d(10, 10, 0));
  points2.push_back(Point3d(10,  0, 0));
  points2.push_back(Point3d(0,   0, 0));
  Surface surface2(points2, model);
  surface2.setSpace(space2);
  double surface2Area = surface2.grossArea();

  Plane plane1(points1);
  Plane plane2(points2);
  EXPECT_TRUE(plane1.reverseEqual(plane2));
  EXPECT_TRUE(plane2.reverseEqual(plane1));
  EXPECT_FALSE(plane1.equal(plane2));

  EXPECT_EQ(1u, space1.surfaces().size());
  EXPECT_EQ(4u, surface1.vertices().size());
  EXPECT_EQ(1u, space2.surfaces().size());
  EXPECT_EQ(4u, surface2.vertices().size());

  EXPECT_FALSE(surface1.intersect(surface2));

  EXPECT_EQ(1u, space1.surfaces().size());
  EXPECT_EQ(4u, surface1.vertices().size());
  EXPECT_EQ(1u, space2.surfaces().size());
  EXPECT_EQ(4u, surface2.vertices().size());

  Point3dVector points;

  points.push_back(Point3d(0,   0, 0));
  points.push_back(Point3d(10,  0, 0));
  points.push_back(Point3d(10, 10, 0));
  points.push_back(Point3d(0,  10, 0));
  EXPECT_TRUE(circularEqual(surface1.vertices(), points));
  EXPECT_DOUBLE_EQ(surface1Area, surface1.grossArea());

  points.clear();
  points.push_back(Point3d(0,  10, 0));
  points.push_back(Point3d(10, 10, 0));
  points.push_back(Point3d(10,  0, 0));
  points.push_back(Point3d(0,   0, 0));
  EXPECT_TRUE(circularEqual(surface2.vertices(), points));
  EXPECT_DOUBLE_EQ(surface2Area, surface2.grossArea());
}


TEST_F(ModelFixture, Surface_Intersect_CutIntoTwo){
  Model model;
  Space space1(model);
  Space space2(model);

  Point3dVector points1;
  points1.push_back(Point3d(0,  0,  0));
  points1.push_back(Point3d(10, 0,  0));
  points1.push_back(Point3d(10, 10, 0));
  points1.push_back(Point3d(0,  10, 0));
  Surface surface1(points1, model);
  surface1.setSpace(space1);
  double surface1Area = surface1.grossArea();

  Point3dVector points2;
  points2.push_back(Point3d(3, 10, 0));
  points2.push_back(Point3d(7, 10, 0));
  points2.push_back(Point3d(7,  0, 0));
  points2.push_back(Point3d(3,  0, 0));
  Surface surface2(points2, model);
  surface2.setSpace(space2);
  double surface2Area = surface2.grossArea();

  Plane plane1(points1);
  Plane plane2(points2);
  EXPECT_TRUE(plane1.reverseEqual(plane2));
  EXPECT_TRUE(plane2.reverseEqual(plane1));
  EXPECT_FALSE(plane1.equal(plane2));

  EXPECT_EQ(1u, space1.surfaces().size());
  EXPECT_EQ(4u, surface1.vertices().size());
  EXPECT_EQ(1u, space2.surfaces().size());
  EXPECT_EQ(4u, surface2.vertices().size());

  EXPECT_TRUE(surface1.intersect(surface2));

  EXPECT_EQ(3u, space1.surfaces().size());
  EXPECT_EQ(4u, surface1.vertices().size());
  EXPECT_EQ(1u, space2.surfaces().size());
  EXPECT_EQ(4u, surface2.vertices().size());

  Point3dVector points;

  // intersecting piece is assigned back to surface one
  points.push_back(Point3d(3,  0, 0));
  points.push_back(Point3d(7,  0, 0));
  points.push_back(Point3d(7, 10, 0));
  points.push_back(Point3d(3, 10, 0));
  EXPECT_TRUE(circularEqual(surface1.vertices(), points));
  EXPECT_DOUBLE_EQ(surface2Area, surface1.grossArea());

  BOOST_FOREACH(const Surface& surface, space1.surfaces()){
    if (surface.handle() != surface1.handle()){
      bool test = false;

      points.clear();
      points.push_back(Point3d(0,  0, 0));
      points.push_back(Point3d(3,  0, 0));
      points.push_back(Point3d(3, 10, 0));
      points.push_back(Point3d(0, 10, 0));
      test = circularEqual(surface.vertices(), points);

      points.clear();
      points.push_back(Point3d(7,  0, 0));
      points.push_back(Point3d(10,  0, 0));
      points.push_back(Point3d(10, 10, 0));
      points.push_back(Point3d(7, 10, 0));
      test = (test || circularEqual(surface.vertices(), points));
      EXPECT_TRUE(test);

      EXPECT_DOUBLE_EQ(3*10, surface.grossArea());
    }
  }

  points.clear();
  points.push_back(Point3d(3, 10, 0));
  points.push_back(Point3d(7, 10, 0));
  points.push_back(Point3d(7,  0, 0));
  points.push_back(Point3d(3,  0, 0));
  EXPECT_TRUE(circularEqual(surface2.vertices(), points));
  EXPECT_DOUBLE_EQ(surface2Area, surface2.grossArea());
}

TEST_F(ModelFixture, Surface_Intersect_DifferentPlane){
  Model model;
  Space space1(model);
  Space space2(model);

  Point3dVector points1;
  points1.push_back(Point3d(0,  0,  0));
  points1.push_back(Point3d(10, 0,  0));
  points1.push_back(Point3d(10, 10, 0));
  points1.push_back(Point3d(0,  10, 0));
  Surface surface1(points1, model);
  surface1.setSpace(space1);

  Point3dVector points2;
  points2.push_back(Point3d(7,  12, 1));
  points2.push_back(Point3d(13, 12, 1));
  points2.push_back(Point3d(13,  0, 1));
  points2.push_back(Point3d(7,   0, 1));
  Surface surface2(points2, model);
  surface2.setSpace(space2);

  Plane plane1(points1);
  Plane plane2(points2);
  EXPECT_FALSE(plane1.reverseEqual(plane2));
  EXPECT_FALSE(plane2.reverseEqual(plane1));
  EXPECT_FALSE(plane1.equal(plane2));

  EXPECT_EQ(1u, space1.surfaces().size());
  EXPECT_EQ(4u, surface1.vertices().size());
  EXPECT_EQ(1u, space2.surfaces().size());
  EXPECT_EQ(4u, surface2.vertices().size());

  EXPECT_FALSE(surface1.intersect(surface2));

  EXPECT_EQ(1u, space1.surfaces().size());
  EXPECT_EQ(4u, surface1.vertices().size());
  EXPECT_EQ(1u, space2.surfaces().size());
  EXPECT_EQ(4u, surface2.vertices().size());
}

TEST_F(ModelFixture, Surface_Intersect_SameOutwardNormal){
  Model model;
  Space space1(model);
  Space space2(model);

  Point3dVector points1;
  points1.push_back(Point3d(0,  0,  0));
  points1.push_back(Point3d(10, 0,  0));
  points1.push_back(Point3d(10, 10, 0));
  points1.push_back(Point3d(0,  10, 0));
  Surface surface1(points1, model);
  surface1.setSpace(space1);

  Point3dVector points2;
  points2.push_back(Point3d(7,   0, 0));
  points2.push_back(Point3d(13,  0, 0));
  points2.push_back(Point3d(13, 12, 0));
  points2.push_back(Point3d(7,  12, 0));
  Surface surface2(points2, model);
  surface2.setSpace(space2);

  Plane plane1(points1);
  Plane plane2(points2);
  EXPECT_FALSE(plane1.reverseEqual(plane2));
  EXPECT_FALSE(plane2.reverseEqual(plane1));
  EXPECT_TRUE(plane1.equal(plane2));

  EXPECT_EQ(1u, space1.surfaces().size());
  EXPECT_EQ(4u, surface1.vertices().size());
  EXPECT_EQ(1u, space2.surfaces().size());
  EXPECT_EQ(4u, surface2.vertices().size());

  EXPECT_FALSE(surface1.intersect(surface2));

  EXPECT_EQ(1u, space1.surfaces().size());
  EXPECT_EQ(4u, surface1.vertices().size());
  EXPECT_EQ(1u, space2.surfaces().size());
  EXPECT_EQ(4u, surface2.vertices().size());
}

TEST_F(ModelFixture, Surface_Intersect_SameSpace){
  Model model;
  Space space(model);

  Point3dVector points1;
  points1.push_back(Point3d(0,  0,  0));
  points1.push_back(Point3d(10, 0,  0));
  points1.push_back(Point3d(10, 10, 0));
  points1.push_back(Point3d(0,  10, 0));
  Surface surface1(points1, model);
  surface1.setSpace(space);

  Point3dVector points2;
  points2.push_back(Point3d(7,  12, 0));
  points2.push_back(Point3d(13, 12, 0));
  points2.push_back(Point3d(13,  0, 0));
  points2.push_back(Point3d(7,   0, 0));
  Surface surface2(points2, model);
  surface2.setSpace(space);

  Plane plane1(points1);
  Plane plane2(points2);
  EXPECT_TRUE(plane1.reverseEqual(plane2));
  EXPECT_TRUE(plane2.reverseEqual(plane1));
  EXPECT_FALSE(plane1.equal(plane2));

  EXPECT_EQ(2u, space.surfaces().size());
  EXPECT_EQ(4u, surface1.vertices().size());
  EXPECT_EQ(4u, surface2.vertices().size());

  EXPECT_FALSE(surface1.intersect(surface2));
}

TEST_F(ModelFixture, Surface_Intersect_AdjacentSurface){
  Model model;
  Space space(model);

  Point3dVector points1;
  points1.push_back(Point3d(0,  0,  0));
  points1.push_back(Point3d(10, 0,  0));
  points1.push_back(Point3d(10, 10, 0));
  points1.push_back(Point3d(0,  10, 0));
  Surface surface1(points1, model);
  surface1.setSpace(space);

  Point3dVector points2;
  points2.push_back(Point3d(7,  12, 0));
  points2.push_back(Point3d(13, 12, 0));
  points2.push_back(Point3d(13,  0, 0));
  points2.push_back(Point3d(7,   0, 0));
  Surface surface2(points2, model);
  surface2.setSpace(space);

  EXPECT_TRUE(surface1.setAdjacentSurface(surface2));

  Plane plane1(points1);
  Plane plane2(points2);
  EXPECT_TRUE(plane1.reverseEqual(plane2));
  EXPECT_TRUE(plane2.reverseEqual(plane1));
  EXPECT_FALSE(plane1.equal(plane2));

  EXPECT_EQ(2u, space.surfaces().size());
  EXPECT_EQ(4u, surface1.vertices().size());
  EXPECT_EQ(4u, surface2.vertices().size());

  EXPECT_FALSE(surface1.intersect(surface2));
}

TEST_F(ModelFixture, Surface_Intersect_WithSubSurfaces){
  Model model;
  Space space(model);

  Point3dVector points1;
  points1.push_back(Point3d(0,  0,  0));
  points1.push_back(Point3d(10, 0,  0));
  points1.push_back(Point3d(10, 10, 0));
  points1.push_back(Point3d(0,  10, 0));
  Surface surface1(points1, model);
  surface1.setSpace(space);

  Point3dVector points2;
  points2.push_back(Point3d(7,  12, 0));
  points2.push_back(Point3d(13, 12, 0));
  points2.push_back(Point3d(13,  0, 0));
  points2.push_back(Point3d(7,   0, 0));
  Surface surface2(points2, model);
  surface2.setSpace(space);

  Point3dVector points3;
  points3.push_back(Point3d(2, 2, 0));
  points3.push_back(Point3d(8, 2, 0));
  points3.push_back(Point3d(8, 8, 0));
  points3.push_back(Point3d(2, 8, 0));
  SubSurface subSurface1(points3, model);
  subSurface1.setSurface(surface1);

  Plane plane1(points1);
  Plane plane2(points2);
  EXPECT_TRUE(plane1.reverseEqual(plane2));
  EXPECT_TRUE(plane2.reverseEqual(plane1));
  EXPECT_FALSE(plane1.equal(plane2));

  EXPECT_EQ(2u, space.surfaces().size());
  EXPECT_EQ(4u, surface1.vertices().size());
  EXPECT_EQ(4u, surface2.vertices().size());

  EXPECT_FALSE(surface1.intersect(surface2));
}

TEST_F(ModelFixture, Surface_Intersect_Complex){
  Model model;
  Space space1(model);
  Space space2(model);

  Point3dVector points1;
  points1.push_back(Point3d(0,  0,  0));
  points1.push_back(Point3d(10, 0,  0));
  points1.push_back(Point3d(10, 10, 0));
  points1.push_back(Point3d(0,  10, 0));
  Surface surface1(points1, model);
  surface1.setSpace(space1);
  double surface1Area = surface1.grossArea();

  Point3dVector points2;
  points2.push_back(Point3d(5, 15, 0));
  points2.push_back(Point3d(8, 15, 0));
  points2.push_back(Point3d(8,  8, 0));
  points2.push_back(Point3d(15, 8, 0));
  points2.push_back(Point3d(15, 5, 0));
  points2.push_back(Point3d(8,  5, 0));
  points2.push_back(Point3d(8, -5, 0));
  points2.push_back(Point3d(5, -5, 0));
  Surface surface2(points2, model);
  surface2.setSpace(space2);
  double surface2Area = surface2.grossArea();

  Plane plane1(points1);
  Plane plane2(points2);
  EXPECT_TRUE(plane1.reverseEqual(plane2));
  EXPECT_TRUE(plane2.reverseEqual(plane1));
  EXPECT_FALSE(plane1.equal(plane2));

  EXPECT_EQ(1u, space1.surfaces().size());
  EXPECT_EQ(4u, surface1.vertices().size());
  EXPECT_EQ(1u, space2.surfaces().size());
  EXPECT_EQ(8u, surface2.vertices().size());

  EXPECT_TRUE(surface1.intersect(surface2));

  EXPECT_EQ(4u, space1.surfaces().size());
  EXPECT_EQ(8u, surface1.vertices().size());
  EXPECT_EQ(4u, space2.surfaces().size());
  EXPECT_EQ(8u, surface2.vertices().size());

  Point3dVector points;
  points.push_back(Point3d(5,  0, 0));
  points.push_back(Point3d(8,  0, 0));
  points.push_back(Point3d(8,  5, 0));
  points.push_back(Point3d(10, 5, 0));
  points.push_back(Point3d(10, 8, 0));
  points.push_back(Point3d(8,  8, 0));
  points.push_back(Point3d(8, 10, 0));
  points.push_back(Point3d(5, 10, 0));
  EXPECT_TRUE(circularEqual(surface1.vertices(), points));

  BOOST_FOREACH(const Surface& surface, space1.surfaces()){
    if (surface.handle() != surface1.handle()){
      bool test = false;

      points.clear();
      points.push_back(Point3d(0,   0, 0));
      points.push_back(Point3d(5,   0, 0));
      points.push_back(Point3d(5,  10, 0));
      points.push_back(Point3d(0,  10, 0));
      test = circularEqual(surface.vertices(), points);

      points.clear();
      points.push_back(Point3d(8,  0, 0));
      points.push_back(Point3d(10, 0, 0));
      points.push_back(Point3d(10, 5, 0));
      points.push_back(Point3d(8,  5, 0));
      test = (test || circularEqual(surface.vertices(), points));

      points.clear();
      points.push_back(Point3d(8,   8, 0));
      points.push_back(Point3d(10,  8, 0));
      points.push_back(Point3d(10, 10, 0));
      points.push_back(Point3d(8,  10, 0));
      test = (test || circularEqual(surface.vertices(), points));

      EXPECT_TRUE(test);
    }
  }

  points.clear();
  points.push_back(Point3d(5, 10, 0));
  points.push_back(Point3d(8, 10, 0));
  points.push_back(Point3d(8,  8, 0));
  points.push_back(Point3d(10, 8, 0));
  points.push_back(Point3d(10, 5, 0));
  points.push_back(Point3d(8,  5, 0));
  points.push_back(Point3d(8,  0, 0));
  points.push_back(Point3d(5,  0, 0));
  EXPECT_TRUE(circularEqual(surface2.vertices(), points));

  BOOST_FOREACH(const Surface& surface, space2.surfaces()){
    if (surface.handle() != surface2.handle()){
      bool test = false;

      points.clear();
      points.push_back(Point3d(5, 15, 0));
      points.push_back(Point3d(8, 15, 0));
      points.push_back(Point3d(8, 10, 0));
      points.push_back(Point3d(5, 10, 0));
      test = circularEqual(surface.vertices(), points);

      points.clear();
      points.push_back(Point3d(10, 8, 0));
      points.push_back(Point3d(15, 8, 0));
      points.push_back(Point3d(15, 5, 0));
      points.push_back(Point3d(10, 5, 0));
      test = (test || circularEqual(surface.vertices(), points));

      points.clear();
      points.push_back(Point3d(5,  0, 0));
      points.push_back(Point3d(8,  0, 0));
      points.push_back(Point3d(8, -5, 0));
      points.push_back(Point3d(5, -5, 0));
      test = (test || circularEqual(surface.vertices(), points));

      EXPECT_TRUE(test);
    }
  }
}

TEST_F(ModelFixture, Surface_Intersect_UShape){
  Model model;
  Space space1(model);
  Space space2(model);

  Point3dVector points1;
  points1.push_back(Point3d(0,  0,  0));
  points1.push_back(Point3d(10, 0,  0));
  points1.push_back(Point3d(10, 10, 0));
  points1.push_back(Point3d(0,  10, 0));
  Surface surface1(points1, model);
  surface1.setSpace(space1);
  double surface1Area = surface1.grossArea();

  Point3dVector points2;
  points2.push_back(Point3d(2, 15, 0));
  points2.push_back(Point3d(8, 15, 0));
  points2.push_back(Point3d(8,  8, 0));
  points2.push_back(Point3d(6,  8, 0));
  points2.push_back(Point3d(6, 12, 0));
  points2.push_back(Point3d(4, 12, 0));
  points2.push_back(Point3d(4,  8, 0));
  points2.push_back(Point3d(2,  8, 0));
  Surface surface2(points2, model);
  surface2.setSpace(space2);
  double surface2Area = surface2.grossArea();

  Plane plane1(points1);
  Plane plane2(points2);
  EXPECT_TRUE(plane1.reverseEqual(plane2));
  EXPECT_TRUE(plane2.reverseEqual(plane1));
  EXPECT_FALSE(plane1.equal(plane2));

  EXPECT_EQ(1u, space1.surfaces().size());
  EXPECT_EQ(4u, surface1.vertices().size());
  EXPECT_EQ(1u, space2.surfaces().size());
  EXPECT_EQ(8u, surface2.vertices().size());

  EXPECT_TRUE(surface1.intersect(surface2));

  EXPECT_EQ(3u, space1.surfaces().size());
  EXPECT_EQ(4u, surface1.vertices().size());
  EXPECT_EQ(3u, space2.surfaces().size());
  EXPECT_EQ(4u, surface2.vertices().size());

  BOOST_FOREACH(const Surface& surface, space1.surfaces()){
    bool test = false;

    Point3dVector points;
    points.push_back(Point3d(2,  8, 0));
    points.push_back(Point3d(4,  8, 0));
    points.push_back(Point3d(4, 10, 0));
    points.push_back(Point3d(2, 10, 0));
    test = circularEqual(surface.vertices(), points);

    points.clear();
    points.push_back(Point3d(6,  8, 0));
    points.push_back(Point3d(8,  8, 0));
    points.push_back(Point3d(8, 10, 0));
    points.push_back(Point3d(6, 10, 0));
    test = (test || circularEqual(surface.vertices(), points));

    points.clear();
    points.push_back(Point3d(0,  0, 0));
    points.push_back(Point3d(10, 0, 0));
    points.push_back(Point3d(10,10, 0));
    points.push_back(Point3d(8, 10, 0));
    points.push_back(Point3d(8,  8, 0));
    points.push_back(Point3d(6,  8, 0));
    points.push_back(Point3d(6, 10, 0));
    points.push_back(Point3d(4, 10, 0));
    points.push_back(Point3d(4,  8, 0));
    points.push_back(Point3d(2,  8, 0));
    points.push_back(Point3d(2, 10, 0));
    points.push_back(Point3d(0, 10, 0));
    test = (test || circularEqual(surface.vertices(), points));
    EXPECT_TRUE(test);
  }

  
  BOOST_FOREACH(const Surface& surface, space2.surfaces()){
    bool test = false;

    Point3dVector points;
    points.push_back(Point3d(2, 10, 0));
    points.push_back(Point3d(4, 10, 0));
    points.push_back(Point3d(4,  8, 0));
    points.push_back(Point3d(2,  8, 0));
    test = circularEqual(surface.vertices(), points);

    points.clear();
    points.push_back(Point3d(6, 10, 0));
    points.push_back(Point3d(8, 10, 0));
    points.push_back(Point3d(8,  8, 0));
    points.push_back(Point3d(6,  8, 0));
    test = (test || circularEqual(surface.vertices(), points));

    points.clear();
    points.push_back(Point3d(2, 15, 0));
    points.push_back(Point3d(8, 15, 0));
    points.push_back(Point3d(8, 10, 0));
    points.push_back(Point3d(6, 10, 0));
    points.push_back(Point3d(6, 12, 0));
    points.push_back(Point3d(4, 12, 0));
    points.push_back(Point3d(4, 10, 0));
    points.push_back(Point3d(2, 10, 0));
    test = (test || circularEqual(surface.vertices(), points));
    EXPECT_TRUE(test);
  }
}

TEST_F(ModelFixture, Surface_Figure8_SameSense){

  Model model;
  Space space(model);

  Point3dVector points;
  points.push_back(Point3d(10, 0,  0));
  points.push_back(Point3d(20, 0,  0));
  points.push_back(Point3d(20, 10, 0));
  points.push_back(Point3d(10, 10, 0));
  points.push_back(Point3d(10, 20, 0));
  points.push_back(Point3d(0,  20, 0));
  points.push_back(Point3d(0,  10, 0));
  points.push_back(Point3d(10, 10, 0));
  points.push_back(Point3d(10,  5, 0));

  EXPECT_NO_THROW(Surface(points, model));
}

TEST_F(ModelFixture, Surface_Figure8_OppositeSense){

  Model model;
  Space space(model);

  Point3dVector points;
  points.push_back(Point3d(10, 0,  0));
  points.push_back(Point3d(20, 0,  0));
  points.push_back(Point3d(20, 10, 0));
  points.push_back(Point3d(10, 10, 0));
  points.push_back(Point3d(0,  10, 0));
  points.push_back(Point3d(0,  20, 0));
  points.push_back(Point3d(10, 20, 0));
  points.push_back(Point3d(10, 10, 0));
  points.push_back(Point3d(10,  5, 0));
  
  EXPECT_THROW(Surface(points, model), openstudio::Exception);
}

TEST_F(ModelFixture, GroundSurface)
{
  Model model;

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0,1,0));
  vertices.push_back(Point3d(1,1,0));
  vertices.push_back(Point3d(1,0,0));
  vertices.push_back(Point3d(0,0,0));

  Surface surface(vertices, model);
  EXPECT_EQ("Ground", surface.outsideBoundaryCondition());
  EXPECT_TRUE(surface.isGroundSurface());
  EXPECT_TRUE(surface.setOutsideBoundaryCondition("GroundFCfactorMethod"));
  EXPECT_EQ("GroundFCfactorMethod", surface.outsideBoundaryCondition());
  EXPECT_TRUE(surface.isGroundSurface());
}

