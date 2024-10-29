/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "gbXMLFixture.hpp"

#include "../ReverseTranslator.hpp"
#include "../ForwardTranslator.hpp"

#include "../../energyplus/ForwardTranslator.hpp"

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

#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/core/Optional.hpp"
#include "../../utilities/geometry/Plane.hpp"
#include "../../utilities/time/Date.hpp"
#include "../../utilities/xml/XMLValidator.hpp"

#include <utilities/idd/OS_Surface_FieldEnums.hxx>
#include <utilities/idd/OS_SubSurface_FieldEnums.hxx>
#include <resources.hxx>

#include <sstream>
#include <utility>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(gbXMLFixture, ReverseTranslator_ZNETH) {
  //openstudio::Logger::instance().standardOutLogger().enable();
  //openstudio::Logger::instance().standardOutLogger().setLogLevel(Debug);

  openstudio::path inputPath = resourcesPath() / openstudio::toPath("gbxml/ZNETH.xml");
  openstudio::path outputPath = resourcesPath() / openstudio::toPath("gbxml/ZNETH2.xml");

  openstudio::gbxml::ReverseTranslator reverseTranslator;
  boost::optional<openstudio::model::Model> model = reverseTranslator.loadModel(inputPath);
  ASSERT_TRUE(model);

  // check for additional properties
  for (const auto& object : model->getModelObjects<ModelObject>()) {
    if (object.optionalCast<Space>() || object.optionalCast<Surface>() || object.optionalCast<ShadingSurface>()
        || object.optionalCast<SubSurface>()) {

      EXPECT_TRUE(object.hasAdditionalProperties()) << object;
      if (object.optionalCast<Surface>() && object.cast<Surface>().isAirWall()) {
        // air walls don't have cad object ids in this file
      } else {
        EXPECT_TRUE(object.additionalProperties().hasFeature("CADObjectId")) << object;
      }
      EXPECT_TRUE(object.additionalProperties().hasFeature("gbXMLId")) << object;
    }
  }

  model->save(resourcesPath() / openstudio::toPath("gbxml/ZNETH.osm"), true);

  // add test to see that surfaces that reference two spaces get "surface" boundary condition
  // e.g. surface named "su-76" should have "Surface" string for OutsideBoundaryCondition
  OptionalSurface osurf = model->getConcreteModelObjectByName<Surface>("su-76");  // su-76 is the id
  //OptionalSurface osurf = model->getConcreteModelObjectByName<Surface>("B-101-201-I-F-76");  // B-101-201-I-F-76 is the name
  ASSERT_TRUE(osurf);
  EXPECT_EQ("Surface", osurf->outsideBoundaryCondition());

  openstudio::energyplus::ForwardTranslator energyPlusTranslator;
  openstudio::Workspace workspace = energyPlusTranslator.translateModel(*model);

  EXPECT_TRUE(workspace.numObjects() > 0);

  workspace.save(resourcesPath() / openstudio::toPath("gbxml/ZNETH.idf"), true);

  openstudio::gbxml::ForwardTranslator forwardTranslator;
  bool test = forwardTranslator.modelToGbXML(*model, outputPath);
  EXPECT_TRUE(test);
}

TEST_F(gbXMLFixture, ReverseTranslator_Constructions) {

  openstudio::path inputPath = resourcesPath() / openstudio::toPath("gbxml/TestCube.xml");

  openstudio::gbxml::ReverseTranslator reverseTranslator;
  boost::optional<openstudio::model::Model> model = reverseTranslator.loadModel(inputPath);
  ASSERT_TRUE(model);

  model->save(resourcesPath() / openstudio::toPath("gbxml/TestCube.osm"), true);

  auto osurf = model->getConcreteModelObjectByName<Surface>("aim0757");  // aim0757 is the id
  //auto osurf = model->getConcreteModelObjectByName<Surface>("T-1-5-I-F-6 Reversed");  // T-1-5-I-F-6 Reversed is the name
  ASSERT_TRUE(osurf);
  EXPECT_EQ("Surface", osurf->outsideBoundaryCondition());

  auto oconstruct = osurf->construction();
  ASSERT_TRUE(oconstruct);
  EXPECT_EQ("aim0634", oconstruct->name().get());

  int count = 0;
  for (auto& srf : model->getConcreteModelObjects<Surface>()) {
    if (srf.outsideBoundaryCondition() != "Surface") {
      continue;
    }
    if (srf.name().get().find("Reversed") == std::string::npos) {
      auto other_name = srf.name().get() + " Reversed";
      auto other_surf = model->getConcreteModelObjectByName<Surface>(other_name);
      ASSERT_TRUE(other_surf);

      // the construction will be assigned to one of these surfaces, the other surface will have an empty construction
      // the reversed construction will be created and assigned at translation time
      auto srfConstructionName = srf.getString(OS_SurfaceFields::ConstructionName, false, true);
      auto other_srfConstructionName = other_surf->getString(OS_SurfaceFields::ConstructionName, false, true);
      if (srfConstructionName) {
        EXPECT_FALSE(other_srfConstructionName);
        auto oc = srf.construction();
        ASSERT_TRUE(oc);
        EXPECT_EQ(oc->name().get(), srfConstructionName.get());
      } else {
        EXPECT_FALSE(srfConstructionName);
        auto oc = other_surf->construction();
        ASSERT_TRUE(oc);
        EXPECT_EQ(oc->name().get(), other_srfConstructionName.get());
      }
      ++count;
    }
  }
  EXPECT_EQ(20, count);
}

TEST_F(gbXMLFixture, ReverseTranslator_SubSurfaceConstructions) {

  openstudio::path inputPath = resourcesPath() / openstudio::toPath("gbxml/seb.xml");

  openstudio::gbxml::ReverseTranslator reverseTranslator;
  boost::optional<openstudio::model::Model> model = reverseTranslator.loadModel(inputPath);
  ASSERT_TRUE(model);

  model->save(resourcesPath() / openstudio::toPath("gbxml/seb.osm"), true);

  auto osurf = model->getConcreteModelObjectByName<SubSurface>("Sub_Surface_8");
  ASSERT_TRUE(osurf);
  EXPECT_EQ("Outdoors", osurf->outsideBoundaryCondition());

  auto oconstruct = osurf->construction();
  ASSERT_TRUE(oconstruct);
  EXPECT_EQ("id_3'0\"_x_3'0\"_Double_pane__Alum_Construction", oconstruct->name().get());
  auto ofield = osurf->getString(OS_SubSurfaceFields::ConstructionName);
  ASSERT_TRUE(ofield);
  EXPECT_EQ("id_3'0\"_x_3'0\"_Double_pane__Alum_Construction", ofield.get());

  int count = 0;
  for (auto& srf : model->getConcreteModelObjects<SubSurface>()) {
    auto oc = srf.construction();
    ASSERT_TRUE(oc);
    ofield = srf.getString(OS_SubSurfaceFields::ConstructionName);
    ASSERT_TRUE(ofield);
    EXPECT_EQ(oc->name().get(), ofield.get());
    ++count;
  }
  EXPECT_EQ(8, count);
}

