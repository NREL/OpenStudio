/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../ThreeJSForwardTranslator.hpp"
#include "../ThreeJSReverseTranslator.hpp"
#include "../Model.hpp"
#include "../Space.hpp"
#include "../Space_Impl.hpp"
#include "../Surface.hpp"
#include "../Surface_Impl.hpp"
#include "../SubSurface.hpp"
#include "../SubSurface_Impl.hpp"
#include "../ConstructionAirBoundary.hpp"
#include "../Construction.hpp"
#include "../../osversion/VersionTranslator.hpp"
#include "../../utilities/geometry/ThreeJS.hpp"

#include <algorithm>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ThreeJSForwardTranslator_ParkUnder_Retail_Office_C2) {
  ThreeJSForwardTranslator ft;
  openstudio::path out;

  osversion::VersionTranslator translator;
  openstudio::path modelpath = resourcesPath() / toPath("model/ParkUnder_Retail_Office_C2.osm");
  model::OptionalModel model = translator.loadModel(modelpath);
  ThreeScene scene = ft.modelToThreeJS(model.get(), true);
  EXPECT_EQ(0, ft.errors().size());
  EXPECT_EQ(0, ft.warnings().size());
  for (const auto& error : ft.errors()) {
    EXPECT_TRUE(false) << "Error:" << error.logMessage();
  }
  for (const auto& warn : ft.warnings()) {
    EXPECT_TRUE(false) << "Warning:" << warn.logMessage();
  }
  std::string json = scene.toJSON();
  EXPECT_TRUE(ThreeScene::load(json));

  out = resourcesPath() / toPath("model/MParkUnder_Retail_Office_C2.json");
  openstudio::filesystem::ofstream file1(out);
  ASSERT_TRUE(file1.is_open());
  file1 << json;
  file1.close();
}

TEST_F(ModelFixture, ThreeJSForwardTranslator_7_7_Windows_Complete) {
  ThreeJSForwardTranslator ft;
  openstudio::path out;

  osversion::VersionTranslator translator;
  openstudio::path modelpath = resourcesPath() / toPath("model/7-7_Windows_Complete.osm");
  model::OptionalModel model = translator.loadModel(modelpath);
  ThreeScene scene = ft.modelToThreeJS(model.get(), true);
  EXPECT_EQ(0, ft.errors().size());
  EXPECT_EQ(0, ft.warnings().size());
  for (const auto& error : ft.errors()) {
    EXPECT_TRUE(false) << "Error:" << error.logMessage();
  }
  for (const auto& warn : ft.warnings()) {
    EXPECT_TRUE(false) << "Warning:" << warn.logMessage();
  }
  std::string json = scene.toJSON(true);
  EXPECT_TRUE(ThreeScene::load(json));

  out = resourcesPath() / toPath("model/M7-7_Windows_Complete.json");
  openstudio::filesystem::ofstream file1(out);
  ASSERT_TRUE(file1.is_open());
  file1 << json;
  file1.close();
}

TEST_F(ModelFixture, ThreeJSForwardTranslator_7_7_Windows_Complete_GeometryDiagnostics) {
  ThreeJSForwardTranslator ft;
  ft.setIncludeGeometryDiagnostics(true);

  openstudio::path out;

  osversion::VersionTranslator translator;
  openstudio::path modelpath = resourcesPath() / toPath("model/7-7_Windows_Complete.osm");
  model::OptionalModel model = translator.loadModel(modelpath);
  ThreeScene scene = ft.modelToThreeJS(model.get(), true);
  // Ensure we get no errors or warnings, generally speaking.
  EXPECT_EQ(0, ft.errors().size());
  auto warnings = ft.warnings();
  warnings.erase(std::remove_if(warnings.begin(), warnings.end(),
                                [](auto& warn) {
                                  return ((warn.logMessage().find("Polyhedron is not enclosed") != std::string::npos)
                                          || (warn.logMessage().find("edges that aren't used exactly twice") != std::string::npos)
                                          || (warn.logMessage().find("Can't compute a floorPrint") != std::string::npos));
                                }),
                 warnings.end());
  EXPECT_EQ(0, warnings.size());

  for (const auto& error : ft.errors()) {
    EXPECT_TRUE(false) << "Error: " << error.logMessage();
  }

  for (const auto& warning : warnings) {
    EXPECT_TRUE(false) << "Warning: " << warning.logMessage();
  }

  std::string json = scene.toJSON(true);
  EXPECT_TRUE(ThreeScene::load(json));

  out = resourcesPath() / toPath("model/M7-7_Windows_Complete_geometryDiags.json");
  openstudio::filesystem::ofstream file1(out);
  ASSERT_TRUE(file1.is_open());
  file1 << json;
  file1.close();
}

