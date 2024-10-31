/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "gbXMLFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/Model_Impl.hpp"
#include "../../model/ModelMerger.hpp"
#include "../../model/AdditionalProperties.hpp"
#include "../../model/AdditionalProperties_Impl.hpp"
#include "../../model/Building.hpp"
#include "../../model/Building_Impl.hpp"
#include "../../model/BuildingStory.hpp"
#include "../../model/BuildingStory_Impl.hpp"
#include "../../model/Construction.hpp"
#include "../../model/Construction_Impl.hpp"
#include "../../model/Facility.hpp"
#include "../../model/Facility_Impl.hpp"
#include "../../model/MasslessOpaqueMaterial.hpp"
#include "../../model/MasslessOpaqueMaterial_Impl.hpp"
#include "../../model/Material.hpp"
#include "../../model/Material_Impl.hpp"
#include "../../model/ScheduleDay.hpp"
#include "../../model/ScheduleDay_Impl.hpp"
#include "../../model/ScheduleWeek.hpp"
#include "../../model/ScheduleWeek_Impl.hpp"
#include "../../model/ScheduleYear.hpp"
#include "../../model/ScheduleYear_Impl.hpp"
#include "../../model/ShadingSurface.hpp"
#include "../../model/ShadingSurface_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/StandardOpaqueMaterial.hpp"
#include "../../model/StandardOpaqueMaterial_Impl.hpp"
#include "../../model/SubSurface.hpp"
#include "../../model/SubSurface_Impl.hpp"
#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/YearDescription.hpp"
#include "../../model/YearDescription_Impl.hpp"

#include "../../utilities/core/Compare.hpp"
#include "../../utilities/geometry/BoundingBox.hpp"
#include "../../utilities/xml/XMLValidator.hpp"

#include <resources.hxx>

#include <pugixml.hpp>

#include <algorithm>
#include <iostream>
#include <sstream>

using namespace openstudio::model;
using namespace openstudio::gbxml;
using namespace openstudio;

TEST_F(gbXMLFixture, ForwardTranslator_exampleModel) {
  Model model = exampleModel();

  path p = resourcesPath() / openstudio::toPath("gbxml/exampleModel.xml");

  ForwardTranslator forwardTranslator;
  bool test = forwardTranslator.modelToGbXML(model, p);

  EXPECT_TRUE(test);

  ReverseTranslator reverseTranslator;
  boost::optional<Model> model2 = reverseTranslator.loadModel(p);

  ASSERT_TRUE(model2);

  path p1 = resourcesPath() / openstudio::toPath("gbxml/exampleModel.osm");
  model.save(p1, true);

  path p2 = resourcesPath() / openstudio::toPath("gbxml/exampleModel2.osm");
  model2->save(p2, true);
}

TEST_F(gbXMLFixture, ForwardTranslator_AdiabaticSurface) {
  Model model = exampleModel();

  std::string surfname("Adiabatic_Surface");

  // Find a surface, make it adiabatic
  for (auto& surf : model.getConcreteModelObjects<Surface>()) {
    if (surf.outsideBoundaryCondition() == "Outdoors") {
      surf.setOutsideBoundaryCondition("Adiabatic");
      surf.setSunExposure("NoSun");
      surf.setWindExposure("NoWind");
      surf.setName(surfname);
      break;
    }
  }

  // Write out the XML
  path p = resourcesPath() / openstudio::toPath("gbxml/exampleModelAdiabatic.xml");

  ForwardTranslator forwardTranslator;
  bool test = forwardTranslator.modelToGbXML(model, p);

  EXPECT_TRUE(test);

  // Read the XML back in and check the surface
  ReverseTranslator reverseTranslator;
  boost::optional<Model> model2 = reverseTranslator.loadModel(p);

  ASSERT_TRUE(model2);

  auto osurf = model2->getConcreteModelObjectByName<Surface>(surfname);
  ASSERT_TRUE(osurf);
  EXPECT_EQ("Adiabatic", osurf->outsideBoundaryCondition());
  EXPECT_EQ("NoSun", osurf->sunExposure());
  EXPECT_EQ("NoWind", osurf->windExposure());
}

TEST_F(gbXMLFixture, ForwardTranslator_ConstructionLayers) {
  Model model;

  Construction construction(model);
  construction.setName("Construction1");

  MaterialVector layers;

  MasslessOpaqueMaterial material1(model);
  material1.setName("Material1");
  layers.push_back(material1);

  StandardOpaqueMaterial material2(model);
  material2.setName("Material2");
  layers.push_back(material2);

  MasslessOpaqueMaterial material3(model);
  material3.setName("Material3");
  layers.push_back(material3);

  StandardOpaqueMaterial material4(model);
  material4.setName("Material4");
  layers.push_back(material4);

  construction.setLayers(layers);

  auto facility = model.getUniqueModelObject<Facility>();

  Building building = model.getUniqueModelObject<Building>();

  Space space(model);
  space.setName("Space1");

  Point3dVector points{
    {0, 0, 1},
    {0, 0, 0},
    {1, 0, 0},
    {1, 0, 1},
  };

  //std::string surfname("Surface 1"); // DLM: note this will fail because "Surface 1" gets round tripped as "Surface_1"
  std::string surfname("Surface1");
  Surface surface(points, model);
  surface.setName(surfname);
  surface.setConstruction(construction);
  surface.setSpace(space);

  ThermalZone zone(model);
  space.setThermalZone(zone);

  // Write out the XML
  path p = resourcesPath() / openstudio::toPath("gbxml/ForwardTranslator_ConstructionLayers.xml");

  ForwardTranslator forwardTranslator;
  bool test = forwardTranslator.modelToGbXML(model, p);

  EXPECT_TRUE(test);

  // Read the XML back in and check the surface
  ReverseTranslator reverseTranslator;
  boost::optional<Model> model2 = reverseTranslator.loadModel(p);

  ASSERT_TRUE(model2);
  //std::cout << *model2 << '\n';
  auto osurf = model2->getConcreteModelObjectByName<Surface>(surfname);
  ASSERT_TRUE(osurf);
  auto ocons = osurf->construction();
  ASSERT_TRUE(ocons);
  auto olayeredcons = ocons->optionalCast<Construction>();
  ASSERT_TRUE(olayeredcons);
  ASSERT_EQ(4u, olayeredcons->layers().size());
  EXPECT_TRUE(olayeredcons->layers()[0].optionalCast<MasslessOpaqueMaterial>());
  EXPECT_TRUE(olayeredcons->layers()[1].optionalCast<StandardOpaqueMaterial>());
  EXPECT_TRUE(olayeredcons->layers()[2].optionalCast<MasslessOpaqueMaterial>());
  EXPECT_TRUE(olayeredcons->layers()[3].optionalCast<StandardOpaqueMaterial>());
}