TEST_F(gbXMLFixture, ReverseTranslator_UndergroundWalls) {

  openstudio::path inputPath = resourcesPath() / openstudio::toPath("gbxml/TestCube.xml");

  openstudio::gbxml::ReverseTranslator reverseTranslator;
  boost::optional<openstudio::model::Model> model = reverseTranslator.loadModel(inputPath);
  ASSERT_TRUE(model);

  // Check all the surfaces that are supposed to be underground
  OptionalSurface osurf = model->getConcreteModelObjectByName<Surface>("aim0826");  // aim0826 is the id
  //OptionalSurface osurf = model->getConcreteModelObjectByName<Surface>("S-3-U-W-12");  // S-3-U-W-12 is the name
  ASSERT_TRUE(osurf);
  EXPECT_EQ("Ground", osurf->outsideBoundaryCondition());

  osurf = model->getConcreteModelObjectByName<Surface>("aim0780");  // aim0780 is the id
  //osurf = model->getConcreteModelObjectByName<Surface>("S-2-U-W-8");  // S-2-U-W-8 is the name
  ASSERT_TRUE(osurf);
  EXPECT_EQ("Ground", osurf->outsideBoundaryCondition());

  osurf = model->getConcreteModelObjectByName<Surface>("aim0769");  // aim0769 is the id
  //osurf = model->getConcreteModelObjectByName<Surface>("E-2-U-W-7");  // E-2-U-W-7 is the name
  ASSERT_TRUE(osurf);
  EXPECT_EQ("Ground", osurf->outsideBoundaryCondition());

  osurf = model->getConcreteModelObjectByName<Surface>("aim0700");  // aim0700 is the id
  //osurf = model->getConcreteModelObjectByName<Surface>("E-1-U-W-1");  // E-1-U-W-1 is the name
  ASSERT_TRUE(osurf);
  EXPECT_EQ("Ground", osurf->outsideBoundaryCondition());

  osurf = model->getConcreteModelObjectByName<Surface>("aim0711");  // aim0711 is the id
  //osurf = model->getConcreteModelObjectByName<Surface>("N-1-U-W-2");  // N-1-U-W-2 is the name
  ASSERT_TRUE(osurf);
  EXPECT_EQ("Ground", osurf->outsideBoundaryCondition());

  osurf = model->getConcreteModelObjectByName<Surface>("aim0894");  // aim0894 is the id
  //osurf = model->getConcreteModelObjectByName<Surface>("N-4-U-W-18");  // N-4-U-W-18 is the name
  ASSERT_TRUE(osurf);
  EXPECT_EQ("Ground", osurf->outsideBoundaryCondition());

  osurf = model->getConcreteModelObjectByName<Surface>("aim0883");  // aim0883 is the id
  //osurf = model->getConcreteModelObjectByName<Surface>("W-4-U-W-17");  // W-4-U-W-17 is the name
  ASSERT_TRUE(osurf);
  EXPECT_EQ("Ground", osurf->outsideBoundaryCondition());

  osurf = model->getConcreteModelObjectByName<Surface>("aim0837");  // aim0837 is the id
  //osurf = model->getConcreteModelObjectByName<Surface>("W-3-U-W-13");  // W-3-U-W-13 is the name
  ASSERT_TRUE(osurf);
  EXPECT_EQ("Ground", osurf->outsideBoundaryCondition());

  // Count the underground surfaces
  int count = 0;
  for (auto& surf : model->getConcreteModelObjects<Surface>()) {
    if (surf.outsideBoundaryCondition() == "Ground") {
      count += 1;
    }
  }
  EXPECT_EQ(12, count);  // 4 slabs + 8 walls
}

TEST_F(gbXMLFixture, ReverseTranslator_FloorSurfaces) {

  openstudio::path inputPath = resourcesPath() / openstudio::toPath("gbxml/TestCube.xml");

  openstudio::gbxml::ReverseTranslator reverseTranslator;
  boost::optional<openstudio::model::Model> model = reverseTranslator.loadModel(inputPath);
  ASSERT_TRUE(model);

  // Check all the surfaces that are supposed to be floors and ceilings

  struct ExpectedSurfaceInfo
  {
    ExpectedSurfaceInfo(std::string t_name, std::string t_surfaceType, std::string t_spaceName)
      : name(std::move(t_name)), surfaceType(std::move(t_surfaceType)), spaceName(std::move(t_spaceName)){};

    const std::string name;
    const std::string surfaceType;
    const std::string spaceName;
  };

  std::vector<ExpectedSurfaceInfo> expectedSurfaceInfos({ExpectedSurfaceInfo("aim0722", "Floor", "aim0046"),
                                                         ExpectedSurfaceInfo("aim0791", "Floor", "aim0106"),
                                                         ExpectedSurfaceInfo("aim0848", "Floor", "aim0154"),
                                                         ExpectedSurfaceInfo("aim0905", "Floor", "aim0202"),
                                                         ExpectedSurfaceInfo("aim0757", "Floor", "aim0250"),
                                                         ExpectedSurfaceInfo("aim0757 Reversed", "RoofCeiling", "aim0046"),
                                                         ExpectedSurfaceInfo("aim1191", "RoofCeiling", "aim0490"),
                                                         ExpectedSurfaceInfo("aim1236", "RoofCeiling", "aim0538"),
                                                         ExpectedSurfaceInfo("aim1281", "RoofCeiling", "aim0586"),
                                                         ExpectedSurfaceInfo("aim0814", "Floor", "aim0298"),
                                                         ExpectedSurfaceInfo("aim0814 Reversed", "RoofCeiling", "aim0106"),
                                                         ExpectedSurfaceInfo("aim0871", "Floor", "aim0346"),
                                                         ExpectedSurfaceInfo("aim0871 Reversed", "RoofCeiling", "aim0154"),
                                                         ExpectedSurfaceInfo("aim0916", "Floor", "aim0394"),
                                                         ExpectedSurfaceInfo("aim0916 Reversed", "RoofCeiling", "aim0202"),
                                                         ExpectedSurfaceInfo("aim0974", "Floor", "aim0442"),
                                                         ExpectedSurfaceInfo("aim0974 Reversed", "RoofCeiling", "aim0250"),
                                                         ExpectedSurfaceInfo("aim1020", "Floor", "aim0490"),
                                                         ExpectedSurfaceInfo("aim1020 Reversed", "RoofCeiling", "aim0298"),
                                                         ExpectedSurfaceInfo("aim1066", "Floor", "aim0538"),
                                                         ExpectedSurfaceInfo("aim1066 Reversed", "RoofCeiling", "aim0346"),
                                                         ExpectedSurfaceInfo("aim1100", "Floor", "aim0586"),
                                                         ExpectedSurfaceInfo("aim1100 Reversed", "RoofCeiling", "aim0394"),
                                                         ExpectedSurfaceInfo("aim1134", "RoofCeiling", "aim0442")});

  for (auto& expectedSurfaceInfo : expectedSurfaceInfos) {
    OptionalSurface _surf = model->getConcreteModelObjectByName<Surface>(expectedSurfaceInfo.name);
    ASSERT_TRUE(_surf);
    EXPECT_EQ(expectedSurfaceInfo.surfaceType, _surf->surfaceType()) << "Wrong surfaceType for " << expectedSurfaceInfo.name;
    auto _space = _surf->space();
    ASSERT_TRUE(_space);
    EXPECT_EQ(expectedSurfaceInfo.spaceName, _space->nameString()) << "Wrong space for " << expectedSurfaceInfo.name;
  }
}

