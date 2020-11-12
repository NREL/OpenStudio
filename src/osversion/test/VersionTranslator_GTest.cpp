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
#include "OSVersionFixture.hpp"
#include "../VersionTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/Component.hpp"
#include "../../model/Component_Impl.hpp"
#include "../../model/ComponentData.hpp"
#include "../../model/Construction.hpp"
#include "../../model/Construction_Impl.hpp"
#include "../../model/Material.hpp"
#include "../../model/Material_Impl.hpp"
#include "../../model/ConstructionBase.hpp"
#include "../../model/ConstructionBase_Impl.hpp"
#include "../../model/Building.hpp"
#include "../../model/Building_Impl.hpp"
#include "../../model/Version.hpp"
#include "../../model/Version_Impl.hpp"

#include "../../utilities/core/StringHelpers.hpp"

#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/OS_Version_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/core/Compare.hpp"

#include <resources.hxx>
#include <OpenStudio.hxx>

using namespace openstudio;
using namespace model;
using namespace osversion;

void testExampleModel(int minor, int major) {
  osversion::VersionTranslator translator;

  // iterate through osversion subfolders
  openstudio::path resources = resourcesPath() / toPath("osversion");
  for (openstudio::filesystem::directory_iterator it(resources); it != openstudio::filesystem::directory_iterator(); ++it) {
    if (openstudio::filesystem::is_directory(it->status())) {

      const auto stem = openstudio::replace(openstudio::toString(it->path().stem()), "_", ".");
      VersionString vs(stem);
      if (vs.major() == major && vs.minor() == minor) {

        // run version translator on each example.osm
        openstudio::path modelPath = it->path() / toPath("example.osm");
        model::OptionalModel result = translator.loadModel(modelPath);
        EXPECT_TRUE(result);
        // check warnings, errors, affected objects
        EXPECT_TRUE(translator.errors().empty());
        EXPECT_TRUE(translator.warnings().empty());
        // print info for visual inspection
        ASSERT_TRUE(result);
        LOG_FREE(Debug, "OSVersionFixture", "Updated '" << toString(modelPath) << "' to OpenStudio Version " << result->version().str() << ".");
        LOG_FREE(Debug, "OSVersionFixture", "Deprecated objects: ");
        for (const IdfObject& object : translator.deprecatedObjects()) {
          LOG_FREE(Debug, "OSVersionFixture", object);
        }
        LOG_FREE(Debug, "OSVersionFixture", "Untranslated objects: ");
        for (const IdfObject& object : translator.untranslatedObjects()) {
          LOG_FREE(Debug, "OSVersionFixture", object);
        }
        LOG_FREE(Debug, "OSVersionFixture", "New objects: ");
        for (const IdfObject& object : translator.newObjects()) {
          LOG_FREE(Debug, "OSVersionFixture", object);
        }
        LOG_FREE(Debug, "OSVersionFixture", "Refactored objects: ");
        for (const RefactoredObjectData& p : translator.refactoredObjects()) {
          LOG_FREE(Debug, "OSVersionFixture", p.oldObject() << "replaced with" << std::endl << std::endl << p.newObject());
        }
        // make sure save and load is ok
        modelPath = it->path() / toPath("example_updated.osm");
        result->save(modelPath, true);
        result = model::Model::load(modelPath);
        EXPECT_TRUE(result);
      }
    }
  }
}
/*
TEST_F(OSVersionFixture, VersionTranslator_ExampleModel_0_7) {
  testExampleModel(0, 7);
}
TEST_F(OSVersionFixture, VersionTranslator_ExampleModel_0_8) {
  testExampleModel(0, 8);
}
TEST_F(OSVersionFixture, VersionTranslator_ExampleModel_0_9) {
  testExampleModel(0, 9);
}
TEST_F(OSVersionFixture, VersionTranslator_ExampleModel_0_10) {
  testExampleModel(0, 10);
}
TEST_F(OSVersionFixture, VersionTranslator_ExampleModel_0_11) {
  testExampleModel(0, 11);
}
TEST_F(OSVersionFixture, VersionTranslator_ExampleModel_1_0) {
  testExampleModel(1, 0);
}
TEST_F(OSVersionFixture, VersionTranslator_ExampleModel_1_1) {
  testExampleModel(1, 1);
}
TEST_F(OSVersionFixture, VersionTranslator_ExampleModel_1_2) {
  testExampleModel(1, 2);
}
TEST_F(OSVersionFixture, VersionTranslator_ExampleModel_1_3) {
  testExampleModel(1, 3);
}
TEST_F(OSVersionFixture, VersionTranslator_ExampleModel_1_4) {
  testExampleModel(1, 4);
}
TEST_F(OSVersionFixture, VersionTranslator_ExampleModel_1_5) {
  testExampleModel(1, 5);
}
*/
void testExampleComponent(int major, int minor) {
  osversion::VersionTranslator translator;

  // iterate through osversion subfolders
  openstudio::path resources = resourcesPath() / toPath("osversion");
  for (openstudio::filesystem::directory_iterator it(resources); it != openstudio::filesystem::directory_iterator(); ++it) {
    if (openstudio::filesystem::is_directory(it->status())) {
      const auto stem = openstudio::replace(openstudio::toString(it->path().stem()), "_", ".");
      VersionString vs(stem);
      if (vs.major() == major && vs.minor() == minor) {

        // run version translator on each example.osm
        openstudio::path componentPath = it->path() / toPath("example.osc");
        EXPECT_TRUE(exists(componentPath));
        model::OptionalComponent result = translator.loadComponent(componentPath);
        EXPECT_TRUE(result);
        // check warnings, errors, affected objects
        EXPECT_TRUE(translator.errors().empty());
        EXPECT_TRUE(translator.warnings().empty());
        // print info for visual inspection
        ASSERT_TRUE(result);
        LOG_FREE(Debug, "OSVersionFixture", "Updated '" << toString(componentPath) << "' to OpenStudio Version " << result->version().str() << ".");
        LOG_FREE(Debug, "OSVersionFixture", "Deprecated objects: ");
        for (const IdfObject& object : translator.deprecatedObjects()) {
          LOG_FREE(Debug, "OSVersionFixture", object);
        }
        LOG_FREE(Debug, "OSVersionFixture", "Untranslated objects: ");
        for (const IdfObject& object : translator.untranslatedObjects()) {
          LOG_FREE(Debug, "OSVersionFixture", object);
        }
        LOG_FREE(Debug, "OSVersionFixture", "New objects: ");
        for (const IdfObject& object : translator.newObjects()) {
          LOG_FREE(Debug, "OSVersionFixture", object);
        }
        LOG_FREE(Debug, "OSVersionFixture", "Refactored objects: ");
        for (const RefactoredObjectData& p : translator.refactoredObjects()) {
          LOG_FREE(Debug, "OSVersionFixture", p.oldObject() << "replaced with" << std::endl << std::endl << p.newObject());
        }
        // make sure component came out ok
        ASSERT_TRUE(result);
        model::Component translated = result.get();
        model::ComponentData contents = translated.componentData();
        ASSERT_NO_THROW(contents.primaryComponentObject());
        model::ModelObject prime = contents.primaryComponentObject();
        ASSERT_TRUE(prime.optionalCast<model::Construction>());
        model::Construction construction = prime.cast<model::Construction>();
        EXPECT_FALSE(construction.layers().empty());
        // make sure save and load is ok
        componentPath = it->path() / toPath("example_updated.osc");
        result->save(componentPath, true);
        result = model::Component::load(componentPath);
        EXPECT_TRUE(result);
      }
    }
  }
}
/*
TEST_F(OSVersionFixture, VersionTranslator_ExampleComponent_0_7) {
  testExampleComponent(0, 7);
}
TEST_F(OSVersionFixture, VersionTranslator_ExampleComponent_0_8) {
  testExampleComponent(0, 8);
}
TEST_F(OSVersionFixture, VersionTranslator_ExampleComponent_0_9) {
  testExampleComponent(0, 9);
}
TEST_F(OSVersionFixture, VersionTranslator_ExampleComponent_0_10) {
  testExampleComponent(0, 10);
}
TEST_F(OSVersionFixture, VersionTranslator_ExampleComponent_0_11) {
  testExampleComponent(0, 11);
}
TEST_F(OSVersionFixture, VersionTranslator_ExampleComponent_1_0) {
  testExampleComponent(1, 0);
}
TEST_F(OSVersionFixture, VersionTranslator_ExampleComponent_1_1) {
  testExampleComponent(1, 1);
}
TEST_F(OSVersionFixture, VersionTranslator_ExampleComponent_1_2) {
  testExampleComponent(1, 2);
}
TEST_F(OSVersionFixture, VersionTranslator_ExampleComponent_1_3) {
  testExampleComponent(1, 3);
}
TEST_F(OSVersionFixture, VersionTranslator_ExampleComponent_1_4) {
  testExampleComponent(1, 4);
}
TEST_F(OSVersionFixture, VersionTranslator_ExampleComponent_1_5) {
  testExampleComponent(1, 5);
}
*/
TEST_F(OSVersionFixture, VersionTranslator_FutureVersion_ExampleModel) {
  osversion::VersionTranslator translator;

  model::Model model = model::exampleModel();

  // it is not generally a good idea to mess with the version like this
  boost::optional<WorkspaceObject> object = model.versionObject();
  ASSERT_TRUE(object);
  boost::optional<model::Version> version = object->optionalCast<model::Version>();
  ASSERT_TRUE(version);
  VersionString vs(version->versionIdentifier());

  int major = vs.major();
  int minor = vs.minor();
  boost::optional<int> patch = vs.patch();
  ASSERT_TRUE(patch);

  std::stringstream ss;

  // version translate current model
  ss.str("");
  ss << model;
  boost::optional<model::Model> m2 = translator.loadModel(ss);
  EXPECT_TRUE(m2);

  // increment patch
  VersionString nextPatch(major, minor, *patch + 1);
  EXPECT_TRUE(vs.isNextVersion(nextPatch));
  EXPECT_TRUE(version->setString(OS_VersionFields::VersionIdentifier, nextPatch.str()));
  ss.str("");
  ss << model;
  m2 = translator.loadModel(ss);
  EXPECT_TRUE(m2);

  // increment minor
  VersionString nextMinor(major, minor + 1, 0);
  EXPECT_TRUE(vs.isNextVersion(nextMinor));
  EXPECT_TRUE(version->setString(OS_VersionFields::VersionIdentifier, nextMinor.str()));
  ss.str("");
  ss << model;
  m2 = translator.loadModel(ss);
  EXPECT_TRUE(m2);

  // increment major
  VersionString nextMajor(major + 1, 0, 0);
  EXPECT_TRUE(version->setString(OS_VersionFields::VersionIdentifier, nextMajor.str()));
  ss.str("");
  ss << model;
  m2 = translator.loadModel(ss);
  EXPECT_TRUE(m2);

  // too far ahead
  VersionString aStepTooFar(major, minor + 1, 1);
  EXPECT_TRUE(version->setString(OS_VersionFields::VersionIdentifier, aStepTooFar.str()));
  ss.str("");
  ss << model;
  m2 = translator.loadModel(ss);
  EXPECT_FALSE(m2);

  aStepTooFar = VersionString(major + 1, 1, 0);
  EXPECT_TRUE(version->setString(OS_VersionFields::VersionIdentifier, aStepTooFar.str()));
  ss.str("");
  ss << model;
  m2 = translator.loadModel(ss);
  EXPECT_FALSE(m2);

  aStepTooFar = VersionString(major + 1, 4);
  EXPECT_TRUE(version->setString(OS_VersionFields::VersionIdentifier, aStepTooFar.str()));
  ss.str("");
  ss << model;
  m2 = translator.loadModel(ss);
  EXPECT_FALSE(m2);
}