TEST_F(gbXMLFixture, ForwardTranslator_NoFacility) {
  // Test for #3314: gbXML translation does not roundtrip unless Facility object present

  Model model;

  Construction construction(model);
  construction.setName("Construction1");

  MaterialVector layers;

  MasslessOpaqueMaterial material1(model);
  material1.setName("Material1");
  layers.push_back(material1);

  StandardOpaqueMaterial material2(model);
  material2.setName("Material2");
  layers.push_back(material2);

  MasslessOpaqueMaterial material3(model);
  material3.setName("Material3");
  layers.push_back(material3);

  StandardOpaqueMaterial material4(model);
  material4.setName("Material4");
  material4.setRoughness("MediumSmooth");
  layers.push_back(material4);

  construction.setLayers(layers);

  // Not instantiating facility nor building on purpose
  // Facility facility = model.getUniqueModelObject<Facility>();
  // Building building = model.getUniqueModelObject<Building>();

  Space space(model);
  space.setName("Space1");

  Point3dVector points{
    {0, 0, 1},
    {0, 0, 0},
    {1, 0, 0},
    {1, 0, 1},
  };

  //std::string surfname("Surface 1"); // DLM: note this will fail because "Surface 1" gets round tripped as "Surface_1"
  std::string surfname("Surface1");
  Surface surface(points, model);
  surface.setName(surfname);
  surface.setConstruction(construction);
  surface.setSpace(space);

  ThermalZone zone(model);
  zone.setName("Zone1");
  space.setThermalZone(zone);

  // save model for diffing
  bool debug = false;
  if (debug) {
    path modelPath = resourcesPath() / openstudio::toPath("gbxml/ForwardTranslator_NoFacility_original.osm");
    model.save(modelPath, true);
  }

  // Write out the XML
  path p = resourcesPath() / openstudio::toPath("gbxml/ForwardTranslator_NoFacility.xml");

  ForwardTranslator forwardTranslator;
  bool test = forwardTranslator.modelToGbXML(model, p);

  EXPECT_TRUE(test);

  // Read the XML back in and check surface/space/zone were all translated
  ReverseTranslator reverseTranslator;
  boost::optional<Model> model2 = reverseTranslator.loadModel(p);

  ASSERT_TRUE(model2);
  //std::cout << *model2 << '\n';
  auto osurf = model2->getConcreteModelObjectByName<Surface>(surfname);
  ASSERT_TRUE(osurf);
  auto ospace = model2->getConcreteModelObjectByName<Space>(space.nameString());
  ASSERT_TRUE(ospace);
  auto ozone = model2->getConcreteModelObjectByName<ThermalZone>(zone.nameString());  // Dragostea Din Tei!
  ASSERT_TRUE(ozone);

  // This really tests a RT feature, but doesn't really matter. When diffing original & rountripped, I noticed a diff in Material:
  // the roundtripped model has Roughness missing
  auto omat = model2->getConcreteModelObjectByName<StandardOpaqueMaterial>("Material4");
  ASSERT_TRUE(omat);
  EXPECT_EQ("MediumSmooth", omat->roughness());

  if (debug) {
    path modelPath2 = resourcesPath() / openstudio::toPath("gbxml/ForwardTranslator_NoFacility_roundtripped.osm");
    model2->save(modelPath2, true);
  }
}

TEST_F(gbXMLFixture, ForwardTranslator_surfaceType_4001) {
  // Test for #4001 : surfaceType is written twice when SlabOnGrade
  Model model = exampleModel();

  // Write out the XML
  path p = resourcesPath() / openstudio::toPath("gbxml/exampleModel.xml");

  ForwardTranslator forwardTranslator;
  bool test = forwardTranslator.modelToGbXML(model, p);

  EXPECT_TRUE(test);

  ASSERT_TRUE(openstudio::filesystem::exists(p));

  openstudio::filesystem::ifstream file(p, std::ios_base::binary);
  ASSERT_TRUE(file.is_open());
  pugi::xml_document doc;
  auto load_result = doc.load(file);
  file.close();
  ASSERT_TRUE(load_result) << "'" << p << "' Failed to load:\n"
                           << "Error description: " << load_result.description() << "\n"
                           << "Error offset: " << load_result.offset;

  // Now go select the Surface_1, which currently ends up with two surfaceType attributes before fix for #4001
  pugi::xpath_node surfaceXPath = doc.select_node("//Surface[@id='Surface_1']");
  ASSERT_TRUE(surfaceXPath);
  pugi::xml_node surfaceNode = surfaceXPath.node();
  EXPECT_EQ(1u, std::count_if(surfaceNode.attributes_begin(), surfaceNode.attributes_end(),
                              [](const auto& att) { return openstudio::istringEqual(att.name(), "surfaceType"); }));
  std::string surfaceType = surfaceNode.attribute("surfaceType").value();
  std::string expectedSurfaceType("SlabOnGrade");
  EXPECT_EQ(expectedSurfaceType, surfaceType);
}