TEST_F(gbXMLFixture, ReverseTranslator_AlternateUnits) {
  openstudio::path inputPath = resourcesPath() / openstudio::toPath("gbxml/TestCubeAlternateUnits.xml");

  openstudio::gbxml::ReverseTranslator reverseTranslator;
  boost::optional<openstudio::model::Model> model = reverseTranslator.loadModel(inputPath);
  ASSERT_TRUE(model);

  auto surfs = model->getConcreteModelObjects<Surface>();

  OptionalSurface osurf = model->getConcreteModelObjectByName<Surface>("aim0757");  // aim0757 is the id
  //OptionalSurface osurf = model->getConcreteModelObjectByName<Surface>("T-1-5-I-F-6");  // T-1-5-I-F-6 is the name
  ASSERT_TRUE(osurf);
  auto points = osurf->vertices();
  ASSERT_EQ(4, points.size());
  EXPECT_EQ(0.9144, points[1].y());

  //auto omat = model->getConcreteModelObjectByName<StandardOpaqueMaterial>("Concrete: 100 [mm]");
  //ASSERT_TRUE(omat);
  //EXPECT_NEAR(0.07407407, omat->thermalResistance(), 1.0e-8);
  //EXPECT_NEAR(1570.0, omat->density(), 1.0e-8);
  //EXPECT_NEAR(1.35, omat->conductivity(), 1.0e-8);
  //EXPECT_NEAR(0.1, omat->thickness(), 1.0e-8);
  //EXPECT_NEAR(840.0, omat->specificHeat(), 1.0e-8);
  //omat = model->getConcreteModelObjectByName<StandardOpaqueMaterial>("RockWool: 50 [mm]");
  //ASSERT_TRUE(omat);
  //EXPECT_NEAR(1.470588, omat->thermalResistance(), 1.0e-8);
  //EXPECT_NEAR(200.0, omat->density(), 1.0e-8);
  //EXPECT_NEAR(0.034, omat->conductivity(), 1.0e-8);
  //EXPECT_NEAR(0.05, omat->thickness(), 1.0e-8);
  //EXPECT_NEAR(710.0, omat->specificHeat(), 1.0e-8);
  //omat = model->getConcreteModelObjectByName<StandardOpaqueMaterial>("Concrete: 150 [mm]");
  //ASSERT_TRUE(omat);
  //EXPECT_NEAR(0.1111111, omat->thermalResistance(), 1.0e-8);
  //EXPECT_NEAR(1570.0, omat->density(), 1.0e-8);
  //EXPECT_NEAR(1.35, omat->conductivity(), 1.0e-8);
  //EXPECT_NEAR(0.15, omat->thickness(), 1.0e-8);
  //EXPECT_NEAR(840.0, omat->specificHeat(), 1.0e-8);
}

TEST_F(gbXMLFixture, ReverseTranslator_HandleMapping) {
  //openstudio::Logger::instance().standardOutLogger().enable();
  //openstudio::Logger::instance().standardOutLogger().setLogLevel(Debug);

  openstudio::path inputPath = resourcesPath() / openstudio::toPath("gbxml/ZNETH.xml");

  openstudio::gbxml::ReverseTranslator reverseTranslator;
  boost::optional<openstudio::model::Model> model1 = reverseTranslator.loadModel(inputPath);
  ASSERT_TRUE(model1);

  unsigned numSpaces = model1->getConcreteModelObjects<model::Space>().size();
  unsigned numSurfaces = model1->getConcreteModelObjects<model::Surface>().size();
  unsigned numPlanarSurfaces = model1->getModelObjects<model::PlanarSurface>().size();
  EXPECT_NE(0, numSpaces);
  EXPECT_NE(0, numSurfaces);
  EXPECT_NE(0, numPlanarSurfaces);

  boost::optional<openstudio::model::Model> model2 = reverseTranslator.loadModel(inputPath);
  ASSERT_TRUE(model2);
  EXPECT_EQ(numSpaces, model2->getConcreteModelObjects<model::Space>().size());
  EXPECT_EQ(numSurfaces, model2->getConcreteModelObjects<model::Surface>().size());
  EXPECT_EQ(numPlanarSurfaces, model2->getModelObjects<model::PlanarSurface>().size());

  for (const auto& object : model2->objects()) {
    EXPECT_FALSE(model1->getObject(object.handle()));
  }

  auto model1Clone1 = model1->clone(false).cast<model::Model>();
  auto model1Clone2 = model1->clone(false).cast<model::Model>();

  for (const auto& object : model1Clone1.objects()) {
    EXPECT_FALSE(model1->getObject(object.handle()));
    EXPECT_FALSE(model2->getObject(object.handle()));
    EXPECT_FALSE(model1Clone2.getObject(object.handle()));
  }

  model::ModelMerger mm;

  // no mapping merge, objects not in mapping are removed
  mm.mergeModels(model1Clone1, *model2, std::map<UUID, UUID>());
  EXPECT_EQ(numSpaces, model2->getConcreteModelObjects<model::Space>().size());
  EXPECT_EQ(numSurfaces, model2->getConcreteModelObjects<model::Surface>().size());
  EXPECT_EQ(numPlanarSurfaces, model2->getModelObjects<model::PlanarSurface>().size());
  EXPECT_EQ(numSpaces, model1Clone1.getConcreteModelObjects<model::Space>().size());
  EXPECT_EQ(numSurfaces, model1Clone1.getConcreteModelObjects<model::Surface>().size());
  EXPECT_EQ(numPlanarSurfaces, model1Clone1.getModelObjects<model::PlanarSurface>().size());

  // merge with mapping
  const auto mapping = mm.suggestHandleMapping(model1Clone2, *model2);
  for (const auto& space : model1Clone2.getConcreteModelObjects<model::Space>()) {
    const auto it = mapping.find(space.handle());
    // spaces are mapped
    ASSERT_TRUE(it != mapping.end());
    EXPECT_TRUE(model2->getObject(it->second));
  }
  for (const auto& planarSurface : model1Clone2.getModelObjects<model::PlanarSurface>()) {
    const auto it = mapping.find(planarSurface.handle());
    // surfaces are not mapped
    EXPECT_TRUE(it == mapping.end());
  }
  mm.mergeModels(model1Clone2, *model2, mapping);
  EXPECT_EQ(numSpaces, model2->getConcreteModelObjects<model::Space>().size());
  EXPECT_EQ(numSurfaces, model2->getConcreteModelObjects<model::Surface>().size());
  EXPECT_EQ(numPlanarSurfaces, model2->getModelObjects<model::PlanarSurface>().size());
  EXPECT_EQ(numSpaces, model1Clone2.getConcreteModelObjects<model::Space>().size());
  EXPECT_EQ(numSurfaces, model1Clone2.getConcreteModelObjects<model::Surface>().size());
  EXPECT_EQ(numPlanarSurfaces, model1Clone2.getModelObjects<model::PlanarSurface>().size());
}