TEST_F(ModelFixture, ThreeJSForwardTranslator_RefBldgOutPatientNew2004_Chicago) {
  ThreeJSForwardTranslator ft;
  openstudio::path out;
  osversion::VersionTranslator translator;
  openstudio::path modelPath = resourcesPath() / toPath("model/Sample_DOE-RefBldgOutPatientNew2004_Chicago.osm");
  model::OptionalModel model = translator.loadModel(modelPath);
  ThreeScene scene = ft.modelToThreeJS(model.get(), true);
  EXPECT_EQ(0, ft.errors().size());
  EXPECT_EQ(0, ft.warnings().size());
  for (const auto& error : ft.errors()) {
    EXPECT_TRUE(false) << "Error: " << error.logMessage();
  }
  for (const auto& warning : ft.warnings()) {
    EXPECT_TRUE(false) << "Warning: " << warning.logMessage();
  }
  std::string json = scene.toJSON(true);
  EXPECT_TRUE(ThreeScene::load(json));
  out = resourcesPath() / toPath("model/MSample_DOE-RefBldgOutPatientNew2004_Chicago.json");
  openstudio::filesystem::ofstream file1(out);
  ASSERT_TRUE(file1.is_open());
  file1 << json;
  file1.close();
}

TEST_F(ModelFixture, ThreeJSForwardTranslator_RefBldgOutPatientNew2004_Chicago_GeometryDiagnostics) {
  ThreeJSForwardTranslator ft;
  ft.setIncludeGeometryDiagnostics(true);

  openstudio::path out;
  osversion::VersionTranslator translator;
  openstudio::path modelPath = resourcesPath() / toPath("model/Sample_DOE-RefBldgOutPatientNew2004_Chicago.osm");
  model::OptionalModel model = translator.loadModel(modelPath);
  ThreeScene scene = ft.modelToThreeJS(model.get(), true);
  // Ensure we get no errors or warnings, generally speaking.
  EXPECT_EQ(0, ft.errors().size());
  auto warnings = ft.warnings();
  warnings.erase(std::remove_if(warnings.begin(), warnings.end(),
                                [](auto& warn) {
                                  return ((warn.logMessage().find("Polyhedron is not enclosed") != std::string::npos)
                                          // || (warn.logMessage().find("edges that aren't used exactly twice") != std::string::npos)
                                          // || (warn.logMessage().find("Can't compute a floorPrint") != std::string::npos)
                                  );
                                }),
                 warnings.end());
  EXPECT_EQ(0, warnings.size());

  for (const auto& error : ft.errors()) {
    EXPECT_TRUE(false) << "Error: " << error.logMessage();
  }

  for (const auto& warning : warnings) {
    EXPECT_TRUE(false) << "Warning: " << warning.logMessage();
  }

  std::string json = scene.toJSON(true);
  EXPECT_TRUE(ThreeScene::load(json));
  out = resourcesPath() / toPath("model/MSample_DOE-RefBldgOutPatientNew2004_Chicago_geometryDiags.json");
  openstudio::filesystem::ofstream file1(out);
  ASSERT_TRUE(file1.is_open());
  file1 << json;
  file1.close();
}