TEST_F(gbXMLFixture, ForwardTranslator_exampleModel_State) {
  // Test for #4135: translating a model twice should produce the same result
  Model model = exampleModel();

  ForwardTranslator forwardTranslator;
  std::string gbXML_str1 = forwardTranslator.modelToGbXMLString(model);
  EXPECT_FALSE(gbXML_str1.empty());

  std::string gbXML_str2 = forwardTranslator.modelToGbXMLString(model);
  EXPECT_FALSE(gbXML_str2.empty());

  EXPECT_EQ(gbXML_str1.length(), gbXML_str2.length());
  EXPECT_GT(gbXML_str1.length(), 50000);
}

// Opens a model with known issues(TropicBird). In this model some surfaces
// are oriented incorrectly, this test verifies that the surfaces have been corrected.
// One example is surface T-00-316-I-F-32 that has  a z value of 121.035
// The two related spaces are 00 Plenum (with a z range of 119.635 - 120.535) and
// 316 LivingKitchen (121.033 - 126.835). So LivingKitchen is above 00 Plenum so the surface
// should be a floor for the LivingKitchen and a ceiling for 00 Plenum,
// The polyloop in the file is clockwise so the normal is pointing down, this orientation
// is applcable to 00 Plenum because it is the first space in the list but the surface
// is a ceiling for this space and so the orientation should be anti-clickwise.
// Thats the problem in a nutshell anyway!
// If the fix worked then when we check the orientation of all the surfaces we should not
// find any errors
TEST_F(gbXMLFixture, ForwardTranslator_Issue_4375) {

  // Load the file with known issues
  path p = resourcesPath() / openstudio::toPath("gbxml/TropicBird.xml");
  ASSERT_TRUE(openstudio::filesystem::exists(p));
  ReverseTranslator reverseTranslator;
  boost::optional<Model> model2 = reverseTranslator.loadModel(p);

  ASSERT_TRUE(model2);

  // Chck the surfaces
  const auto& spaces = model2->getConcreteModelObjects<Space>();
  for (const auto& space : spaces) {
    std::string spaceName = space.name().value();

    const auto& bounds = space.boundingBox();
    const auto& surfaces = space.surfaces();
    for (const auto& surface : surfaces) {
      std::string surfType = surface.surfaceType();
      std::string surfName = surface.name().value();

      double tol = 0.01;
      boost::optional<openstudio::model::Surface> adjacentSurf = surface.adjacentSurface();
      if ((surfType == "RoofCeiling" || surfType == "Floor") && adjacentSurf) {
        const auto& vertices = surface.vertices();

        if (std::abs(vertices[0].z() - bounds.maxZ().value()) > tol && std::abs(vertices[0].z() - bounds.minZ().value()) > tol) {

          // Log this because we cant do a face orientation check because the space
          // isnt a prism (it has > 2 levels of horizontal surfaces)
          continue;
        }

        if (std::abs(vertices[0].z() - bounds.maxZ().value()) <= tol) {

          // Surface is at the top of the space bounding box so it should be a roof/ceiling
          // and the normal should be up (z should be > 0)
          auto surfType = surface.surfaceType();
          ASSERT_EQ(surfType, "RoofCeiling");
          auto normal = surface.outwardNormal();
          ASSERT_TRUE(normal.z() > 0);

        } else if (std::abs(vertices[0].z() - bounds.minZ().value()) <= tol) {

          // Surface is at the bottom of the space's bounding box and so should be a floor
          // and the normal shuld be down (z < 0)
          auto surfType = surface.surfaceType();
          ASSERT_EQ(surfType, "Floor");
          auto normal = surface.outwardNormal();
          ASSERT_TRUE(normal.z() < 0);
        }
      }
    }
  }
}