TEST_F(gbXMLFixture, ReverseTranslator_SimpleBox_Vasari) {
  //openstudio::Logger::instance().standardOutLogger().enable();
  //openstudio::Logger::instance().standardOutLogger().setLogLevel(Debug);

  openstudio::path inputPath = resourcesPath() / openstudio::toPath("gbxml/simpleBox_vasari.xml");
  openstudio::path outputPath = resourcesPath() / openstudio::toPath("gbxml/simpleBox_vasari2.xml");

  openstudio::gbxml::ReverseTranslator reverseTranslator;
  boost::optional<openstudio::model::Model> model = reverseTranslator.loadModel(inputPath);
  ASSERT_TRUE(model);

  model->save(resourcesPath() / openstudio::toPath("gbxml/simpleBox_vasari.osm"), true);

  openstudio::energyplus::ForwardTranslator energyPlusTranslator;
  openstudio::Workspace workspace = energyPlusTranslator.translateModel(*model);

  EXPECT_TRUE(workspace.numObjects() > 0);

  workspace.save(resourcesPath() / openstudio::toPath("gbxml/simpleBox_vasari.idf"), true);

  openstudio::gbxml::ForwardTranslator forwardTranslator;
  bool test = forwardTranslator.modelToGbXML(*model, outputPath);
  EXPECT_TRUE(test);
}

TEST_F(gbXMLFixture, ReverseTranslator_TwoStoryOffice_Trane) {
  //openstudio::Logger::instance().standardOutLogger().enable();
  //openstudio::Logger::instance().standardOutLogger().setLogLevel(Debug);

  openstudio::path inputPath = resourcesPath() / openstudio::toPath("gbxml/TwoStoryOffice_Trane.xml");
  openstudio::path outputPath = resourcesPath() / openstudio::toPath("gbxml/TwoStoryOffice_Trane2.xml");

  openstudio::gbxml::ReverseTranslator reverseTranslator;
  boost::optional<openstudio::model::Model> model = reverseTranslator.loadModel(inputPath);
  ASSERT_TRUE(model);

  // check for additional properties
  for (const auto& object : model->getModelObjects<ModelObject>()) {
    if (object.optionalCast<ThermalZone>() || object.optionalCast<Space>() || object.optionalCast<Surface>() || object.optionalCast<ShadingSurface>()
        || object.optionalCast<SubSurface>()) {

      EXPECT_TRUE(object.hasAdditionalProperties()) << object;
      if (object.optionalCast<Surface>() && object.cast<Surface>().isAirWall()) {
        // air walls don't have cad object ids in this file
      } else {
        EXPECT_TRUE(object.additionalProperties().hasFeature("CADObjectId")) << object;
      }
      EXPECT_TRUE(object.additionalProperties().hasFeature("gbXMLId")) << object;
    }
  }

  model->save(resourcesPath() / openstudio::toPath("gbxml/TwoStoryOffice_Trane.osm"), true);

  openstudio::energyplus::ForwardTranslator energyPlusTranslator;
  openstudio::Workspace workspace = energyPlusTranslator.translateModel(*model);

  EXPECT_TRUE(workspace.numObjects() > 0);

  workspace.save(resourcesPath() / openstudio::toPath("gbxml/TwoStoryOffice_Trane.idf"), true);

  openstudio::gbxml::ForwardTranslator forwardTranslator;
  bool test = forwardTranslator.modelToGbXML(*model, outputPath);
  EXPECT_TRUE(test);
}

TEST_F(gbXMLFixture, ReverseTranslator_3951_Surface) {

  openstudio::path inputPath = resourcesPath() / openstudio::toPath("gbxml/3951_Geometry_bug.xml");

  openstudio::gbxml::ReverseTranslator reverseTranslator;
  boost::optional<openstudio::model::Model> _model = reverseTranslator.loadModel(inputPath);
  ASSERT_TRUE(_model);

  // Check all the surfaces that are supposed to be floors and ceilings
  {
    auto _surf = _model->getConcreteModelObjectByName<Surface>("surface-2");  // surface-2 is the id
    //auto _surf = _model->getConcreteModelObjectByName<Surface>("storey-1-slabongrade-space-1");  // storey-1-slabongrade-space-1 is the name
    ASSERT_TRUE(_surf);
    EXPECT_EQ("Floor", _surf->surfaceType());
    auto _space = _surf->space();
    ASSERT_TRUE(_space);
    EXPECT_EQ("space-1", _space->nameString());
  }

  {
    auto _surf = _model->getConcreteModelObjectByName<Surface>("surface-1");  // surface-1 is the id
    //auto _surf = _model->getConcreteModelObjectByName<Surface>("storey-1-ceiling-space-1");  // storey-1-ceiling-space-1 is the name
    ASSERT_TRUE(_surf);
    EXPECT_EQ("RoofCeiling", _surf->surfaceType());
    auto _space = _surf->space();
    ASSERT_TRUE(_space);
    EXPECT_EQ("space-1", _space->nameString());
  }
}