TEST_F(ModelFixture, ThreeJSForwardTranslator_RefBldgSecondarySchoolNew2004_Chicago) {
  ThreeJSForwardTranslator ft;
  openstudio::path out;

  osversion::VersionTranslator translator;
  openstudio::path modelPath = resourcesPath() / toPath("model/Sample_DOE-RefBldgSecondarySchoolNew2004_Chicago.osm");
  model::OptionalModel model = translator.loadModel(modelPath);
  // triangulated, for display
  ThreeScene scene = ft.modelToThreeJS(model.get(), true);
  // Ensure we get no errors or warnings, generally speaking.
  EXPECT_EQ(0, ft.errors().size());
  EXPECT_EQ(0, ft.warnings().size());

  for (const auto& error : ft.errors()) {
    EXPECT_TRUE(false) << "Error: " << error.logMessage();
  }

  for (const auto& warning : ft.warnings()) {
    EXPECT_TRUE(false) << "Warning: " << warning.logMessage();
  }

  std::string json = scene.toJSON();
  EXPECT_TRUE(ThreeScene::load(json));

  out = resourcesPath() / toPath("model/MSample_DOE-RefBldgSecondarySchoolNew2004_Chicago.json");
  openstudio::filesystem::ofstream file1(out);
  ASSERT_TRUE(file1.is_open());
  file1 << json;
  file1.close();
}

TEST_F(ModelFixture, ThreeJSForwardTranslator_RefBldgLargeOfficeNew2004_Chicago) {
  ThreeJSForwardTranslator ft;
  openstudio::path out;

  osversion::VersionTranslator translator;
  openstudio::path modelPath = resourcesPath() / toPath("model/RefBldgLargeOfficeNew2004_Chicago.osm");
  model::OptionalModel model = translator.loadModel(modelPath);
  // triangulated, for display
  ThreeScene scene = ft.modelToThreeJS(model.get(), true);
  // Ensure we get no errors or warnings, generally speaking.
  EXPECT_EQ(0, ft.errors().size());
  EXPECT_EQ(0, ft.warnings().size());

  for (const auto& error : ft.errors()) {
    EXPECT_TRUE(false) << "Error: " << error.logMessage();
  }

  for (const auto& warning : ft.warnings()) {
    EXPECT_TRUE(false) << "Warning: " << warning.logMessage();
  }

  std::string json = scene.toJSON();
  EXPECT_TRUE(ThreeScene::load(json));

  out = resourcesPath() / toPath("model/MRefBldgLargeOfficeNew2004_Chicago.json");
  openstudio::filesystem::ofstream file1(out);
  ASSERT_TRUE(file1.is_open());
  file1 << json;
  file1.close();
}

TEST_F(ModelFixture, ThreeJSForwardTranslator_RefBldgFullServiceRestaurantNew2004_Chicago) {
  ThreeJSForwardTranslator ft;
  openstudio::path out;

  osversion::VersionTranslator translator;
  openstudio::path modelPath = resourcesPath() / toPath("model/RefBldgFullServiceRestaurantNew2004_Chicago.osm");
  model::OptionalModel model = translator.loadModel(modelPath);
  // triangulated, for display
  ThreeScene scene = ft.modelToThreeJS(model.get(), true);
  // Ensure we get no errors or warnings, generally speaking.
  EXPECT_EQ(0, ft.errors().size());
  EXPECT_EQ(0, ft.warnings().size());

  for (const auto& error : ft.errors()) {
    EXPECT_TRUE(false) << "Error: " << error.logMessage();
  }

  for (const auto& warning : ft.warnings()) {
    EXPECT_TRUE(false) << "Warning: " << warning.logMessage();
  }

  std::string json = scene.toJSON();
  EXPECT_TRUE(ThreeScene::load(json));

  out = resourcesPath() / toPath("model/MRefBldgFullServiceRestaurantNew2004_Chicago.json");
  openstudio::filesystem::ofstream file1(out);
  ASSERT_TRUE(file1.is_open());
  file1 << json;
  file1.close();
}

