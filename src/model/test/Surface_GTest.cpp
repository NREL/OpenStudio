/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../BuildingStory.hpp"
#include "../BuildingStory_Impl.hpp"
#include "../FoundationKiva.hpp"
#include "../Space.hpp"
#include "../Space_Impl.hpp"
#include "../Surface.hpp"
#include "../Surface_Impl.hpp"
#include "../SubSurface.hpp"
#include "../SubSurface_Impl.hpp"
#include "../SurfacePropertyConvectionCoefficients.hpp"
#include "../ResourceObject.hpp"
#include "../ResourceObject_Impl.hpp"
#include "../ComponentData.hpp"
#include "../ComponentData_Impl.hpp"
#include "../ConstructionBase.hpp"
#include "../ConstructionBase_Impl.hpp"
#include "../LayeredConstruction.hpp"
#include "../LayeredConstruction_Impl.hpp"
#include "../Construction.hpp"
#include "../Construction_Impl.hpp"
#include "../Material.hpp"
#include "../Material_Impl.hpp"
#include "../AirGap.hpp"
#include "../AirGap_Impl.hpp"
#include "../AirWallMaterial.hpp"
#include "../AirWallMaterial_Impl.hpp"
#include "../StandardOpaqueMaterial.hpp"
#include "../StandardOpaqueMaterial_Impl.hpp"
#include "../DefaultConstructionSet.hpp"
#include "../DefaultConstructionSet_Impl.hpp"
#include "../DefaultSurfaceConstructions.hpp"
#include "../DefaultSurfaceConstructions_Impl.hpp"
#include "../SpaceType.hpp"
#include "../SpaceType_Impl.hpp"
#include "../DaylightingDeviceShelf.hpp"
#include "../InteriorPartitionSurface.hpp"
#include "../InteriorPartitionSurfaceGroup.hpp"
#include "../ShadingSurface.hpp"
#include "../ShadingSurfaceGroup.hpp"
#include "../SurfacePropertyOtherSideCoefficients.hpp"
#include "../SurfacePropertyOtherSideConditionsModel.hpp"

#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceWatcher.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/OS_Surface_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/geometry/Geometry.hpp"
#include "../../utilities/geometry/Point3d.hpp"
#include "../../utilities/geometry/Vector3d.hpp"

#include "../../utilities/math/FloatCompare.hpp"

#include "../../utilities/core/Finder.hpp"

#include "../../utilities/core/Assert.hpp"

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/math/constants/constants.hpp>

using namespace openstudio;
using namespace openstudio::model;
using std::string;
using boost::to_upper_copy;

double triangulatedArea(const std::vector<std::vector<Point3d>>& triangulation) {
  double result = 0;
  for (const std::vector<Point3d>& triangle : triangulation) {
    if (3u == triangle.size()) {
      boost::optional<double> area = getArea(triangle);
      if (area) {
        result += *area;
      }
    }
  }
  return result;
}

TEST_F(ModelFixture, Surface_SetVertices) {
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
  for (unsigned i = 0; i < points.size(); ++i) {
    EXPECT_EQ(points[i], testPoints[i]);
  }

  // set will fail, object is unchanged
  EXPECT_FALSE(surface.setVertices(Point3dVector()));
  EXPECT_EQ("RoofCeiling", surface.surfaceType());
  EXPECT_EQ("Outdoors", surface.outsideBoundaryCondition());

  testPoints = surface.vertices();
  ASSERT_EQ(points.size(), testPoints.size());
  for (unsigned i = 0; i < points.size(); ++i) {
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
  for (unsigned i = 0; i < points.size(); ++i) {
    EXPECT_EQ(points[i], testPoints[i]);
  }

  // u shape (concave) with 5 unit area, includes collinear points
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
  for (unsigned i = 0; i < points.size(); ++i) {
    EXPECT_EQ(points[i], testPoints[i]);
  }
}

TEST_F(ModelFixture, Surface_Initialization) {
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

TEST_F(ModelFixture, Surface_Area) {
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
  EXPECT_EQ(1.0, triangulatedArea(surface.triangulation()));

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
  EXPECT_EQ(1.0, triangulatedArea(surface.triangulation()));

  // u shape (concave) with 5 unit area, includes collinear points
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
  EXPECT_EQ(5.0, triangulatedArea(surface.triangulation()));
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

  for (const Surface& surface : surfaces){
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

    // check that we can get interior and exterior visible absorptances
    OptionalDouble interiorVisibleAbsorptance = surface.interiorVisibleAbsorptance();
    ASSERT_TRUE(interiorVisibleAbsorptance);
    EXPECT_TRUE(*interiorVisibleAbsorptance > 0);

    OptionalDouble exteriorVisibleAbsorptance = surface.exteriorVisibleAbsorptance();
    ASSERT_TRUE(exteriorVisibleAbsorptance);
    EXPECT_TRUE(*exteriorVisibleAbsorptance > 0);

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
  for (Surface& surface : surfaces) {
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
TEST_F(ModelFixture, AirWall) {
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

TEST_F(ModelFixture, 0_Vertex_Surface) {
  Model model;

  std::vector<Point3d> vertices;

  EXPECT_THROW(Surface(vertices, model), openstudio::Exception);
}

TEST_F(ModelFixture, 1_Vertex_Surface) {
  Model model;

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0, 0, 1));

  EXPECT_THROW(Surface(vertices, model), openstudio::Exception);
}

TEST_F(ModelFixture, 2_Vertex_Surface) {
  Model model;

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0, 0, 1));
  vertices.push_back(Point3d(0, 0, 0));

  EXPECT_THROW(Surface(vertices, model), openstudio::Exception);
}

TEST_F(ModelFixture, 3_Vertex_Surface) {
  Model model;

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0, 0, 1));
  vertices.push_back(Point3d(0, 0, 0));
  vertices.push_back(Point3d(1, 0, 0));

  EXPECT_NO_THROW(Surface(vertices, model));
}

TEST_F(ModelFixture, 4_Vertex_Surface) {
  Model model;

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0, 0, 1));
  vertices.push_back(Point3d(0, 0, 0));
  vertices.push_back(Point3d(1, 0, 0));
  vertices.push_back(Point3d(1, 0, 1));

  EXPECT_NO_THROW(Surface(vertices, model));
}

TEST_F(ModelFixture, 5_Vertex_Surface) {
  Model model;

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0, 0, 1));
  vertices.push_back(Point3d(0, 0, 0));
  vertices.push_back(Point3d(1, 0, 0));
  vertices.push_back(Point3d(1, 0, 1));
  vertices.push_back(Point3d(0.5, 0, 1.5));

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
  vertices.push_back(Point3d(0, 0, 1));
  vertices.push_back(Point3d(0, 0, 0));
  vertices.push_back(Point3d(1, 0, 0));
  vertices.push_back(Point3d(1, 0, 1));
  Surface surface(vertices, model);
  EXPECT_TRUE(surface.setConstruction(construction));
  ASSERT_TRUE(surface.construction());
  EXPECT_TRUE(surface.construction().get() == construction);

  SurfacePropertyConvectionCoefficients cc(surface);
  ASSERT_TRUE(surface.surfacePropertyConvectionCoefficients());
  EXPECT_TRUE(surface.surfacePropertyConvectionCoefficients().get() == cc);

  auto optprop = surface.createSurfacePropertyExposedFoundationPerimeter("TotalExposedPerimeter", 100);
  ASSERT_TRUE(optprop);

  FoundationKiva kiva(model);
  surface.setAdjacentFoundation(kiva);
  EXPECT_TRUE(surface.adjacentFoundation());

  // clone should maintain connection to Construction
  Surface clone1 = surface.clone().cast<Surface>();
  ASSERT_TRUE(clone1.model() == surface.model());
  ASSERT_TRUE(clone1.construction());
  EXPECT_TRUE(clone1.construction().get() == construction);
  EXPECT_TRUE(clone1.surfacePropertyConvectionCoefficients());

  ASSERT_TRUE(clone1.surfacePropertyExposedFoundationPerimeter());
  EXPECT_TRUE(clone1.surfacePropertyExposedFoundationPerimeter().get() != optprop.get());
  ASSERT_TRUE(clone1.surfacePropertyExposedFoundationPerimeter()->parent());
  EXPECT_TRUE(clone1.surfacePropertyExposedFoundationPerimeter()->parent().get() == clone1);

  EXPECT_TRUE(clone1.adjacentFoundation());

  // even if through ModelObject
  Surface clone2 = surface.cast<ModelObject>().clone().cast<Surface>();
  ASSERT_TRUE(clone2.model() == surface.model());
  ASSERT_TRUE(clone2.construction());
  EXPECT_TRUE(clone2.construction().get() == construction);
  EXPECT_TRUE(clone2.surfacePropertyConvectionCoefficients());

  ASSERT_TRUE(clone2.surfacePropertyExposedFoundationPerimeter());
  EXPECT_TRUE(clone2.surfacePropertyExposedFoundationPerimeter().get() != optprop.get());
  ASSERT_TRUE(clone2.surfacePropertyExposedFoundationPerimeter()->parent());
  EXPECT_TRUE(clone2.surfacePropertyExposedFoundationPerimeter()->parent().get() == clone2);

  EXPECT_TRUE(clone2.adjacentFoundation());
}

TEST_F(ModelFixture, OutsideBoundaryConditionCapitalization) {
  Model model;

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0, 0, 3));
  vertices.push_back(Point3d(0, 0, 0));
  vertices.push_back(Point3d(3, 0, 0));
  vertices.push_back(Point3d(3, 0, 3));

  Space space(model);
  Surface wall(vertices, model);
  wall.setSpace(space);
  EXPECT_FALSE(wall.adjacentSurface());
  EXPECT_EQ("Outdoors", wall.outsideBoundaryCondition());
  EXPECT_EQ("SunExposed", wall.sunExposure());
  EXPECT_EQ("WindExposed", wall.windExposure());

  EXPECT_TRUE(wall.setOutsideBoundaryCondition("ADIABATIC"));
  EXPECT_FALSE(wall.adjacentSurface());
  EXPECT_EQ("ADIABATIC", wall.outsideBoundaryCondition());
  EXPECT_EQ("NoSun", wall.sunExposure());
  EXPECT_EQ("NoWind", wall.windExposure());

  EXPECT_TRUE(wall.setOutsideBoundaryCondition("outdoors"));
  EXPECT_FALSE(wall.adjacentSurface());
  EXPECT_EQ("outdoors", wall.outsideBoundaryCondition());
  EXPECT_EQ("SunExposed", wall.sunExposure());
  EXPECT_EQ("WindExposed", wall.windExposure());

  EXPECT_FALSE(wall.setOutsideBoundaryCondition("FlibbityGibbit"));
  EXPECT_FALSE(wall.adjacentSurface());
  EXPECT_EQ("outdoors", wall.outsideBoundaryCondition());
  EXPECT_EQ("SunExposed", wall.sunExposure());
  EXPECT_EQ("WindExposed", wall.windExposure());
}

TEST_F(ModelFixture, AdjacentSurface) {
  Model model;

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0, 0, 3));
  vertices.push_back(Point3d(0, 0, 0));
  vertices.push_back(Point3d(3, 0, 0));
  vertices.push_back(Point3d(3, 0, 3));

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

TEST_F(ModelFixture, AdjacentSurface_SurfacePropertyOtherSideCoefficients) {
  Model model;

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0, 0, 3));
  vertices.push_back(Point3d(0, 0, 0));
  vertices.push_back(Point3d(3, 0, 0));
  vertices.push_back(Point3d(3, 0, 3));

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

  SurfacePropertyOtherSideCoefficients osc(model);
  EXPECT_TRUE(wall1.setSurfacePropertyOtherSideCoefficients(osc));
  ASSERT_TRUE(wall1.surfacePropertyOtherSideCoefficients());
  EXPECT_EQ(osc.handle(), wall1.surfacePropertyOtherSideCoefficients()->handle());
  EXPECT_FALSE(wall2.surfacePropertyOtherSideCoefficients());
  EXPECT_FALSE(wall1.adjacentSurface());
  EXPECT_FALSE(wall2.adjacentSurface());
  EXPECT_EQ("OtherSideCoefficients", wall1.outsideBoundaryCondition());
  EXPECT_EQ("NoSun", wall1.sunExposure());
  EXPECT_EQ("NoWind", wall1.windExposure());
  EXPECT_EQ("Outdoors", wall2.outsideBoundaryCondition());
  EXPECT_EQ("SunExposed", wall2.sunExposure());
  EXPECT_EQ("WindExposed", wall2.windExposure());

  EXPECT_TRUE(wall1.setAdjacentSurface(wall2));
  ASSERT_TRUE(wall1.adjacentSurface());
  EXPECT_EQ(wall2.handle(), wall1.adjacentSurface()->handle());
  ASSERT_TRUE(wall2.adjacentSurface());
  EXPECT_EQ(wall1.handle(), wall2.adjacentSurface()->handle());
  EXPECT_FALSE(wall1.surfacePropertyOtherSideConditionsModel());
  EXPECT_EQ("Surface", wall1.outsideBoundaryCondition());
  EXPECT_EQ("NoSun", wall1.sunExposure());
  EXPECT_EQ("NoWind", wall1.windExposure());
  EXPECT_EQ("Surface", wall2.outsideBoundaryCondition());
  EXPECT_EQ("NoSun", wall2.sunExposure());
  EXPECT_EQ("NoWind", wall2.windExposure());
}