TEST_F(gbXMLFixture, ReverseTranslator_3997_WindowScaling) {
  openstudio::path inputPath = resourcesPath() / openstudio::toPath("gbxml/3997_WindowScaling_bug.xml");

  openstudio::gbxml::ReverseTranslator reverseTranslator;
  boost::optional<openstudio::model::Model> _model = reverseTranslator.loadModel(inputPath);
  ASSERT_TRUE(_model);

  // Check the SubSurface is contained on the same plane as its Surface
  auto _subSurfaces = _model->getConcreteModelObjects<SubSurface>();
  ASSERT_EQ(1u, _subSurfaces.size());
  SubSurface ss = _subSurfaces[0];
  ASSERT_TRUE(ss.surface());
  Surface s = ss.surface().get();
  EXPECT_TRUE(ss.plane().equal(s.plane()));

  // Might as well retest #3951 while we're at it
  // Check all the surfaces for their surfaceTypes and boundary conditions
  {
    auto _surf = _model->getConcreteModelObjectByName<Surface>("surface-2");  // surface-2 is the id
    //auto _surf = _model->getConcreteModelObjectByName<Surface>("storey-1-slabongrade-space-1");  // storey-1-slabongrade-space-1 is the name
    ASSERT_TRUE(_surf);
    EXPECT_EQ("Floor", _surf->surfaceType());
    auto _space = _surf->space();
    ASSERT_TRUE(_space);
    EXPECT_EQ("space-1", _space->nameString());  // space-1 is the id
    //EXPECT_EQ("storey-1-space-1", _space->nameString());  // storey-1-space-1 is the name
    EXPECT_EQ(0u, _surf->subSurfaces().size());
    EXPECT_EQ("Ground", _surf->outsideBoundaryCondition());
  }

  {
    auto _surf = _model->getConcreteModelObjectByName<Surface>("surface-1");  // surface-1 is the id
    //auto _surf = _model->getConcreteModelObjectByName<Surface>("storey-1-roof-space-1");  // storey-1-roof-space-1 is the name
    ASSERT_TRUE(_surf);
    EXPECT_EQ("RoofCeiling", _surf->surfaceType());
    auto _space = _surf->space();
    ASSERT_TRUE(_space);
    EXPECT_EQ("space-1", _space->nameString());  // space-1 is the id
    //EXPECT_EQ("storey-1-space-1", _space->nameString());  // storey-1-space-1 is the name
    EXPECT_EQ(0u, _surf->subSurfaces().size());
    EXPECT_EQ("Outdoors", _surf->outsideBoundaryCondition());
  }

  {
    auto _surf = _model->getConcreteModelObjectByName<Surface>("surface-19");  // surface-19 is the id
    //auto _surf = _model->getConcreteModelObjectByName<Surface>("storey-1-exterior-wall-1-space-1");  // storey-1-exterior-wall-1-space-1 is the name
    ASSERT_TRUE(_surf);
    EXPECT_EQ("Wall", _surf->surfaceType());
    auto _space = _surf->space();
    ASSERT_TRUE(_space);
    EXPECT_EQ("space-1", _space->nameString());  // space-1 is the id
    //EXPECT_EQ("storey-1-space-1", _space->nameString());  // storey-1-space-1 is the name
    EXPECT_EQ(1u, _surf->subSurfaces().size());
    EXPECT_EQ("Outdoors", _surf->outsideBoundaryCondition());
  }

  {
    auto _surf = _model->getConcreteModelObjectByName<Surface>("surface-11");  // surface-11 is the id
    //auto _surf = _model->getConcreteModelObjectByName<Surface>("storey-1-exterior-wall-2-space-1");  // storey-1-exterior-wall-2-space-1 is the name
    ASSERT_TRUE(_surf);
    EXPECT_EQ("Wall", _surf->surfaceType());
    auto _space = _surf->space();
    ASSERT_TRUE(_space);
    EXPECT_EQ("space-1", _space->nameString());  // space-1 is the id
    //EXPECT_EQ("storey-1-space-1", _space->nameString());  // storey-1-space-1 is the name
    EXPECT_EQ(0u, _surf->subSurfaces().size());
    EXPECT_EQ("Outdoors", _surf->outsideBoundaryCondition());
  }

  {
    auto _surf = _model->getConcreteModelObjectByName<Surface>("surface-15");  // surface-15 is the id
    //auto _surf = _model->getConcreteModelObjectByName<Surface>("storey-1-exterior-wall-diagonal-1-space-1");  // storey-1-exterior-wall-diagonal-1-space-1 is the name
    ASSERT_TRUE(_surf);
    EXPECT_EQ("Wall", _surf->surfaceType());
    auto _space = _surf->space();
    ASSERT_TRUE(_space);
    EXPECT_EQ("space-1", _space->nameString());  // space-1 is the id
    //EXPECT_EQ("storey-1-space-1", _space->nameString());  // storey-1-space-1 is the name
    EXPECT_EQ(0u, _surf->subSurfaces().size());
    EXPECT_EQ("Outdoors", _surf->outsideBoundaryCondition());
  }

  {
    auto _surf = _model->getConcreteModelObjectByName<Surface>("surface-16");  // surface-16 is the id
    //auto _surf = _model->getConcreteModelObjectByName<Surface>("storey-1-exterior-wall-diagonal-2-space-1");  // storey-1-exterior-wall-diagonal-2-space-1 is the name
    ASSERT_TRUE(_surf);
    EXPECT_EQ("Wall", _surf->surfaceType());
    auto _space = _surf->space();
    ASSERT_TRUE(_space);
    EXPECT_EQ("space-1", _space->nameString());  // space-1 is the id
    //EXPECT_EQ("storey-1-space-1", _space->nameString());  // storey-1-space-1 is the name
    EXPECT_EQ(0u, _surf->subSurfaces().size());
    EXPECT_EQ("Outdoors", _surf->outsideBoundaryCondition());
  }
}

TEST_F(gbXMLFixture, ReverseTranslator_Schedules_Basic) {

  // Test for #4439 - Properly RT gbxml Schedules
  openstudio::path inputPath = resourcesPath() / openstudio::toPath("gbxml/TestCube.xml");

  openstudio::gbxml::ReverseTranslator reverseTranslator;
  boost::optional<openstudio::model::Model> model = reverseTranslator.loadModel(inputPath);
  ASSERT_TRUE(model);

  EXPECT_EQ(2U, model->getConcreteModelObjects<ScheduleYear>().size());
  EXPECT_EQ(2U, model->getConcreteModelObjects<ScheduleWeek>().size());
  EXPECT_EQ(2U, model->getConcreteModelObjects<ScheduleDay>().size());
}

