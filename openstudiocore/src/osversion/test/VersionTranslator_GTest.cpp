/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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
#include "../../utilities/bcl/OnDemandGenerator.hpp"
#include "../../utilities/bcl/BCLComponent.hpp"

#include "../../utilities/idf/IdfObject.hpp"
#include <utilities/idd/OS_Version_FieldEnums.hxx>

#include "../../utilities/core/Compare.hpp"

#include <boost/filesystem.hpp>

#include <resources.hxx>
#include <OpenStudio.hxx>

using namespace openstudio;
using namespace model;
using namespace osversion;

TEST_F(OSVersionFixture,VersionTranslator_ExampleModel) {
  osversion::VersionTranslator translator;

  // iterate through osversion subfolders
  openstudio::path resources = resourcesPath() / toPath("osversion");
  for (boost::filesystem::directory_iterator it(resources); it != boost::filesystem::directory_iterator(); ++it) {
    if (boost::filesystem::is_directory(it->status())) {
      // run version translator on each example.osm
      openstudio::path modelPath = it->path() / toPath("example.osm");
      model::OptionalModel result = translator.loadModel(modelPath);
      EXPECT_TRUE(result);
      // check warnings, errors, affected objects
      EXPECT_TRUE(translator.errors().empty());
      EXPECT_TRUE(translator.warnings().empty());
      // print info for visual inspection
      ASSERT_TRUE(result);
      LOG(Debug,"Updated '" << toString(modelPath) << "' to OpenStudio Version "
          << result->version().str() << ".");
      LOG(Debug,"Deprecated objects: ");
      for (const IdfObject& object : translator.deprecatedObjects()) {
        LOG(Debug,object);
      }
      LOG(Debug,"Untranslated objects: ");
      for (const IdfObject& object : translator.untranslatedObjects()) {
        LOG(Debug,object);
      }
      LOG(Debug,"New objects: ");
      for (const IdfObject& object : translator.newObjects()) {
        LOG(Debug,object);
      }
      LOG(Debug,"Refactored objects: ");
      for (const IdfObjectPair& p : translator.refactoredObjects()) {
        LOG(Debug,p.first << "replaced with" << std::endl << std::endl << p.second);
      }
      // make sure save and load is ok
      modelPath = it->path() / toPath("example_updated.osm");
      result->save(modelPath,true);
      result = model::Model::load(modelPath);
      EXPECT_TRUE(result);
    }
  }
}