Model testModel() {
  Model model;

  Building building = model.getUniqueModelObject<Building>();
  BuildingStory buildingStory(model);
  buildingStory.setNominalZCoordinate(0);
  buildingStory.setNominalFloortoFloorHeight(3);

  ThermalZone thermalZone(model);

  std::vector<Point3d> floorPrint;
  floorPrint.push_back(Point3d(0, 0, 0));
  floorPrint.push_back(Point3d(0, 10, 0));
  floorPrint.push_back(Point3d(10, 10, 0));
  floorPrint.push_back(Point3d(10, 0, 0));

  // make spaces
  boost::optional<Space> space1 = Space::fromFloorPrint(floorPrint, 3, model);
  OS_ASSERT(space1);
  space1->setThermalZone(thermalZone);
  space1->setBuildingStory(buildingStory);

  boost::optional<Space> space2 = Space::fromFloorPrint(floorPrint, 3, model);
  OS_ASSERT(space2);
  space2->setXOrigin(10);
  space2->setThermalZone(thermalZone);
  space2->setBuildingStory(buildingStory);

  boost::optional<Space> space3 = Space::fromFloorPrint(floorPrint, 3, model);
  OS_ASSERT(space3);
  space3->setYOrigin(10);
  space3->setThermalZone(thermalZone);
  space3->setBuildingStory(buildingStory);

  boost::optional<Space> space4 = Space::fromFloorPrint(floorPrint, 3, model);
  OS_ASSERT(space4);
  space4->setXOrigin(10);
  space4->setYOrigin(10);
  space4->setThermalZone(thermalZone);
  space4->setBuildingStory(buildingStory);

  // add a door to south wall of space1
  std::vector<Point3d> doorPoints;
  doorPoints.push_back(Point3d(2, 0, 2));
  doorPoints.push_back(Point3d(2, 0, 0));
  doorPoints.push_back(Point3d(4, 0, 0));
  doorPoints.push_back(Point3d(4, 0, 2));

  // find south wall
  std::vector<Surface> searchResults;

  searchResults = space1->findSurfaces(180.0, 180.0, 90.0, 90.0);
  OS_ASSERT(searchResults.size() >= 1);

  // add door
  SubSurface door(doorPoints, model);
  door.setSurface(searchResults[0]);

  // add a window to east wall of space2
  std::vector<Point3d> windowPoints;
  windowPoints.push_back(Point3d(10, 2, 2));
  windowPoints.push_back(Point3d(10, 2, 1));
  windowPoints.push_back(Point3d(10, 8, 1));
  windowPoints.push_back(Point3d(10, 8, 2));

  // find east wall
  searchResults = space2->findSurfaces(90.0, 90.0, 90.0, 90.0);
  OS_ASSERT(searchResults.size() >= 1);

  // add window
  SubSurface window(windowPoints, model);
  window.setSurface(searchResults[0]);

  // match surfaces
  std::vector<Space> spaces = model.getConcreteModelObjects<Space>();
  matchSurfaces(spaces);

  return model;
}

// Create two gbxml models from two identical osm models should produce
// gbxml models with surfaces in the same order. issue 4438
TEST_F(gbXMLFixture, ForwardTranslator_4438_Deterministic) {
  Model model1 = testModel();

  path p1 = resourcesPath() / openstudio::toPath("gbxml/exampleModel1.xml");

  ForwardTranslator forwardTranslator;
  bool test1 = forwardTranslator.modelToGbXML(model1, p1);

  EXPECT_TRUE(test1);

  path ps1 = resourcesPath() / openstudio::toPath("gbxml/exampleModel1.osm");
  model1.save(ps1, true);

  Model model2 = testModel();

  path p2 = resourcesPath() / openstudio::toPath("gbxml/exampleModel2.xml");

  //ForwardTranslator forwardTranslator;
  bool test2 = forwardTranslator.modelToGbXML(model2, p2);

  EXPECT_TRUE(test2);

  path ps2 = resourcesPath() / openstudio::toPath("gbxml/exampleModel2.osm");
  model1.save(ps2, true);
}