TEST_F(gbXMLFixture, ReverseTranslator_Schedules_Complex) {

  // Test for #4439 - Properly RT gbxml Schedules
  openstudio::path inputPath = resourcesPath() / openstudio::toPath("gbxml/TestSchedules.xml");

  openstudio::gbxml::ReverseTranslator reverseTranslator;
  boost::optional<openstudio::model::Model> model_ = reverseTranslator.loadModel(inputPath);
  ASSERT_TRUE(model_);
  auto m = model_.get();

  m.save(resourcesPath() / openstudio::toPath("gbxml/TestSchedules.osm"), true);

  // One Schedule is simple: 1 YearSchedule, 1 WeekSchedule, 1 DaySchedule
  // The other is complex: Two week schedules, one with a single day schedules, and one with two
  EXPECT_EQ(2U, m.getConcreteModelObjects<ScheduleYear>().size());
  EXPECT_EQ(3U, m.getConcreteModelObjects<ScheduleWeek>().size());
  EXPECT_EQ(4U, m.getConcreteModelObjects<ScheduleDay>().size());

  ASSERT_TRUE(m.yearDescription());
  auto yd = m.getUniqueModelObject<model::YearDescription>();
  ASSERT_TRUE(yd.calendarYear());
  EXPECT_EQ(2017, yd.calendarYear().get());

  // auto jan1 = yd.makeDate(1, 1);
  auto june30 = yd.makeDate(6, 30);
  // auto july1 = yd.makeDate(7, 1);
  auto dec31 = yd.makeDate(12, 31);

  // Test simple Schedule
  {
    auto schYear_ = m.getConcreteModelObjectByName<ScheduleYear>("Simple Year Schedule");
    EXPECT_TRUE(schYear_);
    ASSERT_EQ(1U, schYear_->dates().size());
    EXPECT_EQ(dec31, schYear_->dates()[0]);
    ASSERT_EQ(1U, schYear_->scheduleWeeks().size());
    auto schWeek = schYear_->scheduleWeeks()[0];
    EXPECT_EQ("Simple Year Schedule - Typical Week", schWeek.nameString());

    auto schDay_ = m.getConcreteModelObjectByName<ScheduleDay>("Simple Year Schedule - Typical Day");
    ASSERT_TRUE(schDay_);
    auto schDay = schDay_.get();

    ASSERT_TRUE(schWeek.sundaySchedule());
    EXPECT_EQ(schDay, schWeek.sundaySchedule().get());
    ASSERT_TRUE(schWeek.mondaySchedule());
    EXPECT_EQ(schDay, schWeek.mondaySchedule().get());
    ASSERT_TRUE(schWeek.tuesdaySchedule());
    EXPECT_EQ(schDay, schWeek.tuesdaySchedule().get());
    ASSERT_TRUE(schWeek.wednesdaySchedule());
    EXPECT_EQ(schDay, schWeek.wednesdaySchedule().get());
    ASSERT_TRUE(schWeek.thursdaySchedule());
    EXPECT_EQ(schDay, schWeek.thursdaySchedule().get());
    ASSERT_TRUE(schWeek.fridaySchedule());
    EXPECT_EQ(schDay, schWeek.fridaySchedule().get());
    ASSERT_TRUE(schWeek.saturdaySchedule());
    EXPECT_EQ(schDay, schWeek.saturdaySchedule().get());
    ASSERT_TRUE(schWeek.holidaySchedule());
    EXPECT_EQ(schDay, schWeek.holidaySchedule().get());
    ASSERT_TRUE(schWeek.summerDesignDaySchedule());
    EXPECT_EQ(schDay, schWeek.summerDesignDaySchedule().get());
    ASSERT_TRUE(schWeek.winterDesignDaySchedule());
    EXPECT_EQ(schDay, schWeek.winterDesignDaySchedule().get());
    ASSERT_TRUE(schWeek.customDay1Schedule());
    EXPECT_EQ(schDay, schWeek.customDay1Schedule().get());
    ASSERT_TRUE(schWeek.customDay2Schedule());
    EXPECT_EQ(schDay, schWeek.customDay2Schedule().get());
  }

  // Test Complex Schedule
  {
    auto schYear_ = m.getConcreteModelObjectByName<ScheduleYear>("Complex Year Schedule");
    EXPECT_TRUE(schYear_);
    ASSERT_EQ(2U, schYear_->dates().size());
    EXPECT_EQ(june30, schYear_->dates()[0]);
    EXPECT_EQ(dec31, schYear_->dates()[1]);
    ASSERT_EQ(2U, schYear_->scheduleWeeks().size());

    {
      auto schWeek = schYear_->scheduleWeeks()[0];
      EXPECT_EQ("Complex Year Schedule - Typical Week January to June", schWeek.nameString());

      auto schDay_ = m.getConcreteModelObjectByName<ScheduleDay>("Complex Year Schedule - Typical Day January to June");
      ASSERT_TRUE(schDay_);
      auto schDay = schDay_.get();

      ASSERT_TRUE(schWeek.sundaySchedule());
      EXPECT_EQ(schDay, schWeek.sundaySchedule().get());
      ASSERT_TRUE(schWeek.mondaySchedule());
      EXPECT_EQ(schDay, schWeek.mondaySchedule().get());
      ASSERT_TRUE(schWeek.tuesdaySchedule());
      EXPECT_EQ(schDay, schWeek.tuesdaySchedule().get());
      ASSERT_TRUE(schWeek.wednesdaySchedule());
      EXPECT_EQ(schDay, schWeek.wednesdaySchedule().get());
      ASSERT_TRUE(schWeek.thursdaySchedule());
      EXPECT_EQ(schDay, schWeek.thursdaySchedule().get());
      ASSERT_TRUE(schWeek.fridaySchedule());
      EXPECT_EQ(schDay, schWeek.fridaySchedule().get());
      ASSERT_TRUE(schWeek.saturdaySchedule());
      EXPECT_EQ(schDay, schWeek.saturdaySchedule().get());
      ASSERT_TRUE(schWeek.holidaySchedule());
      EXPECT_EQ(schDay, schWeek.holidaySchedule().get());
      ASSERT_TRUE(schWeek.summerDesignDaySchedule());
      EXPECT_EQ(schDay, schWeek.summerDesignDaySchedule().get());
      ASSERT_TRUE(schWeek.winterDesignDaySchedule());
      EXPECT_EQ(schDay, schWeek.winterDesignDaySchedule().get());
      ASSERT_TRUE(schWeek.customDay1Schedule());
      EXPECT_EQ(schDay, schWeek.customDay1Schedule().get());
      ASSERT_TRUE(schWeek.customDay2Schedule());
      EXPECT_EQ(schDay, schWeek.customDay2Schedule().get());
    }

    {
      auto schWeek = schYear_->scheduleWeeks()[1];
      EXPECT_EQ("Complex Year Schedule - Typical Week July to December", schWeek.nameString());

      auto schDayWeekday_ = m.getConcreteModelObjectByName<ScheduleDay>("Complex Year Schedule - Typical Weekday July to December");
      ASSERT_TRUE(schDayWeekday_);
      auto schDayWeekday = schDayWeekday_.get();

      auto schDayWeekend_ = m.getConcreteModelObjectByName<ScheduleDay>("Complex Year Schedule - Typical Weekend July to December");
      ASSERT_TRUE(schDayWeekend_);
      auto schDayWeekend = schDayWeekend_.get();

      ASSERT_TRUE(schWeek.mondaySchedule());
      EXPECT_EQ(schDayWeekday, schWeek.mondaySchedule().get());
      ASSERT_TRUE(schWeek.tuesdaySchedule());
      EXPECT_EQ(schDayWeekday, schWeek.tuesdaySchedule().get());
      ASSERT_TRUE(schWeek.wednesdaySchedule());
      EXPECT_EQ(schDayWeekday, schWeek.wednesdaySchedule().get());
      ASSERT_TRUE(schWeek.thursdaySchedule());
      EXPECT_EQ(schDayWeekday, schWeek.thursdaySchedule().get());
      ASSERT_TRUE(schWeek.fridaySchedule());
      EXPECT_EQ(schDayWeekday, schWeek.fridaySchedule().get());

      ASSERT_TRUE(schWeek.saturdaySchedule());
      EXPECT_EQ(schDayWeekend, schWeek.saturdaySchedule().get());
      ASSERT_TRUE(schWeek.sundaySchedule());
      EXPECT_EQ(schDayWeekend, schWeek.sundaySchedule().get());

      EXPECT_FALSE(schWeek.holidaySchedule());
      EXPECT_FALSE(schWeek.summerDesignDaySchedule());
      EXPECT_FALSE(schWeek.winterDesignDaySchedule());
      EXPECT_FALSE(schWeek.customDay1Schedule());
      EXPECT_FALSE(schWeek.customDay2Schedule());
    }
  }
}