TEST_F(OSVersionFixture,VersionTranslator_ExampleComponent) {
  osversion::VersionTranslator translator;

  // iterate through osversion subfolders
  openstudio::path resources = resourcesPath() / toPath("osversion");
  for (boost::filesystem::directory_iterator it(resources); it != boost::filesystem::directory_iterator(); ++it) {
    if (boost::filesystem::is_directory(it->status())) {
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
      LOG(Debug,"Updated '" << toString(componentPath) << "' to OpenStudio Version "
          << result->version().str() << ".");
      LOG(Debug,"Deprecated objects: ");
      for (const IdfObject& object : translator.deprecatedObjects()) {
        LOG(Debug,object);
      }
      LOG(Debug,"Untranslated objects: ");
      for (const IdfObject& object : translator.untranslatedObjects()) {
        LOG(Debug,object);
      }
      LOG(Debug,"New objects: ");
      for (const IdfObject& object : translator.newObjects()) {
        LOG(Debug,object);
      }
      LOG(Debug,"Refactored objects: ");
      for (const IdfObjectPair& p : translator.refactoredObjects()) {
        LOG(Debug,p.first << "replaced with" << std::endl  << std::endl << p.second);
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
      result->save(componentPath,true);
      result = model::Component::load(componentPath);
      EXPECT_TRUE(result);
    }
  }
}

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

TEST_F(OSVersionFixture,VersionTranslator_AllDefaultObjects) {
  // iterate through osversion subfolders
  osversion::VersionTranslator translator;

  // iterate through osversion subfolders
  openstudio::path resources = resourcesPath() / toPath("osversion");
  for (boost::filesystem::directory_iterator it(resources); it != boost::filesystem::directory_iterator(); ++it) {
    if (boost::filesystem::is_directory(it->status())) {
      // load each IddFile and create an IdfFile containing one of each of its objects
      openstudio::path iddPath = it->path() / toPath("OpenStudio.idd");
      IddFile iddFile = IddFile::load(iddPath).get();
      IdfFile idfFile(iddFile);
      for (const IddObject& iddObj : iddFile.objects()) {
        if (iddObj.name() != "OS:ComponentData"){
          idfFile.addObject(IdfObject(iddObj));
        }
      }
      openstudio::path modelPath = toPath("temp.osm");
      idfFile.save(modelPath,true);
      // run version translator
      model::OptionalModel result = translator.loadModel(modelPath);
      EXPECT_TRUE(result);

      // number of warnings varies. to check, need to compose starting version from it->path().
      // does not seem worth the effort at the moment, log messages already in log
      //EXPECT_TRUE(translator.warnings().empty());

      // check warnings, errors, affected objects, log messages already in log
      EXPECT_TRUE(translator.errors().empty());

      // print info for visual inspection
      ASSERT_TRUE(result);
      LOG(Debug,"Updated '" << toString(modelPath) << "' to OpenStudio Version "
          << result->version().str() << ".");
      LOG(Debug,"Deprecated objects: ");
      for (const IdfObject& object : translator.deprecatedObjects()) {
        LOG(Debug,object);
      }
      LOG(Debug,"Untranslated objects: ");
      for (const IdfObject& object : translator.untranslatedObjects()) {
        LOG(Debug,object);
      }
      LOG(Debug,"New objects: ");
      for (const IdfObject& object : translator.newObjects()) {
        LOG(Debug,object);
      }
      LOG(Debug,"Refactored objects: ");
      for (const IdfObjectPair& p : translator.refactoredObjects()) {
        LOG(Debug,p.first << "replaced with" << std::endl << std::endl << p.second);
      }
    }
  }
}

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
    ASSERT_FALSE(material.attributeNames().empty());
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

  ASSERT_TRUE(boost::filesystem::exists(path));

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

TEST_F(OSVersionFixture,OnDemandComponent) {
  RemoteBCL remoteBCL;
  bool ok = remoteBCL.downloadOnDemandGenerator("bb8aa6a0-6a25-012f-9521-00ff10704b07");
  EXPECT_TRUE(ok);
  boost::optional<OnDemandGenerator> oGenerator = remoteBCL.waitForOnDemandGenerator();
  ASSERT_TRUE(oGenerator);
  OnDemandGenerator generator = *oGenerator;

  generator.setArgumentValue("NREL_reference_building_vintage","ASHRAE_90.1-2004");
  generator.setArgumentValue("Climate_zone","ClimateZone 1-8");
  generator.setArgumentValue("NREL_reference_building_primary_space_type","SmallOffice");
  generator.setArgumentValue("NREL_reference_building_secondary_space_type","WholeBuilding");

  boost::optional<BCLComponent> oBCLComponent = LocalBCL::instance().getOnDemandComponent(generator);
  if (!oBCLComponent) {
    oBCLComponent = remoteBCL.getOnDemandComponent(generator);
  }
  ASSERT_TRUE(oBCLComponent);
  BCLComponent bclComponent = *oBCLComponent;

  StringVector fileStrs = bclComponent.files("osc");
  ASSERT_FALSE(fileStrs.empty());
  openstudio::path oscPath = toPath(fileStrs[0]);

  VersionTranslator translator;
  OptionalComponent oComponent = translator.loadComponent(oscPath);
  ASSERT_TRUE(oComponent);
  Component component = *oComponent;
  EXPECT_EQ(IddObjectType("OS:SpaceType"),component.primaryObject().iddObjectType());
}