TEST_F(gbXMLFixture, ForwardTranslator_IDs_Names) {
  // Test for #4457 - Support gbXML translation where user-input <Name> is different from the id
  Model model = exampleModel();

  model.save(resourcesPath() / openstudio::toPath("gbxml/exampleModelIDsNames.osm"), true);

  ForwardTranslator forwardTranslator;
  ReverseTranslator reverseTranslator;

  // does not have additional properties (gbXMLId, displayName, CADObjectId)
  {
    path p = resourcesPath() / openstudio::toPath("gbxml/exampleModelIDsNames_1.xml");
    bool test = forwardTranslator.modelToGbXML(model, p);
    EXPECT_TRUE(test);

    boost::optional<Model> model2 = reverseTranslator.loadModel(p);
    ASSERT_TRUE(model2);

    model2->save(resourcesPath() / openstudio::toPath("gbxml/exampleModelIDsNames_1.osm"), true);

    {
      auto _obj = model2->getConcreteModelObjectByName<Building>("Building_1");
      ASSERT_TRUE(_obj);
      EXPECT_FALSE(_obj->additionalProperties().hasFeature("CADObjectId"));
      ASSERT_FALSE(_obj->additionalProperties().getFeatureAsString("CADObjectId"));
      ASSERT_FALSE(_obj->cadObjectId());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("displayName"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("displayName"));
      ASSERT_TRUE(_obj->displayName());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("displayName").get(), _obj->displayName().get());
      EXPECT_EQ("Building 1", _obj->displayName().get());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("gbXMLId"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("gbXMLId"));
      ASSERT_TRUE(_obj->gbXMLId());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("gbXMLId").get(), _obj->gbXMLId().get());
    }

    {
      auto _obj = model2->getConcreteModelObjectByName<BuildingStory>("Building_Story_1");
      ASSERT_TRUE(_obj);
      EXPECT_FALSE(_obj->additionalProperties().hasFeature("CADObjectId"));
      ASSERT_FALSE(_obj->additionalProperties().getFeatureAsString("CADObjectId"));
      ASSERT_FALSE(_obj->cadObjectId());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("displayName"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("displayName"));
      ASSERT_TRUE(_obj->displayName());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("displayName").get(), _obj->displayName().get());
      EXPECT_EQ("Building Story 1", _obj->displayName().get());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("gbXMLId"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("gbXMLId"));
      ASSERT_TRUE(_obj->gbXMLId());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("gbXMLId").get(), _obj->gbXMLId().get());
    }

    {
      auto _obj = model2->getConcreteModelObjectByName<Space>("Space_1");
      ASSERT_TRUE(_obj);
      EXPECT_FALSE(_obj->additionalProperties().hasFeature("CADObjectId"));
      ASSERT_FALSE(_obj->additionalProperties().getFeatureAsString("CADObjectId"));
      ASSERT_FALSE(_obj->cadObjectId());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("displayName"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("displayName"));
      ASSERT_TRUE(_obj->displayName());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("displayName").get(), _obj->displayName().get());
      EXPECT_EQ("Space 1", _obj->displayName().get());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("gbXMLId"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("gbXMLId"));
      ASSERT_TRUE(_obj->gbXMLId());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("gbXMLId").get(), _obj->gbXMLId().get());
    }

    {
      auto _obj = model2->getConcreteModelObjectByName<Surface>("Surface_1");
      ASSERT_TRUE(_obj);
      EXPECT_FALSE(_obj->additionalProperties().hasFeature("CADObjectId"));
      ASSERT_FALSE(_obj->additionalProperties().getFeatureAsString("CADObjectId"));
      ASSERT_FALSE(_obj->cadObjectId());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("displayName"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("displayName"));
      ASSERT_TRUE(_obj->displayName());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("displayName").get(), _obj->displayName().get());
      EXPECT_EQ("Surface 1", _obj->displayName().get());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("gbXMLId"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("gbXMLId"));
      ASSERT_TRUE(_obj->gbXMLId());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("gbXMLId").get(), _obj->gbXMLId().get());
    }

    {
      auto _obj = model2->getConcreteModelObjectByName<SubSurface>("Sub_Surface_1");
      ASSERT_TRUE(_obj);
      EXPECT_FALSE(_obj->additionalProperties().hasFeature("CADObjectId"));
      ASSERT_FALSE(_obj->additionalProperties().getFeatureAsString("CADObjectId"));
      ASSERT_FALSE(_obj->cadObjectId());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("displayName"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("displayName"));
      ASSERT_TRUE(_obj->displayName());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("displayName").get(), _obj->displayName().get());
      EXPECT_EQ("Sub Surface 1", _obj->displayName().get());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("gbXMLId"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("gbXMLId"));
      ASSERT_TRUE(_obj->gbXMLId());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("gbXMLId").get(), _obj->gbXMLId().get());
    }

    {
      auto _obj = model2->getConcreteModelObjectByName<ThermalZone>("Thermal_Zone_1");
      ASSERT_TRUE(_obj);
      EXPECT_FALSE(_obj->additionalProperties().hasFeature("CADObjectId"));
      ASSERT_FALSE(_obj->additionalProperties().getFeatureAsString("CADObjectId"));
      ASSERT_FALSE(_obj->cadObjectId());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("displayName"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("displayName"));
      ASSERT_TRUE(_obj->displayName());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("displayName").get(), _obj->displayName().get());
      EXPECT_EQ("Thermal Zone 1", _obj->displayName().get());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("gbXMLId"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("gbXMLId"));
      ASSERT_TRUE(_obj->gbXMLId());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("gbXMLId").get(), _obj->gbXMLId().get());
    }

    {
      auto _obj = model2->getConcreteModelObjectByName<Construction>("Exterior_Wall");
      ASSERT_TRUE(_obj);
      EXPECT_FALSE(_obj->additionalProperties().hasFeature("CADObjectId"));
      ASSERT_FALSE(_obj->additionalProperties().getFeatureAsString("CADObjectId"));
      ASSERT_FALSE(_obj->cadObjectId());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("displayName"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("displayName"));
      ASSERT_TRUE(_obj->displayName());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("displayName").get(), _obj->displayName().get());
      EXPECT_EQ("Exterior Wall", _obj->displayName().get());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("gbXMLId"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("gbXMLId"));
      ASSERT_TRUE(_obj->gbXMLId());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("gbXMLId").get(), _obj->gbXMLId().get());
    }

    {
      auto _obj = model2->getModelObjectByName<Material>("I02_25mm_insulation_board");
      ASSERT_TRUE(_obj);
      EXPECT_FALSE(_obj->additionalProperties().hasFeature("CADObjectId"));
      ASSERT_FALSE(_obj->additionalProperties().getFeatureAsString("CADObjectId"));
      ASSERT_FALSE(_obj->cadObjectId());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("displayName"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("displayName"));
      ASSERT_TRUE(_obj->displayName());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("displayName").get(), _obj->displayName().get());
      EXPECT_EQ("I02 25mm insulation board", _obj->displayName().get());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("gbXMLId"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("gbXMLId"));
      ASSERT_TRUE(_obj->gbXMLId());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("gbXMLId").get(), _obj->gbXMLId().get());
    }
  }

  // does have additional properties (gbXMLId, displayName, CADObjectId)
  {
    auto _building = model.getConcreteModelObjectByName<Building>("Building 1");
    ASSERT_TRUE(_building);
    EXPECT_TRUE(_building->setGBXMLId("Building1"));
    EXPECT_TRUE(_building->setDisplayName("Building 1"));
    EXPECT_TRUE(_building->setCADObjectId("1234"));

    auto _buildingStory = model.getConcreteModelObjectByName<BuildingStory>("Building Story 1");
    ASSERT_TRUE(_buildingStory);
    EXPECT_TRUE(_buildingStory->setGBXMLId("BuildingStory1"));
    EXPECT_TRUE(_buildingStory->setDisplayName("Building Story 1"));
    EXPECT_TRUE(_buildingStory->setCADObjectId("1234"));

    auto _space = model.getConcreteModelObjectByName<Space>("Space 1");
    ASSERT_TRUE(_space);
    EXPECT_TRUE(_space->setGBXMLId("Space1"));
    EXPECT_TRUE(_space->setDisplayName("Space 1"));
    EXPECT_TRUE(_space->setCADObjectId("1234"));

    auto _surface = model.getConcreteModelObjectByName<Surface>("Surface 1");
    ASSERT_TRUE(_surface);
    EXPECT_TRUE(_surface->setGBXMLId("Surface1"));
    EXPECT_TRUE(_surface->setDisplayName("Surface 1"));
    EXPECT_TRUE(_surface->setCADObjectId("1234"));

    auto _subSurface = model.getConcreteModelObjectByName<SubSurface>("Sub Surface 1");
    ASSERT_TRUE(_subSurface);
    EXPECT_TRUE(_subSurface->setGBXMLId("SubSurface1"));
    EXPECT_TRUE(_subSurface->setDisplayName("Sub Surface 1"));
    EXPECT_TRUE(_subSurface->setCADObjectId("1234"));

    auto _zone = model.getConcreteModelObjectByName<ThermalZone>("Thermal Zone 1");
    ASSERT_TRUE(_zone);
    EXPECT_TRUE(_zone->setGBXMLId("ThermalZone1"));
    EXPECT_TRUE(_zone->setDisplayName("Thermal Zone 1"));
    EXPECT_TRUE(_zone->setCADObjectId("1234"));

    auto _construction = model.getConcreteModelObjectByName<Construction>("Exterior Wall");
    ASSERT_TRUE(_construction);
    EXPECT_TRUE(_construction->setGBXMLId("ExteriorWall"));
    EXPECT_TRUE(_construction->setDisplayName("Exterior Wall"));
    EXPECT_TRUE(_construction->setCADObjectId("1234"));

    auto _material = model.getModelObjectByName<Material>("I02 25mm insulation board");
    ASSERT_TRUE(_material);
    EXPECT_TRUE(_material->setGBXMLId("I0225mminsulationboard"));
    EXPECT_TRUE(_material->setDisplayName("I02 25mm insulation board"));
    EXPECT_TRUE(_material->setCADObjectId("1234"));

    path p = resourcesPath() / openstudio::toPath("gbxml/exampleModelIDsNames_2.xml");
    bool test = forwardTranslator.modelToGbXML(model, p);
    EXPECT_TRUE(test);

    boost::optional<Model> model2 = reverseTranslator.loadModel(p);
    ASSERT_TRUE(model2);

    model2->save(resourcesPath() / openstudio::toPath("gbxml/exampleModelIDsNames_2.osm"), true);

    {
      auto _obj = model2->getConcreteModelObjectByName<Building>("Building_1");  // Building doesn't use gbXMLId
      ASSERT_TRUE(_obj);
      EXPECT_FALSE(_obj->additionalProperties().hasFeature("CADObjectId"));  // Building doesn't use CADObjectId
      ASSERT_FALSE(_obj->additionalProperties().getFeatureAsString("CADObjectId"));
      ASSERT_FALSE(_obj->cadObjectId());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("displayName"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("displayName"));
      ASSERT_TRUE(_obj->displayName());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("displayName").get(), _obj->displayName().get());
      EXPECT_EQ("Building 1", _obj->displayName().get());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("gbXMLId"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("gbXMLId"));
      ASSERT_TRUE(_obj->gbXMLId());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("gbXMLId").get(), _obj->gbXMLId().get());
      EXPECT_EQ("Building_1", _obj->gbXMLId().get());
    }

    {
      auto _obj = model2->getConcreteModelObjectByName<BuildingStory>("BuildingStory1");
      ASSERT_TRUE(_obj);
      EXPECT_FALSE(_obj->additionalProperties().hasFeature("CADObjectId"));  // BuildingStory doesn't use CADObjectId
      ASSERT_FALSE(_obj->additionalProperties().getFeatureAsString("CADObjectId"));
      ASSERT_FALSE(_obj->cadObjectId());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("displayName"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("displayName"));
      ASSERT_TRUE(_obj->displayName());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("displayName").get(), _obj->displayName().get());
      EXPECT_EQ("Building Story 1", _obj->displayName().get());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("gbXMLId"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("gbXMLId"));
      ASSERT_TRUE(_obj->gbXMLId());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("gbXMLId").get(), _obj->gbXMLId().get());
      EXPECT_EQ("BuildingStory1", _obj->gbXMLId().get());
    }

    {
      auto _obj = model2->getConcreteModelObjectByName<Space>("Space1");
      ASSERT_TRUE(_obj);
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("CADObjectId"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("CADObjectId"));
      ASSERT_TRUE(_obj->cadObjectId());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("CADObjectId").get(), _obj->cadObjectId().get());
      EXPECT_EQ("1234", _obj->cadObjectId().get());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("displayName"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("displayName"));
      ASSERT_TRUE(_obj->displayName());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("displayName").get(), _obj->displayName().get());
      EXPECT_EQ("Space 1", _obj->displayName().get());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("gbXMLId"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("gbXMLId"));
      ASSERT_TRUE(_obj->gbXMLId());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("gbXMLId").get(), _obj->gbXMLId().get());
      EXPECT_EQ("Space1", _obj->gbXMLId().get());
    }

    {
      auto _obj = model2->getConcreteModelObjectByName<Surface>("Surface1");
      ASSERT_TRUE(_obj);
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("CADObjectId"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("CADObjectId"));
      ASSERT_TRUE(_obj->cadObjectId());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("CADObjectId").get(), _obj->cadObjectId().get());
      EXPECT_EQ("1234", _obj->cadObjectId().get());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("displayName"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("displayName"));
      ASSERT_TRUE(_obj->displayName());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("displayName").get(), _obj->displayName().get());
      EXPECT_EQ("Surface 1", _obj->displayName().get());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("gbXMLId"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("gbXMLId"));
      ASSERT_TRUE(_obj->gbXMLId());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("gbXMLId").get(), _obj->gbXMLId().get());
      EXPECT_EQ("Surface1", _obj->gbXMLId().get());
    }

    {
      auto _obj = model2->getConcreteModelObjectByName<SubSurface>("SubSurface1");
      ASSERT_TRUE(_obj);
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("CADObjectId"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("CADObjectId"));
      ASSERT_TRUE(_obj->cadObjectId());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("CADObjectId").get(), _obj->cadObjectId().get());
      EXPECT_EQ("1234", _obj->cadObjectId().get());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("displayName"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("displayName"));
      ASSERT_TRUE(_obj->displayName());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("displayName").get(), _obj->displayName().get());
      EXPECT_EQ("Sub Surface 1", _obj->displayName().get());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("gbXMLId"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("gbXMLId"));
      ASSERT_TRUE(_obj->gbXMLId());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("gbXMLId").get(), _obj->gbXMLId().get());
      EXPECT_EQ("SubSurface1", _obj->gbXMLId().get());
    }

    {
      auto _obj = model2->getConcreteModelObjectByName<ThermalZone>("ThermalZone1");
      ASSERT_TRUE(_obj);
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("CADObjectId"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("CADObjectId"));
      ASSERT_TRUE(_obj->cadObjectId());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("CADObjectId").get(), _obj->cadObjectId().get());
      EXPECT_EQ("1234", _obj->cadObjectId().get());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("displayName"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("displayName"));
      ASSERT_TRUE(_obj->displayName());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("displayName").get(), _obj->displayName().get());
      EXPECT_EQ("Thermal Zone 1", _obj->displayName().get());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("gbXMLId"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("gbXMLId"));
      ASSERT_TRUE(_obj->gbXMLId());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("gbXMLId").get(), _obj->gbXMLId().get());
      EXPECT_EQ("ThermalZone1", _obj->gbXMLId().get());
    }

    {
      auto _obj = model2->getConcreteModelObjectByName<Construction>("ExteriorWall");
      ASSERT_TRUE(_obj);
      EXPECT_FALSE(_obj->additionalProperties().hasFeature("CADObjectId"));  // Construction doesn't use CADObjectId
      ASSERT_FALSE(_obj->additionalProperties().getFeatureAsString("CADObjectId"));
      ASSERT_FALSE(_obj->cadObjectId());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("displayName"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("displayName"));
      ASSERT_TRUE(_obj->displayName());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("displayName").get(), _obj->displayName().get());
      EXPECT_EQ("Exterior Wall", _obj->displayName().get());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("gbXMLId"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("gbXMLId"));
      ASSERT_TRUE(_obj->gbXMLId());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("gbXMLId").get(), _obj->gbXMLId().get());
      EXPECT_EQ("ExteriorWall", _obj->gbXMLId().get());
    }

    {
      auto _obj = model2->getModelObjectByName<Material>("I0225mminsulationboard");
      ASSERT_TRUE(_obj);
      EXPECT_FALSE(_obj->additionalProperties().hasFeature("CADObjectId"));  // Material doesn't use CADObjectId
      ASSERT_FALSE(_obj->additionalProperties().getFeatureAsString("CADObjectId"));
      ASSERT_FALSE(_obj->cadObjectId());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("displayName"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("displayName"));
      ASSERT_TRUE(_obj->displayName());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("displayName").get(), _obj->displayName().get());
      EXPECT_EQ("I02 25mm insulation board", _obj->displayName().get());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("gbXMLId"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("gbXMLId"));
      ASSERT_TRUE(_obj->gbXMLId());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("gbXMLId").get(), _obj->gbXMLId().get());
      EXPECT_EQ("I0225mminsulationboard", _obj->gbXMLId().get());
    }
  }
}