TEST_F(gbXMLFixture, ReverseTranslator_IDs_Names) {
  // Test for #4457 - Support gbXML translation where user-input <Name> is different from the id
  openstudio::path inputPath = resourcesPath() / openstudio::toPath("gbxml/gbXMLStandard_Single_Family_Residential_2016.xml");

  openstudio::gbxml::ReverseTranslator reverseTranslator;
  openstudio::gbxml::ForwardTranslator forwardTranslator;

  boost::optional<openstudio::model::Model> model = reverseTranslator.loadModel(inputPath);
  ASSERT_TRUE(model);

  model->save(resourcesPath() / openstudio::toPath("gbxml/gbXMLStandard_Single_Family_Residential_2016.osm"), true);

  {
    auto _obj = model->getConcreteModelObjectByName<Building>("aim0013");
    ASSERT_TRUE(_obj);
    EXPECT_FALSE(_obj->additionalProperties().hasFeature("CADObjectId"));
    ASSERT_FALSE(_obj->additionalProperties().getFeatureAsString("CADObjectId"));
    ASSERT_FALSE(_obj->cadObjectId());
    EXPECT_TRUE(_obj->additionalProperties().hasFeature("displayName"));
    ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("displayName"));
    ASSERT_TRUE(_obj->displayName());
    EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("displayName").get(), _obj->displayName().get());
    EXPECT_EQ("Sample House", _obj->displayName().get());
    EXPECT_TRUE(_obj->additionalProperties().hasFeature("gbXMLId"));
    ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("gbXMLId"));
    ASSERT_TRUE(_obj->gbXMLId());
    EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("gbXMLId").get(), _obj->gbXMLId().get());
  }

  {
    auto _obj = model->getConcreteModelObjectByName<BuildingStory>("aim0013-Storey-0");
    ASSERT_TRUE(_obj);
    EXPECT_FALSE(_obj->additionalProperties().hasFeature("CADObjectId"));
    ASSERT_FALSE(_obj->additionalProperties().getFeatureAsString("CADObjectId"));
    ASSERT_FALSE(_obj->cadObjectId());
    EXPECT_TRUE(_obj->additionalProperties().hasFeature("displayName"));
    ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("displayName"));
    ASSERT_TRUE(_obj->displayName());
    EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("displayName").get(), _obj->displayName().get());
    EXPECT_EQ("Storey-0-Name", _obj->displayName().get());
    EXPECT_TRUE(_obj->additionalProperties().hasFeature("gbXMLId"));
    ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("gbXMLId"));
    ASSERT_TRUE(_obj->gbXMLId());
    EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("gbXMLId").get(), _obj->gbXMLId().get());
  }

  {
    auto _obj = model->getConcreteModelObjectByName<Space>("aim0014");
    ASSERT_TRUE(_obj);
    EXPECT_TRUE(_obj->additionalProperties().hasFeature("CADObjectId"));
    ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("CADObjectId"));
    ASSERT_TRUE(_obj->cadObjectId());
    EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("CADObjectId").get(), _obj->cadObjectId().get());
    EXPECT_TRUE(_obj->additionalProperties().hasFeature("displayName"));
    ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("displayName"));
    ASSERT_TRUE(_obj->displayName());
    EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("displayName").get(), _obj->displayName().get());
    EXPECT_EQ("Hall_105", _obj->displayName().get());
    EXPECT_TRUE(_obj->additionalProperties().hasFeature("gbXMLId"));
    ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("gbXMLId"));
    ASSERT_TRUE(_obj->gbXMLId());
    EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("gbXMLId").get(), _obj->gbXMLId().get());
  }

  {
    auto _obj = model->getConcreteModelObjectByName<Surface>("aim0065");
    ASSERT_TRUE(_obj);
    EXPECT_TRUE(_obj->additionalProperties().hasFeature("CADObjectId"));
    ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("CADObjectId"));
    ASSERT_TRUE(_obj->cadObjectId());
    EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("CADObjectId").get(), _obj->cadObjectId().get());
    EXPECT_FALSE(_obj->additionalProperties().hasFeature("displayName"));
    ASSERT_FALSE(_obj->additionalProperties().getFeatureAsString("displayName"));
    ASSERT_FALSE(_obj->displayName());
    EXPECT_TRUE(_obj->additionalProperties().hasFeature("gbXMLId"));
    ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("gbXMLId"));
    ASSERT_TRUE(_obj->gbXMLId());
    EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("gbXMLId").get(), _obj->gbXMLId().get());
  }

  {
    auto _obj = model->getConcreteModelObjectByName<SubSurface>("aim2707");
    ASSERT_TRUE(_obj);
    EXPECT_FALSE(_obj->additionalProperties().hasFeature("CADObjectId"));
    ASSERT_FALSE(_obj->additionalProperties().getFeatureAsString("CADObjectId"));
    ASSERT_FALSE(_obj->cadObjectId());
    EXPECT_TRUE(_obj->additionalProperties().hasFeature("displayName"));
    ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("displayName"));
    ASSERT_TRUE(_obj->displayName());
    EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("displayName").get(), _obj->displayName().get());
    EXPECT_EQ("aim2707_NW_FxWin_aim0014", _obj->displayName().get());
    EXPECT_TRUE(_obj->additionalProperties().hasFeature("gbXMLId"));
    ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("gbXMLId"));
    ASSERT_TRUE(_obj->gbXMLId());
    EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("gbXMLId").get(), _obj->gbXMLId().get());
  }

  {
    auto _obj = model->getConcreteModelObjectByName<ThermalZone>("aim9374");
    ASSERT_TRUE(_obj);
    EXPECT_TRUE(_obj->additionalProperties().hasFeature("CADObjectId"));
    ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("CADObjectId"));
    ASSERT_TRUE(_obj->cadObjectId());
    EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("CADObjectId").get(), _obj->cadObjectId().get());
    EXPECT_TRUE(_obj->additionalProperties().hasFeature("displayName"));
    ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("displayName"));
    ASSERT_TRUE(_obj->displayName());
    EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("displayName").get(), _obj->displayName().get());
    EXPECT_EQ("Zone Default", _obj->displayName().get());
    EXPECT_TRUE(_obj->additionalProperties().hasFeature("gbXMLId"));
    ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("gbXMLId"));
    ASSERT_TRUE(_obj->gbXMLId());
    EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("gbXMLId").get(), _obj->gbXMLId().get());
  }

  {
    auto _obj = model->getConcreteModelObjectByName<Construction>("construction-51");
    ASSERT_TRUE(_obj);
    EXPECT_FALSE(_obj->additionalProperties().hasFeature("CADObjectId"));
    ASSERT_FALSE(_obj->additionalProperties().getFeatureAsString("CADObjectId"));
    ASSERT_FALSE(_obj->cadObjectId());
    EXPECT_TRUE(_obj->additionalProperties().hasFeature("displayName"));
    ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("displayName"));
    ASSERT_TRUE(_obj->displayName());
    EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("displayName").get(), _obj->displayName().get());
    EXPECT_EQ("R20 over Roof Deck - Cool Roof", _obj->displayName().get());
    EXPECT_TRUE(_obj->additionalProperties().hasFeature("gbXMLId"));
    ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("gbXMLId"));
    ASSERT_TRUE(_obj->gbXMLId());
    EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("gbXMLId").get(), _obj->gbXMLId().get());
  }

  {
    auto _obj = model->getModelObjectByName<Material>("mat-247");
    ASSERT_TRUE(_obj);
    EXPECT_FALSE(_obj->additionalProperties().hasFeature("CADObjectId"));
    ASSERT_FALSE(_obj->additionalProperties().getFeatureAsString("CADObjectId"));
    ASSERT_FALSE(_obj->cadObjectId());
    EXPECT_TRUE(_obj->additionalProperties().hasFeature("displayName"));
    ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("displayName"));
    ASSERT_TRUE(_obj->displayName());
    EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("displayName").get(), _obj->displayName().get());
    EXPECT_EQ("Blt-Up Roof 3/8in  (BR01)", _obj->displayName().get());
    EXPECT_TRUE(_obj->additionalProperties().hasFeature("gbXMLId"));
    ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("gbXMLId"));
    ASSERT_TRUE(_obj->gbXMLId());
    EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("gbXMLId").get(), _obj->gbXMLId().get());
  }

  openstudio::path outputPath = resourcesPath() / openstudio::toPath("gbxml/gbXMLStandard_Single_Family_Residential_2016_2.xml");
  bool test = forwardTranslator.modelToGbXML(*model, outputPath);
  EXPECT_TRUE(test);
}