TEST_F(OSVersionFixture, VersionTranslator_FutureVersion_ExampleModel2) {
  osversion::VersionTranslator translator;
  translator.setAllowNewerVersions(false);

  model::Model model = model::exampleModel();

  // it is not generally a good idea to mess with the version like this
  boost::optional<WorkspaceObject> object = model.versionObject();
  ASSERT_TRUE(object);
  boost::optional<model::Version> version = object->optionalCast<model::Version>();
  ASSERT_TRUE(version);
  VersionString vs(version->versionIdentifier());

  int major = vs.major();
  int minor = vs.minor();
  boost::optional<int> patch = vs.patch();
  ASSERT_TRUE(patch);

  std::stringstream ss;

  // version translate current model
  ss.str("");
  ss << model;
  boost::optional<model::Model> m2 = translator.loadModel(ss);
  EXPECT_TRUE(m2);

  // increment patch
  VersionString nextPatch(major, minor, *patch + 1);
  EXPECT_TRUE(vs.isNextVersion(nextPatch));
  EXPECT_TRUE(version->setString(OS_VersionFields::VersionIdentifier, nextPatch.str()));
  ss.str("");
  ss << model;
  m2 = translator.loadModel(ss);
  EXPECT_FALSE(m2);

  // increment minor
  VersionString nextMinor(major, minor + 1, 0);
  EXPECT_TRUE(vs.isNextVersion(nextMinor));
  EXPECT_TRUE(version->setString(OS_VersionFields::VersionIdentifier, nextMinor.str()));
  ss.str("");
  ss << model;
  m2 = translator.loadModel(ss);
  EXPECT_FALSE(m2);

  // increment major
  VersionString nextMajor(major + 1, 0, 0);
  EXPECT_TRUE(version->setString(OS_VersionFields::VersionIdentifier, nextMajor.str()));
  ss.str("");
  ss << model;
  m2 = translator.loadModel(ss);
  EXPECT_FALSE(m2);

  // too far ahead
  VersionString aStepTooFar(major, minor + 1, 1);
  EXPECT_TRUE(version->setString(OS_VersionFields::VersionIdentifier, aStepTooFar.str()));
  ss.str("");
  ss << model;
  m2 = translator.loadModel(ss);
  EXPECT_FALSE(m2);

  aStepTooFar = VersionString(major + 1, 1, 0);
  EXPECT_TRUE(version->setString(OS_VersionFields::VersionIdentifier, aStepTooFar.str()));
  ss.str("");
  ss << model;
  m2 = translator.loadModel(ss);
  EXPECT_FALSE(m2);

  aStepTooFar = VersionString(major + 1, 4);
  EXPECT_TRUE(version->setString(OS_VersionFields::VersionIdentifier, aStepTooFar.str()));
  ss.str("");
  ss << model;
  m2 = translator.loadModel(ss);
  EXPECT_FALSE(m2);
}
/*
TEST_F(OSVersionFixture,VersionTranslator_0_7_4_NameRefsTranslated) {
  // Translator adds handle fields, but leaves initial name references as-is.
  //
  // This should be okay, because WorkspaceObject code looks to match name references
  // on import.
  //
  // The only exception to this is \object-list AllObjects fields. For 0.7.4, only
  // OS:ComponentData is affected, and it is explicitly translated field by field.

  // Start with 0_7_3 example.osm
  openstudio::path modelPath = resourcesPath() / toPath("osversion/0_7_3/example.osm");

  // Translate to current version.
  osversion::VersionTranslator translator;
  model::OptionalModel result = translator.loadModel(modelPath);
  ASSERT_TRUE(result);
  model::Model model = *result;
  EXPECT_EQ(VersionString("0.7.3"),translator.originalVersion());

  // Confirm that expected pointers are still there
  model::ConstructionVector constructions = model.getModelObjects<model::Construction>();
  EXPECT_FALSE(constructions.empty());
  for (const model::Construction construction : constructions) {
    ASSERT_FALSE(construction.layers().empty());
    model::Material material = construction.layers()[0];

    // Removed due to removal of attributes
    // ASSERT_FALSE(material.attributeNames().empty());
  }
}

TEST_F(OSVersionFixture,Profile_ModelLoading_LatestVersion) {
  VersionString thisVersion(openStudioVersion());
  openstudio::path modelPath = exampleModelPath(thisVersion);

  model::OptionalModel oModel = model::Model::load(modelPath);

  osversion::VersionTranslator translator;
  oModel = translator.loadModel(modelPath);
}

TEST_F(OSVersionFixture,Profile_ModelLoading_OldestVersion) {
  VersionString oldestVersion("0.7.0");
  openstudio::path modelPath = exampleModelPath(oldestVersion);

  OptionalIddFile oIddFile = IddFile::load(iddPath(oldestVersion));
  ASSERT_TRUE(oIddFile);
  OptionalIdfFile oIdfFile = IdfFile::load(modelPath,*oIddFile);
  ASSERT_TRUE(oIdfFile);

  osversion::VersionTranslator translator;
  model::OptionalModel oModel = translator.loadModel(modelPath);
  ASSERT_TRUE(oModel);
}

TEST_F(OSVersionFixture,ModelLoading_PreserveHandles) {
  VersionString firstVersionWithHandlesEmbedded("0.7.4");
  openstudio::path modelPath = exampleModelPath(firstVersionWithHandlesEmbedded);

  OptionalIddFile oIddFile = IddFile::load(iddPath(firstVersionWithHandlesEmbedded));
  ASSERT_TRUE(oIddFile);
  OptionalIdfFile oIdfFile = IdfFile::load(modelPath,*oIddFile);
  ASSERT_TRUE(oIdfFile);
  Handle buildingHandle1 = oIdfFile->getObjectsByType(oIddFile->getObject("OS:Building").get())[0].handle();

  osversion::VersionTranslator translator;
  model::OptionalModel oModel = translator.loadModel(modelPath);
  ASSERT_TRUE(oModel);
  Handle buildingHandle2 = oModel->getUniqueModelObject<model::Building>().handle();
  EXPECT_TRUE(buildingHandle1 == buildingHandle2);
}

TEST_F(OSVersionFixture,Profile_ComponentLoading_LatestVersion) {
  VersionString thisVersion(openStudioVersion());
  openstudio::path componentPath = exampleComponentPath(thisVersion);

  model::OptionalComponent oComponent = model::Component::load(componentPath);

  osversion::VersionTranslator translator;
  oComponent = translator.loadComponent(componentPath);
}

TEST_F(OSVersionFixture,Profile_ComponentLoading_OldestVersion) {
  VersionString oldestVersion("0.7.0");
  openstudio::path componentPath = exampleComponentPath(oldestVersion);

  OptionalIddFile oIddFile = IddFile::load(iddPath(oldestVersion));
  ASSERT_TRUE(oIddFile);
  OptionalIdfFile oIdfFile = IdfFile::load(componentPath,*oIddFile);

  osversion::VersionTranslator translator;
  model::OptionalComponent oComponent = translator.loadComponent(componentPath);
}

TEST_F(OSVersionFixture,PrimaryObject) {

  openstudio::path path = resourcesPath() / toPath("osversion/unknown.osc");

  ASSERT_TRUE(openstudio::filesystem::exists(path));

  osversion::VersionTranslator translator;
  boost::optional<model::Component> component = translator.loadComponent(path);

  ASSERT_TRUE(component);

  model::Model m;

  ASSERT_FALSE(component->primaryObject().handle().isNull());

  model::ModelObject mo = component->primaryObject().clone(m);

  ASSERT_FALSE(mo.handle().isNull());

  boost::optional<model::ConstructionBase> cb = mo.optionalCast<model::ConstructionBase>();

  EXPECT_TRUE(cb);
}

TEST_F(OSVersionFixture,KeepHandles) {

  openstudio::path iddpath = resourcesPath() / toPath("osversion/0_7_4/OpenStudio.idd");
  openstudio::path path = resourcesPath() / toPath("osversion/0_7_4/example.osm");

  // open as idf
  boost::optional<IddFile> oldIdd = IddFile::load(iddpath);
  ASSERT_TRUE(oldIdd);
  boost::optional<IdfFile> oldIdf = IdfFile::load(path);
  ASSERT_TRUE(oldIdf);

  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;;

  // spot check a few model objects
  std::vector<IdfObject> idfObjects = oldIdf->getObjectsByName("OS:RunPeriod 1");
  ASSERT_EQ(1u, idfObjects.size());
  std::vector<WorkspaceObject> workspaceObjects = model->getObjectsByName("OS:RunPeriod 1");
  ASSERT_EQ(1u, workspaceObjects.size());
  EXPECT_TRUE(idfObjects[0].handle() == workspaceObjects[0].handle());

  idfObjects = oldIdf->getObjectsByName("OS:Space 1");
  ASSERT_EQ(1u, idfObjects.size());
  workspaceObjects = model->getObjectsByName("OS:Space 1");
  ASSERT_EQ(1u, workspaceObjects.size());
  EXPECT_TRUE(idfObjects[0].handle() == workspaceObjects[0].handle());

  idfObjects = oldIdf->getObjectsByName("OS:Surface 1");
  ASSERT_EQ(1u, idfObjects.size());
  workspaceObjects = model->getObjectsByName("OS:Surface 1");
  ASSERT_EQ(1u, workspaceObjects.size());
  EXPECT_TRUE(idfObjects[0].handle() == workspaceObjects[0].handle());
}
*/