TEST_F(ModelFixture, AdjacentSurface_SurfacePropertyOtherSideConditionsModel) {
  Model model;

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0, 0, 3));
  vertices.push_back(Point3d(0, 0, 0));
  vertices.push_back(Point3d(3, 0, 0));
  vertices.push_back(Point3d(3, 0, 3));

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

  SurfacePropertyOtherSideConditionsModel oscm(model);
  EXPECT_TRUE(wall1.setSurfacePropertyOtherSideConditionsModel(oscm));
  ASSERT_TRUE(wall1.surfacePropertyOtherSideConditionsModel());
  EXPECT_EQ(oscm.handle(), wall1.surfacePropertyOtherSideConditionsModel()->handle());
  EXPECT_FALSE(wall2.surfacePropertyOtherSideConditionsModel());
  EXPECT_FALSE(wall1.adjacentSurface());
  EXPECT_FALSE(wall2.adjacentSurface());
  EXPECT_EQ("OtherSideConditionsModel", wall1.outsideBoundaryCondition());
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
  EXPECT_FALSE(wall1.surfacePropertyOtherSideConditionsModel());
  EXPECT_EQ("Surface", wall1.outsideBoundaryCondition());
  EXPECT_EQ("NoSun", wall1.sunExposure());
  EXPECT_EQ("NoWind", wall1.windExposure());
  EXPECT_EQ("Surface", wall2.outsideBoundaryCondition());
  EXPECT_EQ("NoSun", wall2.sunExposure());
  EXPECT_EQ("NoWind", wall2.windExposure());
}

TEST_F(ModelFixture, SurfacePropertyOtherSideCoefficients) {
  Model model;

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0, 0, 3));
  vertices.push_back(Point3d(0, 0, 0));
  vertices.push_back(Point3d(3, 0, 0));
  vertices.push_back(Point3d(3, 0, 3));

  Space space1(model);
  Surface wall1(vertices, model);
  wall1.setSpace(space1);
  EXPECT_FALSE(wall1.surfacePropertyOtherSideConditionsModel());
  EXPECT_EQ("Outdoors", wall1.outsideBoundaryCondition());
  EXPECT_EQ("SunExposed", wall1.sunExposure());
  EXPECT_EQ("WindExposed", wall1.windExposure());

  SurfacePropertyOtherSideCoefficients osc(model);

  EXPECT_TRUE(wall1.setSurfacePropertyOtherSideCoefficients(osc));
  ASSERT_TRUE(wall1.surfacePropertyOtherSideCoefficients());
  EXPECT_EQ(osc.handle(), wall1.surfacePropertyOtherSideCoefficients()->handle());
  EXPECT_EQ("OtherSideCoefficients", wall1.outsideBoundaryCondition());
  EXPECT_EQ("NoSun", wall1.sunExposure());
  EXPECT_EQ("NoWind", wall1.windExposure());

  EXPECT_FALSE(wall1.setOutsideBoundaryCondition("FlibbityGibbit"));
  EXPECT_EQ("OtherSideCoefficients", wall1.outsideBoundaryCondition());
  EXPECT_EQ("NoSun", wall1.sunExposure());
  EXPECT_EQ("NoWind", wall1.windExposure());

  EXPECT_TRUE(wall1.setOutsideBoundaryCondition("Adiabatic"));
  EXPECT_EQ("Adiabatic", wall1.outsideBoundaryCondition());
  EXPECT_EQ("NoSun", wall1.sunExposure());
  EXPECT_EQ("NoWind", wall1.windExposure());
}

TEST_F(ModelFixture, SurfacePropertyOtherSideConditionsModel) {
  Model model;

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0, 0, 3));
  vertices.push_back(Point3d(0, 0, 0));
  vertices.push_back(Point3d(3, 0, 0));
  vertices.push_back(Point3d(3, 0, 3));

  Space space1(model);
  Surface wall1(vertices, model);
  wall1.setSpace(space1);
  EXPECT_FALSE(wall1.surfacePropertyOtherSideConditionsModel());
  EXPECT_EQ("Outdoors", wall1.outsideBoundaryCondition());
  EXPECT_EQ("SunExposed", wall1.sunExposure());
  EXPECT_EQ("WindExposed", wall1.windExposure());

  SurfacePropertyOtherSideConditionsModel oscm(model);

  EXPECT_TRUE(wall1.setSurfacePropertyOtherSideConditionsModel(oscm));
  ASSERT_TRUE(wall1.surfacePropertyOtherSideConditionsModel());
  EXPECT_EQ(oscm.handle(), wall1.surfacePropertyOtherSideConditionsModel()->handle());
  EXPECT_EQ("OtherSideConditionsModel", wall1.outsideBoundaryCondition());
  EXPECT_EQ("SunExposed", wall1.sunExposure());
  EXPECT_EQ("WindExposed", wall1.windExposure());

  EXPECT_FALSE(wall1.setOutsideBoundaryCondition("FlibbityGibbit"));
  EXPECT_EQ("OtherSideConditionsModel", wall1.outsideBoundaryCondition());
  EXPECT_EQ("SunExposed", wall1.sunExposure());
  EXPECT_EQ("WindExposed", wall1.windExposure());

  EXPECT_TRUE(wall1.setOutsideBoundaryCondition("Outdoors"));
  EXPECT_EQ("Outdoors", wall1.outsideBoundaryCondition());
  EXPECT_EQ("SunExposed", wall1.sunExposure());
  EXPECT_EQ("WindExposed", wall1.windExposure());
}

TEST_F(ModelFixture, Surface_BadSurfaceType) {
  Model model;

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0, 1, 0));
  vertices.push_back(Point3d(0, 0, 0));
  vertices.push_back(Point3d(1, 0, 0));
  vertices.push_back(Point3d(1, 1, 0));

  Surface roof(vertices, model);
  EXPECT_FALSE(roof.setSurfaceType("Roof"));  // should be RoofCeiling

  vertices.clear();
  vertices.push_back(Point3d(1, 1, 0));
  vertices.push_back(Point3d(1, 0, 0));
  vertices.push_back(Point3d(0, 0, 0));
  vertices.push_back(Point3d(0, 1, 0));

  EXPECT_NO_THROW(Surface floor(vertices, model));
}

TEST_F(ModelFixture, Surface_IddAssumptions) {

  // Choice Fields

  // outside boundary condition is hand-mapped to similar enums in standardsinterface.
  // if this test starts failing, please re-map the standardsinterface
  StringVector keys = Surface::validOutsideBoundaryConditionValues();

  EXPECT_EQ(15u, keys.size());
  EXPECT_FALSE(std::find_if(keys.begin(), keys.end(), std::bind(istringEqual, "Adiabatic", std::placeholders::_1)) == keys.end());
  EXPECT_FALSE(std::find_if(keys.begin(), keys.end(), std::bind(istringEqual, "Surface", std::placeholders::_1)) == keys.end());
  EXPECT_FALSE(std::find_if(keys.begin(), keys.end(), std::bind(istringEqual, "Outdoors", std::placeholders::_1)) == keys.end());
  EXPECT_FALSE(std::find_if(keys.begin(), keys.end(), std::bind(istringEqual, "Foundation", std::placeholders::_1)) == keys.end());
  EXPECT_FALSE(std::find_if(keys.begin(), keys.end(), std::bind(istringEqual, "Ground", std::placeholders::_1)) == keys.end());
  EXPECT_FALSE(std::find_if(keys.begin(), keys.end(), std::bind(istringEqual, "GroundFCfactorMethod", std::placeholders::_1)) == keys.end());
  EXPECT_FALSE(std::find_if(keys.begin(), keys.end(), std::bind(istringEqual, "OtherSideCoefficients", std::placeholders::_1)) == keys.end());
  EXPECT_FALSE(std::find_if(keys.begin(), keys.end(), std::bind(istringEqual, "OtherSideConditionsModel", std::placeholders::_1)) == keys.end());
  EXPECT_FALSE(std::find_if(keys.begin(), keys.end(), std::bind(istringEqual, "GroundSlabPreprocessorAverage", std::placeholders::_1)) == keys.end());
  EXPECT_FALSE(std::find_if(keys.begin(), keys.end(), std::bind(istringEqual, "GroundSlabPreprocessorCore", std::placeholders::_1)) == keys.end());
  EXPECT_FALSE(std::find_if(keys.begin(), keys.end(), std::bind(istringEqual, "GroundSlabPreprocessorPerimeter", std::placeholders::_1))
               == keys.end());
  EXPECT_FALSE(std::find_if(keys.begin(), keys.end(), std::bind(istringEqual, "GroundBasementPreprocessorAverageWall", std::placeholders::_1))
               == keys.end());
  EXPECT_FALSE(std::find_if(keys.begin(), keys.end(), std::bind(istringEqual, "GroundBasementPreprocessorAverageFloor", std::placeholders::_1))
               == keys.end());
  EXPECT_FALSE(std::find_if(keys.begin(), keys.end(), std::bind(istringEqual, "GroundBasementPreprocessorUpperWall", std::placeholders::_1))
               == keys.end());
  EXPECT_FALSE(std::find_if(keys.begin(), keys.end(), std::bind(istringEqual, "GroundBasementPreprocessorLowerWall", std::placeholders::_1))
               == keys.end());
}

void expect_point_eq(const Point3d& p1, const Point3d& p2) {
  EXPECT_NEAR(p1.x(), p2.x(), 0.001);
  EXPECT_NEAR(p1.y(), p2.y(), 0.001);
  EXPECT_NEAR(p1.z(), p2.z(), 0.001);
}

TEST_F(ModelFixture, Surface_SouthWall_WWR) {
  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0, 0, 3));
  vertices.push_back(Point3d(0, 0, 0));
  vertices.push_back(Point3d(10, 0, 0));
  vertices.push_back(Point3d(10, 0, 3));

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
    double wwr = (W - 2.0 * x) * (H - y - offset) / (W * H);
    boost::optional<SubSurface> window = wall.setWindowToWallRatio(wwr, 1.0, true);
    ASSERT_TRUE(window);
    ASSERT_EQ(4u, window->vertices().size());
    SCOPED_TRACE("regular case");
    expect_point_eq(Point3d(x, 0, H - y), window->vertices()[0]);
    expect_point_eq(Point3d(x, 0, offset), window->vertices()[1]);
    expect_point_eq(Point3d(W - x, 0, offset), window->vertices()[2]);
    expect_point_eq(Point3d(W - x, 0, H - y), window->vertices()[3]);
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
    double wwr = (W - 2.0 * x) * (H - y - offset) / (W * H);
    boost::optional<SubSurface> window = wall.setWindowToWallRatio(wwr, 1.0, false);
    ASSERT_TRUE(window);
    ASSERT_EQ(4u, window->vertices().size());
    SCOPED_TRACE("daylighting window");
    expect_point_eq(Point3d(x, 0, H - offset), window->vertices()[0]);
    expect_point_eq(Point3d(x, 0, y), window->vertices()[1]);
    expect_point_eq(Point3d(W - x, 0, y), window->vertices()[2]);
    expect_point_eq(Point3d(W - x, 0, H - offset), window->vertices()[3]);
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
    double wwr = (W - 2.0 * x) * (H - y - offset) / (W * H);
    boost::optional<SubSurface> window = wall.setWindowToWallRatio(wwr, 1.0, true);
    ASSERT_TRUE(window);
    ASSERT_EQ(4u, window->vertices().size());
    SCOPED_TRACE("window goes all the way up to min limit at ceiling");
    expect_point_eq(Point3d(x, 0, H - y), window->vertices()[0]);
    expect_point_eq(Point3d(x, 0, offset), window->vertices()[1]);
    expect_point_eq(Point3d(W - x, 0, offset), window->vertices()[2]);
    expect_point_eq(Point3d(W - x, 0, H - y), window->vertices()[3]);
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
    double wwr = (W - 2.0 * x) * (H - y - offset) / (W * H);
    boost::optional<SubSurface> window = wall.setWindowToWallRatio(wwr, 1.0, true);
    ASSERT_TRUE(window);
    ASSERT_EQ(4u, window->vertices().size());
    SCOPED_TRACE("window extends below desired sill height");
    expect_point_eq(Point3d(x, 0, H - y), window->vertices()[0]);
    expect_point_eq(Point3d(x, 0, offset), window->vertices()[1]);
    expect_point_eq(Point3d(W - x, 0, offset), window->vertices()[2]);
    expect_point_eq(Point3d(W - x, 0, H - y), window->vertices()[3]);
    EXPECT_NEAR(wwr, wall.windowToWallRatio(), 0.001);
  }

  /*
  DLM: this is no longer supported
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
*/
  {
    // existing window is no problem
    Model model;
    Surface wall(vertices, model);
    EXPECT_EQ("Wall", wall.surfaceType());
    EXPECT_EQ(0.0, wall.windowToWallRatio());

    std::vector<Point3d> windowVertices;
    windowVertices.push_back(Point3d(1, 0, 2));
    windowVertices.push_back(Point3d(1, 0, 1));
    windowVertices.push_back(Point3d(9, 0, 1));
    windowVertices.push_back(Point3d(9, 0, 2));
    SubSurface window(windowVertices, model);
    window.setSurface(wall);
    EXPECT_EQ("FixedWindow", window.subSurfaceType());

    double wwr = 8.0 / 30.0;  // 0.26666
    EXPECT_EQ(1u, wall.subSurfaces().size());
    EXPECT_NEAR(wwr, wall.windowToWallRatio(), 0.001);

    wwr = 10.0 / 30.0;  // 0.3333
    boost::optional<SubSurface> newWindow = wall.setWindowToWallRatio(wwr);
    ASSERT_TRUE(newWindow);
    EXPECT_EQ(1u, wall.subSurfaces().size());
    EXPECT_NEAR(wwr, wall.windowToWallRatio(), 0.001);
  }
  /*
  DLM: this is no longer supported
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
  */
}