TEST_F(gbXMLFixture, ForwardTranslator_exampleModelValid) {
  Model model = exampleModel();

  path p = resourcesPath() / openstudio::toPath("gbxml/exampleModel.xml");

  ForwardTranslator forwardTranslator;
  bool test = forwardTranslator.modelToGbXML(model, p);

  EXPECT_TRUE(test);

  auto xmlValidator = XMLValidator::gbxmlValidator();

  EXPECT_TRUE(xmlValidator.validate(p));
  EXPECT_TRUE(xmlValidator.isValid());
  EXPECT_EQ(0, xmlValidator.warnings().size());

  auto errors = xmlValidator.errors();
  EXPECT_EQ(0, errors.size());
}

TEST_F(gbXMLFixture, ForwardTranslator_exposedToSun) {
  // Test for #4559 - OpenStudio exported gbXML 'exposedToSun' attribute not written
  Model model = exampleModel();

  // Write out the XML
  path p = resourcesPath() / openstudio::toPath("gbxml/exampleModel.xml");

  ForwardTranslator forwardTranslator;
  bool test = forwardTranslator.modelToGbXML(model, p);

  EXPECT_TRUE(test);

  ASSERT_TRUE(openstudio::filesystem::exists(p));

  openstudio::filesystem::ifstream file(p, std::ios_base::binary);
  ASSERT_TRUE(file.is_open());
  pugi::xml_document doc;
  auto load_result = doc.load(file);
  file.close();
  ASSERT_TRUE(load_result) << "'" << p << "' Failed to load:\n"
                           << "Error description: " << load_result.description() << "\n"
                           << "Error offset: " << load_result.offset;

  std::string exposedToSun;
  std::string expectedExposedToSun;

  pugi::xpath_node surfaceXPath1 = doc.select_node("//Surface[@id='Surface_1']");
  ASSERT_TRUE(surfaceXPath1);
  pugi::xml_node surfaceNode1 = surfaceXPath1.node();
  EXPECT_EQ(1u, std::count_if(surfaceNode1.attributes_begin(), surfaceNode1.attributes_end(),
                              [](const auto& att) { return openstudio::istringEqual(att.name(), "exposedToSun"); }));
  exposedToSun = surfaceNode1.attribute("exposedToSun").value();
  expectedExposedToSun = "false";
  EXPECT_EQ(expectedExposedToSun, exposedToSun);

  pugi::xpath_node surfaceXPath2 = doc.select_node("//Surface[@id='Surface_2']");
  ASSERT_TRUE(surfaceXPath2);
  pugi::xml_node surfaceNode2 = surfaceXPath2.node();
  EXPECT_EQ(1u, std::count_if(surfaceNode2.attributes_begin(), surfaceNode2.attributes_end(),
                              [](const auto& att) { return openstudio::istringEqual(att.name(), "exposedToSun"); }));
  exposedToSun = surfaceNode2.attribute("exposedToSun").value();
  expectedExposedToSun = "true";
  EXPECT_EQ(expectedExposedToSun, exposedToSun);
}