TEST_F(ModelFixture, ThreeJSForwardTranslator_ExampleModel) {

  ThreeJSForwardTranslator ft;
  ThreeJSReverseTranslator rt;
  openstudio::path out;

  Model model = exampleModel();
  model.save(resourcesPath() / toPath("model/exampleModel.osm"), true);

  // triangulated, for display
  ThreeScene scene = ft.modelToThreeJS(model, true);
  // Ensure we get no errors or warnings, generally speaking.
  EXPECT_EQ(0, ft.errors().size());
  EXPECT_EQ(0, ft.warnings().size());

  for (const auto& error : ft.errors()) {
    EXPECT_TRUE(false) << "Error: " << error.logMessage();
  }

  for (const auto& warning : ft.warnings()) {
    EXPECT_TRUE(false) << "Warning: " << warning.logMessage();
  }

  std::string json = scene.toJSON();
  EXPECT_TRUE(ThreeScene::load(json));

  out = resourcesPath() / toPath("model/ModelToThreeJSTriangulated.json");
  openstudio::filesystem::ofstream file1(out);
  ASSERT_TRUE(file1.is_open());
  file1 << json;
  file1.close();

  // not triangulated, for model transport/translation
  scene = ft.modelToThreeJS(model, false);

  // Ensure we get no errors or warnings, generally speaking.
  EXPECT_EQ(0, ft.errors().size());
  EXPECT_EQ(0, ft.warnings().size());

  for (const auto& error : ft.errors()) {
    EXPECT_TRUE(false) << "Error: " << error.logMessage();
  }

  for (const auto& warning : ft.warnings()) {
    EXPECT_TRUE(false) << "Warning: " << warning.logMessage();
  }

  json = scene.toJSON();
  EXPECT_TRUE(ThreeScene::load(json));

  out = resourcesPath() / toPath("model/ModelToThreeJSNonTriangulated.json");
  openstudio::filesystem::ofstream file2(out);
  ASSERT_TRUE(file2.is_open());
  file2 << json;
  file2.close();

  boost::optional<Model> model2 = rt.modelFromThreeJS(scene);
  ASSERT_TRUE(model2);

  model2->save(resourcesPath() / toPath("model/ModelToThreeJSToModel.osm"), true);

  EXPECT_EQ(model.getConcreteModelObjects<Space>().size(), model2->getConcreteModelObjects<Space>().size());
  EXPECT_EQ(model.getConcreteModelObjects<Surface>().size(), model2->getConcreteModelObjects<Surface>().size());
  EXPECT_EQ(model.getConcreteModelObjects<SubSurface>().size(), model2->getConcreteModelObjects<SubSurface>().size());
}

TEST_F(ModelFixture, ThreeJSForwardTranslator_ConstructionAirBoundary) {

  ThreeJSForwardTranslator ft;

  Model m;
  ConstructionAirBoundary cAirBoundary(m);
  cAirBoundary.setName("Construction_Air_Boundary");

  Construction c(m);
  c.setName("RegularConstruction");

  ThreeScene scene = ft.modelToThreeJS(m, false);

  // Ensure we get no errors or warnings, generally speaking.
  // Here I'm especially after #3943: "Unknown iddObjectType 'OS:Construction:AirBoundary'"
  EXPECT_EQ(0, ft.errors().size());
  EXPECT_EQ(0, ft.warnings().size());

  for (const auto& error : ft.errors()) {
    EXPECT_TRUE(false) << "Error: " << error.logMessage();
  }

  for (const auto& warning : ft.warnings()) {
    EXPECT_TRUE(false) << "Warning: " << warning.logMessage();
  }

  // Materials are named like "prefix_" + <construction.name>
  auto checkIfMaterialExist = [](const auto& materials, const std::string& containedString) {
    auto it = std::find_if(materials.cbegin(), materials.cend(),
                           [&containedString](const auto& mat) { return mat.name().find(containedString) != std::string::npos; });
    return it != materials.cend();
  };

  auto materials = scene.materials();
  EXPECT_TRUE(checkIfMaterialExist(materials, "RegularConstruction"));
  EXPECT_FALSE(checkIfMaterialExist(materials, "Construction_Air_Boundary"));  // Instead it should have been skipped to be replace by "AirWall"
  EXPECT_TRUE(checkIfMaterialExist(materials, "AirWall"));
}