TEST_F(gbXMLFixture, ReverseTranslator_Absorptance) {
  // Test for #4570 - Enhance gbXML reverse translation to bring over more data from gbXML

  {
    openstudio::path inputPath = resourcesPath() / openstudio::toPath("gbxml/seb.xml");

    openstudio::gbxml::ReverseTranslator reverseTranslator;

    boost::optional<openstudio::model::Model> model = reverseTranslator.loadModel(inputPath);
    ASSERT_TRUE(model);

    auto _material = model->getConcreteModelObjectByName<StandardOpaqueMaterial>("Stuco");
    ASSERT_TRUE(_material);
    EXPECT_EQ(0.5, _material->thermalAbsorptance());
    EXPECT_EQ(0.5, _material->solarAbsorptance());
    EXPECT_EQ(0.5, _material->visibleAbsorptance());

    auto _material2 = model->getConcreteModelObjectByName<StandardOpaqueMaterial>("Expanded_Polystyrene_-_Extruded_-_1_in.");
    ASSERT_TRUE(_material2);
    EXPECT_EQ(0.9, _material2->thermalAbsorptance());
    EXPECT_EQ(0.7, _material2->solarAbsorptance());
    EXPECT_EQ(0.7, _material2->visibleAbsorptance());
  }

  {
    openstudio::path inputPath = resourcesPath() / openstudio::toPath("gbxml/gbXMLStandard_Single_Family_Residential_2016.xml");

    openstudio::gbxml::ReverseTranslator reverseTranslator;

    boost::optional<openstudio::model::Model> model = reverseTranslator.loadModel(inputPath);
    ASSERT_TRUE(model);

    auto _material = model->getConcreteModelObjectByName<StandardOpaqueMaterial>("mat-247");  // outside layer
    ASSERT_TRUE(_material);
    EXPECT_EQ(0.3, _material->thermalAbsorptance());  // from the Construction
    EXPECT_EQ(0.7, _material->solarAbsorptance());    // default
    EXPECT_EQ(0.7, _material->visibleAbsorptance());  // default

    auto _material2 = model->getConcreteModelObjectByName<StandardOpaqueMaterial>("mat-416");  // not outside layer
    ASSERT_TRUE(_material2);
    EXPECT_EQ(0.9, _material2->thermalAbsorptance());  // default
    EXPECT_EQ(0.7, _material2->solarAbsorptance());    // default
    EXPECT_EQ(0.7, _material2->visibleAbsorptance());  // default
  }
}

TEST_P(RoundTripGbXMLParametrizedFixture, RoundTripped_v703_GbXMLs_AreStillValid) {
  const openstudio::path xmlPath = resourcesPath() / openstudio::toPath("gbxml") / GetParam();

  openstudio::gbxml::ReverseTranslator reverseTranslator;
  openstudio::gbxml::ForwardTranslator forwardTranslator;

  boost::optional<openstudio::model::Model> model = reverseTranslator.loadModel(xmlPath);
  ASSERT_TRUE(model);

  const openstudio::path outputPath = xmlPath.parent_path() / toPath(xmlPath.stem().string() + "_RoundTripped" + xmlPath.extension().string());
  ASSERT_TRUE(forwardTranslator.modelToGbXML(*model, outputPath));

  auto xmlValidator = XMLValidator::gbxmlValidator();

  EXPECT_TRUE(xmlValidator.validate(outputPath));
  EXPECT_TRUE(xmlValidator.isValid());
  EXPECT_EQ(0, xmlValidator.warnings().size());

  auto errors = xmlValidator.errors();
  EXPECT_EQ(0, errors.size());
}

INSTANTIATE_TEST_SUITE_P(gbXMLFixture, RoundTripGbXMLParametrizedFixture,
                         ::testing::Values(  //
                           "11_Jay_St.xml",  //
                           "A00.xml"         //
                           // "Building_Central_Conceptual_Model.xml" // TODO: disabled because it throws in Debug
                           ),
                         [](const testing::TestParamInfo<RoundTripGbXMLParametrizedFixture::ParamType>& info) {
                           return info.param.stem().generic_string();
                         });
