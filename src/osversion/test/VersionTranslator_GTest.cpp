/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include <utilities/idd/OS_Fan_SystemModel_FieldEnums.hxx>
#include <utilities/idd/OS_Connection_FieldEnums.hxx>
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
          LOG_FREE(Debug, "OSVersionFixture", p.oldObject() << "replaced with" << '\n' << '\n' << p.newObject());
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
          LOG_FREE(Debug, "OSVersionFixture", p.oldObject() << "replaced with" << '\n' << '\n' << p.newObject());
        }
        // make sure component came out ok
        ASSERT_TRUE(result);
        model::Component translated = result.get();
        model::ComponentData contents = translated.componentData();
        ASSERT_NO_THROW(contents.primaryComponentObject());
        model::ModelObject prime = contents.primaryComponentObject();
        ASSERT_TRUE(prime.optionalCast<model::Construction>());
        auto construction = prime.cast<model::Construction>();
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
  model::ConstructionVector constructions = model.getConcreteModelObjects<model::Construction>();
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

  openstudio::path outPath = resourcesPath() / toPath("osversion/3_0_1/test_vt_CoilCoolingDXSingleSpeed_updated.osm");
  model->save(outPath, true);

  ASSERT_EQ(1u, model->getObjectsByType("OS:Coil:Cooling:DX:SingleSpeed").size());
  WorkspaceObject c = model->getObjectsByType("OS:Coil:Cooling:DX:SingleSpeed")[0];

  // 3.4.0 to 3.5.0: (1) field added

  // Field before insertion point is a curve, should still be
  ASSERT_TRUE(c.getTarget(15));
  EXPECT_EQ("CC DX SingleSpeed PartLoadFrac Correlation Curve", c.getTarget(15)->nameString());

  // Insertion point is at index 15, and is set to -25 (same as model Ctor and E+ IDD default)
  ASSERT_TRUE(c.getDouble(16));
  EXPECT_EQ(-25.0, c.getDouble(16).get());

  // After should be 1000.0
  ASSERT_TRUE(c.getDouble(17));
  EXPECT_EQ(1000.0, c.getDouble(17).get());

  // Last field
  ASSERT_TRUE(c.getTarget(32));
  EXPECT_EQ("Always Off Discrete", c.getTarget(32)->nameString());
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

  // After is the inlet node, via a Connection
  ASSERT_TRUE(c.getTarget(8));
  // We have to resolve to computing or it'll fail in > 3.1.0, since we removed the Name field
  EXPECT_EQ("Coil Inlet Node Name", c.getTarget(8)->getTarget(OS_ConnectionFields::SourceObject)->nameString());

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

  // Branch List
  EXPECT_FALSE(a.getString(7, false, true));
  // Connector List
  EXPECT_FALSE(a.getString(8, false, true));

  // Supply Side Inlet Node Name, via a Connection
  ASSERT_TRUE(a.getTarget(9));
  // We have to resolve to computing or it'll fail in > 3.1.0, since we removed the Name field
  EXPECT_EQ("Supply Inlet Node", a.getTarget(9)->getTarget(OS_ConnectionFields::TargetObject)->nameString());
}

TEST_F(OSVersionFixture, update_3_0_1_to_3_1_0_fuelTypesRenames) {
  openstudio::path path = resourcesPath() / toPath("osversion/3_1_0/test_vt_fuelTypeRenames.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;

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
    auto w_eg = eg.cast<WorkspaceExtensibleGroup>();
    ASSERT_TRUE(w_eg.getTarget(0));
    EXPECT_EQ("OS:Material", w_eg.getTarget(0).get().iddObject().name());
  }
}