TEST_F(gbXMLFixture, ForwardTranslator_spaceVolume) {
  // Test for #4561 - gbxml export does not transfer thermalZone Volume
  {
    Model model = exampleModel();

    auto ospace1 = model.getConcreteModelObjectByName<Space>("Space 1");
    ASSERT_TRUE(ospace1);
    EXPECT_EQ(300.0, ospace1->volume());

    // Write out the XML
    path p = resourcesPath() / openstudio::toPath("gbxml/exampleModel.xml");

    ForwardTranslator forwardTranslator;
    bool test = forwardTranslator.modelToGbXML(model, p);

    EXPECT_TRUE(test);

    ASSERT_TRUE(openstudio::filesystem::exists(p));

    openstudio::filesystem::ifstream file(p, std::ios_base::binary);
    ASSERT_TRUE(file.is_open());
    pugi::xml_document doc;
    auto load_result = doc.load(file);
    file.close();
    ASSERT_TRUE(load_result) << "'" << p << "' Failed to load:\n"
                             << "Error description: " << load_result.description() << "\n"
                             << "Error offset: " << load_result.offset;

    pugi::xpath_node spaceXPath1 = doc.select_node("//Space[@id='Space_1']");
    ASSERT_TRUE(spaceXPath1);
    pugi::xml_node spaceNode1 = spaceXPath1.node();
    double volume = spaceNode1.child("Volume").text().as_double();
    EXPECT_EQ(300.0, volume);

    // Read the XML back in and check the space
    ReverseTranslator reverseTranslator;
    boost::optional<Model> model2 = reverseTranslator.loadModel(p);

    ASSERT_TRUE(model2);

    auto ospace2 = model2->getConcreteModelObjectByName<Space>("Space_1");
    ASSERT_TRUE(ospace2);
    EXPECT_EQ(300.0, ospace2->volume());
    EXPECT_FALSE(ospace2->isVolumeAutocalculated());

    auto ospace3 = model2->getConcreteModelObjectByName<Space>("Shading_Surface_Group_1");
    ASSERT_TRUE(ospace3);
    EXPECT_EQ(0.0, ospace3->volume());
    EXPECT_TRUE(ospace3->isVolumeAutocalculated());
  }

  {
    Model model = exampleModel();

    auto ospace1 = model.getConcreteModelObjectByName<Space>("Space 1");
    ASSERT_TRUE(ospace1);
    EXPECT_TRUE(ospace1->setVolume(305.0));
    EXPECT_EQ(305.0, ospace1->volume());

    // Write out the XML
    path p = resourcesPath() / openstudio::toPath("gbxml/exampleModel.xml");

    ForwardTranslator forwardTranslator;
    bool test = forwardTranslator.modelToGbXML(model, p);

    EXPECT_TRUE(test);

    ASSERT_TRUE(openstudio::filesystem::exists(p));

    openstudio::filesystem::ifstream file(p, std::ios_base::binary);
    ASSERT_TRUE(file.is_open());
    pugi::xml_document doc;
    auto load_result = doc.load(file);
    file.close();
    ASSERT_TRUE(load_result) << "'" << p << "' Failed to load:\n"
                             << "Error description: " << load_result.description() << "\n"
                             << "Error offset: " << load_result.offset;

    pugi::xpath_node spaceXPath1 = doc.select_node("//Space[@id='Space_1']");
    ASSERT_TRUE(spaceXPath1);
    pugi::xml_node spaceNode1 = spaceXPath1.node();
    double volume = spaceNode1.child("Volume").text().as_double();
    EXPECT_EQ(305.0, volume);

    // Read the XML back in and check the space
    ReverseTranslator reverseTranslator;
    boost::optional<Model> model2 = reverseTranslator.loadModel(p);

    ASSERT_TRUE(model2);

    auto ospace2 = model2->getConcreteModelObjectByName<Space>("Space_1");
    ASSERT_TRUE(ospace2);
    EXPECT_EQ(305.0, ospace2->volume());
    EXPECT_FALSE(ospace2->isVolumeAutocalculated());
  }
}
