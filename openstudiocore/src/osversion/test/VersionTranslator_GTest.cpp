/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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

#include "../../utilities/bcl/RemoteBCL.hpp"
#include "../../utilities/bcl/LocalBCL.hpp"
#include "../../utilities/bcl/BCLComponent.hpp"

#include "../../utilities/idf/IdfObject.hpp"
#include <utilities/idd/OS_Version_FieldEnums.hxx>

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

      QString stem = toQString(it->path().stem()).replace("_", ".");
      VersionString vs(toString(stem));
      if (vs.major() == major && vs.minor() == minor){

        // run version translator on each example.osm
        openstudio::path modelPath = it->path() / toPath("example.osm");
        model::OptionalModel result = translator.loadModel(modelPath);
        EXPECT_TRUE(result);
        // check warnings, errors, affected objects
        EXPECT_TRUE(translator.errors().empty());
        EXPECT_TRUE(translator.warnings().empty());
        // print info for visual inspection
        ASSERT_TRUE(result);
        LOG_FREE(Debug, "OSVersionFixture", "Updated '" << toString(modelPath) << "' to OpenStudio Version "
                 << result->version().str() << ".");
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
        for (const IdfObjectPair& p : translator.refactoredObjects()) {
          LOG_FREE(Debug, "OSVersionFixture", p.first << "replaced with" << std::endl << std::endl << p.second);
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

      QString stem = toQString(it->path().stem()).replace("_", ".");
      VersionString vs(toString(stem));
      if (vs.major() == major && vs.minor() == minor){

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
        LOG_FREE(Debug, "OSVersionFixture", "Updated '" << toString(componentPath) << "' to OpenStudio Version "
                 << result->version().str() << ".");
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
        for (const IdfObjectPair& p : translator.refactoredObjects()) {
          LOG_FREE(Debug, "OSVersionFixture", p.first << "replaced with" << std::endl << std::endl << p.second);
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
TEST_F(OSVersionFixture,VersionTranslator_FutureVersion_ExampleModel) {
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
  VersionString nextPatch(major,minor,*patch+1);
  EXPECT_TRUE(vs.isNextVersion(nextPatch));
  EXPECT_TRUE(version->setString(OS_VersionFields::VersionIdentifier, nextPatch.str()));
  ss.str("");
  ss << model;
  m2 = translator.loadModel(ss);
  EXPECT_TRUE(m2);

  // increment minor
  VersionString nextMinor(major,minor+1,0);
  EXPECT_TRUE(vs.isNextVersion(nextMinor));
  EXPECT_TRUE(version->setString(OS_VersionFields::VersionIdentifier, nextMinor.str()));
  ss.str("");
  ss << model;
  m2 = translator.loadModel(ss);
  EXPECT_TRUE(m2);

  // increment major
  VersionString nextMajor(major+1,0,0);
  EXPECT_TRUE(version->setString(OS_VersionFields::VersionIdentifier, nextMajor.str()));
  ss.str("");
  ss << model;
  m2 = translator.loadModel(ss);
  EXPECT_TRUE(m2);

  // too far ahead
  VersionString aStepTooFar(major,minor+1,1);
  EXPECT_TRUE(version->setString(OS_VersionFields::VersionIdentifier, aStepTooFar.str()));
  ss.str("");
  ss << model;
  m2 = translator.loadModel(ss);
  EXPECT_FALSE(m2);

  aStepTooFar = VersionString(major+1,1,0);
  EXPECT_TRUE(version->setString(OS_VersionFields::VersionIdentifier, aStepTooFar.str()));
  ss.str("");
  ss << model;
  m2 = translator.loadModel(ss);
  EXPECT_FALSE(m2);

  aStepTooFar = VersionString(major+1,4);
  EXPECT_TRUE(version->setString(OS_VersionFields::VersionIdentifier, aStepTooFar.str()));
  ss.str("");
  ss << model;
  m2 = translator.loadModel(ss);
  EXPECT_FALSE(m2);
}

TEST_F(OSVersionFixture,VersionTranslator_FutureVersion_ExampleModel2) {
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
  VersionString nextPatch(major,minor,*patch+1);
  EXPECT_TRUE(vs.isNextVersion(nextPatch));
  EXPECT_TRUE(version->setString(OS_VersionFields::VersionIdentifier, nextPatch.str()));
  ss.str("");
  ss << model;
  m2 = translator.loadModel(ss);
  EXPECT_FALSE(m2);

  // increment minor
  VersionString nextMinor(major,minor+1,0);
  EXPECT_TRUE(vs.isNextVersion(nextMinor));
  EXPECT_TRUE(version->setString(OS_VersionFields::VersionIdentifier, nextMinor.str()));
  ss.str("");
  ss << model;
  m2 = translator.loadModel(ss);
  EXPECT_FALSE(m2);

  // increment major
  VersionString nextMajor(major+1,0,0);
  EXPECT_TRUE(version->setString(OS_VersionFields::VersionIdentifier, nextMajor.str()));
  ss.str("");
  ss << model;
  m2 = translator.loadModel(ss);
  EXPECT_FALSE(m2);

  // too far ahead
  VersionString aStepTooFar(major,minor+1,1);
  EXPECT_TRUE(version->setString(OS_VersionFields::VersionIdentifier, aStepTooFar.str()));
  ss.str("");
  ss << model;
  m2 = translator.loadModel(ss);
  EXPECT_FALSE(m2);

  aStepTooFar = VersionString(major+1,1,0);
  EXPECT_TRUE(version->setString(OS_VersionFields::VersionIdentifier, aStepTooFar.str()));
  ss.str("");
  ss << model;
  m2 = translator.loadModel(ss);
  EXPECT_FALSE(m2);

  aStepTooFar = VersionString(major+1,4);
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
  ASSERT_TRUE(model);

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