TEST_F(OSVersionFixture, update_2_9_1_to_3_0_0_fuelTypeRenames) {

  openstudio::path path = resourcesPath() / toPath("osversion/3_0_0/test_vt_fuel.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;
  ;
  openstudio::path outPath = resourcesPath() / toPath("osversion/3_0_0/test_vt_fuel_updated.osm");
  model->save(outPath, true);

  IddFile oldIddFile = getOpenStudioIddFileForVersion(VersionString(2, 9, 1));
  OptionalIdfFile _oldIdfFile = IdfFile::load(path, oldIddFile);
  ASSERT_TRUE(_oldIdfFile);

  // Making the map case-insentive by providing a Comparator `IstringCompare`
  const std::map<std::string, std::string, openstudio::IstringCompare> replaceFuelTypesMap({
    {"FuelOil#1", "FuelOilNo1"},
    {"FuelOil#2", "FuelOilNo2"},
    {"Gas", "NaturalGas"},
    {"PropaneGas", "Propane"},
  });

  const std::multimap<std::string, int> fuelTypeRenamesMap({
    {"OS:OtherEquipment", 6},                           // Fuel Type
    {"OS:Exterior:FuelEquipment", 4},                   // Fuel Use Type
    {"OS:AirConditioner:VariableRefrigerantFlow", 67},  // Fuel Type
    {"OS:Boiler:Steam", 2},                             // Fuel Type
    {"OS:Coil:Cooling:DX:MultiSpeed", 16},              // Fuel Type
    {"OS:Coil:Heating:Gas", 11},                        // Fuel Type
    {"OS:Coil:Heating:DX:MultiSpeed", 16},              // Fuel Type
    {"OS:WaterHeater:Mixed", 11},                       // Heater Fuel Type
    {"OS:WaterHeater:Mixed", 15},                       // Off Cycle Parasitic Fuel Type
    {"OS:WaterHeater:Mixed", 18},                       // On Cycle Parasitic Fuel Type
    {"OS:WaterHeater:Stratified", 17},                  // Heater Fuel Type
    {"OS:WaterHeater:Stratified", 20},                  // Off Cycle Parasitic Fuel Type
    {"OS:WaterHeater:Stratified", 24},                  // On Cycle Parasitic Fuel Type
    {"OS:Generator:MicroTurbine", 13},                  // Fuel Type
    // {"OS:LifeCycleCost:UsePriceEscalation", 2},  // Resource - UNUSED!
    {"OS:Meter:Custom", 2},                                  // Fuel Type
    {"OS:Meter:CustomDecrement", 2},                         // Fuel Type
    {"OS:EnergyManagementSystem:MeteredOutputVariable", 5},  // Resource Type
    {"OS:Boiler:HotWater", 2},                               // Fuel Type
  });

  for (const auto& mapEntry : fuelTypeRenamesMap) {
    const std::string iddname = mapEntry.first;
    const int fieldIndex = mapEntry.second;

    std::string old_fuelType = _oldIdfFile->getObjectsByType(oldIddFile.getObject(iddname).get())[0].getString(fieldIndex).get();
    // Check that the test model (in 2.9.1), actually has bad starting fuels
    EXPECT_TRUE(replaceFuelTypesMap.find(old_fuelType) != replaceFuelTypesMap.end());

    int newFieldIndex = fieldIndex;
    if (iddname == "OS:Coil:Cooling:DX:MultiSpeed") {
      // Fuel Type, was 16 on 3.0.0, 17 on 3.0.1
      newFieldIndex = 17;
    }
    std::string new_fuelType = model->getObjectsByType(iddname)[0].getString(newFieldIndex).get();
    EXPECT_NE(old_fuelType, new_fuelType);
    EXPECT_EQ(replaceFuelTypesMap.at(old_fuelType), new_fuelType) << "Failed for " << iddname;
  }
}

TEST_F(OSVersionFixture, update_2_9_1_to_3_0_0_ShadowCaculation_default) {

  openstudio::path path = resourcesPath() / toPath("osversion/3_0_0/test_vt_ShadowCalculation_default.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;
  ;
  openstudio::path outPath = resourcesPath() / toPath("osversion/3_0_0/test_vt_ShadowCalculation_default_updated.osm");
  model->save(outPath, true);

  /*
 *  ShadowCalculation sc = model.getUniqueModelObject<ShadowCalculation>();
 *
 *  EXPECT_TRUE(sc.isShadingCalculationUpdateFrequencyMethodDefaulted());
 *  EXPECT_EQ("Periodic", sc.shadingCalculationUpdateFrequencyMethod());
 */

  ASSERT_EQ(1u, model->getObjectsByType("OS:ShadowCalculation").size());
  WorkspaceObject sc = model->getObjectsByType("OS:ShadowCalculation")[0];

  // 2.9.1
  //OS:ShadowCalculation,
  //  {0f93d9e1-bdda-4e2a-829b-e4fff92527d0}, !- Handle
  //  20,                                     !- Calculation Frequency
  //  15000;                                  !- Maximum Figures in Shadow Overlap Calculations

  // Shading Calculation Method
  EXPECT_EQ("PolygonClipping", sc.getString(1, false, true).get());
  // Shading Calculation Update Frequency Method
  EXPECT_FALSE(sc.getString(2, false, true));
  // Shading Calculation Update Frequency
  EXPECT_EQ(20, sc.getInt(3, false).get());
  // Maximum Figures in Shadow Overlap Calculations
  EXPECT_EQ(15000, sc.getInt(4, false).get());
  // Polygon Clipping Algorithm
  EXPECT_FALSE(sc.getString(5, false, true));
  // Pixel Counting Resolution
  EXPECT_EQ(512, sc.getInt(6, false).get());
  // Sky Diffuse Modeling Algorithm
  EXPECT_FALSE(sc.getString(7, false, true));
  // Output External Shading Calculation Results
  EXPECT_EQ("No", sc.getString(8, false, true).get());
  // Disable Self-Shading Within Shading Zone Groups
  EXPECT_EQ("No", sc.getString(9, false, true).get());
  // Disable Self-Shading From Shading Zone Groups to Other Zones
  EXPECT_EQ("No", sc.getString(10, false, true).get());
  EXPECT_EQ(0u, sc.numExtensibleGroups());
}

TEST_F(OSVersionFixture, update_2_9_1_to_3_0_0_ShadowCaculation_default_expanded) {

  openstudio::path path = resourcesPath() / toPath("osversion/3_0_0/test_vt_ShadowCalculation_default_expanded.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;
  ;
  openstudio::path outPath = resourcesPath() / toPath("osversion/3_0_0/test_vt_ShadowCalculation_default_expanded_updated.osm");
  model->save(outPath, true);

  ASSERT_EQ(1u, model->getObjectsByType("OS:ShadowCalculation").size());
  WorkspaceObject sc = model->getObjectsByType("OS:ShadowCalculation")[0];

  // 2.9.1
  //OS:ShadowCalculation,
  //  {0f93d9e1-bdda-4e2a-829b-e4fff92527d0}, !- Handle
  //  20,                                     !- Calculation Frequency
  //  15000,                                  !- Maximum Figures in Shadow Overlap Calculations
  //  ,                                       !- Polygon Clipping Algorithm
  //  ,                                       !- Sky Diffuse Modeling Algorithm
  //  ;                                       !- Calculation Method

  // Shading Calculation Method
  EXPECT_EQ("PolygonClipping", sc.getString(1, false, true).get());
  // Shading Calculation Update Frequency Method
  EXPECT_FALSE(sc.getString(2, false, true));
  // Shading Calculation Update Frequency
  EXPECT_EQ(20, sc.getInt(3, false).get());
  // Maximum Figures in Shadow Overlap Calculations
  EXPECT_EQ(15000, sc.getInt(4, false).get());
  // Polygon Clipping Algorithm
  EXPECT_FALSE(sc.getString(5, false, true));
  // Pixel Counting Resolution
  EXPECT_EQ(512, sc.getInt(6, false).get());
  // Sky Diffuse Modeling Algorithm
  EXPECT_FALSE(sc.getString(7, false, true));
  // Output External Shading Calculation Results
  EXPECT_EQ("No", sc.getString(8, false, true).get());
  // Disable Self-Shading Within Shading Zone Groups
  EXPECT_EQ("No", sc.getString(9, false, true).get());
  // Disable Self-Shading From Shading Zone Groups to Other Zones
  EXPECT_EQ("No", sc.getString(10, false, true).get());
  EXPECT_EQ(0u, sc.numExtensibleGroups());
}

TEST_F(OSVersionFixture, update_2_9_1_to_3_0_0_ShadowCaculation_nondefault) {

  openstudio::path path = resourcesPath() / toPath("osversion/3_0_0/test_vt_ShadowCalculation_nondefault.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;
  ;
  openstudio::path outPath = resourcesPath() / toPath("osversion/3_0_0/test_vt_ShadowCalculation_nondefault_updated.osm");
  model->save(outPath, true);

  ASSERT_EQ(1u, model->getObjectsByType("OS:ShadowCalculation").size());
  WorkspaceObject sc = model->getObjectsByType("OS:ShadowCalculation")[0];

  // 2.9.1
  //OS:ShadowCalculation,
  //  {0f93d9e1-bdda-4e2a-829b-e4fff92527d0}, !- Handle
  //  19,                                     !- Calculation Frequency
  //  14999,                                  !- Maximum Figures in Shadow Overlap Calculations
  //  ConvexWeilerAtherton,                   !- Polygon Clipping Algorithm
  //  DetailedSkyDiffuseModeling,             !- Sky Diffuse Modeling Algorithm
  //  AverageOverDaysInFrequency;             !- Calculation Method

  // Shading Calculation Method
  EXPECT_EQ("PolygonClipping", sc.getString(1, false, true).get());
  // Shading Calculation Update Frequency Method
  // AverageOverDaysInFrequency maps to Periodic now
  EXPECT_EQ("Periodic", sc.getString(2, false, true).get());
  // Shading Calculation Update Frequency
  EXPECT_EQ(19, sc.getInt(3, false).get());
  // Maximum Figures in Shadow Overlap Calculations
  EXPECT_EQ(14999, sc.getInt(4, false).get());
  // Polygon Clipping Algorithm
  EXPECT_EQ("ConvexWeilerAtherton", sc.getString(5, false, true).get());
  // Pixel Counting Resolution
  EXPECT_EQ(512, sc.getInt(6, false).get());
  // Sky Diffuse Modeling Algorithm
  EXPECT_EQ("DetailedSkyDiffuseModeling", sc.getString(7, false, true).get());
  // Output External Shading Calculation Results
  EXPECT_EQ("No", sc.getString(8, false, true).get());
  // Disable Self-Shading Within Shading Zone Groups
  EXPECT_EQ("No", sc.getString(9, false, true).get());
  // Disable Self-Shading From Shading Zone Groups to Other Zones
  EXPECT_EQ("No", sc.getString(10, false, true).get());
  EXPECT_EQ(0u, sc.numExtensibleGroups());
}

TEST_F(OSVersionFixture, update_3_0_0_to_3_0_1_CoilCoolingDXSingleSpeed_minOATCompressor) {

  openstudio::path path = resourcesPath() / toPath("osversion/3_0_1/test_vt_CoilCoolingDXSingleSpeed.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;
  ;
  openstudio::path outPath = resourcesPath() / toPath("osversion/3_0_1/test_vt_CoilCoolingDXSingleSpeed_updated.osm");
  model->save(outPath, true);

  ASSERT_EQ(1u, model->getObjectsByType("OS:Coil:Cooling:DX:SingleSpeed").size());
  WorkspaceObject c = model->getObjectsByType("OS:Coil:Cooling:DX:SingleSpeed")[0];

  // Field before insertion point is a curve, should still be
  ASSERT_TRUE(c.getTarget(14));
  EXPECT_EQ("CC DX SingleSpeed PartLoadFrac Correlation Curve", c.getTarget(14)->nameString());

  // Insertion point is at index 15, and is set to -25 (same as model Ctor and E+ IDD default)
  ASSERT_TRUE(c.getDouble(15));
  EXPECT_EQ(-25.0, c.getDouble(15).get());

  // After should be 1000.0
  ASSERT_TRUE(c.getDouble(16));
  EXPECT_EQ(1000.0, c.getDouble(16).get());

  // Last field
  ASSERT_TRUE(c.getTarget(31));
  EXPECT_EQ("Always Off Discrete", c.getTarget(31)->nameString());
}

TEST_F(OSVersionFixture, update_3_0_0_to_3_0_1_CoilCoolingDXTwoStageWithHumidityControlMode_minOATCompressor) {

  openstudio::path path = resourcesPath() / toPath("osversion/3_0_1/test_vt_CoilCoolingDXTwoStageWithHumidityControlMode.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;
  openstudio::path outPath = resourcesPath() / toPath("osversion/3_0_1/test_vt_CoilCoolingDXTwoStageWithHumidityControlMode_updated.osm");
  model->save(outPath, true);

  ASSERT_EQ(1u, model->getObjectsByType("OS:Coil:Cooling:DX:TwoStageWithHumidityControlMode").size());
  WorkspaceObject c = model->getObjectsByType("OS:Coil:Cooling:DX:TwoStageWithHumidityControlMode")[0];

  // Field before insertion point is unused (storage tank)
  EXPECT_FALSE(c.getString(14, false, true));

  // Insertion point is at index 15, and is set to -25 (same as model Ctor and E+ IDD default)
  ASSERT_TRUE(c.getDouble(15));
  EXPECT_EQ(-25.0, c.getDouble(15).get());

  // After should be 100.0
  ASSERT_TRUE(c.getDouble(16));
  EXPECT_EQ(100.0, c.getDouble(16).get());

  // Last field
  ASSERT_TRUE(c.getDouble(17));
  EXPECT_EQ(3.0, c.getDouble(17).get());
}

TEST_F(OSVersionFixture, update_3_0_0_to_3_0_1_CoilCoolingDXMultiSpeed_minOATCompressor) {

  openstudio::path path = resourcesPath() / toPath("osversion/3_0_1/test_vt_CoilCoolingDXMultiSpeed.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;
  ;
  openstudio::path outPath = resourcesPath() / toPath("osversion/3_0_1/test_vt_CoilCoolingDXMultiSpeed_updated.osm");
  model->save(outPath, true);

  ASSERT_EQ(1u, model->getObjectsByType("OS:Coil:Cooling:DX:MultiSpeed").size());
  WorkspaceObject c = model->getObjectsByType("OS:Coil:Cooling:DX:MultiSpeed")[0];

  // Field before insertion point
  ASSERT_TRUE(c.getString(6, false, true));
  EXPECT_EQ("EvaporativelyCooled", c.getString(6, false, true).get());

  // Insertion point is at index 7, and is set to -25 (same as model Ctor and E+ IDD default)
  ASSERT_TRUE(c.getDouble(7));
  EXPECT_EQ(-25.0, c.getDouble(7).get());

  // After is unused (storage tank)
  EXPECT_FALSE(c.getString(8, false, true));

  // Last field
  ASSERT_TRUE(c.getString(17, false, true));
  EXPECT_EQ("Electricity", c.getString(17, false, true).get());
}

TEST_F(OSVersionFixture, update_3_0_0_to_3_0_1_CoilCoolingDXVariableSpeed_minOATCompressor) {

  openstudio::path path = resourcesPath() / toPath("osversion/3_0_1/test_vt_CoilCoolingDXVariableSpeed.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;
  ;
  openstudio::path outPath = resourcesPath() / toPath("osversion/3_0_1/test_vt_CoilCoolingDXVariableSpeed_updated.osm");
  model->save(outPath, true);

  ASSERT_EQ(1u, model->getObjectsByType("OS:Coil:Cooling:DX:VariableSpeed").size());
  WorkspaceObject c = model->getObjectsByType("OS:Coil:Cooling:DX:VariableSpeed")[0];

  // Field before insertion point
  ASSERT_TRUE(c.getDouble(14));
  EXPECT_EQ(11.0, c.getDouble(14).get());

  // Insertion point is at index 15, and is set to -25 (same as model Ctor and E+ IDD default)
  ASSERT_TRUE(c.getDouble(15));
  EXPECT_EQ(-25.0, c.getDouble(15).get());

  // After is unused (storage tank)
  EXPECT_FALSE(c.getString(16, false, true));

  // Last field is the SpeedDataList
  ASSERT_TRUE(c.getTarget(21));
  EXPECT_EQ("Coil Cooling DX Variable Speed 1 Speed Data List", c.getTarget(21)->nameString());
}

TEST_F(OSVersionFixture, update_3_0_0_to_3_0_1_CoilCoolingDXTwoSpeed_minOATCompressor) {

  openstudio::path path = resourcesPath() / toPath("osversion/3_0_1/test_vt_CoilCoolingDXTwoSpeed.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;
  ;
  openstudio::path outPath = resourcesPath() / toPath("osversion/3_0_1/test_vt_CoilCoolingDXTwoSpeed_updated.osm");
  model->save(outPath, true);

  ASSERT_EQ(1u, model->getObjectsByType("OS:Coil:Cooling:DX:TwoSpeed").size());
  WorkspaceObject c = model->getObjectsByType("OS:Coil:Cooling:DX:TwoSpeed")[0];

  // First insertion
  // Field before insertion point
  ASSERT_TRUE(c.getDouble(6));
  EXPECT_EQ(1.2, c.getDouble(6).get());

  // Insertion point is at index 7, and is set to 773.3 (same as model Ctor and E+ source code default)
  ASSERT_TRUE(c.getDouble(7));
  EXPECT_EQ(773.3, c.getDouble(7).get());

  // After is the inlet node, via a PortList
  ASSERT_TRUE(c.getTarget(8));
  EXPECT_EQ("Coil Inlet Node Name", c.getTarget(8)->getTarget(2)->nameString());

  // Second insertion
  // Field before insertion point
  ASSERT_TRUE(c.getString(22, false, true));
  EXPECT_EQ("EvaporativelyCooled", c.getString(22, false, true).get());

  // Insertion point is at index 23, and is set to -25 (same as model Ctor and E+ IDD default)
  ASSERT_TRUE(c.getDouble(23));
  EXPECT_EQ(-25.0, c.getDouble(23).get());

  // After
  ASSERT_TRUE(c.getDouble(24));
  EXPECT_EQ(0.5, c.getDouble(24).get());

  // Last field is a schedule
  ASSERT_TRUE(c.getTarget(34));
  EXPECT_EQ("Basin Heater Operating Schedule Name", c.getTarget(34)->nameString());
}

TEST_F(OSVersionFixture, update_3_0_1_to_3_1_0_AvailabilityManagerHybridVentilation) {

  openstudio::path path = resourcesPath() / toPath("osversion/3_1_0/test_vt_AvailabilityManagerHybridVentilation.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;
  ;
  openstudio::path outPath = resourcesPath() / toPath("osversion/3_1_0/test_vt_AvailabilityManagerHybridVentilation_updated.osm");
  model->save(outPath, true);

  ASSERT_EQ(1u, model->getObjectsByType("OS:AvailabilityManager:HybridVentilation").size());
  WorkspaceObject avm = model->getObjectsByType("OS:AvailabilityManager:HybridVentilation")[0];

  // Maximum Outdoor Dewpoint
  ASSERT_TRUE(avm.getDouble(11));
  EXPECT_EQ(30.0, avm.getDouble(11).get());

  // Minimum Outdoor Ventilation Air Schedule
  ASSERT_TRUE(avm.getTarget(12));
  EXPECT_EQ("Min OA Schedule", avm.getTarget(12)->nameString());

  // Opening Factor Function of Wind Speed Curve
  ASSERT_TRUE(avm.getTarget(13));
  EXPECT_EQ("Opening Factor Function of Wind Speed Curve", avm.getTarget(13)->nameString());

  // The following three were existing in the IDD but not implemented, and they are optional
  // AirflowNetwork Control Type Schedule
  EXPECT_FALSE(avm.getTarget(14));

  // Simple Airflow Control Type Schedule
  EXPECT_FALSE(avm.getTarget(15));

  // ZoneVentilation Object
  EXPECT_FALSE(avm.getTarget(16));

  // Following two fields added, required-fields, set to the IDD default in Ctor
  // Minimum HVAC Operation Time
  ASSERT_TRUE(avm.getDouble(17));
  EXPECT_EQ(0.0, avm.getDouble(17).get());

  // Minimum Ventilation Time
  ASSERT_TRUE(avm.getDouble(18));
  EXPECT_EQ(0.0, avm.getDouble(18).get());
}

TEST_F(OSVersionFixture, update_3_0_1_to_3_1_0_AirLoopHVAC) {
  openstudio::path path = resourcesPath() / toPath("osversion/3_1_0/test_vt_AirLoopHVAC.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;
  ;
  openstudio::path outPath = resourcesPath() / toPath("osversion/3_1_0/test_vt_AirLoopHVAC_updated.osm");
  model->save(outPath, true);

  ASSERT_EQ(1u, model->getObjectsByType("OS:AirLoopHVAC").size());
  WorkspaceObject a = model->getObjectsByType("OS:AirLoopHVAC")[0];

  // Before insertion point
  ASSERT_TRUE(a.getString(5, false, true));
  EXPECT_EQ("autosize", a.getString(5, false, true).get());

  // Insertion point
  ASSERT_TRUE(a.getDouble(6));
  EXPECT_EQ(1.0, a.getDouble(6).get());

  // Brana. List
  EXPECT_FALSE(a.getString(7, false, true));
  // Connector List
  EXPECT_FALSE(a.getString(8, false, true));

  // Supply Side Inlet Node Name, via a PortList
  ASSERT_TRUE(a.getTarget(9));
  EXPECT_EQ("Supply Inlet Node", a.getTarget(9)->getTarget(4)->nameString());
}

TEST_F(OSVersionFixture, update_3_0_1_to_3_1_0_fuelTypesRenames) {
  openstudio::path path = resourcesPath() / toPath("osversion/3_1_0/test_vt_fuelTypeRenames.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;
  ;
  openstudio::path outPath = resourcesPath() / toPath("osversion/3_1_0/test_vt_fuelTypeRenames_updated.osm");
  model->save(outPath, true);

  std::vector<WorkspaceObject> outputMeters = model->getObjectsByType("OS:Output:Meter");
  ASSERT_EQ(3u, outputMeters.size());
  EXPECT_NE(std::find_if(outputMeters.begin(), outputMeters.end(),
                         [](const WorkspaceObject& wo) { return openstudio::istringEqual(wo.nameString(), "NaturalGas:Facility"); }),
            outputMeters.end());
  EXPECT_NE(std::find_if(outputMeters.begin(), outputMeters.end(),
                         [](const WorkspaceObject& wo) { return openstudio::istringEqual(wo.nameString(), "Heating:FuelOilNo1"); }),
            outputMeters.end());
  EXPECT_NE(std::find_if(outputMeters.begin(), outputMeters.end(),
                         [](const WorkspaceObject& wo) { return openstudio::istringEqual(wo.nameString(), "WaterSystems:Propane"); }),
            outputMeters.end());

  // In the preparation of the test model, I assigned the same value to the Output:Variable 'name' (unused in E+) and the 'Variable Name' field we
  // care about. That allows us to easily have on the object the initial and the final values.
  // {Old, New}
  const std::map<std::string, std::string> renameMap({
    {"Boiler FuelOil#1 Energy", "Boiler FuelOilNo1 Energy"},
    {"Boiler Gas Rate", "Boiler NaturalGas Rate"},
    {"Boiler Electric Power", "Boiler Electricity Rate"},
    {"Cooling Coil Water Heating Electric Power", "Cooling Coil Water Heating Electricity Rate"},
    {"Generator Requested Electric Power", "Generator Requested Electricity Rate"},
    {"Air System DX Cooling Coil Electric Energy", "Air System DX Cooling Coil Electricity Energy"},
    // Introduce extra spaces on purpose...
    {"Water  Heater  Gas Rate", "Water Heater NaturalGas Rate"},
  });

  std::vector<WorkspaceObject> outputVariables = model->getObjectsByType("OS:Output:Variable");
  ASSERT_EQ(renameMap.size(), outputVariables.size());
  ASSERT_EQ(7u, outputVariables.size());

  for (const auto& outputVariable : outputVariables) {
    auto name = outputVariable.nameString();
    auto it = renameMap.find(name);
    ASSERT_NE(it, renameMap.end()) << "Output:Variable named " << name << " not in replaceMap";
    EXPECT_EQ(it->second, outputVariable.getString(3).get())
      << "Output:Variable named " << name << " did not get the expected rename for Variable Name field";
  }

  std::vector<WorkspaceObject> emsSensors = model->getObjectsByType("OS:EnergyManagementSystem:Sensor");
  ASSERT_EQ(11u, emsSensors.size());

  EXPECT_NE(
    std::find_if(emsSensors.begin(), emsSensors.end(),
                 [](const WorkspaceObject& wo) { return openstudio::istringEqual(wo.nameString(), "Facility_Total_HVAC_Electric_Demand_Power"); }),
    emsSensors.end());

  for (const auto& emsSensor : emsSensors) {
    if (openstudio::istringEqual(emsSensor.nameString(), "Facility_Total_HVAC_Electric_Demand_Power")) {
      // Facility Total HVAC Electric Demand Power => Facility Total HVAC Electricity Demand Rate
      EXPECT_EQ("Facility Total HVAC Electricity Demand Rate", emsSensor.getString(3).get());
    } else {
      // All of these have actual handles stored at string, these shouldn't have been touched
      boost::optional<std::string> handle = emsSensor.getString(3);
      ASSERT_TRUE(handle.is_initialized());
      UUID uid = toUUID(handle.get());
      boost::optional<WorkspaceObject> object = model->getObject(uid);
      ASSERT_TRUE(object);
      EXPECT_TRUE(openstudio::istringEqual(object->iddObject().name(), "OS:Output:Variable")
                  || openstudio::istringEqual(object->iddObject().name(), "OS:Output:Meter"));
    }
  }
}

TEST_F(OSVersionFixture, update_3_0_1_to_3_1_0_fuelTypesRenames_MeterCustoms) {
  openstudio::path path = resourcesPath() / toPath("osversion/3_1_0/test_vt_fuelTypeRenames_MeterCustoms.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;
  ;
  openstudio::path outPath = resourcesPath() / toPath("osversion/3_1_0/test_vt_fuelTypeRenames_MeterCustoms_updated.osm");
  model->save(outPath, true);

  std::vector<WorkspaceObject> meterCustoms = model->getObjectsByType("OS:Meter:Custom");
  ASSERT_EQ(1u, meterCustoms.size());
  std::vector<WorkspaceObject> meterCustomDecrements = model->getObjectsByType("OS:Meter:CustomDecrement");
  ASSERT_EQ(1u, meterCustomDecrements.size());

  for (const auto& wo : {meterCustoms[0], meterCustomDecrements[0]}) {

    for (const IdfExtensibleGroup& eg : wo.extensibleGroups()) {
      const auto varName = eg.getString(1).get();
      // Facility Total HVAC Electric Demand Power => Facility Total HVAC Electricity Demand Rate
      // Generator Blower Electric Power => Generator Blower Electricity Rate
      EXPECT_TRUE(openstudio::istringEqual(varName, "Facility Total HVAC Electricity Demand Rate")
                  || openstudio::istringEqual(varName, "Generator Blower Electricity Rate"))
        << "Failed for " << wo.nameString() << ", found '" << varName << "'.";
    }
  }
}

TEST_F(OSVersionFixture, update_3_0_1_to_3_1_0_ConstructionWithInternalSource) {
  openstudio::path path = resourcesPath() / toPath("osversion/3_1_0/test_vt_ConstructionWithInternalSource.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;
  ;
  openstudio::path outPath = resourcesPath() / toPath("osversion/3_1_0/test_vt_ConstructionWithInternalSource_updated.osm");
  model->save(outPath, true);

  std::vector<WorkspaceObject> constructions = model->getObjectsByType("OS:Construction:InternalSource");
  ASSERT_EQ(1u, constructions.size());
  WorkspaceObject c = constructions[0];

  // Before insertion point: Tube Spacing
  ASSERT_TRUE(c.getDouble(5, false));
  EXPECT_EQ(0.2, c.getDouble(5).get());

  // Insertion point
  // ASSERT_TRUE(c.getDouble(6));
  // EXPECT_EQ(0.0, c.getDouble(6).get());
  EXPECT_EQ(0.0, c.getDouble(6, true).get());

  // Surface rendering name
  ASSERT_TRUE(c.getTarget(7));
  EXPECT_EQ("RenderingColor for InternalSource", c.getTarget(7).get().nameString());

  EXPECT_EQ(3u, c.extensibleGroups().size());
  for (const IdfExtensibleGroup& eg : c.extensibleGroups()) {
    WorkspaceExtensibleGroup w_eg = eg.cast<WorkspaceExtensibleGroup>();
    ASSERT_TRUE(w_eg.getTarget(0));
    EXPECT_EQ("OS:Material", w_eg.getTarget(0).get().iddObject().name());
  }
}

TEST_F(OSVersionFixture, update_3_0_1_to_3_1_0_ZoneHVACLowTemp) {
  openstudio::path path = resourcesPath() / toPath("osversion/3_1_0/test_vt_ZoneHVACLowTemp.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;
  ;
  openstudio::path outPath = resourcesPath() / toPath("osversion/3_1_0/test_vt_ZoneHVACLowTemp_updated.osm");
  model->save(outPath, true);

  // New fields: have defaults... If we make them required-field, switch these two bools to the opposite
  bool returnDefault = true;
  bool returnUninitializedEmpty = false;

  {
    std::vector<WorkspaceObject> lowtempradiants = model->getObjectsByType("OS:ZoneHVAC:LowTemperatureRadiant:ConstantFlow");
    ASSERT_EQ(1u, lowtempradiants.size());
    WorkspaceObject lowtempradiant = lowtempradiants[0];

    EXPECT_EQ("Floors", lowtempradiant.getString(3, false, true).get());

    // New fields
    EXPECT_EQ("ConvectionOnly", lowtempradiant.getString(4, returnDefault, returnUninitializedEmpty).get());
    EXPECT_EQ(0.016, lowtempradiant.getDouble(6, returnDefault).get());
    EXPECT_EQ(0.35, lowtempradiant.getDouble(8, returnDefault).get());
    EXPECT_EQ(0.8, lowtempradiant.getDouble(10, returnDefault).get());

    EXPECT_EQ(0.154, lowtempradiant.getDouble(5, false).get());
    EXPECT_EQ(200.0, lowtempradiant.getDouble(7, false).get());
    EXPECT_EQ("MeanRadiantTemperature", lowtempradiant.getString(9, false, true).get());

    ASSERT_TRUE(lowtempradiant.getTarget(11));
    EXPECT_EQ("OS:Coil:Heating:LowTemperatureRadiant:ConstantFlow", lowtempradiant.getTarget(11).get().iddObject().name());
    ASSERT_TRUE(lowtempradiant.getTarget(12));
    EXPECT_EQ("OS:Coil:Cooling:LowTemperatureRadiant:ConstantFlow", lowtempradiant.getTarget(12).get().iddObject().name());

    EXPECT_EQ(0.005, lowtempradiant.getDouble(13, false).get());
    ASSERT_TRUE(lowtempradiant.getTarget(14));  // Pump Flow Rate Schedule Name

    EXPECT_EQ(30000.0, lowtempradiant.getDouble(15, false).get());  // head
    EXPECT_EQ(1200.0, lowtempradiant.getDouble(16, false).get());   // Rated power
    EXPECT_EQ(0.9, lowtempradiant.getDouble(17, false).get());      // Motor eff
    EXPECT_EQ(0.7, lowtempradiant.getDouble(18, false).get());      /// Fraction of motor inef

    EXPECT_EQ("CalculateFromCircuitLength", lowtempradiant.getString(19, false, true).get());
    EXPECT_EQ(120.0, lowtempradiant.getDouble(20, false).get());  /// Fraction of motor inef

    // Changeover Delay Time Period Schedule
    EXPECT_FALSE(lowtempradiant.getString(21, false, true));
  }

  {
    std::vector<WorkspaceObject> lowtempradiants = model->getObjectsByType("OS:ZoneHVAC:LowTemperatureRadiant:VariableFlow");
    ASSERT_EQ(1u, lowtempradiants.size());
    WorkspaceObject lowtempradiant = lowtempradiants[0];

    ASSERT_TRUE(lowtempradiant.getTarget(3));
    EXPECT_EQ("OS:Coil:Heating:LowTemperatureRadiant:VariableFlow", lowtempradiant.getTarget(3).get().iddObject().name());
    ASSERT_TRUE(lowtempradiant.getTarget(4));
    EXPECT_EQ("OS:Coil:Cooling:LowTemperatureRadiant:VariableFlow", lowtempradiant.getTarget(4).get().iddObject().name());
    EXPECT_EQ("Floors", lowtempradiant.getString(5, false, true).get());

    // New Fields
    EXPECT_EQ("ConvectionOnly", lowtempradiant.getString(6, returnDefault, returnUninitializedEmpty).get());
    EXPECT_EQ(0.016, lowtempradiant.getDouble(8, returnDefault).get());
    EXPECT_EQ(0.35, lowtempradiant.getDouble(10, returnDefault).get());
    EXPECT_EQ("HalfFlowPower", lowtempradiant.getString(12, returnDefault, returnUninitializedEmpty).get());

    EXPECT_EQ(0.154, lowtempradiant.getDouble(7, false).get());
    EXPECT_EQ(200, lowtempradiant.getDouble(9, false).get());
    EXPECT_EQ("MeanRadiantTemperature", lowtempradiant.getString(11, false, true).get());
    EXPECT_EQ("CalculateFromCircuitLength", lowtempradiant.getString(13, false, true).get());
    EXPECT_EQ(120, lowtempradiant.getDouble(14, false).get());

    // Changeover Delay Time Period Schedule
    EXPECT_FALSE(lowtempradiant.getString(15, false, true));
  }

  {
    std::vector<WorkspaceObject> lowtempradiants = model->getObjectsByType("OS:ZoneHVAC:LowTemperatureRadiant:Electric");
    ASSERT_EQ(1u, lowtempradiants.size());
    WorkspaceObject lowtempradiant = lowtempradiants[0];

    EXPECT_EQ("Floors", lowtempradiant.getString(3, false, true).get());
    EXPECT_EQ(1000, lowtempradiant.getDouble(4, returnDefault).get());
    EXPECT_EQ("MeanRadiantTemperature", lowtempradiant.getString(5, false, true).get());

    // New fields
    EXPECT_EQ("HalfFlowPower", lowtempradiant.getString(6, returnDefault, returnUninitializedEmpty).get());

    EXPECT_EQ(2, lowtempradiant.getDouble(7, returnDefault).get());
    ASSERT_TRUE(lowtempradiant.getTarget(8));  // Heating Setpoint Temperature Schedule Name
  }
}

TEST_F(OSVersionFixture, update_3_0_1_to_3_1_0_WaterHeaterHeatPump) {
  openstudio::path path = resourcesPath() / toPath("osversion/3_1_0/test_vt_WaterHeaterHeatPump.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;
  ;
  openstudio::path outPath = resourcesPath() / toPath("osversion/3_1_0/test_vt_WaterHeaterHeatPump_updated.osm");
  model->save(outPath, true);

  std::vector<WorkspaceObject> whs = model->getObjectsByType("OS:WaterHeater:HeatPump");
  ASSERT_EQ(1u, whs.size());
  WorkspaceObject wh = whs[0];

  // Before insertion point: Minimum Inlet Air Temperature for Compressor Operation
  ASSERT_TRUE(wh.getDouble(15, false));
  EXPECT_EQ(12.5, wh.getDouble(15).get());

  // Insertion point
  ASSERT_TRUE(wh.getDouble(16, false));
  EXPECT_EQ(48.89, wh.getDouble(16).get());

  // Compressor Location
  ASSERT_TRUE(wh.getString(17, false, true));
  EXPECT_EQ("Outdoors", wh.getString(17, false, true).get());

  // Last Field: Control Sensor Location In Stratified Tank
  ASSERT_TRUE(wh.getString(25, false, true));
  EXPECT_EQ("Heater2", wh.getString(25, false, true).get());
}

TEST_F(OSVersionFixture, update_3_0_1_to_3_1_0_ShadingControl_and_SubSurfaces) {
  openstudio::path path = resourcesPath() / toPath("osversion/3_1_0/test_vt_ShadingControl.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;
  ;
  openstudio::path outPath = resourcesPath() / toPath("osversion/3_1_0/test_vt_ShadingControl_updated.osm");
  model->save(outPath, true);

  std::vector<WorkspaceObject> sss = model->getObjectsByType("OS:SubSurface");
  ASSERT_EQ(1u, sss.size());
  WorkspaceObject ss = sss[0];

  EXPECT_EQ(0.5, ss.getDouble(6));
  EXPECT_EQ(4u, ss.numExtensibleGroups());
  auto eg = ss.extensibleGroups()[3];
  EXPECT_EQ(1, eg.getInt(0, false).get());
  EXPECT_EQ(0, eg.getInt(1, false).get());
  EXPECT_EQ(0, eg.getInt(2, false).get());

  std::vector<WorkspaceObject> scs = model->getObjectsByType("OS:ShadingControl");
  ASSERT_EQ(1u, scs.size());
  WorkspaceObject sc = scs[0];

  EXPECT_EQ("Sequential", sc.getString(13, false, true).get());
  ASSERT_EQ(1u, sc.numExtensibleGroups());

  WorkspaceExtensibleGroup w_eg = sc.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
  ASSERT_TRUE(w_eg.getTarget(0));
  EXPECT_EQ("OS:SubSurface", w_eg.getTarget(0).get().iddObject().name());
}