TEST_F(OSVersionFixture, update_3_0_1_to_3_1_0_ZoneHVACLowTemp) {
  openstudio::path path = resourcesPath() / toPath("osversion/3_1_0/test_vt_ZoneHVACLowTemp.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;

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

  openstudio::path outPath = resourcesPath() / toPath("osversion/3_1_0/test_vt_ShadingControl_updated.osm");
  model->save(outPath, true);

  std::vector<WorkspaceObject> sss = model->getObjectsByType("OS:SubSurface");
  ASSERT_EQ(1u, sss.size());
  WorkspaceObject ss = sss[0];

  EXPECT_EQ(0.5, ss.getDouble(6).get());
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

  auto w_eg = sc.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
  ASSERT_TRUE(w_eg.getTarget(0));
  EXPECT_EQ("OS:SubSurface", w_eg.getTarget(0).get().iddObject().name());
}

TEST_F(OSVersionFixture, update_3_1_0_to_3_2_0_ZoneHVACLowTempRadiantVarFlow_Coils) {
  openstudio::path path = resourcesPath() / toPath("osversion/3_2_0/test_vt_ZoneHVACLowTempRadiantVarFlow_Coils.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;

  openstudio::path outPath = resourcesPath() / toPath("osversion/3_2_0/test_vt_ZoneHVACLowTempRadiantVarFlow_Coils_updated.osm");
  model->save(outPath, true);

  // New fields: have defaults... If we make them required-field, switch these two bools to the opposite
  bool returnDefault = true;
  bool returnUninitializedEmpty = false;

  {
    std::vector<WorkspaceObject> coils = model->getObjectsByType("OS:Coil:Heating:LowTemperatureRadiant:VariableFlow");
    ASSERT_EQ(1u, coils.size());
    WorkspaceObject coil = coils[0];

    // New fields
    EXPECT_EQ("HeatingDesignCapacity", coil.getString(2, returnDefault, returnUninitializedEmpty).get());
    EXPECT_EQ("Autosize", coil.getString(3, returnDefault, returnUninitializedEmpty).get());
    EXPECT_EQ(0.0, coil.getDouble(4, returnDefault).get());
    EXPECT_EQ(1.0, coil.getDouble(5, returnDefault).get());

    // First field after insertion, Maximum Water Flow
    EXPECT_EQ(0.85, coil.getDouble(6, returnDefault).get());

    // After is the inlet node, via a Connection
    ASSERT_TRUE(coil.getTarget(7));
    // After is the inlet node, via a Connection
    EXPECT_EQ("Heating Coil Inlet Node", coil.getTarget(7)->getTarget(OS_ConnectionFields::SourceObject)->nameString());
    ASSERT_TRUE(coil.getTarget(8));
    EXPECT_EQ("Heating Coil Outlet Node", coil.getTarget(8)->getTarget(OS_ConnectionFields::TargetObject)->nameString());

    // Throttling range
    EXPECT_EQ(0.65, coil.getDouble(9, returnDefault).get());

    // Control Temp schedule
    ASSERT_TRUE(coil.getTarget(10));
    EXPECT_EQ("Heating Control Temperature Schedule", coil.getTarget(10)->nameString());
  }

  {
    std::vector<WorkspaceObject> coils = model->getObjectsByType("OS:Coil:Cooling:LowTemperatureRadiant:VariableFlow");
    ASSERT_EQ(1u, coils.size());
    WorkspaceObject coil = coils[0];

    // New fields
    EXPECT_EQ("CoolingDesignCapacity", coil.getString(2, returnDefault, returnUninitializedEmpty).get());
    EXPECT_EQ("Autosize", coil.getString(3, returnDefault, returnUninitializedEmpty).get());
    EXPECT_EQ(0.0, coil.getDouble(4, returnDefault).get());
    EXPECT_EQ(1.0, coil.getDouble(5, returnDefault).get());

    // First field after insertion, Maximum Water Flow
    EXPECT_EQ(1.05, coil.getDouble(6, returnDefault).get());

    // After is the inlet node, via a Connection
    ASSERT_TRUE(coil.getTarget(7));
    EXPECT_EQ("Cooling Coil Inlet Node", coil.getTarget(7)->getTarget(OS_ConnectionFields::SourceObject)->nameString());
    ASSERT_TRUE(coil.getTarget(8));
    EXPECT_EQ("Cooling Coil Outlet Node", coil.getTarget(8)->getTarget(OS_ConnectionFields::TargetObject)->nameString());

    // Throttling range
    EXPECT_EQ(0.75, coil.getDouble(9, returnDefault).get());

    // Control Temp schedule
    ASSERT_TRUE(coil.getTarget(10));
    EXPECT_EQ("Cooling Control Temperature Schedule", coil.getTarget(10)->nameString());

    EXPECT_EQ("VariableOff", coil.getString(11, returnDefault, returnUninitializedEmpty).get());
    EXPECT_EQ(1.3, coil.getDouble(12, returnDefault).get());
  }
}

TEST_F(OSVersionFixture, update_3_1_0_to_3_2_0_ZoneAirMassFlowConservation) {
  openstudio::path path = resourcesPath() / toPath("osversion/3_2_0/test_vt_ZoneAirMassFlowConservation.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;

  openstudio::path outPath = resourcesPath() / toPath("osversion/3_2_0/test_vt_ZoneAirMassFlowConservation_updated.osm");
  model->save(outPath, true);

  std::vector<WorkspaceObject> zcs = model->getObjectsByType("OS:ZoneAirMassFlowConservation");
  ASSERT_EQ(1u, zcs.size());
  WorkspaceObject zc = zcs[0];
  // Yes becomes AdjustMixingOnly
  EXPECT_EQ("AdjustMixingOnly", zc.getString(1).get());
  // Unchanged
  EXPECT_EQ("AdjustInfiltrationFlow", zc.getString(2).get());
  EXPECT_EQ("AllZones", zc.getString(3).get());
}

TEST_F(OSVersionFixture, update_3_1_0_to_3_2_0_ConstructionAirBoundary) {
  openstudio::path path = resourcesPath() / toPath("osversion/3_2_0/test_vt_ConstructionAirBoundary.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;

  openstudio::path outPath = resourcesPath() / toPath("osversion/3_2_0/test_vt_ConstructionAirBoundary_updated.osm");
  model->save(outPath, true);

  std::vector<WorkspaceObject> cs = model->getObjectsByType("OS:Construction:AirBoundary");
  ASSERT_EQ(1u, cs.size());
  WorkspaceObject c = cs[0];
  EXPECT_EQ("My Construction Air Boundary", c.getString(1).get());
  // SolarAndDaylightingMethod and RadiantExchangeMethod are gone
  EXPECT_EQ("SimpleMixing", c.getString(2).get());
  EXPECT_EQ(0.3, c.getDouble(3).get());
}

TEST_F(OSVersionFixture, update_3_1_0_to_3_2_0_CoilCoolingWaterToAirHeatPumpEquationFit) {
  openstudio::path path = resourcesPath() / toPath("osversion/3_2_0/test_vt_CoilCoolingWaterToAirHeatPumpEquationFit.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;

  openstudio::path outPath = resourcesPath() / toPath("osversion/3_2_0/test_vt_CoilCoolingWaterToAirHeatPumpEquationFit_updated.osm");
  model->save(outPath, true);

  std::vector<WorkspaceObject> coils = model->getObjectsByType("OS:Coil:Cooling:WaterToAirHeatPump:EquationFit");
  ASSERT_EQ(1u, coils.size());
  WorkspaceObject coil = coils[0];

  // Field before: Rated COP
  EXPECT_EQ(4.2, coil.getDouble(10).get());

  // 3.4.0 to 3.5.0: (3) fields added

  // Curves
  {
    ASSERT_TRUE(coil.getTarget(14));
    WorkspaceObject totalCoolingCapacityCurve = coil.getTarget(14).get();
    EXPECT_EQ(coil.nameString() + " TotCoolCapCurve", totalCoolingCapacityCurve.nameString());

    EXPECT_EQ(-0.68126221, totalCoolingCapacityCurve.getDouble(2).get());
    EXPECT_EQ(1.99529297, totalCoolingCapacityCurve.getDouble(3).get());
    EXPECT_EQ(-0.93611888, totalCoolingCapacityCurve.getDouble(4).get());
    EXPECT_EQ(0.02081177, totalCoolingCapacityCurve.getDouble(5).get());
    EXPECT_EQ(0.008438868, totalCoolingCapacityCurve.getDouble(6).get());
    EXPECT_EQ(-100.0, totalCoolingCapacityCurve.getDouble(7).get());
    EXPECT_EQ(100.0, totalCoolingCapacityCurve.getDouble(8).get());
    EXPECT_EQ(-100.0, totalCoolingCapacityCurve.getDouble(9).get());
    EXPECT_EQ(100.0, totalCoolingCapacityCurve.getDouble(10).get());
    EXPECT_EQ(0.0, totalCoolingCapacityCurve.getDouble(11).get());
    EXPECT_EQ(100.0, totalCoolingCapacityCurve.getDouble(12).get());
    EXPECT_EQ(0.0, totalCoolingCapacityCurve.getDouble(13).get());
    EXPECT_EQ(100.0, totalCoolingCapacityCurve.getDouble(14).get());
  }

  {
    ASSERT_TRUE(coil.getTarget(15));
    WorkspaceObject sensibleCoolingCapacityCurve = coil.getTarget(15).get();
    // This is a CurveQuintLinear
    EXPECT_EQ(coil.nameString() + " SensCoolCapCurve", sensibleCoolingCapacityCurve.nameString());
    EXPECT_EQ(2.24209455, sensibleCoolingCapacityCurve.getDouble(2).get());
    EXPECT_EQ(7.28913391, sensibleCoolingCapacityCurve.getDouble(3).get());
    EXPECT_EQ(-9.06079896, sensibleCoolingCapacityCurve.getDouble(4).get());
    EXPECT_EQ(-0.36729404, sensibleCoolingCapacityCurve.getDouble(5).get());
    EXPECT_EQ(0.218826161, sensibleCoolingCapacityCurve.getDouble(6).get());
    EXPECT_EQ(0.00901534, sensibleCoolingCapacityCurve.getDouble(7).get());
    EXPECT_EQ(-100.0, sensibleCoolingCapacityCurve.getDouble(8).get());
    EXPECT_EQ(100.0, sensibleCoolingCapacityCurve.getDouble(9).get());
    EXPECT_EQ(-100.0, sensibleCoolingCapacityCurve.getDouble(10).get());
    EXPECT_EQ(100.0, sensibleCoolingCapacityCurve.getDouble(11).get());
    EXPECT_EQ(-100.0, sensibleCoolingCapacityCurve.getDouble(12).get());
    EXPECT_EQ(100.0, sensibleCoolingCapacityCurve.getDouble(13).get());
    EXPECT_EQ(0.0, sensibleCoolingCapacityCurve.getDouble(14).get());
    EXPECT_EQ(100.0, sensibleCoolingCapacityCurve.getDouble(15).get());
    EXPECT_EQ(0.0, sensibleCoolingCapacityCurve.getDouble(16).get());
    EXPECT_EQ(100.0, sensibleCoolingCapacityCurve.getDouble(17).get());
  }

  {
    ASSERT_TRUE(coil.getTarget(16));
    WorkspaceObject coolingPowerConsumptionCurve = coil.getTarget(16).get();
    EXPECT_EQ(coil.nameString() + " CoolPowCurve", coolingPowerConsumptionCurve.nameString());
    EXPECT_EQ(-3.20456384, coolingPowerConsumptionCurve.getDouble(2).get());
    EXPECT_EQ(0.47656454, coolingPowerConsumptionCurve.getDouble(3).get());
    EXPECT_EQ(3.16734236, coolingPowerConsumptionCurve.getDouble(4).get());
    EXPECT_EQ(0.10244637, coolingPowerConsumptionCurve.getDouble(5).get());
    EXPECT_EQ(-0.038132556, coolingPowerConsumptionCurve.getDouble(6).get());
    EXPECT_EQ(-100.0, coolingPowerConsumptionCurve.getDouble(7).get());
    EXPECT_EQ(100.0, coolingPowerConsumptionCurve.getDouble(8).get());
    EXPECT_EQ(-100.0, coolingPowerConsumptionCurve.getDouble(9).get());
    EXPECT_EQ(100.0, coolingPowerConsumptionCurve.getDouble(10).get());
    EXPECT_EQ(0.0, coolingPowerConsumptionCurve.getDouble(11).get());
    EXPECT_EQ(100.0, coolingPowerConsumptionCurve.getDouble(12).get());
    EXPECT_EQ(0.0, coolingPowerConsumptionCurve.getDouble(13).get());
    EXPECT_EQ(100.0, coolingPowerConsumptionCurve.getDouble(14).get());
  }

  // Field after: Nominal Time for Condensate Removal to Begin
  EXPECT_EQ(360.0, coil.getDouble(17).get());

  // Last field
  EXPECT_EQ(0.1, coil.getDouble(18).get());
}

TEST_F(OSVersionFixture, update_3_1_0_to_3_2_0_CoilHeatingWaterToAirHeatPumpEquationFit) {
  openstudio::path path = resourcesPath() / toPath("osversion/3_2_0/test_vt_CoilHeatingWaterToAirHeatPumpEquationFit.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;

  openstudio::path outPath = resourcesPath() / toPath("osversion/3_2_0/test_vt_CoilHeatingWaterToAirHeatPumpEquationFit_updated.osm");
  model->save(outPath, true);

  std::vector<WorkspaceObject> coils = model->getObjectsByType("OS:Coil:Heating:WaterToAirHeatPump:EquationFit");
  ASSERT_EQ(1u, coils.size());
  WorkspaceObject coil = coils[0];

  // Field before: Rated COP
  EXPECT_EQ(4.5, coil.getDouble(9).get());

  // 3.5.0 to 3.5.0: (3) fields added

  // Curves
  {
    ASSERT_TRUE(coil.getTarget(13));
    WorkspaceObject heatingCapacityCurve = coil.getTarget(13).get();
    EXPECT_EQ(coil.nameString() + " HeatCapCurve", heatingCapacityCurve.nameString());
    EXPECT_EQ(-5.50102734, heatingCapacityCurve.getDouble(2).get());
    EXPECT_EQ(-0.96688754, heatingCapacityCurve.getDouble(3).get());
    EXPECT_EQ(7.70755007, heatingCapacityCurve.getDouble(4).get());
    EXPECT_EQ(0.031928881, heatingCapacityCurve.getDouble(5).get());
    EXPECT_EQ(0.028112522, heatingCapacityCurve.getDouble(6).get());
    EXPECT_EQ(-100.0, heatingCapacityCurve.getDouble(7).get());
    EXPECT_EQ(100.0, heatingCapacityCurve.getDouble(8).get());
    EXPECT_EQ(-100.0, heatingCapacityCurve.getDouble(9).get());
    EXPECT_EQ(100.0, heatingCapacityCurve.getDouble(10).get());
    EXPECT_EQ(0.0, heatingCapacityCurve.getDouble(11).get());
    EXPECT_EQ(100.0, heatingCapacityCurve.getDouble(12).get());
    EXPECT_EQ(0.0, heatingCapacityCurve.getDouble(13).get());
    EXPECT_EQ(100.0, heatingCapacityCurve.getDouble(14).get());
  }

  {
    ASSERT_TRUE(coil.getTarget(14));
    WorkspaceObject heatingPowerConsumptionCurve = coil.getTarget(14).get();
    EXPECT_EQ(coil.nameString() + " HeatPowCurve", heatingPowerConsumptionCurve.nameString());
    EXPECT_EQ(-7.47517858, heatingPowerConsumptionCurve.getDouble(2).get());
    EXPECT_EQ(6.40876653, heatingPowerConsumptionCurve.getDouble(3).get());
    EXPECT_EQ(1.99711665, heatingPowerConsumptionCurve.getDouble(4).get());
    EXPECT_EQ(-0.050682973, heatingPowerConsumptionCurve.getDouble(5).get());
    EXPECT_EQ(0.011385145, heatingPowerConsumptionCurve.getDouble(6).get());
    EXPECT_EQ(-100.0, heatingPowerConsumptionCurve.getDouble(7).get());
    EXPECT_EQ(100.0, heatingPowerConsumptionCurve.getDouble(8).get());
    EXPECT_EQ(-100.0, heatingPowerConsumptionCurve.getDouble(9).get());
    EXPECT_EQ(100.0, heatingPowerConsumptionCurve.getDouble(10).get());
    EXPECT_EQ(0.0, heatingPowerConsumptionCurve.getDouble(11).get());
    EXPECT_EQ(100.0, heatingPowerConsumptionCurve.getDouble(12).get());
    EXPECT_EQ(0.0, heatingPowerConsumptionCurve.getDouble(13).get());
    EXPECT_EQ(100.0, heatingPowerConsumptionCurve.getDouble(14).get());
  }
}

TEST_F(OSVersionFixture, update_3_1_0_to_3_2_0_HeatPumpWaterToWaterEquationFitCooling) {
  openstudio::path path = resourcesPath() / toPath("osversion/3_2_0/test_vt_HeatPumpWaterToWaterEquationFitCooling.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;

  openstudio::path outPath = resourcesPath() / toPath("osversion/3_2_0/test_vt_HeatPumpWaterToWaterEquationFitCooling_updated.osm");
  model->save(outPath, true);

  std::vector<WorkspaceObject> hps = model->getObjectsByType("OS:HeatPump:WaterToWater:EquationFit:Cooling");
  ASSERT_EQ(1u, hps.size());
  WorkspaceObject hp = hps[0];

  // Field before
  EXPECT_EQ("Autosize", hp.getString(9, false, true).get());

  // Curves
  {
    ASSERT_TRUE(hp.getTarget(10));
    WorkspaceObject coolingCapacityCurve = hp.getTarget(10).get();
    EXPECT_EQ(hp.nameString() + " CoolCapCurve", coolingCapacityCurve.nameString());
    EXPECT_EQ(-1.52030596, coolingCapacityCurve.getDouble(2).get());
    EXPECT_EQ(3.46625667, coolingCapacityCurve.getDouble(3).get());
    EXPECT_EQ(-1.32267797, coolingCapacityCurve.getDouble(4).get());
    EXPECT_EQ(0.09395678, coolingCapacityCurve.getDouble(5).get());
    EXPECT_EQ(0.038975504, coolingCapacityCurve.getDouble(6).get());
    EXPECT_EQ(-100.0, coolingCapacityCurve.getDouble(7).get());
    EXPECT_EQ(100.0, coolingCapacityCurve.getDouble(8).get());
    EXPECT_EQ(-100.0, coolingCapacityCurve.getDouble(9).get());
    EXPECT_EQ(100.0, coolingCapacityCurve.getDouble(10).get());
    EXPECT_EQ(0.0, coolingCapacityCurve.getDouble(11).get());
    EXPECT_EQ(100.0, coolingCapacityCurve.getDouble(12).get());
    EXPECT_EQ(0.0, coolingCapacityCurve.getDouble(13).get());
    EXPECT_EQ(100.0, coolingCapacityCurve.getDouble(14).get());
  }

  {
    ASSERT_TRUE(hp.getTarget(11));
    WorkspaceObject coolingCompressorPowerCurve = hp.getTarget(11).get();
    EXPECT_EQ(hp.nameString() + " CoolCompPowerCurve", coolingCompressorPowerCurve.nameString());
    EXPECT_EQ(-8.59564386, coolingCompressorPowerCurve.getDouble(2).get());
    EXPECT_EQ(0.96265085, coolingCompressorPowerCurve.getDouble(3).get());
    EXPECT_EQ(8.69489229, coolingCompressorPowerCurve.getDouble(4).get());
    EXPECT_EQ(0.02501669, coolingCompressorPowerCurve.getDouble(5).get());
    EXPECT_EQ(-0.20132665, coolingCompressorPowerCurve.getDouble(6).get());
    EXPECT_EQ(-100.0, coolingCompressorPowerCurve.getDouble(7).get());
    EXPECT_EQ(100.0, coolingCompressorPowerCurve.getDouble(8).get());
    EXPECT_EQ(-100.0, coolingCompressorPowerCurve.getDouble(9).get());
    EXPECT_EQ(100.0, coolingCompressorPowerCurve.getDouble(10).get());
    EXPECT_EQ(0.0, coolingCompressorPowerCurve.getDouble(11).get());
    EXPECT_EQ(100.0, coolingCompressorPowerCurve.getDouble(12).get());
    EXPECT_EQ(0.0, coolingCompressorPowerCurve.getDouble(13).get());
    EXPECT_EQ(100.0, coolingCompressorPowerCurve.getDouble(14).get());
  }

  // Field after: Reference Coefficient of Performance
  EXPECT_EQ(8.0, hp.getDouble(12).get());
}

TEST_F(OSVersionFixture, update_3_1_0_to_3_2_0_HeatPumpWaterToWaterEquationFitHeating) {
  openstudio::path path = resourcesPath() / toPath("osversion/3_2_0/test_vt_HeatPumpWaterToWaterEquationFitHeating.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;

  openstudio::path outPath = resourcesPath() / toPath("osversion/3_2_0/test_vt_HeatPumpWaterToWaterEquationFitHeating_updated.osm");
  model->save(outPath, true);

  std::vector<WorkspaceObject> hps = model->getObjectsByType("OS:HeatPump:WaterToWater:EquationFit:Heating");
  ASSERT_EQ(1u, hps.size());
  WorkspaceObject hp = hps[0];

  // Field before
  EXPECT_EQ("Autosize", hp.getString(9, false, true).get());

  // Curves
  {
    ASSERT_TRUE(hp.getTarget(10));
    WorkspaceObject heatingCapacityCurve = hp.getTarget(10).get();
    EXPECT_EQ(hp.nameString() + " HeatCapCurve", heatingCapacityCurve.nameString());
    EXPECT_EQ(-3.33491153, heatingCapacityCurve.getDouble(2).get());
    EXPECT_EQ(-0.51451946, heatingCapacityCurve.getDouble(3).get());
    EXPECT_EQ(4.51592706, heatingCapacityCurve.getDouble(4).get());
    EXPECT_EQ(0.01797107, heatingCapacityCurve.getDouble(5).get());
    EXPECT_EQ(0.155797661, heatingCapacityCurve.getDouble(6).get());
    EXPECT_EQ(-100.0, heatingCapacityCurve.getDouble(7).get());
    EXPECT_EQ(100.0, heatingCapacityCurve.getDouble(8).get());
    EXPECT_EQ(-100.0, heatingCapacityCurve.getDouble(9).get());
    EXPECT_EQ(100.0, heatingCapacityCurve.getDouble(10).get());
    EXPECT_EQ(0.0, heatingCapacityCurve.getDouble(11).get());
    EXPECT_EQ(100.0, heatingCapacityCurve.getDouble(12).get());
    EXPECT_EQ(0.0, heatingCapacityCurve.getDouble(13).get());
    EXPECT_EQ(100.0, heatingCapacityCurve.getDouble(14).get());
  }

  {
    ASSERT_TRUE(hp.getTarget(11));
    WorkspaceObject heatingCompressorPowerCurve = hp.getTarget(11).get();
    EXPECT_EQ(hp.nameString() + " HeatCompPowerCurve", heatingCompressorPowerCurve.nameString());
    EXPECT_EQ(-8.93121751, heatingCompressorPowerCurve.getDouble(2).get());
    EXPECT_EQ(8.57035762, heatingCompressorPowerCurve.getDouble(3).get());
    EXPECT_EQ(1.29660976, heatingCompressorPowerCurve.getDouble(4).get());
    EXPECT_EQ(-0.21629222, heatingCompressorPowerCurve.getDouble(5).get());
    EXPECT_EQ(0.033862378, heatingCompressorPowerCurve.getDouble(6).get());
    EXPECT_EQ(-100.0, heatingCompressorPowerCurve.getDouble(7).get());
    EXPECT_EQ(100.0, heatingCompressorPowerCurve.getDouble(8).get());
    EXPECT_EQ(-100.0, heatingCompressorPowerCurve.getDouble(9).get());
    EXPECT_EQ(100.0, heatingCompressorPowerCurve.getDouble(10).get());
    EXPECT_EQ(0.0, heatingCompressorPowerCurve.getDouble(11).get());
    EXPECT_EQ(100.0, heatingCompressorPowerCurve.getDouble(12).get());
    EXPECT_EQ(0.0, heatingCompressorPowerCurve.getDouble(13).get());
    EXPECT_EQ(100.0, heatingCompressorPowerCurve.getDouble(14).get());
  }

  // Field after: Reference Coefficient of Performance
  EXPECT_EQ(7.5, hp.getDouble(12).get());
}

TEST_F(OSVersionFixture, update_3_1_0_to_3_2_0_ZoneHVACTerminalUnitVariableRefrigerantFlow) {
  openstudio::path path = resourcesPath() / toPath("osversion/3_2_0/test_vt_ZoneHVACTerminalUnitVariableRefrigerantFlow.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;

  openstudio::path outPath = resourcesPath() / toPath("osversion/3_2_0/test_vt_ZoneHVACTerminalUnitVariableRefrigerantFlow_updated.osm");
  model->save(outPath, true);

  std::vector<WorkspaceObject> vrfs = model->getObjectsByType("OS:ZoneHVAC:TerminalUnit:VariableRefrigerantFlow");
  ASSERT_EQ(1u, vrfs.size());
  WorkspaceObject vrf = vrfs[0];

  // Field before
  EXPECT_EQ("Fan Op Schedule", vrf.getString(12, false, true).get());
  EXPECT_EQ("DrawThrough", vrf.getString(13, false, true).get());
  EXPECT_EQ("Supply Air Fan", vrf.getString(14, false, true).get());
}

TEST_F(OSVersionFixture, update_3_2_0_to_3_2_1_WaterHeaterSizing) {
  openstudio::path path = resourcesPath() / toPath("osversion/3_2_1/test_vt_WaterHeaterSizing.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;

  openstudio::path outPath = resourcesPath() / toPath("osversion/3_2_1/test_vt_WaterHeaterSizing_updated.osm");
  model->save(outPath, true);

  std::vector<WorkspaceObject> whMixeds = model->getObjectsByType("OS:WaterHeater:Mixed");
  ASSERT_EQ(1u, whMixeds.size());
  auto whMixed = whMixeds[0];

  std::vector<WorkspaceObject> whStratifieds = model->getObjectsByType("OS:WaterHeater:Stratified");
  ASSERT_EQ(1u, whStratifieds.size());
  auto whStratified = whStratifieds[0];

  // VT should have added one WaterHeater:Sizing object for each WaterHeater (Mixed/Stratified)
  std::vector<WorkspaceObject> sizingObjs = model->getObjectsByType("OS:WaterHeater:Sizing");
  ASSERT_EQ(2u, sizingObjs.size());

  auto foundMixed = false;
  auto foundStratified = false;
  for (const auto& sizingObj : sizingObjs) {
    auto wh_ = sizingObj.getTarget(1);
    ASSERT_TRUE(wh_);
    if (wh_->handle() == whMixed.handle()) {
      foundMixed = true;
    } else if (wh_->handle() == whStratified.handle()) {
      foundStratified = true;
    }
    EXPECT_EQ("PeakDraw", sizingObj.getString(2, false, true).get());
    EXPECT_EQ(0.538503, sizingObj.getDouble(3).get());
    EXPECT_EQ(0.0, sizingObj.getDouble(4).get());
    EXPECT_EQ(1.0, sizingObj.getDouble(5).get());
  }
  EXPECT_TRUE(foundMixed);
  EXPECT_TRUE(foundStratified);
}

TEST_F(OSVersionFixture, update_3_2_1_to_3_3_0_AirTerminalSingleDuctInletSideMixer) {
  openstudio::path path = resourcesPath() / toPath("osversion/3_3_0/test_vt_AirTerminalSingleDuctInletSideMixer.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;

  openstudio::path outPath = resourcesPath() / toPath("osversion/3_3_0/test_vt_AirTerminalSingleDuctInletSideMixer_updated.osm");
  model->save(outPath, true);

  std::vector<WorkspaceObject> atus = model->getObjectsByType("OS:AirTerminal:SingleDuct:InletSideMixer");
  ASSERT_EQ(1u, atus.size());
  auto atu = atus[0];

  // Two new fields
  EXPECT_EQ("Yes", atu.getString(5).get());
  EXPECT_EQ("CurrentOccupancy", atu.getString(6).get());
}

TEST_F(OSVersionFixture, update_3_2_1_to_3_3_0_GroundHeatExchangerVertical) {
  openstudio::path path = resourcesPath() / toPath("osversion/3_3_0/test_vt_GroundHeatExchangerVertical.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;

  openstudio::path outPath = resourcesPath() / toPath("osversion/3_3_0/test_vt_GroundHeatExchangerVertical_updated.osm");
  model->save(outPath, true);

  std::vector<WorkspaceObject> ghes = model->getObjectsByType("OS:GroundHeatExchanger:Vertical");
  ASSERT_EQ(1u, ghes.size());
  WorkspaceObject ghe = ghes[0];

  // Check the new field 4 (value is unchanged, but description is...)
  EXPECT_EQ(1.15, ghe.getDouble(4).get());

  // Field before deletion
  EXPECT_EQ(13.385, ghe.getDouble(10).get());
  // Field right after deleted
  EXPECT_EQ(0.71111, ghe.getDouble(11).get());

  EXPECT_EQ(35, ghe.numExtensibleGroups());
}

TEST_F(OSVersionFixture, update_3_2_1_to_3_3_0_ControllerMechanicalVentilation) {
  openstudio::path path = resourcesPath() / toPath("osversion/3_3_0/test_vt_ControllerMechanicalVentilation.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;

  openstudio::path outPath = resourcesPath() / toPath("osversion/3_3_0/test_vt_ControllerMechanicalVentilation_updated.osm");
  model->save(outPath, true);

  std::vector<WorkspaceObject> cs = model->getObjectsByType("OS:Controller:MechanicalVentilation");
  ASSERT_EQ(1u, cs.size());
  WorkspaceObject c = cs[0];

  // Field 4: VentilationRateProcedure -> Standard62.1VentilationRateProcedure
  EXPECT_EQ("Standard62.1VentilationRateProcedure", c.getString(4, false, true).get());
}

TEST_F(OSVersionFixture, update_3_2_1_to_3_3_0_SizingSystem) {
  openstudio::path path = resourcesPath() / toPath("osversion/3_3_0/test_vt_SizingSystem.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;

  openstudio::path outPath = resourcesPath() / toPath("osversion/3_3_0/test_vt_SizingSystem_updated.osm");
  model->save(outPath, true);

  std::vector<WorkspaceObject> szs = model->getObjectsByType("OS:Sizing:System");
  ASSERT_EQ(1u, szs.size());
  WorkspaceObject sz = szs[0];

  // Field 20: VentilationRateProcedure -> Standard62.1VentilationRateProcedure
  EXPECT_EQ("Standard62.1VentilationRateProcedure", sz.getString(20, false, true).get());

  // Field before
  EXPECT_EQ(1.2, sz.getDouble(19).get());  // Heating Design Air Flow Rate
  // Field right after
  EXPECT_EQ(1.0, sz.getDouble(21).get());  // Zone Maximum Outdoor Air Fraction
}

TEST_F(OSVersionFixture, update_3_2_1_to_3_3_0_DesignDay) {
  openstudio::path path = resourcesPath() / toPath("osversion/3_3_0/test_vt_DesignDay.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;

  openstudio::path outPath = resourcesPath() / toPath("osversion/3_3_0/test_vt_DesignDay_updated.osm");
  model->save(outPath, true);

  std::vector<WorkspaceObject> dds = model->getObjectsByType("OS:SizingPeriod:DesignDay");
  ASSERT_EQ(1u, dds.size());
  WorkspaceObject dd = dds[0];

  EXPECT_EQ("", dd.getString(8).get());          // Rain Indicator
  EXPECT_EQ("No", dd.getString(9).get());        // Snow Indicator
  EXPECT_EQ("WetBulb", dd.getString(14).get());  // Humidity Condition Type
  EXPECT_EQ(23, dd.getDouble(16).get());         // Wetbulb or DewPoint at Maximum Dry-Bulb
  EXPECT_EQ("", dd.getString(17).get());         // Humidity Ratio at Maximum Dry-Bulb
  EXPECT_EQ("", dd.getString(18).get());         // Enthalpy at Maximum Dry-Bulb
}

TEST_F(OSVersionFixture, update_3_3_0_to_3_4_0_CoilHeatingDXMultiSpeed) {
  openstudio::path path = resourcesPath() / toPath("osversion/3_4_0/test_vt_CoilHeatingDXMultiSpeed.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;

  openstudio::path outPath = resourcesPath() / toPath("osversion/3_4_0/test_vt_CoilHeatingDXMultiSpeed_updated.osm");
  model->save(outPath, true);

  std::vector<WorkspaceObject> coils = model->getObjectsByType("OS:Coil:Heating:DX:MultiSpeed");
  ASSERT_EQ(1u, coils.size());
  WorkspaceObject coil = coils[0];

  ASSERT_EQ(22u, coil.numFields());
  ASSERT_EQ(4u, coil.numExtensibleGroups());

  ASSERT_EQ(4u, model->getObjectsByType("OS:Coil:Heating:DX:MultiSpeed:StageData").size());
  ASSERT_EQ(0u, model->getObjectsByType("OS:ModelObjectList").size());
}

TEST_F(OSVersionFixture, update_3_4_0_to_3_5_0_AirWallMaterial) {
  openstudio::path path = resourcesPath() / toPath("osversion/3_5_0/test_vt_AirWallMaterial.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;

  openstudio::path outPath = resourcesPath() / toPath("osversion/3_5_0/test_vt_AirWallMaterial_updated.osm");
  model->save(outPath, true);

  ASSERT_EQ(3u, model->numObjects());  // Surface, Construction, RenderingColor

  std::vector<WorkspaceObject> constrs = model->getObjectsByType("OS:Construction");
  ASSERT_EQ(0u, constrs.size());

  std::vector<WorkspaceObject> constrAirBoundarys = model->getObjectsByType("OS:Construction:AirBoundary");
  ASSERT_EQ(1u, constrAirBoundarys.size());
  WorkspaceObject constrAirBoundary = constrAirBoundarys[0];

  EXPECT_EQ("Construction 1", constrAirBoundary.getString(1).get());  // Name
  EXPECT_EQ("", constrAirBoundary.getString(2).get());                // Air Exchange Method
  EXPECT_EQ(0.0, constrAirBoundary.getDouble(3).get());               // Simple Mixing Air Changes Per Hour
  EXPECT_EQ("", constrAirBoundary.getString(4).get());                // Simple Mixing Schedule Name
  //  Surface Rendering Name
  ASSERT_TRUE(constrAirBoundary.getTarget(5));
  EXPECT_EQ("AirWall RenderingColor", constrAirBoundary.getTarget(5)->nameString());

  std::vector<WorkspaceObject> surfaces = model->getObjectsByType("OS:Surface");
  ASSERT_EQ(1u, surfaces.size());
  WorkspaceObject surface = surfaces[0];
  ASSERT_TRUE(surface.getTarget(3));  // Construction Name
  EXPECT_EQ("OS:Construction:AirBoundary", surface.getTarget(3).get().iddObject().name());
  EXPECT_EQ("Construction 1", surface.getTarget(3).get().nameString());  // Construction Name
}

TEST_F(OSVersionFixture, update_3_4_0_to_3_5_0_TableMultiVariableLookup_oneDim) {
  openstudio::path path = resourcesPath() / toPath("osversion/3_5_0/test_vt_TableMultiVariableLookup_oneDim.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;

  openstudio::path outPath = resourcesPath() / toPath("osversion/3_5_0/test_vt_TableMultiVariableLookup_oneDim_updated.osm");
  model->save(outPath, true);

  std::vector<WorkspaceObject> tableLookUps = model->getObjectsByType("OS:Table:Lookup");
  ASSERT_EQ(1u, tableLookUps.size());
  ASSERT_EQ(1u, model->getObjectsByType("OS:Table:IndependentVariable").size());
  ASSERT_EQ(1u, model->getObjectsByType("OS:ModelObjectList").size());

  double norm_ref = 2.4;
  std::vector<double> x1{0.0, 0.05, 0.33333, 0.5, 0.666667, 0.833333, 1.0, 1.333333};
  std::vector<double> y{0.0, 0.0024, 1.704, 2.04, 2.208, 2.328, 2.4, 2.496};

  auto& tableLookUp = tableLookUps.front();
  EXPECT_EQ("CapModFuncOfWaterFlow", tableLookUp.nameString());

  // We have a normalization reference originally, so it's DivisorOnly
  EXPECT_EQ("DivisorOnly", tableLookUp.getString(3).get());    // Normalization Method
  EXPECT_EQ(norm_ref, tableLookUp.getDouble(4).get());         // Normalization Divisor
  EXPECT_EQ(0.0, tableLookUp.getDouble(5).get());              // Minimum Output
  EXPECT_EQ(1.04 * norm_ref, tableLookUp.getDouble(6).get());  // Maximum Output
  EXPECT_EQ("Dimensionless", tableLookUp.getString(7).get());  // Output Unit Type
  EXPECT_TRUE(tableLookUp.isEmpty(8));                         // External File Name
  EXPECT_TRUE(tableLookUp.isEmpty(9));                         // External File Column Number
  EXPECT_TRUE(tableLookUp.isEmpty(10));                        // External File Starting Row Number
  // Output Values
  ASSERT_EQ(y.size(), tableLookUp.numExtensibleGroups());
  for (size_t i = 0; auto& eg : tableLookUp.extensibleGroups()) {
    EXPECT_EQ(y[i], eg.getDouble(0).get());
    ++i;
  }

  auto varList_ = tableLookUp.getTarget(2);
  ASSERT_TRUE(varList_);
  EXPECT_EQ("CapModFuncOfWaterFlow_IndependentVariableList", varList_->nameString());
  ASSERT_EQ(1, varList_->numExtensibleGroups());
  auto var_ = varList_->extensibleGroups().front().cast<WorkspaceExtensibleGroup>().getTarget(0);
  ASSERT_TRUE(var_);
  EXPECT_EQ("CapModFuncOfWaterFlow_IndependentVariable_0", var_->nameString());

  // Interpolation = EvaluateCurveToLimits maps to Cubic/Constant
  EXPECT_EQ("Cubic", var_->getString(2).get());          // Interpolation Method
  EXPECT_EQ("Constant", var_->getString(3).get());       // Extrapolation Method
  EXPECT_EQ(0.0, var_->getDouble(4).get());              // Minimum Value
  EXPECT_EQ(1.33, var_->getDouble(5).get());             // Maximum Value
  EXPECT_TRUE(var_->isEmpty(6));                         // Normalization Reference Value
  EXPECT_EQ("Dimensionless", var_->getString(7).get());  // Unit Type
  EXPECT_TRUE(var_->isEmpty(8));                         // External File Name
  EXPECT_TRUE(var_->isEmpty(9));                         // External File Column Number
  EXPECT_TRUE(var_->isEmpty(10));                        // External File Starting Row Number

  // Values
  ASSERT_EQ(x1.size(), var_->numExtensibleGroups());
  for (size_t i = 0; auto& eg : var_->extensibleGroups()) {
    EXPECT_EQ(x1[i], eg.getDouble(0).get());
    ++i;
  }
}

TEST_F(OSVersionFixture, update_3_4_0_to_3_5_0_TableMultiVariableLookup_twoDims) {
  openstudio::path path = resourcesPath() / toPath("osversion/3_5_0/test_vt_TableMultiVariableLookup_twoDims.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;

  openstudio::path outPath = resourcesPath() / toPath("osversion/3_5_0/test_vt_TableMultiVariableLookup_twoDims_updated.osm");
  model->save(outPath, true);

  std::vector<WorkspaceObject> tableLookUps = model->getObjectsByType("OS:Table:Lookup");
  ASSERT_EQ(1u, tableLookUps.size());
  ASSERT_EQ(2u, model->getObjectsByType("OS:Table:IndependentVariable").size());
  ASSERT_EQ(1u, model->getObjectsByType("OS:ModelObjectList").size());

  std::vector<double> x1{70.0, 72.0, 74.0, 76.0, 78.0};
  std::vector<double> x2{32.0, 45.0, 68.0, 81.0, 94.0, 107.0};

  std::vector<double> y{0.1, 0.3, 0.5, 0.7, 0.9, 0.2, 0.4, 0.6, 0.8, 1.0, 0.3, 0.5, 0.7, 0.9, 1.1,
                        0.4, 0.6, 0.8, 1.0, 1.2, 0.5, 0.7, 0.9, 1.1, 1.3, 0.6, 0.8, 1.0, 1.2, 1.4};

  auto& tableLookUp = tableLookUps.front();
  EXPECT_EQ("Table With 2 dims", tableLookUp.nameString());

  // We don't have a normalization reference originally, so "None" and 1.0 to match the ctor

  EXPECT_EQ("None", tableLookUp.getString(3).get());  // Normalization Method
  EXPECT_EQ(1.0, tableLookUp.getDouble(4).get());     // Normalization Divisor

  EXPECT_EQ(0.05, tableLookUp.getDouble(5).get());             // Minimum Output
  EXPECT_EQ(1.25, tableLookUp.getDouble(6).get());             // Maximum Output
  EXPECT_EQ("Dimensionless", tableLookUp.getString(7).get());  // Output Unit Type
  EXPECT_TRUE(tableLookUp.isEmpty(8));                         // External File Name
  EXPECT_TRUE(tableLookUp.isEmpty(9));                         // External File Column Number
  EXPECT_TRUE(tableLookUp.isEmpty(10));                        // External File Starting Row Number
  // Output Values
  ASSERT_EQ(y.size(), tableLookUp.numExtensibleGroups());
  for (size_t i = 0; auto& eg : tableLookUp.extensibleGroups()) {
    EXPECT_EQ(y[i], eg.getDouble(0).get());
    ++i;
  }

  auto varList_ = tableLookUp.getTarget(2);
  ASSERT_TRUE(varList_);
  EXPECT_EQ("Table With 2 dims_IndependentVariableList", varList_->nameString());
  ASSERT_EQ(2, varList_->numExtensibleGroups());
  {
    auto var_ = varList_->extensibleGroups().front().cast<WorkspaceExtensibleGroup>().getTarget(0);
    ASSERT_TRUE(var_);
    EXPECT_EQ("Table With 2 dims_IndependentVariable_0", var_->nameString());

    // LagrangeInterpolationLinearExtrapolation = EvaluateCurveToLimits maps to Cubic/Linear
    EXPECT_EQ("Cubic", var_->getString(2).get());        // Interpolation Method
    EXPECT_EQ("Linear", var_->getString(3).get());       // Extrapolation Method
    EXPECT_EQ(60.0, var_->getDouble(4).get());           // Minimum Value
    EXPECT_EQ(80.0, var_->getDouble(5).get());           // Maximum Value
    EXPECT_TRUE(var_->isEmpty(6));                       // Normalization Reference Value
    EXPECT_EQ("Temperature", var_->getString(7).get());  // Unit Type
    EXPECT_TRUE(var_->isEmpty(8));                       // External File Name
    EXPECT_TRUE(var_->isEmpty(9));                       // External File Column Number
    EXPECT_TRUE(var_->isEmpty(10));                      // External File Starting Row Number

    // Values
    ASSERT_EQ(x1.size(), var_->numExtensibleGroups());
    for (size_t i = 0; auto& eg : var_->extensibleGroups()) {
      EXPECT_EQ(x1[i], eg.getDouble(0).get());
      ++i;
    }
  }
  {
    auto var_ = varList_->extensibleGroups().back().cast<WorkspaceExtensibleGroup>().getTarget(0);
    ASSERT_TRUE(var_);
    EXPECT_EQ("Table With 2 dims_IndependentVariable_1", var_->nameString());

    // LagrangeInterpolationLinearExtrapolation = EvaluateCurveToLimits maps to Cubic/Linear
    EXPECT_EQ("Cubic", var_->getString(2).get());        // Interpolation Method
    EXPECT_EQ("Linear", var_->getString(3).get());       // Extrapolation Method
    EXPECT_EQ(30, var_->getDouble(4).get());             // Minimum Value
    EXPECT_EQ(100.0, var_->getDouble(5).get());          // Maximum Value
    EXPECT_TRUE(var_->isEmpty(6));                       // Normalization Reference Value
    EXPECT_EQ("Temperature", var_->getString(7).get());  // Unit Type
    EXPECT_TRUE(var_->isEmpty(8));                       // External File Name
    EXPECT_TRUE(var_->isEmpty(9));                       // External File Column Number
    EXPECT_TRUE(var_->isEmpty(10));                      // External File Starting Row Number

    // Values
    ASSERT_EQ(x2.size(), var_->numExtensibleGroups());
    for (size_t i = 0; auto& eg : var_->extensibleGroups()) {
      EXPECT_EQ(x2[i], eg.getDouble(0).get());
      ++i;
    }
  }
}

TEST_F(OSVersionFixture, update_3_4_0_to_3_5_0_CoilHeatingDXSingleSpeed) {
  openstudio::path path = resourcesPath() / toPath("osversion/3_5_0/test_vt_CoilHeatingDXSingleSpeed.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;

  openstudio::path outPath = resourcesPath() / toPath("osversion/3_5_0/test_vt_CoilHeatingDXSingleSpeed_updated.osm");
  model->save(outPath, true);

  std::vector<WorkspaceObject> coils = model->getObjectsByType("OS:Coil:Heating:DX:SingleSpeed");
  ASSERT_EQ(1u, coils.size());
  WorkspaceObject coil = coils[0];

  EXPECT_EQ(773.3, coil.getDouble(6).get());  // Rated Supply Fan Power Per Volume Flow Rate 2017
  EXPECT_EQ(934.4, coil.getDouble(7).get());  // Rated Supply Fan Power Per Volume Flow Rate 2023
}

TEST_F(OSVersionFixture, update_3_4_0_to_3_5_0_CoilCoolingDXSingleSpeed) {
  openstudio::path path = resourcesPath() / toPath("osversion/3_5_0/test_vt_CoilCoolingDXSingleSpeed.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;

  openstudio::path outPath = resourcesPath() / toPath("osversion/3_5_0/test_vt_CoilCoolingDXSingleSpeed_updated.osm");
  model->save(outPath, true);

  std::vector<WorkspaceObject> ccs = model->getObjectsByType("OS:Coil:Cooling:DX:SingleSpeed");
  ASSERT_EQ(2, ccs.size());

  {
    auto cc = model->getObjectByTypeAndName("OS:Coil:Cooling:DX:SingleSpeed", "CC with numbers").get();
    EXPECT_EQ(16, cc.getDouble(16).get());
    EXPECT_EQ(17, cc.getDouble(17).get());
    EXPECT_EQ(0.18, cc.getDouble(18).get());
    EXPECT_EQ(0.19, cc.getDouble(19).get());
    EXPECT_EQ(20, cc.getDouble(20).get());

    EXPECT_EQ("EvaporativelyCooled", cc.getString(22).get());
    EXPECT_EQ(0.23, cc.getDouble(23).get());
    EXPECT_EQ("Autosize", cc.getString(24).get());
    EXPECT_EQ("Autosize", cc.getString(25).get());
    EXPECT_EQ(26, cc.getDouble(26).get());
    EXPECT_EQ(27, cc.getDouble(27).get());

    EXPECT_EQ(773.3, cc.getDouble(7).get());  // Rated Evaporator Fan Power Per Volume Flow Rate 2017
    EXPECT_EQ(934.4, cc.getDouble(8).get());  // Rated Evaporator Fan Power Per Volume Flow Rate 2023
  }

  {
    auto cc = model->getObjectByTypeAndName("OS:Coil:Cooling:DX:SingleSpeed", "CC with blanks").get();
    EXPECT_EQ(-25, cc.getDouble(16).get());
    EXPECT_EQ(0, cc.getInt(17).get());
    EXPECT_EQ(0, cc.getInt(18).get());
    EXPECT_EQ(0, cc.getInt(19).get());
    EXPECT_EQ(0, cc.getInt(20).get());

    EXPECT_EQ("AirCooled", cc.getString(22).get());
    EXPECT_EQ(0.0, cc.getInt(23).get());
    EXPECT_EQ("Autosize", cc.getString(24).get());
    EXPECT_EQ("Autosize", cc.getString(25).get());
    EXPECT_EQ(0, cc.getInt(26).get());
    EXPECT_EQ(0, cc.getInt(27).get());

    EXPECT_EQ(773.3, cc.getDouble(7).get());  // Rated Evaporator Fan Power Per Volume Flow Rate 2017
    EXPECT_EQ(934.4, cc.getDouble(8).get());  // Rated Evaporator Fan Power Per Volume Flow Rate 2023
  }
}

TEST_F(OSVersionFixture, update_3_4_0_to_3_5_0_CoilHeatingDXMultiSpeedStageData) {
  openstudio::path path = resourcesPath() / toPath("osversion/3_5_0/test_vt_CoilHeatingDXMultiSpeedStageData.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;

  openstudio::path outPath = resourcesPath() / toPath("osversion/3_5_0/test_vt_CoilHeatingDXMultiSpeedStageData_updated.osm");
  model->save(outPath, true);

  std::vector<WorkspaceObject> coils = model->getObjectsByType("OS:Coil:Heating:DX:MultiSpeed:StageData");
  ASSERT_EQ(1u, coils.size());
  WorkspaceObject coil = coils[0];

  EXPECT_EQ(773.3, coil.getDouble(5).get());  // Rated Supply Air Fan Power Per Volume Flow Rate 2017
  EXPECT_EQ(934.4, coil.getDouble(6).get());  // Rated Supply Air Fan Power Per Volume Flow Rate 2023
}

TEST_F(OSVersionFixture, update_3_4_0_to_3_5_0_CoilCoolingDXMultiSpeedStageData) {
  openstudio::path path = resourcesPath() / toPath("osversion/3_5_0/test_vt_CoilCoolingDXMultiSpeedStageData.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;

  openstudio::path outPath = resourcesPath() / toPath("osversion/3_5_0/test_vt_CoilCoolingDXMultiSpeedStageData_updated.osm");
  model->save(outPath, true);

  std::vector<WorkspaceObject> coils = model->getObjectsByType("OS:Coil:Cooling:DX:MultiSpeed:StageData");
  ASSERT_EQ(1u, coils.size());
  WorkspaceObject coil = coils[0];

  EXPECT_EQ(773.3, coil.getDouble(6).get());  // Rated Evaporator Fan Power Per Volume Flow Rate 2017
  EXPECT_EQ(934.4, coil.getDouble(7).get());  // Rated Evaporator Fan Power Per Volume Flow Rate 2023
}

TEST_F(OSVersionFixture, update_3_4_0_to_3_5_0_CoilCoolingDXCurveFitSpeed) {
  openstudio::path path = resourcesPath() / toPath("osversion/3_5_0/test_vt_CoilCoolingDXCurveFitSpeed.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;

  openstudio::path outPath = resourcesPath() / toPath("osversion/3_5_0/test_vt_CoilCoolingDXCurveFitSpeed_updated.osm");
  model->save(outPath, true);

  std::vector<WorkspaceObject> coils = model->getObjectsByType("OS:Coil:Cooling:DX:CurveFit:Speed");
  ASSERT_EQ(1u, coils.size());
  WorkspaceObject coil = coils[0];

  EXPECT_EQ(773.3, coil.getDouble(8).get());  // Rated Evaporator Fan Power Per Volume Flow Rate 2017
  EXPECT_EQ(934.4, coil.getDouble(9).get());  // Rated Evaporator Fan Power Per Volume Flow Rate 2023
}

TEST_F(OSVersionFixture, update_3_4_0_to_3_5_0_CoilHeatingWaterToAirHeatPumpEquationFit) {
  openstudio::path path = resourcesPath() / toPath("osversion/3_5_0/test_vt_CoilHeatingWaterToAirHeatPumpEquationFit.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;

  openstudio::path outPath = resourcesPath() / toPath("osversion/3_5_0/test_vt_CoilHeatingWaterToAirHeatPumpEquationFit_updated.osm");
  model->save(outPath, true);

  std::vector<WorkspaceObject> coils = model->getObjectsByType("OS:Coil:Heating:WaterToAirHeatPump:EquationFit");
  ASSERT_EQ(1u, coils.size());
  WorkspaceObject coil = coils[0];

  EXPECT_TRUE(coil.isEmpty(9));              // Rated Heating Coefficient of Performance
  EXPECT_EQ(20, coil.getDouble(10).get());   // Rated Entering Water Temperature
  EXPECT_EQ(20, coil.getDouble(11).get());   // Rated Entering Air Dry-Bulb Temperature
  EXPECT_EQ(1.0, coil.getDouble(12).get());  // Ratio of Rated Heating Capacity to Rated Cooling Capacity
  ASSERT_TRUE(coil.getTarget(14));           // Heating Power Consumption Curve Name
  EXPECT_EQ("Curve Quad Linear 2", coil.getTarget(14)->nameString());
}

TEST_F(OSVersionFixture, update_3_4_0_to_3_5_0_CoilCoolingWaterToAirHeatPumpEquationFit) {
  openstudio::path path = resourcesPath() / toPath("osversion/3_5_0/test_vt_CoilCoolingWaterToAirHeatPumpEquationFit.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;

  openstudio::path outPath = resourcesPath() / toPath("osversion/3_5_0/test_vt_CoilCoolingWaterToAirHeatPumpEquationFit_updated.osm");
  model->save(outPath, true);

  std::vector<WorkspaceObject> coils = model->getObjectsByType("OS:Coil:Cooling:WaterToAirHeatPump:EquationFit");
  ASSERT_EQ(1u, coils.size());
  WorkspaceObject coil = coils[0];

  EXPECT_TRUE(coil.isEmpty(10));              // Rated Cooling Coefficient of Performance
  EXPECT_EQ(30, coil.getDouble(11).get());    // Rated Entering Water Temperature
  EXPECT_EQ(27, coil.getDouble(12).get());    // Rated Entering Air Dry-Bulb Temperature
  EXPECT_EQ(19.0, coil.getDouble(13).get());  // Rated Entering Air Wet-Bulb Temperature
  EXPECT_TRUE(coil.isEmpty(18));              // Ratio of Initial Moisture Evaporation Rate and Steady State Latent Capacity
}

TEST_F(OSVersionFixture, update_3_4_0_to_3_5_0_SizingZone) {
  openstudio::path path = resourcesPath() / toPath("osversion/3_5_0/test_vt_SizingZone.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;

  openstudio::path outPath = resourcesPath() / toPath("osversion/3_5_0/test_vt_SizingZone_updated.osm");
  model->save(outPath, true);

  std::vector<WorkspaceObject> szs = model->getObjectsByType("OS:Sizing:Zone");
  ASSERT_EQ(1u, szs.size());
  auto& sz = szs.front();

  // 9 new fields inserted at position 26, with some harcoding
  // Field Before
  EXPECT_EQ(18.0, sz.getDouble(24).get());
  EXPECT_EQ(19.0, sz.getDouble(25).get());

  EXPECT_EQ("Sensible Load Only No Latent Load", sz.getString(26).get());
  EXPECT_EQ("HumidityRatioDifference", sz.getString(27).get());
  EXPECT_TRUE(sz.isEmpty(28));
  EXPECT_EQ(0.005, sz.getDouble(29).get());
  EXPECT_EQ("HumidityRatioDifference", sz.getString(30).get());
  EXPECT_TRUE(sz.isEmpty(31));
  EXPECT_EQ(0.005, sz.getDouble(32).get());

  // Field after
  EXPECT_EQ(0.8, sz.getDouble(35).get());
  EXPECT_EQ(0.7, sz.getDouble(36).get());
  EXPECT_EQ(0.6, sz.getDouble(37).get());
  EXPECT_EQ(0.5, sz.getDouble(38).get());
}

TEST_F(OSVersionFixture, update_3_4_0_to_3_5_0_CoilHeatingGasMultiStage) {
  openstudio::path path = resourcesPath() / toPath("osversion/3_5_0/test_vt_CoilHeatingGasMultiStage.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;

  openstudio::path outPath = resourcesPath() / toPath("osversion/3_5_0/test_vt_CoilHeatingGasMultiStage_updated.osm");
  model->save(outPath, true);

  std::vector<WorkspaceObject> coils = model->getObjectsByType("OS:Coil:Heating:Gas:MultiStage");
  ASSERT_EQ(1u, coils.size());
  WorkspaceObject coil = coils[0];

  ASSERT_TRUE(coil.getTarget(2));
  EXPECT_EQ("Always On Discrete", coil.getTarget(2)->nameString());
}

TEST_F(OSVersionFixture, update_3_4_0_to_3_5_0_ZoneHVACPackaged) {
  openstudio::path path = resourcesPath() / toPath("osversion/3_5_0/test_vt_ZoneHVACPackaged.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;

  openstudio::path outPath = resourcesPath() / toPath("osversion/3_5_0/test_vt_ZoneHVACPackaged_updated.osm");
  model->save(outPath, true);

  EXPECT_EQ(2, model->getObjectsByType("OS:Fan:SystemModel").size());
  {
    std::vector<WorkspaceObject> ptacs = model->getObjectsByType("OS:ZoneHVAC:PackagedTerminalAirConditioner");
    ASSERT_EQ(1u, ptacs.size());
    auto& ptac = ptacs.front();

    // Check the Supply Air Fan Operating Mode Schedule
    ASSERT_TRUE(ptac.getTarget(17));
    WorkspaceObject fanOpSch = ptac.getTarget(17).get();
    EXPECT_EQ("Always Off Discrete", fanOpSch.nameString());
    EXPECT_EQ(0.0, fanOpSch.getDouble(3).get());

    // Check the Fan, converted from Fan:ConstantVolume to Fan:SystemModel
    ASSERT_TRUE(ptac.getTarget(13));
    WorkspaceObject fan = ptac.getTarget(13).get();

    EXPECT_EQ(IddObjectType(IddObjectType::OS_Fan_SystemModel), fan.iddObject().type());
    EXPECT_EQ("PTAC Fan", fan.getString(OS_Fan_SystemModelFields::Name).get());
    EXPECT_EQ("Always On Discrete", fan.getString(OS_Fan_SystemModelFields::AvailabilityScheduleName).get());
    EXPECT_TRUE(fan.isEmpty(OS_Fan_SystemModelFields::AirInletNodeName));
    EXPECT_TRUE(fan.isEmpty(OS_Fan_SystemModelFields::AirOutletNodeName));
    EXPECT_EQ(0.5, fan.getDouble(OS_Fan_SystemModelFields::DesignMaximumAirFlowRate).get());
    EXPECT_EQ("Discrete", fan.getString(OS_Fan_SystemModelFields::SpeedControlMethod).get());
    EXPECT_EQ(0.0, fan.getDouble(OS_Fan_SystemModelFields::ElectricPowerMinimumFlowRateFraction).get());
    EXPECT_EQ(265.0, fan.getDouble(OS_Fan_SystemModelFields::DesignPressureRise).get());
    EXPECT_EQ(0.89, fan.getDouble(OS_Fan_SystemModelFields::MotorEfficiency).get());
    EXPECT_EQ(0.92, fan.getDouble(OS_Fan_SystemModelFields::MotorInAirStreamFraction).get());
    EXPECT_EQ("Autosize", fan.getString(OS_Fan_SystemModelFields::DesignElectricPowerConsumption).get());
    EXPECT_EQ("TotalEfficiencyAndPressure", fan.getString(OS_Fan_SystemModelFields::DesignPowerSizingMethod).get());
    EXPECT_EQ(840.0, fan.getDouble(OS_Fan_SystemModelFields::ElectricPowerPerUnitFlowRate).get());
    EXPECT_EQ(1.66667, fan.getDouble(OS_Fan_SystemModelFields::ElectricPowerPerUnitFlowRatePerUnitPressure).get());
    EXPECT_EQ(0.75, fan.getDouble(OS_Fan_SystemModelFields::FanTotalEfficiency).get());
    EXPECT_TRUE(fan.isEmpty(OS_Fan_SystemModelFields::ElectricPowerFunctionofFlowFractionCurveName));
    EXPECT_TRUE(fan.isEmpty(OS_Fan_SystemModelFields::NightVentilationModePressureRise));
    EXPECT_TRUE(fan.isEmpty(OS_Fan_SystemModelFields::NightVentilationModeFlowFraction));
    EXPECT_TRUE(fan.isEmpty(OS_Fan_SystemModelFields::MotorLossZoneName));
    EXPECT_EQ(0.0, fan.getDouble(OS_Fan_SystemModelFields::MotorLossRadiativeFraction).get());
    EXPECT_EQ("PTAC Fans", fan.getString(OS_Fan_SystemModelFields::EndUseSubcategory).get());
  }

  {
    std::vector<WorkspaceObject> pthps = model->getObjectsByType("OS:ZoneHVAC:PackagedTerminalHeatPump");
    ASSERT_EQ(1u, pthps.size());
    auto& pthp = pthps.front();

    // Check the Supply Air Fan Operating Mode Schedule
    ASSERT_TRUE(pthp.getTarget(23));
    WorkspaceObject fanOpSch = pthp.getTarget(23).get();
    EXPECT_EQ("Always Off Discrete", fanOpSch.nameString());
    EXPECT_EQ(0.0, fanOpSch.getDouble(3).get());

    // Check the Fan, converted from Fan:ConstantVolume to Fan:SystemModel
    ASSERT_TRUE(pthp.getTarget(13));
    WorkspaceObject fan = pthp.getTarget(13).get();

    // This one is all defaulted, ensure we get the SAME values
    EXPECT_EQ(IddObjectType(IddObjectType::OS_Fan_SystemModel), fan.iddObject().type());
    EXPECT_EQ("PTHP Fan", fan.getString(OS_Fan_SystemModelFields::Name).get());
    EXPECT_EQ("Always On Discrete", fan.getString(OS_Fan_SystemModelFields::AvailabilityScheduleName).get());
    EXPECT_TRUE(fan.isEmpty(OS_Fan_SystemModelFields::AirInletNodeName));
    EXPECT_TRUE(fan.isEmpty(OS_Fan_SystemModelFields::AirOutletNodeName));
    EXPECT_EQ("AutoSize", fan.getString(OS_Fan_SystemModelFields::DesignMaximumAirFlowRate).get());
    EXPECT_EQ("Discrete", fan.getString(OS_Fan_SystemModelFields::SpeedControlMethod).get());
    EXPECT_EQ(0.0, fan.getDouble(OS_Fan_SystemModelFields::ElectricPowerMinimumFlowRateFraction).get());
    EXPECT_EQ(250.0, fan.getDouble(OS_Fan_SystemModelFields::DesignPressureRise).get());
    EXPECT_EQ(0.9, fan.getDouble(OS_Fan_SystemModelFields::MotorEfficiency).get());
    EXPECT_EQ(1.0, fan.getDouble(OS_Fan_SystemModelFields::MotorInAirStreamFraction).get());
    EXPECT_EQ("Autosize", fan.getString(OS_Fan_SystemModelFields::DesignElectricPowerConsumption).get());
    EXPECT_EQ("TotalEfficiencyAndPressure", fan.getString(OS_Fan_SystemModelFields::DesignPowerSizingMethod).get());
    EXPECT_EQ(840.0, fan.getDouble(OS_Fan_SystemModelFields::ElectricPowerPerUnitFlowRate).get());
    EXPECT_EQ(1.66667, fan.getDouble(OS_Fan_SystemModelFields::ElectricPowerPerUnitFlowRatePerUnitPressure).get());
    EXPECT_EQ(0.7, fan.getDouble(OS_Fan_SystemModelFields::FanTotalEfficiency).get());
    EXPECT_TRUE(fan.isEmpty(OS_Fan_SystemModelFields::ElectricPowerFunctionofFlowFractionCurveName));
    EXPECT_TRUE(fan.isEmpty(OS_Fan_SystemModelFields::NightVentilationModePressureRise));
    EXPECT_TRUE(fan.isEmpty(OS_Fan_SystemModelFields::NightVentilationModeFlowFraction));
    EXPECT_TRUE(fan.isEmpty(OS_Fan_SystemModelFields::MotorLossZoneName));
    EXPECT_EQ(0.0, fan.getDouble(OS_Fan_SystemModelFields::MotorLossRadiativeFraction).get());
    EXPECT_EQ("General", fan.getString(OS_Fan_SystemModelFields::EndUseSubcategory).get());
  }
}

TEST_F(OSVersionFixture, update_3_4_0_to_3_5_0_TableMultiVariableLookup_oneDim_osc) {
  openstudio::path p = resourcesPath() / toPath("osversion/3_5_0/test_vt_TableMultiVariableLookup_oneDim.osc");
  osversion::VersionTranslator vt;
  boost::optional<model::Component> comp_ = vt.loadComponent(p);
  ASSERT_TRUE(comp_) << "Failed to load Component " << p;

  openstudio::path outPath = resourcesPath() / toPath("osversion/3_5_0/test_vt_TableMultiVariableLookup_oneDim_updated.osc");
  comp_->save(outPath, true);

  // Ori OSC has ONE TableMultiVariableLookup object.
  // We change that to 1 TableLookup, 1 ModelObjectList (=TableIndependentVariableList) and 1 TableIndependentVariable
  auto compData = comp_->componentData();
  EXPECT_EQ(3, compData.numComponentObjects());

  Model model;
  OptionalComponentData ocd = model.insertComponent(comp_.get());
  ASSERT_TRUE(ocd);

  std::vector<WorkspaceObject> tableLookUps = model.getObjectsByType("OS:Table:Lookup");
  ASSERT_EQ(1u, tableLookUps.size());
  ASSERT_EQ(1u, model.getObjectsByType("OS:Table:IndependentVariable").size());
  ASSERT_EQ(1u, model.getObjectsByType("OS:ModelObjectList").size());

  double norm_ref = 2.4;
  std::vector<double> x1{0.0, 0.05, 0.33333, 0.5, 0.666667, 0.833333, 1.0, 1.333333};
  std::vector<double> y{0.0, 0.0024, 1.704, 2.04, 2.208, 2.328, 2.4, 2.496};

  auto& tableLookUp = tableLookUps.front();
  EXPECT_EQ("CapModFuncOfWaterFlow", tableLookUp.nameString());

  // We have a normalization reference originally, so it's DivisorOnly
  EXPECT_EQ("DivisorOnly", tableLookUp.getString(3).get());    // Normalization Method
  EXPECT_EQ(norm_ref, tableLookUp.getDouble(4).get());         // Normalization Divisor
  EXPECT_EQ(0.0, tableLookUp.getDouble(5).get());              // Minimum Output
  EXPECT_EQ(1.04 * norm_ref, tableLookUp.getDouble(6).get());  // Maximum Output
  EXPECT_EQ("Dimensionless", tableLookUp.getString(7).get());  // Output Unit Type
  EXPECT_TRUE(tableLookUp.isEmpty(8));                         // External File Name
  EXPECT_TRUE(tableLookUp.isEmpty(9));                         // External File Column Number
  EXPECT_TRUE(tableLookUp.isEmpty(10));                        // External File Starting Row Number
  // Output Values
  ASSERT_EQ(y.size(), tableLookUp.numExtensibleGroups());
  for (size_t i = 0; auto& eg : tableLookUp.extensibleGroups()) {
    EXPECT_EQ(y[i], eg.getDouble(0).get());
    ++i;
  }

  auto varList_ = tableLookUp.getTarget(2);
  ASSERT_TRUE(varList_);
  EXPECT_EQ("CapModFuncOfWaterFlow_IndependentVariableList", varList_->nameString());
  ASSERT_EQ(1, varList_->numExtensibleGroups());
  auto var_ = varList_->extensibleGroups().front().cast<WorkspaceExtensibleGroup>().getTarget(0);
  ASSERT_TRUE(var_);
  EXPECT_EQ("CapModFuncOfWaterFlow_IndependentVariable_0", var_->nameString());

  // Interpolation = EvaluateCurveToLimits maps to Cubic/Constant
  EXPECT_EQ("Cubic", var_->getString(2).get());          // Interpolation Method
  EXPECT_EQ("Constant", var_->getString(3).get());       // Extrapolation Method
  EXPECT_EQ(0.0, var_->getDouble(4).get());              // Minimum Value
  EXPECT_EQ(1.33, var_->getDouble(5).get());             // Maximum Value
  EXPECT_TRUE(var_->isEmpty(6));                         // Normalization Reference Value
  EXPECT_EQ("Dimensionless", var_->getString(7).get());  // Unit Type
  EXPECT_TRUE(var_->isEmpty(8));                         // External File Name
  EXPECT_TRUE(var_->isEmpty(9));                         // External File Column Number
  EXPECT_TRUE(var_->isEmpty(10));                        // External File Starting Row Number

  // Values
  ASSERT_EQ(x1.size(), var_->numExtensibleGroups());
  for (size_t i = 0; auto& eg : var_->extensibleGroups()) {
    EXPECT_EQ(x1[i], eg.getDouble(0).get());
    ++i;
  }
}

TEST_F(OSVersionFixture, update_3_5_0_to_3_5_1_UnitarySystemPerformanceMultispeed) {
  openstudio::path path = resourcesPath() / toPath("osversion/3_5_1/test_vt_UnitarySystemPerformanceMultispeed.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;

  openstudio::path outPath = resourcesPath() / toPath("osversion/3_5_1/test_vt_UnitarySystemPerformanceMultispeed_updated.osm");
  model->save(outPath, true);

  std::vector<WorkspaceObject> perfs = model->getObjectsByType("OS:UnitarySystemPerformance:Multispeed");
  ASSERT_EQ(1u, perfs.size());
  WorkspaceObject perf = perfs[0];

  EXPECT_EQ("Unitary System Performance Multispeed 1", perf.nameString());
  EXPECT_EQ("No", perf.getString(2).get());  // OS_UnitarySystemPerformance_MultispeedFields::SingleModeOperation
  EXPECT_EQ(1.0, perf.getDouble(3).get());   // OS_UnitarySystemPerformance_MultispeedFields::NoLoadSupplyAirFlowRateRatio

  EXPECT_EQ(2u, perf.numExtensibleGroups());
  auto eg1 = perf.extensibleGroups()[0];
  EXPECT_EQ(1.0, eg1.getDouble(0, false).get());
  EXPECT_EQ(2.0, eg1.getDouble(1, false).get());
  auto eg2 = perf.extensibleGroups()[1];
  EXPECT_EQ("autosize", eg2.getString(0, false).get());
  EXPECT_EQ(3.0, eg2.getDouble(1, false).get());
}

TEST_F(OSVersionFixture, update_3_5_0_to_3_5_1_VRF_Terminal_v350_osc) {
  // In this one, the starting version is 3.5.0, so it already has Table:Lookup instead of Table:MultiVariableLookup
  openstudio::path p = resourcesPath() / toPath("osversion/3_5_1/test_VRF_Terminal_v350.osc");
  osversion::VersionTranslator vt;
  boost::optional<model::Component> comp_ = vt.loadComponent(p);
  ASSERT_TRUE(comp_) << "Failed to load Component " << p;

  openstudio::path outPath = resourcesPath() / toPath("osversion/3_5_1/test_VRF_Terminal_v350_updated.osc");
  comp_->save(outPath, true);

  auto compData = comp_->componentData();
  EXPECT_EQ(18, compData.numComponentObjects());
  EXPECT_EQ("OS:ZoneHVAC:TerminalUnit:VariableRefrigerantFlow", compData.primaryComponentObject().iddObject().name());

  Model model;
  OptionalComponentData ocd = model.insertComponent(comp_.get());
  ASSERT_TRUE(ocd);
  EXPECT_EQ("OS:ZoneHVAC:TerminalUnit:VariableRefrigerantFlow", compData.primaryComponentObject().iddObject().name());
  EXPECT_EQ(18, ocd->numComponentObjects());
  EXPECT_EQ(19, model.numObjects());
  EXPECT_EQ("OS:ZoneHVAC:TerminalUnit:VariableRefrigerantFlow", ocd->primaryComponentObject().iddObject().name());
}

TEST_F(OSVersionFixture, update_3_5_0_to_3_5_1_VRF_Terminal_v340_osc) {
  // Here the starting version is 3.4.0, so it has only 12 objects to start with
  openstudio::path p = resourcesPath() / toPath("osversion/3_5_1/test_VRF_Terminal_v340.osc");
  osversion::VersionTranslator vt;
  boost::optional<model::Component> comp_ = vt.loadComponent(p);
  ASSERT_TRUE(comp_) << "Failed to load Component " << p;

  openstudio::path outPath = resourcesPath() / toPath("osversion/3_5_1/test_VRF_Terminal_v340_updated.osc");
  comp_->save(outPath, true);

  // Ori OSC has ONE TableMultiVariableLookup object.
  // We change that to 1 TableLookup, 1 ModelObjectList (=TableIndependentVariableList) and 1 TableIndependentVariable
  auto compData = comp_->componentData();
  EXPECT_EQ(18, compData.numComponentObjects());
  EXPECT_EQ("OS:ZoneHVAC:TerminalUnit:VariableRefrigerantFlow", compData.primaryComponentObject().iddObject().name());

  Model model;
  OptionalComponentData ocd = model.insertComponent(comp_.get());
  ASSERT_TRUE(ocd);
  EXPECT_EQ("OS:ZoneHVAC:TerminalUnit:VariableRefrigerantFlow", compData.primaryComponentObject().iddObject().name());
  EXPECT_EQ(18, ocd->numComponentObjects());
  EXPECT_EQ(19, model.numObjects());
  EXPECT_EQ("OS:ZoneHVAC:TerminalUnit:VariableRefrigerantFlow", ocd->primaryComponentObject().iddObject().name());
}

TEST_F(OSVersionFixture, update_3_5_1_to_3_6_0_GroundHeatExchangerHorizontalTrench) {
  openstudio::path path = resourcesPath() / toPath("osversion/3_6_0/test_vt_GroundHeatExchangerHorizontalTrench.osm");
  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(path);
  ASSERT_TRUE(model) << "Failed to load " << path;

  openstudio::path outPath = resourcesPath() / toPath("osversion/3_6_0/test_vt_GroundHeatExchangerHorizontalTrench_updated.osm");
  model->save(outPath, true);

  std::vector<WorkspaceObject> ghxs = model->getObjectsByType("OS:GroundHeatExchanger:HorizontalTrench");
  ASSERT_EQ(1u, ghxs.size());
  WorkspaceObject ghx = ghxs[0];

  EXPECT_EQ("Ground Heat Exchanger Horizontal Trench 1", ghx.getString(1).get());  // Name
  EXPECT_TRUE(ghx.isEmpty(2));                                                     // Inlet Node Name
  EXPECT_TRUE(ghx.isEmpty(3));                                                     // Outlet Node Name
  EXPECT_EQ(0.004, ghx.getDouble(4).get());                                        // Design Flow Rate
  EXPECT_EQ(75, ghx.getDouble(5).get());                                           // Trench Length in Pipe Axial Direction
  EXPECT_EQ(2, ghx.getInt(6).get());                                               // Number of Trenches
  EXPECT_EQ(2, ghx.getDouble(7).get());                                            // Horizontal Spacing Between Pipes
  EXPECT_EQ(0.016, ghx.getDouble(8).get());                                        // Pipe Inner Diameter
  EXPECT_EQ(0.02667, ghx.getDouble(9).get());                                      // Pipe Outer Diameter
  EXPECT_EQ(1.25, ghx.getDouble(10).get());                                        // Burial Depth
  EXPECT_EQ(1.08, ghx.getDouble(11).get());                                        // Soil Thermal Conductivity
  EXPECT_EQ(962, ghx.getDouble(12).get());                                         // Soil Density
  EXPECT_EQ(2576, ghx.getDouble(13).get());                                        // Soil Specific Heat
  EXPECT_EQ(0.3895, ghx.getDouble(14).get());                                      // Pipe Thermal Conductivity
  EXPECT_EQ(641, ghx.getDouble(15).get());                                         // Pipe Density
  EXPECT_EQ(2405, ghx.getDouble(16).get());                                        // Pipe Specific Heat
  EXPECT_EQ(30, ghx.getDouble(17).get());                                          // Soil Moisture Content Percent
  EXPECT_EQ(50, ghx.getDouble(18).get());                                          // Soil Moisture Content Percent at Saturation
  EXPECT_NE("", ghx.getString(19).get());                                          // Undisturbed Ground Temperature Model
  EXPECT_EQ(0.408, ghx.getDouble(20).get());                                       // Evapotranspiration Ground Cover Parameter

  std::vector<WorkspaceObject> ukas = model->getObjectsByType("OS:Site:GroundTemperature:Undisturbed:KusudaAchenbach");
  ASSERT_EQ(1u, ukas.size());

  ASSERT_TRUE(ghx.getTarget(19));
  WorkspaceObject uka = ghx.getTarget(19).get();
  EXPECT_EQ(uka.nameString(), ghx.getTarget(19)->nameString());
  EXPECT_EQ(IddObjectType(IddObjectType::OS_Site_GroundTemperature_Undisturbed_KusudaAchenbach), uka.iddObject().type());
  EXPECT_EQ("Site Ground Temperature Undisturbed Kusuda Achenbach 1", uka.getString(1).get());  // Name
  EXPECT_EQ(1.08, uka.getDouble(2).get());                                                      // Soil Thermal Conductivity
  EXPECT_EQ(962, uka.getDouble(3).get());                                                       // Soil Density
  EXPECT_EQ(2576, uka.getDouble(4).get());                                                      // Soil Specific Heat
  EXPECT_EQ(15.5, uka.getDouble(5).get());                                                      // Average Soil Surface Temperature
  EXPECT_EQ(12.8, uka.getDouble(6).get());                                                      // Average Amplitude of Surface Temperature
  EXPECT_EQ(17.3, uka.getDouble(7).get());                                                      // Phase Shift of Minimum Surface Temperature
}