TEST_F(ModelFixture, Surface_EastWall_WWR) {
  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0, 0, 3));
  vertices.push_back(Point3d(0, 0, 0));
  vertices.push_back(Point3d(0, 10, 0));
  vertices.push_back(Point3d(0, 10, 3));

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
    double wwr = (W - 2.0 * x) * (H - y - offset) / (W * H);
    boost::optional<SubSurface> window = wall.setWindowToWallRatio(wwr, 1.0, true);
    ASSERT_TRUE(window);
    ASSERT_EQ(4u, window->vertices().size());
    expect_point_eq(Point3d(0, x, H - y), window->vertices()[0]);
    expect_point_eq(Point3d(0, x, offset), window->vertices()[1]);
    expect_point_eq(Point3d(0, W - x, offset), window->vertices()[2]);
    expect_point_eq(Point3d(0, W - x, H - y), window->vertices()[3]);
    EXPECT_NEAR(wwr, wall.windowToWallRatio(), 0.001);
  }
}

TEST_F(ModelFixture, Surface_WWR_Fail) {
  {
    // triangle
    std::vector<Point3d> vertices;
    vertices.push_back(Point3d(0, 0, 1));
    vertices.push_back(Point3d(0, 0, 0));
    vertices.push_back(Point3d(1, 0, 0));

    Model model;
    Surface wall(vertices, model);
    EXPECT_EQ("Wall", wall.surfaceType());
    EXPECT_EQ(0.0, wall.windowToWallRatio());

    EXPECT_FALSE(wall.setWindowToWallRatio(0.2));
  }

  {
    // T-shape
    std::vector<Point3d> vertices;
    vertices.push_back(Point3d(0, 0, 3));
    vertices.push_back(Point3d(0, 0, 2));
    vertices.push_back(Point3d(3, 0, 2));
    vertices.push_back(Point3d(3, 0, 0));
    vertices.push_back(Point3d(4, 0, 0));
    vertices.push_back(Point3d(4, 0, 2));
    vertices.push_back(Point3d(7, 0, 2));
    vertices.push_back(Point3d(7, 0, 3));

    Model model;
    Surface wall(vertices, model);
    EXPECT_EQ("Wall", wall.surfaceType());
    EXPECT_EQ(0.0, wall.windowToWallRatio());

    EXPECT_FALSE(wall.setWindowToWallRatio(0.2));
  }

  {
    // out of limits
    std::vector<Point3d> vertices;
    vertices.push_back(Point3d(0, 0, 3));
    vertices.push_back(Point3d(0, 0, 0));
    vertices.push_back(Point3d(10, 0, 0));
    vertices.push_back(Point3d(10, 0, 3));

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
    vertices.push_back(Point3d(0, 3, 0));
    vertices.push_back(Point3d(0, 0, 0));
    vertices.push_back(Point3d(10, 0, 0));
    vertices.push_back(Point3d(10, 3, 0));

    Model model;
    Surface roof(vertices, model);
    EXPECT_EQ("RoofCeiling", roof.surfaceType());
    EXPECT_FALSE(roof.setWindowToWallRatio(0.4));
  }

  {
    // bad surface type
    std::vector<Point3d> vertices;
    vertices.push_back(Point3d(0, 0, 0));
    vertices.push_back(Point3d(0, 3, 0));
    vertices.push_back(Point3d(10, 3, 0));
    vertices.push_back(Point3d(10, 0, 0));

    Model model;
    Surface roof(vertices, model);
    EXPECT_EQ("Floor", roof.surfaceType());
    EXPECT_FALSE(roof.setWindowToWallRatio(0.4));
  }

  {
    // door prevents window from being added
    std::vector<Point3d> vertices;
    vertices.push_back(Point3d(0, 0, 3));
    vertices.push_back(Point3d(0, 0, 0));
    vertices.push_back(Point3d(10, 0, 0));
    vertices.push_back(Point3d(10, 0, 3));

    Model model;
    Surface wall(vertices, model);
    EXPECT_EQ("Wall", wall.surfaceType());
    EXPECT_EQ(0.0, wall.windowToWallRatio());

    std::vector<Point3d> doorVertices;
    doorVertices.push_back(Point3d(4, 0, 2));
    doorVertices.push_back(Point3d(4, 0, 0));
    doorVertices.push_back(Point3d(6, 0, 0));
    doorVertices.push_back(Point3d(6, 0, 2));
    SubSurface door(doorVertices, model);
    door.setSurface(wall);
    EXPECT_EQ("Door", door.subSurfaceType());
    EXPECT_EQ(1u, wall.subSurfaces().size());

    EXPECT_NEAR(0.0, wall.windowToWallRatio(), 0.001);

    EXPECT_FALSE(wall.setWindowToWallRatio(0.3));
  }
}

// the following tests demonstrate issues with initialization

class SurfaceWorkspaceWatcher : public openstudio::WorkspaceWatcher
{

 public:
  SurfaceWorkspaceWatcher(const Workspace& workspace) : WorkspaceWatcher(workspace) {}

  virtual void onObjectAdd(const WorkspaceObject& addedObject) override {
    WorkspaceWatcher::onObjectAdd(addedObject);

    EXPECT_EQ(IddObjectType::OS_Surface, addedObject.iddObject().type().value());
    boost::optional<Surface> surface = addedObject.optionalCast<Surface>();
    ASSERT_TRUE(surface);
    ASSERT_NO_THROW(surface->vertices());
  }
};

TEST_F(ModelFixture, Surface_Initializer1) {
  Model model;
  SurfaceWorkspaceWatcher watcher(model);
  EXPECT_FALSE(watcher.objectAdded());

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0, 0, 1));
  vertices.push_back(Point3d(0, 0, 0));
  vertices.push_back(Point3d(1, 0, 0));
  vertices.push_back(Point3d(1, 0, 1));
  Surface surface(vertices, model);
  EXPECT_TRUE(watcher.objectAdded());

  EXPECT_EQ(4u, surface.vertices().size());
}

TEST_F(ModelFixture, Surface_Initializer2) {
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
  vertices.push_back(Point3d(0, 0, 1));
  vertices.push_back(Point3d(0, 0, 0));
  vertices.push_back(Point3d(1, 0, 0));
  vertices.push_back(Point3d(1, 0, 1));
  EXPECT_TRUE(surface.setVertices(vertices));
  EXPECT_EQ(4u, surface.vertices().size());
}

TEST_F(ModelFixture, Surface_SearchDistance) {
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

  boost::optional<std::pair<ConstructionBase, int>> test;

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

TEST_F(ModelFixture, Surface_createAdjacentSurface) {
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
  EXPECT_EQ("FixedWindow", subSurface1.subSurfaceType());
  subSurface1.setSurface(surface1);
  EXPECT_EQ("FixedWindow", subSurface1.subSurfaceType());

  Space space2(model);
  boost::optional<Surface> surface2 = surface1.createAdjacentSurface(space2);

  ASSERT_TRUE(surface2);
  EXPECT_EQ(surface1.grossArea(), surface2->grossArea());
  EXPECT_EQ(surface1.netArea(), surface2->netArea());
  EXPECT_EQ(surface1.netArea(), triangulatedArea(surface1.triangulation()));
  for (const auto& triangle : surface1.triangulation()) {
    ASSERT_TRUE(getOutwardNormal(triangle));
    EXPECT_DOUBLE_EQ(1.0, surface1.outwardNormal().dot(getOutwardNormal(triangle).get()));
  }
  EXPECT_EQ(surface2->netArea(), triangulatedArea(surface2->triangulation()));
  for (const auto& triangle : surface2->triangulation()) {
    ASSERT_TRUE(getOutwardNormal(triangle));
    EXPECT_DOUBLE_EQ(1.0, surface2->outwardNormal().dot(getOutwardNormal(triangle).get()));
  }

  ASSERT_EQ(1u, surface2->subSurfaces().size());
  SubSurface subSurface2 = surface2->subSurfaces()[0];
  EXPECT_EQ(subSurface1.grossArea(), subSurface2.grossArea());
  EXPECT_EQ(subSurface1.netArea(), subSurface2.netArea());
  EXPECT_EQ(subSurface1.netArea(), triangulatedArea(subSurface1.triangulation()));
  for (const auto& triangle : subSurface1.triangulation()) {
    ASSERT_TRUE(getOutwardNormal(triangle));
    EXPECT_DOUBLE_EQ(1.0, subSurface1.outwardNormal().dot(getOutwardNormal(triangle).get()));
  }
  EXPECT_EQ(subSurface2.netArea(), triangulatedArea(subSurface2.triangulation()));
  for (const auto& triangle : subSurface2.triangulation()) {
    ASSERT_TRUE(getOutwardNormal(triangle));
    EXPECT_DOUBLE_EQ(1.0, subSurface2.outwardNormal().dot(getOutwardNormal(triangle).get()));
  }
}

TEST_F(ModelFixture, Surface_createAdjacentSurface2) {
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
  points.push_back(Point3d(0, 9, 1));
  points.push_back(Point3d(0, 9, 0));
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(0, 1, 1));
  SubSurface subSurface1(points, model);
  EXPECT_EQ("FixedWindow", subSurface1.subSurfaceType());
  subSurface1.setSurface(surface1);
  EXPECT_EQ("Door", subSurface1.subSurfaceType());

  Space space2(model);
  boost::optional<Surface> surface2 = surface1.createAdjacentSurface(space2);

  ASSERT_TRUE(surface2);
  EXPECT_EQ(surface1.grossArea(), surface2->grossArea());
  EXPECT_EQ(surface1.netArea(), surface2->netArea());
  EXPECT_EQ(surface1.netArea(), triangulatedArea(surface1.triangulation()));
  for (const auto& triangle : surface1.triangulation()) {
    ASSERT_TRUE(getOutwardNormal(triangle));
    EXPECT_DOUBLE_EQ(1.0, surface1.outwardNormal().dot(getOutwardNormal(triangle).get()));
  }
  EXPECT_EQ(surface2->netArea(), triangulatedArea(surface2->triangulation()));
  for (const auto& triangle : surface2->triangulation()) {
    ASSERT_TRUE(getOutwardNormal(triangle));
    EXPECT_DOUBLE_EQ(1.0, surface2->outwardNormal().dot(getOutwardNormal(triangle).get()));
  }

  ASSERT_EQ(1u, surface2->subSurfaces().size());
  SubSurface subSurface2 = surface2->subSurfaces()[0];
  EXPECT_EQ(subSurface1.grossArea(), subSurface2.grossArea());
  EXPECT_EQ(subSurface1.netArea(), subSurface2.netArea());
  EXPECT_EQ(subSurface1.netArea(), triangulatedArea(subSurface1.triangulation()));
  for (const auto& triangle : subSurface1.triangulation()) {
    ASSERT_TRUE(getOutwardNormal(triangle));
    EXPECT_DOUBLE_EQ(1.0, subSurface1.outwardNormal().dot(getOutwardNormal(triangle).get()));
  }
  EXPECT_EQ(subSurface2.netArea(), triangulatedArea(subSurface2.triangulation()));
  for (const auto& triangle : subSurface2.triangulation()) {
    ASSERT_TRUE(getOutwardNormal(triangle));
    EXPECT_DOUBLE_EQ(1.0, subSurface2.outwardNormal().dot(getOutwardNormal(triangle).get()));
  }
}

TEST_F(ModelFixture, Surface_DeleteAdjacentSurface) {
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

TEST_F(ModelFixture, Surface_ChangeAdjacentSurface_SameSpace) {
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

TEST_F(ModelFixture, Surface_ChangeAdjacentSurface_DiffSpaces) {
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

TEST_F(ModelFixture, Surface_Intersect_CompletelyContained) {
  Model model;
  Space space1(model);
  Space space2(model);

  Point3dVector points1;
  points1.push_back(Point3d(0, 0, 0));
  points1.push_back(Point3d(10, 0, 0));
  points1.push_back(Point3d(10, 10, 0));
  points1.push_back(Point3d(0, 10, 0));
  Surface surface1(points1, model);
  surface1.setSpace(space1);

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

  EXPECT_EQ(5u, space1.surfaces().size());
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

  for (const Surface& surface : space1.surfaces()) {
    if (surface.handle() != surface1.handle()) {
      bool test = false;

      points.clear();
      points.push_back(Point3d(0, 0, 0));
      points.push_back(Point3d(10, 0, 0));
      points.push_back(Point3d(6, 4, 0));
      points.push_back(Point3d(4, 4, 0));
      test = (test || circularEqual(surface.vertices(), points));

      points.clear();
      points.push_back(Point3d(10, 0, 0));
      points.push_back(Point3d(10, 10, 0));
      points.push_back(Point3d(6, 6, 0));
      points.push_back(Point3d(6, 4, 0));
      test = (test || circularEqual(surface.vertices(), points));

      points.clear();
      points.push_back(Point3d(10, 10, 0));
      points.push_back(Point3d(0, 10, 0));
      points.push_back(Point3d(4, 6, 0));
      points.push_back(Point3d(6, 6, 0));
      test = (test || circularEqual(surface.vertices(), points));

      points.clear();
      points.push_back(Point3d(0, 10, 0));
      points.push_back(Point3d(0, 0, 0));
      points.push_back(Point3d(4, 4, 0));
      points.push_back(Point3d(4, 6, 0));
      test = (test || circularEqual(surface.vertices(), points));

      EXPECT_TRUE(test) << surface.vertices();
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

TEST_F(ModelFixture, Surface_Intersect_SameHeight_PartialOverlap) {
  Model model;
  Space space1(model);
  Space space2(model);

  Point3dVector points1;
  points1.push_back(Point3d(0, 0, 0));
  points1.push_back(Point3d(10, 0, 0));
  points1.push_back(Point3d(10, 10, 0));
  points1.push_back(Point3d(0, 10, 0));
  Surface surface1(points1, model);
  surface1.setSpace(space1);
  double surface1Area = surface1.grossArea();

  Point3dVector points2;
  points2.push_back(Point3d(7, 10, 0));
  points2.push_back(Point3d(13, 10, 0));
  points2.push_back(Point3d(13, 0, 0));
  points2.push_back(Point3d(7, 0, 0));
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
  points.push_back(Point3d(7, 0, 0));
  points.push_back(Point3d(10, 0, 0));
  points.push_back(Point3d(10, 10, 0));
  points.push_back(Point3d(7, 10, 0));
  EXPECT_TRUE(circularEqual(surface1.vertices(), points));

  for (const Surface& surface : space1.surfaces()) {
    if (surface.handle() != surface1.handle()) {
      points.clear();
      points.push_back(Point3d(0, 0, 0));
      points.push_back(Point3d(7, 0, 0));
      points.push_back(Point3d(7, 10, 0));
      points.push_back(Point3d(0, 10, 0));
      EXPECT_TRUE(circularEqual(surface.vertices(), points));
      EXPECT_DOUBLE_EQ(surface1Area, surface1.grossArea() + surface.grossArea());
      break;
    }
  }

  points.clear();
  points.push_back(Point3d(7, 10, 0));
  points.push_back(Point3d(10, 10, 0));
  points.push_back(Point3d(10, 0, 0));
  points.push_back(Point3d(7, 0, 0));
  EXPECT_TRUE(circularEqual(surface2.vertices(), points));

  for (const Surface& surface : space2.surfaces()) {
    if (surface.handle() != surface2.handle()) {
      points.clear();
      points.push_back(Point3d(10, 10, 0));
      points.push_back(Point3d(13, 10, 0));
      points.push_back(Point3d(13, 0, 0));
      points.push_back(Point3d(10, 0, 0));
      EXPECT_TRUE(circularEqual(surface.vertices(), points));
      EXPECT_DOUBLE_EQ(surface2Area, surface2.grossArea() + surface.grossArea());
      break;
    }
  }
}

TEST_F(ModelFixture, Surface_Intersect_DifferentHeight_PartialOverlap) {
  Model model;
  Space space1(model);
  Space space2(model);

  Point3dVector points1;
  points1.push_back(Point3d(0, 0, 0));
  points1.push_back(Point3d(10, 0, 0));
  points1.push_back(Point3d(10, 10, 0));
  points1.push_back(Point3d(0, 10, 0));
  Surface surface1(points1, model);
  surface1.setSpace(space1);
  double surface1Area = surface1.grossArea();

  Point3dVector points2;
  points2.push_back(Point3d(7, 12, 0));
  points2.push_back(Point3d(13, 12, 0));
  points2.push_back(Point3d(13, -2, 0));
  points2.push_back(Point3d(7, -2, 0));
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
  points.push_back(Point3d(7, 0, 0));
  points.push_back(Point3d(10, 0, 0));
  points.push_back(Point3d(10, 10, 0));
  points.push_back(Point3d(7, 10, 0));
  EXPECT_TRUE(circularEqual(surface1.vertices(), points));

  for (const Surface& surface : space1.surfaces()) {
    if (surface.handle() != surface1.handle()) {
      points.clear();
      points.push_back(Point3d(0, 0, 0));
      points.push_back(Point3d(7, 0, 0));
      points.push_back(Point3d(7, 10, 0));
      points.push_back(Point3d(0, 10, 0));
      EXPECT_TRUE(circularEqual(surface.vertices(), points));
      EXPECT_DOUBLE_EQ(surface1Area, surface1.grossArea() + surface.grossArea());
      break;
    }
  }

  points.clear();
  points.push_back(Point3d(7, 10, 0));
  points.push_back(Point3d(10, 10, 0));
  points.push_back(Point3d(10, 0, 0));
  points.push_back(Point3d(7, 0, 0));
  EXPECT_TRUE(circularEqual(surface2.vertices(), points));

  for (const Surface& surface : space2.surfaces()) {
    if (surface.handle() != surface2.handle()) {
      points.clear();
      points.push_back(Point3d(7, 12, 0));
      points.push_back(Point3d(13, 12, 0));
      points.push_back(Point3d(13, -2, 0));
      points.push_back(Point3d(7, -2, 0));
      points.push_back(Point3d(7, 0, 0));
      points.push_back(Point3d(10, 0, 0));
      points.push_back(Point3d(10, 10, 0));
      points.push_back(Point3d(7, 10, 0));
      EXPECT_TRUE(circularEqual(surface.vertices(), points));
      EXPECT_DOUBLE_EQ(surface2Area, surface2.grossArea() + surface.grossArea());
      break;
    }
  }
}

TEST_F(ModelFixture, Surface_Intersect_DifferentHeight_ShareOneEdge_PartialOverlap) {
  Model model;
  Space space1(model);
  Space space2(model);

  Point3dVector points1;
  points1.push_back(Point3d(0, 0, 0));
  points1.push_back(Point3d(10, 0, 0));
  points1.push_back(Point3d(10, 10, 0));
  points1.push_back(Point3d(0, 10, 0));
  Surface surface1(points1, model);
  surface1.setSpace(space1);
  double surface1Area = surface1.grossArea();

  Point3dVector points2;
  points2.push_back(Point3d(7, 12, 0));
  points2.push_back(Point3d(13, 12, 0));
  points2.push_back(Point3d(13, 0, 0));
  points2.push_back(Point3d(7, 0, 0));
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
  points.push_back(Point3d(7, 0, 0));
  points.push_back(Point3d(10, 0, 0));
  points.push_back(Point3d(10, 10, 0));
  points.push_back(Point3d(7, 10, 0));
  EXPECT_TRUE(circularEqual(surface1.vertices(), points));

  for (const Surface& surface : space1.surfaces()) {
    if (surface.handle() != surface1.handle()) {
      points.clear();
      points.push_back(Point3d(0, 0, 0));
      points.push_back(Point3d(7, 0, 0));
      points.push_back(Point3d(7, 10, 0));
      points.push_back(Point3d(0, 10, 0));
      EXPECT_TRUE(circularEqual(surface.vertices(), points));
      EXPECT_DOUBLE_EQ(surface1Area, surface1.grossArea() + surface.grossArea());
      break;
    }
  }

  points.clear();
  points.push_back(Point3d(7, 10, 0));
  points.push_back(Point3d(10, 10, 0));
  points.push_back(Point3d(10, 0, 0));
  points.push_back(Point3d(7, 0, 0));
  EXPECT_TRUE(circularEqual(surface2.vertices(), points));

  for (const Surface& surface : space2.surfaces()) {
    if (surface.handle() != surface2.handle()) {
      points.clear();
      points.push_back(Point3d(7, 12, 0));
      points.push_back(Point3d(13, 12, 0));
      points.push_back(Point3d(13, 0, 0));
      points.push_back(Point3d(10, 0, 0));
      points.push_back(Point3d(10, 10, 0));
      points.push_back(Point3d(7, 10, 0));
      EXPECT_TRUE(circularEqual(surface.vertices(), points));
      EXPECT_DOUBLE_EQ(surface2Area, surface2.grossArea() + surface.grossArea());
      break;
    }
  }
}

TEST_F(ModelFixture, Surface_Intersect_AdjacentEdges) {
  Model model;
  Space space1(model);
  Space space2(model);

  Point3dVector points1;
  points1.push_back(Point3d(0, 0, 0));
  points1.push_back(Point3d(5, 0, 0));
  points1.push_back(Point3d(5, 10, 0));
  points1.push_back(Point3d(0, 10, 0));
  Surface surface1(points1, model);
  surface1.setSpace(space1);
  double surface1Area = surface1.grossArea();

  Point3dVector points2;
  points2.push_back(Point3d(5, 10, 0));
  points2.push_back(Point3d(10, 10, 0));
  points2.push_back(Point3d(10, 0, 0));
  points2.push_back(Point3d(5, 0, 0));
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

  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(5, 0, 0));
  points.push_back(Point3d(5, 10, 0));
  points.push_back(Point3d(0, 10, 0));
  EXPECT_TRUE(circularEqual(surface1.vertices(), points));
  EXPECT_DOUBLE_EQ(surface1Area, surface1.grossArea());

  points.clear();
  points.push_back(Point3d(5, 10, 0));
  points.push_back(Point3d(10, 10, 0));
  points.push_back(Point3d(10, 0, 0));
  points.push_back(Point3d(5, 0, 0));
  EXPECT_TRUE(circularEqual(surface2.vertices(), points));
  EXPECT_DOUBLE_EQ(surface2Area, surface2.grossArea());
}

TEST_F(ModelFixture, Surface_Intersect_SameSize) {
  Model model;
  Space space1(model);
  Space space2(model);

  Point3dVector points1;
  points1.push_back(Point3d(0, 0, 0));
  points1.push_back(Point3d(10, 0, 0));
  points1.push_back(Point3d(10, 10, 0));
  points1.push_back(Point3d(0, 10, 0));
  Surface surface1(points1, model);
  surface1.setSpace(space1);
  double surface1Area = surface1.grossArea();

  Point3dVector points2;
  points2.push_back(Point3d(0, 10, 0));
  points2.push_back(Point3d(10, 10, 0));
  points2.push_back(Point3d(10, 0, 0));
  points2.push_back(Point3d(0, 0, 0));
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

  EXPECT_EQ(1u, space1.surfaces().size());
  EXPECT_EQ(4u, surface1.vertices().size());
  EXPECT_EQ(1u, space2.surfaces().size());
  EXPECT_EQ(4u, surface2.vertices().size());

  Point3dVector points;

  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(10, 0, 0));
  points.push_back(Point3d(10, 10, 0));
  points.push_back(Point3d(0, 10, 0));
  EXPECT_TRUE(circularEqual(surface1.vertices(), points));
  EXPECT_DOUBLE_EQ(surface1Area, surface1.grossArea());

  points.clear();
  points.push_back(Point3d(0, 10, 0));
  points.push_back(Point3d(10, 10, 0));
  points.push_back(Point3d(10, 0, 0));
  points.push_back(Point3d(0, 0, 0));
  EXPECT_TRUE(circularEqual(surface2.vertices(), points));
  EXPECT_DOUBLE_EQ(surface2Area, surface2.grossArea());
}

TEST_F(ModelFixture, Surface_Intersect_CutIntoTwo) {
  Model model;
  Space space1(model);
  Space space2(model);

  Point3dVector points1;
  points1.push_back(Point3d(0, 0, 0));
  points1.push_back(Point3d(10, 0, 0));
  points1.push_back(Point3d(10, 10, 0));
  points1.push_back(Point3d(0, 10, 0));
  Surface surface1(points1, model);
  surface1.setSpace(space1);
  double surface1Area = surface1.grossArea();
  EXPECT_GT(surface1Area, 0.0);

  Point3dVector points2;
  points2.push_back(Point3d(3, 10, 0));
  points2.push_back(Point3d(7, 10, 0));
  points2.push_back(Point3d(7, 0, 0));
  points2.push_back(Point3d(3, 0, 0));
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
  points.push_back(Point3d(3, 0, 0));
  points.push_back(Point3d(7, 0, 0));
  points.push_back(Point3d(7, 10, 0));
  points.push_back(Point3d(3, 10, 0));
  EXPECT_TRUE(circularEqual(surface1.vertices(), points));
  EXPECT_DOUBLE_EQ(surface2Area, surface1.grossArea());

  for (const Surface& surface : space1.surfaces()) {
    if (surface.handle() != surface1.handle()) {
      bool test = false;

      points.clear();
      points.push_back(Point3d(0, 0, 0));
      points.push_back(Point3d(3, 0, 0));
      points.push_back(Point3d(3, 10, 0));
      points.push_back(Point3d(0, 10, 0));
      test = circularEqual(surface.vertices(), points);

      points.clear();
      points.push_back(Point3d(7, 0, 0));
      points.push_back(Point3d(10, 0, 0));
      points.push_back(Point3d(10, 10, 0));
      points.push_back(Point3d(7, 10, 0));
      test = (test || circularEqual(surface.vertices(), points));
      EXPECT_TRUE(test);

      EXPECT_DOUBLE_EQ(3 * 10, surface.grossArea());
    }
  }

  points.clear();
  points.push_back(Point3d(3, 10, 0));
  points.push_back(Point3d(7, 10, 0));
  points.push_back(Point3d(7, 0, 0));
  points.push_back(Point3d(3, 0, 0));
  EXPECT_TRUE(circularEqual(surface2.vertices(), points));
  EXPECT_DOUBLE_EQ(surface2Area, surface2.grossArea());
}

TEST_F(ModelFixture, Surface_Intersect_DifferentPlane) {
  Model model;
  Space space1(model);
  Space space2(model);

  Point3dVector points1;
  points1.push_back(Point3d(0, 0, 0));
  points1.push_back(Point3d(10, 0, 0));
  points1.push_back(Point3d(10, 10, 0));
  points1.push_back(Point3d(0, 10, 0));
  Surface surface1(points1, model);
  surface1.setSpace(space1);

  Point3dVector points2;
  points2.push_back(Point3d(7, 12, 1));
  points2.push_back(Point3d(13, 12, 1));
  points2.push_back(Point3d(13, 0, 1));
  points2.push_back(Point3d(7, 0, 1));
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

TEST_F(ModelFixture, Surface_Intersect_SameOutwardNormal) {
  Model model;
  Space space1(model);
  Space space2(model);

  Point3dVector points1;
  points1.push_back(Point3d(0, 0, 0));
  points1.push_back(Point3d(10, 0, 0));
  points1.push_back(Point3d(10, 10, 0));
  points1.push_back(Point3d(0, 10, 0));
  Surface surface1(points1, model);
  surface1.setSpace(space1);

  Point3dVector points2;
  points2.push_back(Point3d(7, 0, 0));
  points2.push_back(Point3d(13, 0, 0));
  points2.push_back(Point3d(13, 12, 0));
  points2.push_back(Point3d(7, 12, 0));
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

TEST_F(ModelFixture, Surface_Intersect_SameSpace) {
  Model model;
  Space space(model);

  Point3dVector points1;
  points1.push_back(Point3d(0, 0, 0));
  points1.push_back(Point3d(10, 0, 0));
  points1.push_back(Point3d(10, 10, 0));
  points1.push_back(Point3d(0, 10, 0));
  Surface surface1(points1, model);
  surface1.setSpace(space);

  Point3dVector points2;
  points2.push_back(Point3d(7, 12, 0));
  points2.push_back(Point3d(13, 12, 0));
  points2.push_back(Point3d(13, 0, 0));
  points2.push_back(Point3d(7, 0, 0));
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

TEST_F(ModelFixture, Surface_Intersect_AdjacentSurface) {
  Model model;
  Space space(model);

  Point3dVector points1;
  points1.push_back(Point3d(0, 0, 0));
  points1.push_back(Point3d(10, 0, 0));
  points1.push_back(Point3d(10, 10, 0));
  points1.push_back(Point3d(0, 10, 0));
  Surface surface1(points1, model);
  surface1.setSpace(space);

  Point3dVector points2;
  points2.push_back(Point3d(7, 12, 0));
  points2.push_back(Point3d(13, 12, 0));
  points2.push_back(Point3d(13, 0, 0));
  points2.push_back(Point3d(7, 0, 0));
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

TEST_F(ModelFixture, Surface_Intersect_WithSubSurfaces) {
  Model model;
  Space space(model);

  Point3dVector points1;
  points1.push_back(Point3d(0, 0, 0));
  points1.push_back(Point3d(10, 0, 0));
  points1.push_back(Point3d(10, 10, 0));
  points1.push_back(Point3d(0, 10, 0));
  Surface surface1(points1, model);
  surface1.setSpace(space);

  Point3dVector points2;
  points2.push_back(Point3d(7, 12, 0));
  points2.push_back(Point3d(13, 12, 0));
  points2.push_back(Point3d(13, 0, 0));
  points2.push_back(Point3d(7, 0, 0));
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

TEST_F(ModelFixture, Surface_Intersect_Complex) {
  Model model;
  Space space1(model);
  Space space2(model);

  Point3dVector points1;
  points1.push_back(Point3d(0, 0, 0));
  points1.push_back(Point3d(10, 0, 0));
  points1.push_back(Point3d(10, 10, 0));
  points1.push_back(Point3d(0, 10, 0));
  Surface surface1(points1, model);
  surface1.setSpace(space1);
  double surface1Area = surface1.grossArea();
  EXPECT_GT(surface1Area, 0.0);

  Point3dVector points2;
  points2.push_back(Point3d(5, 15, 0));
  points2.push_back(Point3d(8, 15, 0));
  points2.push_back(Point3d(8, 8, 0));
  points2.push_back(Point3d(15, 8, 0));
  points2.push_back(Point3d(15, 5, 0));
  points2.push_back(Point3d(8, 5, 0));
  points2.push_back(Point3d(8, -5, 0));
  points2.push_back(Point3d(5, -5, 0));
  Surface surface2(points2, model);
  surface2.setSpace(space2);
  double surface2Area = surface2.grossArea();
  EXPECT_GT(surface2Area, 0.0);

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
  points.push_back(Point3d(5, 0, 0));
  points.push_back(Point3d(8, 0, 0));
  points.push_back(Point3d(8, 5, 0));
  points.push_back(Point3d(10, 5, 0));
  points.push_back(Point3d(10, 8, 0));
  points.push_back(Point3d(8, 8, 0));
  points.push_back(Point3d(8, 10, 0));
  points.push_back(Point3d(5, 10, 0));
  EXPECT_TRUE(circularEqual(surface1.vertices(), points));

  for (const Surface& surface : space1.surfaces()) {
    if (surface.handle() != surface1.handle()) {
      bool test = false;

      points.clear();
      points.push_back(Point3d(0, 0, 0));
      points.push_back(Point3d(5, 0, 0));
      points.push_back(Point3d(5, 10, 0));
      points.push_back(Point3d(0, 10, 0));
      test = circularEqual(surface.vertices(), points);

      points.clear();
      points.push_back(Point3d(8, 0, 0));
      points.push_back(Point3d(10, 0, 0));
      points.push_back(Point3d(10, 5, 0));
      points.push_back(Point3d(8, 5, 0));
      test = (test || circularEqual(surface.vertices(), points));

      points.clear();
      points.push_back(Point3d(8, 8, 0));
      points.push_back(Point3d(10, 8, 0));
      points.push_back(Point3d(10, 10, 0));
      points.push_back(Point3d(8, 10, 0));
      test = (test || circularEqual(surface.vertices(), points));

      EXPECT_TRUE(test);
    }
  }

  points.clear();
  points.push_back(Point3d(5, 10, 0));
  points.push_back(Point3d(8, 10, 0));
  points.push_back(Point3d(8, 8, 0));
  points.push_back(Point3d(10, 8, 0));
  points.push_back(Point3d(10, 5, 0));
  points.push_back(Point3d(8, 5, 0));
  points.push_back(Point3d(8, 0, 0));
  points.push_back(Point3d(5, 0, 0));
  EXPECT_TRUE(circularEqual(surface2.vertices(), points));

  for (const Surface& surface : space2.surfaces()) {
    if (surface.handle() != surface2.handle()) {
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
      points.push_back(Point3d(5, 0, 0));
      points.push_back(Point3d(8, 0, 0));
      points.push_back(Point3d(8, -5, 0));
      points.push_back(Point3d(5, -5, 0));
      test = (test || circularEqual(surface.vertices(), points));

      EXPECT_TRUE(test);
    }
  }
}

TEST_F(ModelFixture, Surface_Intersect_UShape) {
  Model model;
  Space space1(model);
  Space space2(model);

  Point3dVector points1;
  points1.push_back(Point3d(0, 0, 0));
  points1.push_back(Point3d(10, 0, 0));
  points1.push_back(Point3d(10, 10, 0));
  points1.push_back(Point3d(0, 10, 0));
  Surface surface1(points1, model);
  surface1.setSpace(space1);
  double surface1Area = surface1.grossArea();
  EXPECT_GT(surface1Area, 0.0);

  Point3dVector points2;
  points2.push_back(Point3d(2, 15, 0));
  points2.push_back(Point3d(7, 15, 0));
  points2.push_back(Point3d(7, 8, 0));
  points2.push_back(Point3d(6, 8, 0));
  points2.push_back(Point3d(6, 12, 0));
  points2.push_back(Point3d(4, 12, 0));
  points2.push_back(Point3d(4, 8, 0));
  points2.push_back(Point3d(2, 8, 0));
  Surface surface2(points2, model);
  surface2.setSpace(space2);
  double surface2Area = surface2.grossArea();
  EXPECT_GT(surface2Area, 0.0);

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

  for (const Surface& surface : space1.surfaces()) {
    bool test = false;

    Point3dVector points;
    points.push_back(Point3d(2, 8, 0));
    points.push_back(Point3d(4, 8, 0));
    points.push_back(Point3d(4, 10, 0));
    points.push_back(Point3d(2, 10, 0));
    EXPECT_TRUE(circularEqual(surface1.vertices(), points));
    test = circularEqual(surface.vertices(), points);

    points.clear();
    points.push_back(Point3d(6, 8, 0));
    points.push_back(Point3d(7, 8, 0));
    points.push_back(Point3d(7, 10, 0));
    points.push_back(Point3d(6, 10, 0));
    test = (test || circularEqual(surface.vertices(), points));

    points.clear();
    points.push_back(Point3d(0, 0, 0));
    points.push_back(Point3d(10, 0, 0));
    points.push_back(Point3d(10, 10, 0));
    points.push_back(Point3d(7, 10, 0));
    points.push_back(Point3d(7, 8, 0));
    points.push_back(Point3d(6, 8, 0));
    points.push_back(Point3d(6, 10, 0));
    points.push_back(Point3d(4, 10, 0));
    points.push_back(Point3d(4, 8, 0));
    points.push_back(Point3d(2, 8, 0));
    points.push_back(Point3d(2, 10, 0));
    points.push_back(Point3d(0, 10, 0));
    test = (test || circularEqual(surface.vertices(), points));
    EXPECT_TRUE(test);
  }

  for (const Surface& surface : space2.surfaces()) {
    bool test = false;

    Point3dVector points;
    points.push_back(Point3d(2, 10, 0));
    points.push_back(Point3d(4, 10, 0));
    points.push_back(Point3d(4, 8, 0));
    points.push_back(Point3d(2, 8, 0));
    EXPECT_TRUE(circularEqual(surface2.vertices(), points));
    test = circularEqual(surface.vertices(), points);

    points.clear();
    points.push_back(Point3d(6, 10, 0));
    points.push_back(Point3d(7, 10, 0));
    points.push_back(Point3d(7, 8, 0));
    points.push_back(Point3d(6, 8, 0));
    test = (test || circularEqual(surface.vertices(), points));

    points.clear();
    points.push_back(Point3d(2, 15, 0));
    points.push_back(Point3d(7, 15, 0));
    points.push_back(Point3d(7, 10, 0));
    points.push_back(Point3d(6, 10, 0));
    points.push_back(Point3d(6, 12, 0));
    points.push_back(Point3d(4, 12, 0));
    points.push_back(Point3d(4, 10, 0));
    points.push_back(Point3d(2, 10, 0));
    test = (test || circularEqual(surface.vertices(), points));
    EXPECT_TRUE(test);
  }
}

TEST_F(ModelFixture, Surface_Intersect_SameShape_Tol_1) {
  double tol = 0.001;

  Model model;
  Space space1(model);
  Space space2(model);

  Point3dVector points1;
  points1.push_back(Point3d(0, 0, 0));
  points1.push_back(Point3d(10, 0, 0));
  points1.push_back(Point3d(10, 10, 0));
  points1.push_back(Point3d(0, 10, 0));
  Surface surface1(points1, model);
  surface1.setSpace(space1);
  double surface1Area = surface1.grossArea();
  EXPECT_GT(surface1Area, 0.0);

  Point3dVector points2;
  points2.push_back(Point3d(0, 10, 0));
  points2.push_back(Point3d(10 + tol, 10, 0));
  points2.push_back(Point3d(10 + tol, 0, 0));
  points2.push_back(Point3d(0, 0, 0));
  Surface surface2(points2, model);
  surface2.setSpace(space2);
  double surface2Area = surface2.grossArea();
  EXPECT_GT(surface2Area, 0.0);

  EXPECT_FALSE(surface1.adjacentSurface());

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

  EXPECT_EQ(1u, space1.surfaces().size());
  EXPECT_EQ(4u, surface1.vertices().size());
  EXPECT_EQ(1u, space2.surfaces().size());
  EXPECT_EQ(4u, surface2.vertices().size());

  EXPECT_FALSE(surface1.adjacentSurface());

  space1.matchSurfaces(space2);

  ASSERT_TRUE(surface1.adjacentSurface());
  EXPECT_EQ(surface2.handle(), surface1.adjacentSurface()->handle());
}

TEST_F(ModelFixture, Surface_Intersect_SameShape_Tol_2) {
  double tol = 0.001;

  Model model;
  Space space1(model);
  Space space2(model);

  Point3dVector points1;
  points1.push_back(Point3d(0, 0, 0));
  points1.push_back(Point3d(10, 0, 0));
  points1.push_back(Point3d(10, 10, 0));
  points1.push_back(Point3d(0, 10, 0));
  Surface surface1(points1, model);
  surface1.setSpace(space1);
  double surface1Area = surface1.grossArea();
  EXPECT_GT(surface1Area, 0.0);

  Point3dVector points2;
  points2.push_back(Point3d(0, 10, 0));
  points2.push_back(Point3d(10 - tol, 10, 0));
  points2.push_back(Point3d(10 - tol, 0, 0));
  points2.push_back(Point3d(0, 0, 0));
  Surface surface2(points2, model);
  surface2.setSpace(space2);
  double surface2Area = surface2.grossArea();
  EXPECT_GT(surface2Area, 0.0);

  EXPECT_FALSE(surface1.adjacentSurface());

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

  EXPECT_EQ(1u, space1.surfaces().size());
  EXPECT_EQ(4u, surface1.vertices().size());
  EXPECT_EQ(1u, space2.surfaces().size());
  EXPECT_EQ(4u, surface2.vertices().size());

  EXPECT_FALSE(surface1.adjacentSurface());

  space1.matchSurfaces(space2);

  ASSERT_TRUE(surface1.adjacentSurface());
  EXPECT_EQ(surface2.handle(), surface1.adjacentSurface()->handle());
}

TEST_F(ModelFixture, Surface_Intersect_Model22_Bug) {

  Model model;
  Space space1(model);
  Space space2(model);
  Space space3(model);
  Space space4(model);

  Point3dVector points1;
  points1.push_back(Point3d(33.5280021336, -29.717998476, 91.44));
  points1.push_back(Point3d(33.5280021336, -29.717998476, 86.868));
  points1.push_back(Point3d(33.5280021336, 29.718001524, 86.868));
  points1.push_back(Point3d(33.5280021336, 29.718001524, 91.44));
  Surface surface1(points1, model);
  surface1.setName("Surface 1292");
  surface1.setSpace(space1);
  double surface1Area = surface1.grossArea();
  EXPECT_GT(surface1Area, 0.0);
  EXPECT_EQ("Wall", surface1.surfaceType());
  EXPECT_EQ("Outdoors", surface1.outsideBoundaryCondition());
  EXPECT_EQ("SunExposed", surface1.sunExposure());
  EXPECT_EQ("WindExposed", surface1.windExposure());

  Point3dVector points2;
  points2.push_back(Point3d(33.5280021336, -29.717998476, 96.012));
  points2.push_back(Point3d(33.5280021336, -29.717998476, 91.44));
  points2.push_back(Point3d(33.5280021336, 29.718001524, 91.44));
  points2.push_back(Point3d(33.5280021336, 29.718001524, 96.012));
  Surface surface2(points2, model);
  surface2.setName("Surface 1358");
  surface2.setSpace(space2);
  double surface2Area = surface2.grossArea();
  EXPECT_GT(surface2Area, 0.0);
  EXPECT_EQ("Wall", surface2.surfaceType());
  EXPECT_EQ("Outdoors", surface2.outsideBoundaryCondition());
  EXPECT_EQ("SunExposed", surface2.sunExposure());
  EXPECT_EQ("WindExposed", surface2.windExposure());

  Point3dVector points3;
  points3.push_back(Point3d(33.5280021336, -29.717998476, 100.584));
  points3.push_back(Point3d(33.5280021336, -29.717998476, 96.012));
  points3.push_back(Point3d(33.5280021336, 29.718001524, 96.012));
  points3.push_back(Point3d(33.5280021336, 29.718001524, 100.584));
  Surface surface3(points3, model);
  surface3.setName("Surface 1424");
  surface3.setSpace(space3);
  double surface3Area = surface3.grossArea();
  EXPECT_GT(surface3Area, 0.0);
  EXPECT_EQ("Wall", surface3.surfaceType());
  EXPECT_EQ("Outdoors", surface3.outsideBoundaryCondition());
  EXPECT_EQ("SunExposed", surface3.sunExposure());
  EXPECT_EQ("WindExposed", surface3.windExposure());

  Point3dVector points4;
  points4.push_back(Point3d(28.9560021336, -25.145998476, 96.012));
  points4.push_back(Point3d(28.9560021336, -25.145998476, 91.44));
  points4.push_back(Point3d(28.9560021336, 25.146001524, 91.44));
  points4.push_back(Point3d(28.9560021336, 25.146001524, 96.012));
  Surface surface4(points4, model);
  surface4.setName("Surface 1384");
  surface4.setSpace(space4);
  double surface4Area = surface4.grossArea();
  EXPECT_GT(surface4Area, 0.0);
  EXPECT_EQ("Wall", surface4.surfaceType());
  EXPECT_EQ("Outdoors", surface4.outsideBoundaryCondition());
  EXPECT_EQ("SunExposed", surface3.sunExposure());
  EXPECT_EQ("WindExposed", surface4.windExposure());

  EXPECT_EQ(4u, model.getModelObjects<Surface>().size());
  EXPECT_FALSE(surface1.intersect(surface2));
  EXPECT_FALSE(surface1.intersect(surface3));
  EXPECT_FALSE(surface1.intersect(surface4));
  EXPECT_FALSE(surface2.intersect(surface3));
  EXPECT_FALSE(surface2.intersect(surface4));
  EXPECT_FALSE(surface3.intersect(surface4));
  EXPECT_EQ(4u, model.getModelObjects<Surface>().size());
}

TEST_F(ModelFixture, Surface_Intersect_Model131_Bug1) {

  Model model;
  Space space1(model);
  Space space2(model);
  Space space3(model);

  Point3dVector points1;
  points1.push_back(Point3d(17.0306990856, -25.1460018288, 3.6576));
  points1.push_back(Point3d(17.0306990856, 5.3339981712, 3.6576));
  points1.push_back(Point3d(3.3146990856, 5.3339981712, 3.6576));
  points1.push_back(Point3d(3.3146990856, -17.8308018288, 3.6576));
  Surface surface1(points1, model);
  surface1.setName("Surface 36");
  surface1.setSpace(space1);
  double surface1Area = surface1.grossArea();
  EXPECT_GT(surface1Area, 0.0);
  EXPECT_EQ("RoofCeiling", surface1.surfaceType());
  EXPECT_EQ("Outdoors", surface1.outsideBoundaryCondition());
  EXPECT_EQ("SunExposed", surface1.sunExposure());
  EXPECT_EQ("WindExposed", surface1.windExposure());

  Point3dVector points2;
  points2.push_back(Point3d(17.0306987808, 5.3339993904, 3.6576));
  points2.push_back(Point3d(17.0306987808, 15.6971993904, 3.6576));
  points2.push_back(Point3d(3.3146987808, 15.6971993904, 3.6576));
  points2.push_back(Point3d(3.3146987808, 5.3339993904, 3.6576));
  Surface surface2(points2, model);
  surface2.setName("Surface 42");
  surface2.setSpace(space2);
  double surface2Area = surface2.grossArea();
  EXPECT_GT(surface2Area, 0.0);
  EXPECT_EQ("RoofCeiling", surface2.surfaceType());
  EXPECT_EQ("Outdoors", surface2.outsideBoundaryCondition());
  EXPECT_EQ("SunExposed", surface2.sunExposure());
  EXPECT_EQ("WindExposed", surface2.windExposure());

  Point3dVector points3;
  points3.push_back(Point3d(3.3146990856, -17.8308018288, 3.6576));
  points3.push_back(Point3d(3.3146990856, 15.6971981712, 3.6576));
  points3.push_back(Point3d(17.0306990856, 15.6971981712, 3.6576));
  points3.push_back(Point3d(17.0306990856, -25.1460018288, 3.6576));
  Surface surface3(points3, model);
  surface3.setName("Surface 31");
  surface3.setSpace(space3);
  double surface3Area = surface3.grossArea();
  EXPECT_GT(surface3Area, 0.0);
  EXPECT_EQ("Floor", surface3.surfaceType());
  EXPECT_EQ("Ground", surface3.outsideBoundaryCondition());
  EXPECT_EQ("NoSun", surface3.sunExposure());
  EXPECT_EQ("NoWind", surface3.windExposure());

  EXPECT_NEAR(surface1.grossArea() + surface2.grossArea(), surface3.grossArea(), 0.01);

  EXPECT_EQ(3u, model.getModelObjects<Surface>().size());
  boost::optional<SurfaceIntersection> intersect;
  intersect = surface1.computeIntersection(surface2);
  EXPECT_FALSE(intersect);
  intersect = surface1.computeIntersection(surface3);
  ASSERT_TRUE(intersect);
  EXPECT_EQ(0u, intersect->newSurfaces1().size());
  ASSERT_EQ(1u, intersect->newSurfaces2().size());
  Surface surface4 = intersect->newSurfaces2()[0];
  intersect = surface2.computeIntersection(surface3);
  EXPECT_FALSE(intersect);
  intersect = surface2.computeIntersection(surface4);
  ASSERT_TRUE(intersect);
  EXPECT_EQ(0u, intersect->newSurfaces1().size());
  EXPECT_EQ(0u, intersect->newSurfaces2().size());
  EXPECT_EQ(4u, model.getModelObjects<Surface>().size());

  space1.matchSurfaces(space3);
  space2.matchSurfaces(space3);
  ASSERT_TRUE(surface1.adjacentSurface());
  EXPECT_EQ(surface3.handle(), surface1.adjacentSurface()->handle());
  ASSERT_TRUE(surface2.adjacentSurface());
  EXPECT_EQ(surface4.handle(), surface2.adjacentSurface()->handle());
}

TEST_F(ModelFixture, Surface_Intersect_Model131_Bug2) {

  Model model;
  Space space1(model);
  Space space2(model);
  Space space3(model);

  Point3dVector points1;
  points1.push_back(Point3d(17.0306990856, -25.1460018288, 3.6576));
  points1.push_back(Point3d(17.0306990856, 5.3339981712, 3.6576));
  points1.push_back(Point3d(3.3146990856, 5.3339981712, 3.6576));
  points1.push_back(Point3d(3.3146990856, -17.8308018288, 3.6576));
  Surface surface1(points1, model);
  surface1.setName("Surface 36");
  surface1.setSpace(space1);
  double surface1Area = surface1.grossArea();
  EXPECT_GT(surface1Area, 0.0);
  EXPECT_EQ("RoofCeiling", surface1.surfaceType());
  EXPECT_EQ("Outdoors", surface1.outsideBoundaryCondition());
  EXPECT_EQ("SunExposed", surface1.sunExposure());
  EXPECT_EQ("WindExposed", surface1.windExposure());

  Point3dVector points2;
  points2.push_back(Point3d(17.0306987808, 5.3339993904, 3.6576));
  points2.push_back(Point3d(17.0306987808, 15.6971993904, 3.6576));
  points2.push_back(Point3d(3.3146987808, 15.6971993904, 3.6576));
  points2.push_back(Point3d(3.3146987808, 5.3339993904, 3.6576));
  Surface surface2(points2, model);
  surface2.setName("Surface 42");
  surface2.setSpace(space2);
  double surface2Area = surface2.grossArea();
  EXPECT_GT(surface2Area, 0.0);
  EXPECT_EQ("RoofCeiling", surface2.surfaceType());
  EXPECT_EQ("Outdoors", surface2.outsideBoundaryCondition());
  EXPECT_EQ("SunExposed", surface2.sunExposure());
  EXPECT_EQ("WindExposed", surface2.windExposure());

  Point3dVector points3;
  points3.push_back(Point3d(3.3146990856, -17.8308018288, 3.6576));
  points3.push_back(Point3d(3.3146990856, 15.6971981712, 3.6576));
  points3.push_back(Point3d(17.0306990856, 15.6971981712, 3.6576));
  points3.push_back(Point3d(17.0306990856, -25.1460018288, 3.6576));
  Surface surface3(points3, model);
  surface3.setName("Surface 31");
  surface3.setSpace(space3);
  double surface3Area = surface3.grossArea();
  EXPECT_GT(surface3Area, 0.0);
  EXPECT_EQ("Floor", surface3.surfaceType());
  EXPECT_EQ("Ground", surface3.outsideBoundaryCondition());
  EXPECT_EQ("NoSun", surface3.sunExposure());
  EXPECT_EQ("NoWind", surface3.windExposure());

  EXPECT_NEAR(surface1.grossArea() + surface2.grossArea(), surface3.grossArea(), 0.01);

  EXPECT_EQ(3u, model.getModelObjects<Surface>().size());
  boost::optional<SurfaceIntersection> intersect;
  intersect = surface1.computeIntersection(surface2);
  EXPECT_FALSE(intersect);
  intersect = surface2.computeIntersection(surface3);
  ASSERT_TRUE(intersect);
  EXPECT_EQ(0u, intersect->newSurfaces1().size());
  ASSERT_EQ(1u, intersect->newSurfaces2().size());
  Surface surface4 = intersect->newSurfaces2()[0];
  surface4.setName("New Surface");
  EXPECT_EQ(4u, surface4.vertices().size());

  //std::cout << model << std::endl;

  intersect = surface1.computeIntersection(surface3);
  EXPECT_FALSE(intersect);
  intersect = surface1.computeIntersection(surface4);
  ASSERT_TRUE(intersect);
  EXPECT_EQ(0u, intersect->newSurfaces1().size());
  EXPECT_EQ(0u, intersect->newSurfaces2().size());
  EXPECT_EQ(4u, model.getModelObjects<Surface>().size());

  space1.matchSurfaces(space3);
  space2.matchSurfaces(space3);
  ASSERT_TRUE(surface1.adjacentSurface());
  EXPECT_EQ(surface4.handle(), surface1.adjacentSurface()->handle());
  ASSERT_TRUE(surface2.adjacentSurface());
  EXPECT_EQ(surface3.handle(), surface2.adjacentSurface()->handle());

  EXPECT_NEAR(surface1.netArea(), surface4.netArea(), 0.001);
  EXPECT_NEAR(surface2.netArea(), surface3.netArea(), 0.001);
}

TEST_F(ModelFixture, Surface_Intersect_Model131_Bug3) {

  Model model;
  Space space1(model);
  Space space2(model);
  Space space3(model);

  Point3dVector points1;
  points1.push_back(Point3d(17.0306990856, -25.1460018288, 3.6576));
  points1.push_back(Point3d(17.0306990856, 5.3339981712, 3.6576));
  points1.push_back(Point3d(3.3146990856, 5.3339981712, 3.6576));
  points1.push_back(Point3d(3.3146990856, -17.8308018288, 3.6576));
  Surface surface1(points1, model);
  surface1.setName("Surface 36");
  surface1.setSpace(space1);
  double surface1Area = surface1.grossArea();
  EXPECT_GT(surface1Area, 0.0);
  EXPECT_EQ("RoofCeiling", surface1.surfaceType());
  EXPECT_EQ("Outdoors", surface1.outsideBoundaryCondition());
  EXPECT_EQ("SunExposed", surface1.sunExposure());
  EXPECT_EQ("WindExposed", surface1.windExposure());

  Point3dVector points2;
  points2.push_back(Point3d(17.0306987808, 5.3339993904, 3.6576));
  points2.push_back(Point3d(17.0306987808, 15.6971993904, 3.6576));
  points2.push_back(Point3d(3.3146987808, 15.6971993904, 3.6576));
  points2.push_back(Point3d(3.3146987808, 5.3339993904, 3.6576));
  Surface surface2(points2, model);
  surface2.setName("Surface 42");
  surface2.setSpace(space2);
  double surface2Area = surface2.grossArea();
  EXPECT_GT(surface2Area, 0.0);
  EXPECT_EQ("RoofCeiling", surface2.surfaceType());
  EXPECT_EQ("Outdoors", surface2.outsideBoundaryCondition());
  EXPECT_EQ("SunExposed", surface2.sunExposure());
  EXPECT_EQ("WindExposed", surface2.windExposure());

  Point3dVector points3;
  points3.push_back(Point3d(3.3146990856, -17.8308018288, 3.6576));
  points3.push_back(Point3d(3.3146990856, 15.6971981712, 3.6576));
  points3.push_back(Point3d(17.0306990856, 15.6971981712, 3.6576));
  points3.push_back(Point3d(17.0306990856, -25.1460018288, 3.6576));
  Surface surface3(points3, model);
  surface3.setName("Surface 31");
  surface3.setSpace(space3);
  double surface3Area = surface3.grossArea();
  EXPECT_GT(surface3Area, 0.0);
  EXPECT_EQ("Floor", surface3.surfaceType());
  EXPECT_EQ("Ground", surface3.outsideBoundaryCondition());
  EXPECT_EQ("NoSun", surface3.sunExposure());
  EXPECT_EQ("NoWind", surface3.windExposure());

  EXPECT_NEAR(surface1.grossArea() + surface2.grossArea(), surface3.grossArea(), 0.01);

  EXPECT_EQ(3u, model.getModelObjects<Surface>().size());
  boost::optional<SurfaceIntersection> intersect;
  intersect = surface3.computeIntersection(surface1);
  ASSERT_TRUE(intersect);
  EXPECT_EQ(1u, intersect->newSurfaces1().size());
  ASSERT_EQ(0u, intersect->newSurfaces2().size());
  Surface surface4 = intersect->newSurfaces1()[0];
  intersect = surface3.computeIntersection(surface2);
  EXPECT_FALSE(intersect);
  intersect = surface4.computeIntersection(surface2);
  ASSERT_TRUE(intersect);
  EXPECT_EQ(0u, intersect->newSurfaces1().size());
  EXPECT_EQ(0u, intersect->newSurfaces2().size());
  EXPECT_EQ(4u, model.getModelObjects<Surface>().size());

  space1.matchSurfaces(space3);
  space2.matchSurfaces(space3);
  ASSERT_TRUE(surface1.adjacentSurface());
  EXPECT_EQ(surface3.handle(), surface1.adjacentSurface()->handle());
  ASSERT_TRUE(surface2.adjacentSurface());
  EXPECT_EQ(surface4.handle(), surface2.adjacentSurface()->handle());
}

TEST_F(ModelFixture, Surface_Intersect_Model131_Bug4) {

  Model model;
  Space space1(model);
  Space space2(model);
  Space space3(model);

  Point3dVector points1;
  points1.push_back(Point3d(17.0306990856, -25.1460018288, 3.6576));
  points1.push_back(Point3d(17.0306990856, 5.3339981712, 3.6576));
  points1.push_back(Point3d(3.3146990856, 5.3339981712, 3.6576));
  points1.push_back(Point3d(3.3146990856, -17.8308018288, 3.6576));
  Surface surface1(points1, model);
  surface1.setName("Surface 36");
  surface1.setSpace(space1);
  double surface1Area = surface1.grossArea();
  EXPECT_GT(surface1Area, 0.0);
  EXPECT_EQ("RoofCeiling", surface1.surfaceType());
  EXPECT_EQ("Outdoors", surface1.outsideBoundaryCondition());
  EXPECT_EQ("SunExposed", surface1.sunExposure());
  EXPECT_EQ("WindExposed", surface1.windExposure());

  Point3dVector points2;
  points2.push_back(Point3d(17.0306987808, 5.3339993904, 3.6576));
  points2.push_back(Point3d(17.0306987808, 15.6971993904, 3.6576));
  points2.push_back(Point3d(3.3146987808, 15.6971993904, 3.6576));
  points2.push_back(Point3d(3.3146987808, 5.3339993904, 3.6576));
  Surface surface2(points2, model);
  surface2.setName("Surface 42");
  surface2.setSpace(space2);
  double surface2Area = surface2.grossArea();
  EXPECT_GT(surface2Area, 0.0);
  EXPECT_EQ("RoofCeiling", surface2.surfaceType());
  EXPECT_EQ("Outdoors", surface2.outsideBoundaryCondition());
  EXPECT_EQ("SunExposed", surface2.sunExposure());
  EXPECT_EQ("WindExposed", surface2.windExposure());

  Point3dVector points3;
  points3.push_back(Point3d(3.3146990856, -17.8308018288, 3.6576));
  points3.push_back(Point3d(3.3146990856, 15.6971981712, 3.6576));
  points3.push_back(Point3d(17.0306990856, 15.6971981712, 3.6576));
  points3.push_back(Point3d(17.0306990856, -25.1460018288, 3.6576));
  Surface surface3(points3, model);
  surface3.setName("Surface 31");
  surface3.setSpace(space3);
  double surface3Area = surface3.grossArea();
  EXPECT_GT(surface3Area, 0.0);
  EXPECT_EQ("Floor", surface3.surfaceType());
  EXPECT_EQ("Ground", surface3.outsideBoundaryCondition());
  EXPECT_EQ("NoSun", surface3.sunExposure());
  EXPECT_EQ("NoWind", surface3.windExposure());

  EXPECT_NEAR(surface1.grossArea() + surface2.grossArea(), surface3.grossArea(), 0.01);

  EXPECT_EQ(3u, model.getModelObjects<Surface>().size());
  boost::optional<SurfaceIntersection> intersect;
  intersect = surface3.computeIntersection(surface2);
  ASSERT_TRUE(intersect);
  EXPECT_EQ(1u, intersect->newSurfaces1().size());
  ASSERT_EQ(0u, intersect->newSurfaces2().size());
  Surface surface4 = intersect->newSurfaces1()[0];
  intersect = surface3.computeIntersection(surface1);
  EXPECT_FALSE(intersect);
  intersect = surface4.computeIntersection(surface1);
  ASSERT_TRUE(intersect);
  EXPECT_EQ(0u, intersect->newSurfaces1().size());
  EXPECT_EQ(0u, intersect->newSurfaces2().size());
  EXPECT_EQ(4u, model.getModelObjects<Surface>().size());

  space1.matchSurfaces(space3);
  space2.matchSurfaces(space3);
  ASSERT_TRUE(surface1.adjacentSurface());
  EXPECT_EQ(surface4.handle(), surface1.adjacentSurface()->handle());
  ASSERT_TRUE(surface2.adjacentSurface());
  EXPECT_EQ(surface3.handle(), surface2.adjacentSurface()->handle());
}

TEST_F(ModelFixture, Surface_Figure8_SameSense) {

  Model model;
  Space space(model);

  Point3dVector points;
  points.push_back(Point3d(10, 0, 0));
  points.push_back(Point3d(20, 0, 0));
  points.push_back(Point3d(20, 10, 0));
  points.push_back(Point3d(10, 10, 0));
  points.push_back(Point3d(10, 20, 0));
  points.push_back(Point3d(0, 20, 0));
  points.push_back(Point3d(0, 10, 0));
  points.push_back(Point3d(10, 10, 0));
  points.push_back(Point3d(10, 5, 0));

  EXPECT_NO_THROW(Surface(points, model));
}

TEST_F(ModelFixture, Surface_Figure8_OppositeSense) {

  Model model;
  Space space(model);

  Point3dVector points;
  points.push_back(Point3d(10, 0, 0));
  points.push_back(Point3d(20, 0, 0));
  points.push_back(Point3d(20, 10, 0));
  points.push_back(Point3d(10, 10, 0));
  points.push_back(Point3d(0, 10, 0));
  points.push_back(Point3d(0, 20, 0));
  points.push_back(Point3d(10, 20, 0));
  points.push_back(Point3d(10, 10, 0));
  points.push_back(Point3d(10, 5, 0));

  EXPECT_THROW(Surface(points, model), openstudio::Exception);
}

TEST_F(ModelFixture, GroundSurface) {
  Model model;

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0, 1, 0));
  vertices.push_back(Point3d(1, 1, 0));
  vertices.push_back(Point3d(1, 0, 0));
  vertices.push_back(Point3d(0, 0, 0));

  Surface surface(vertices, model);
  EXPECT_EQ("Ground", surface.outsideBoundaryCondition());
  EXPECT_TRUE(surface.isGroundSurface());
  EXPECT_TRUE(surface.setOutsideBoundaryCondition("GroundFCfactorMethod"));
  EXPECT_EQ("GroundFCfactorMethod", surface.outsideBoundaryCondition());
  EXPECT_TRUE(surface.isGroundSurface());
}

TEST_F(ModelFixture, ApplyViewAndDaylightingGlassRatios) {
  double height = 1.2;
  double width = 10;

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0, 0, height));
  vertices.push_back(Point3d(0, 0, 0));
  vertices.push_back(Point3d(width, 0, 0));
  vertices.push_back(Point3d(width, 0, height));

  double area = 12;
  boost::optional<double> testArea = getArea(vertices);
  ASSERT_TRUE(testArea);
  EXPECT_DOUBLE_EQ(area, *testArea);

  {
    // call with all zeros
    double viewGlassToWallRatio = 0.0;
    double daylightingGlassToWallRatio = 0.0;
    double desiredViewGlassSillHeight = 0.0;
    double desiredDaylightingGlassHeaderHeight = 0.0;
    double exteriorShadingProjectionFactor = 0.0;
    double interiorShelfProjectionFactor = 0.0;
    boost::optional<ConstructionBase> viewGlassConstruction;
    boost::optional<ConstructionBase> daylightingGlassConstruction;

    Model model;
    Space space(model);
    Surface surface(vertices, model);
    surface.setSpace(space);
    std::vector<SubSurface> result = surface.applyViewAndDaylightingGlassRatios(
      viewGlassToWallRatio, daylightingGlassToWallRatio, desiredViewGlassSillHeight, desiredDaylightingGlassHeaderHeight,
      exteriorShadingProjectionFactor, interiorShelfProjectionFactor, viewGlassConstruction, daylightingGlassConstruction);
    EXPECT_EQ(0, result.size());
    EXPECT_DOUBLE_EQ(area, surface.grossArea());
    EXPECT_DOUBLE_EQ(area, surface.netArea());
  }

  {
    // just view glass, different sill heights
    double viewGlassToWallRatio = 0.2;
    double daylightingGlassToWallRatio = 0.0;
    double desiredViewGlassSillHeight = 0.0;
    double desiredDaylightingGlassHeaderHeight = 0.0;
    double exteriorShadingProjectionFactor = 0.0;
    double interiorShelfProjectionFactor = 0.0;

    for (desiredViewGlassSillHeight = 0.0; desiredViewGlassSillHeight < height; desiredViewGlassSillHeight += height / 10.0) {

      Model model;
      Construction viewGlassConstruction(model);
      Construction daylightingGlassConstruction(model);
      Space space(model);
      Surface surface(vertices, model);
      surface.setSpace(space);
      std::vector<SubSurface> result = surface.applyViewAndDaylightingGlassRatios(
        viewGlassToWallRatio, daylightingGlassToWallRatio, desiredViewGlassSillHeight, desiredDaylightingGlassHeaderHeight,
        exteriorShadingProjectionFactor, interiorShelfProjectionFactor, viewGlassConstruction, daylightingGlassConstruction);
      ASSERT_EQ(1, result.size());
      EXPECT_DOUBLE_EQ(area, surface.grossArea());
      EXPECT_NEAR((1 - viewGlassToWallRatio) * area, surface.netArea(), 0.001);
      EXPECT_NEAR(viewGlassToWallRatio * area, result[0].netArea(), 0.001);
      ASSERT_TRUE(result[0].construction());
      EXPECT_EQ(viewGlassConstruction.handle(), result[0].construction()->handle());
      EXPECT_FALSE(result[0].daylightingDeviceShelf());
      EXPECT_EQ(0, result[0].shadingSurfaceGroups().size());
    }
  }

  {
    // just daylighting glass, different header heights
    double viewGlassToWallRatio = 0.0;
    double daylightingGlassToWallRatio = 0.2;
    double desiredViewGlassSillHeight = 0.0;
    double desiredDaylightingGlassHeaderHeight = 0.0;
    double exteriorShadingProjectionFactor = 0.0;
    double interiorShelfProjectionFactor = 0.0;

    for (desiredDaylightingGlassHeaderHeight = 0.0; desiredDaylightingGlassHeaderHeight < height;
         desiredDaylightingGlassHeaderHeight += height / 10.0) {

      Model model;
      Construction viewGlassConstruction(model);
      Construction daylightingGlassConstruction(model);
      Space space(model);
      Surface surface(vertices, model);
      surface.setSpace(space);
      std::vector<SubSurface> result = surface.applyViewAndDaylightingGlassRatios(
        viewGlassToWallRatio, daylightingGlassToWallRatio, desiredViewGlassSillHeight, desiredDaylightingGlassHeaderHeight,
        exteriorShadingProjectionFactor, interiorShelfProjectionFactor, viewGlassConstruction, daylightingGlassConstruction);
      ASSERT_EQ(1, result.size());
      EXPECT_DOUBLE_EQ(area, surface.grossArea());
      EXPECT_NEAR((1 - daylightingGlassToWallRatio) * area, surface.netArea(), 0.001);
      EXPECT_NEAR(daylightingGlassToWallRatio * area, result[0].netArea(), 0.001);
      ASSERT_TRUE(result[0].construction());
      EXPECT_EQ(daylightingGlassConstruction.handle(), result[0].construction()->handle());
      EXPECT_FALSE(result[0].daylightingDeviceShelf());
      EXPECT_EQ(0, result[0].shadingSurfaceGroups().size());
    }
  }

  {
    // just daylighting glass with inside shelf
    double viewGlassToWallRatio = 0.0;
    double daylightingGlassToWallRatio = 0.2;
    double desiredViewGlassSillHeight = 0.0;
    double desiredDaylightingGlassHeaderHeight = 0.2;
    double exteriorShadingProjectionFactor = 0.5;
    double interiorShelfProjectionFactor = 0.5;

    Model model;
    Construction viewGlassConstruction(model);
    Construction daylightingGlassConstruction(model);
    Space space(model);
    Surface surface(vertices, model);
    surface.setSpace(space);
    std::vector<SubSurface> result = surface.applyViewAndDaylightingGlassRatios(
      viewGlassToWallRatio, daylightingGlassToWallRatio, desiredViewGlassSillHeight, desiredDaylightingGlassHeaderHeight,
      exteriorShadingProjectionFactor, interiorShelfProjectionFactor, viewGlassConstruction, daylightingGlassConstruction);
    ASSERT_EQ(1, result.size());
    EXPECT_DOUBLE_EQ(area, surface.grossArea());
    EXPECT_NEAR((1 - daylightingGlassToWallRatio) * area, surface.netArea(), 0.001);
    EXPECT_NEAR(daylightingGlassToWallRatio * area, result[0].netArea(), 0.001);
    ASSERT_TRUE(result[0].construction());
    EXPECT_EQ(daylightingGlassConstruction.handle(), result[0].construction()->handle());
    ASSERT_TRUE(result[0].daylightingDeviceShelf());
    ASSERT_TRUE(result[0].daylightingDeviceShelf()->insideShelf());
    EXPECT_FALSE(result[0].daylightingDeviceShelf()->outsideShelf());
    EXPECT_NEAR(interiorShelfProjectionFactor * daylightingGlassToWallRatio * area, result[0].daylightingDeviceShelf()->insideShelf()->netArea(),
                0.001);
    EXPECT_EQ(0, result[0].shadingSurfaceGroups().size());
  }

  {
    // both glass, different header heights
    double viewGlassToWallRatio = 0.2;
    double daylightingGlassToWallRatio = 0.2;
    double desiredViewGlassSillHeight = 0.0;
    double desiredDaylightingGlassHeaderHeight = 0.0;
    double exteriorShadingProjectionFactor = 0.0;
    double interiorShelfProjectionFactor = 0.0;

    for (desiredViewGlassSillHeight = 0.0; desiredViewGlassSillHeight < height; desiredViewGlassSillHeight += height / 10.0) {
      for (desiredDaylightingGlassHeaderHeight = 0.0; desiredDaylightingGlassHeaderHeight < height;
           desiredDaylightingGlassHeaderHeight += height / 10.0) {

        Model model;
        Construction viewGlassConstruction(model);
        Construction daylightingGlassConstruction(model);
        Space space(model);
        Surface surface(vertices, model);
        surface.setSpace(space);
        std::vector<SubSurface> result = surface.applyViewAndDaylightingGlassRatios(
          viewGlassToWallRatio, daylightingGlassToWallRatio, desiredViewGlassSillHeight, desiredDaylightingGlassHeaderHeight,
          exteriorShadingProjectionFactor, interiorShelfProjectionFactor, viewGlassConstruction, daylightingGlassConstruction);
        ASSERT_EQ(2, result.size());
        EXPECT_DOUBLE_EQ(area, surface.grossArea());
        EXPECT_NEAR((1 - viewGlassToWallRatio - daylightingGlassToWallRatio) * area, surface.netArea(), 0.001);
        EXPECT_NEAR(viewGlassToWallRatio * area, result[0].netArea(), 0.001);
        ASSERT_TRUE(result[0].construction());
        EXPECT_EQ(viewGlassConstruction.handle(), result[0].construction()->handle());
        EXPECT_NEAR(daylightingGlassToWallRatio * area, result[1].netArea(), 0.001);
        ASSERT_TRUE(result[1].construction());
        EXPECT_EQ(daylightingGlassConstruction.handle(), result[1].construction()->handle());
        EXPECT_FALSE(result[0].daylightingDeviceShelf());
        EXPECT_EQ(0, result[0].shadingSurfaceGroups().size());
      }
    }
  }
}

TEST_F(ModelFixture, Surface_Intersect_OneToFour) {

  double areaTol = 0.000001;
  double xOrigin = 20.0;

  // space 1 has one large surface, space 2 has 4 rectangles, test that intersection is correct independent of rotation and intersect order
  for (double rotation = 0; rotation < 360.0; rotation += 10.0) {
    for (unsigned iStart = 0; iStart < 4; ++iStart) {

      Transformation t = Transformation::rotation(Vector3d(0, 0, 1), degToRad(rotation));

      Model model;
      Space space1(model);
      Space space2(model);

      Point3dVector points;
      points.push_back(Point3d(xOrigin, 0, 20));
      points.push_back(Point3d(xOrigin, 0, 0));
      points.push_back(Point3d(xOrigin, 10, 0));
      points.push_back(Point3d(xOrigin, 10, 20));
      Surface surface(t * points, model);
      surface.setSpace(space1);
      EXPECT_NEAR(200.0, surface.grossArea(), areaTol);

      std::vector<Surface> surfaces;
      for (unsigned i = 0; i < 4; ++i) {
        points.clear();
        points.push_back(Point3d(xOrigin, 10, (i + 1) * 5));
        points.push_back(Point3d(xOrigin, 10, i * 5));
        points.push_back(Point3d(xOrigin, 0, i * 5));
        points.push_back(Point3d(xOrigin, 0, (i + 1) * 5));
        Surface tempSurface(t * points, model);
        tempSurface.setSpace(space2);
        EXPECT_NEAR(50.0, tempSurface.grossArea(), areaTol);
        surfaces.push_back(tempSurface);
      }

      // shuffle order of intersection
      std::vector<unsigned> indices;
      for (unsigned i = iStart; i < 4; ++i) {
        indices.push_back(i);
      }
      for (unsigned i = 0; i < iStart; ++i) {
        indices.push_back(i);
      }
      ASSERT_EQ(4u, indices.size());

      std::set<Handle> intersectedSpace1Surfaces;

      double expectedArea = 200.0;
      for (const unsigned i : indices) {

        double totalGrossArea = 0.0;
        for (const Surface& s : space1.surfaces()) {
          if (intersectedSpace1Surfaces.find(s.handle()) == intersectedSpace1Surfaces.end()) {
            totalGrossArea += s.grossArea();
          }
        }
        EXPECT_NEAR(expectedArea, totalGrossArea, areaTol);
        EXPECT_NEAR(50.0, surfaces[i].grossArea(), areaTol);

        // one of the non-intersected surfaces should intersect
        boost::optional<Surface> intersectedSurface;
        for (Surface& s : space1.surfaces()) {
          if (intersectedSpace1Surfaces.find(s.handle()) == intersectedSpace1Surfaces.end()) {
            if (s.intersect(surfaces[i])) {
              intersectedSurface = s;
              intersectedSpace1Surfaces.insert(s.handle());
              expectedArea -= 50.0;
              break;
            }
          }
        }
        ASSERT_TRUE(intersectedSurface);
        EXPECT_NEAR(50.0, intersectedSurface->grossArea(), areaTol);
        EXPECT_NEAR(50.0, surfaces[i].grossArea(), areaTol);
      }

      EXPECT_EQ(4u, space1.surfaces().size());
      for (const Surface& s : space1.surfaces()) {
        EXPECT_EQ(4u, s.vertices().size());
        EXPECT_NEAR(50.0, s.grossArea(), areaTol);
      }

      EXPECT_EQ(4u, space2.surfaces().size());
      for (const Surface& s : space2.surfaces()) {
        EXPECT_EQ(4u, s.vertices().size());
        EXPECT_NEAR(50.0, s.grossArea(), areaTol);
      }
    }
  }
}

TEST_F(ModelFixture, Surface_SurfacePropertyOtherSideCoefficients) {
  Model model;
  SurfacePropertyOtherSideCoefficients otherSideCoefficients(model);
}

TEST_F(ModelFixture, Surface_SurfacePropertyOtherSideConditionsModel) {
  Model model;
  SurfacePropertyOtherSideConditionsModel otherSideModel(model);
}
