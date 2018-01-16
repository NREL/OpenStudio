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
#include "IddFixture.hpp"

#include "../IddFieldProperties.hpp"
#include "../IddKey.hpp"

#include "../../time/Time.hpp"


#include "../../core/String.hpp"
#include "../../core/StringStreamLogSink.hpp"
#include "../../core/Path.hpp"
#include "../../core/Containers.hpp"
#include "../../core/Compare.hpp"

#include <OpenStudio.hxx>
#include <resources.hxx>

#include <boost/algorithm/string.hpp>


#include <utilities/idd/IddEnums.hxx>

#include <algorithm>

using namespace std;
using namespace boost;
using namespace openstudio;

// this will call all sub tests, probably should break out into their own file
void testIddFile(const IddFile& iddFile);

// test idd file
TEST_F(IddFixture, EpIddFile)
{
  // from factory
  SCOPED_TRACE("EpIddFile");
  testIddFile(epIddFile);

  StringStreamLogSink ss;
  ss.setLogLevel(Debug);

  // from file
  path iddPath = resourcesPath()/toPath("energyplus/ProposedEnergy+.idd");
  openstudio::filesystem::ifstream inFile(iddPath); ASSERT_TRUE(inFile?true:false);
  OptionalIddFile loadedIddFile = IddFile::load(inFile);
  ASSERT_TRUE(loadedIddFile); inFile.close();

  for (auto iddObject : loadedIddFile->objects()){
    for (auto iddField : iddObject.nonextensibleFields()){
      iddField.properties();
    }
    for (auto iddField : iddObject.extensibleGroup()){
      iddField.properties();
    }
  }

  EXPECT_EQ(0, ss.logMessages().size());
  for (auto logMessage : ss.logMessages()){
    EXPECT_EQ("", logMessage.logMessage());
  }

  EXPECT_EQ("8.8.0",loadedIddFile->version());
  EXPECT_EQ(epIddFile.objects().size(),loadedIddFile->objects().size());
  if (epIddFile.objects().size() != loadedIddFile->objects().size()) {
    // get sets of IddObjectType
    IddObjectTypeSet epIddObjectTypes, loadedIddObjectTypes, diff;
    for (const IddObject& iddObject : epIddFile.objects()) {
      EXPECT_TRUE(iddObject.type() != IddObjectType::UserCustom);
      epIddObjectTypes.insert(iddObject.type());
    }
    for (const IddObject& iddObject : loadedIddFile->objects()) {
      if (iddObject.type() == IddObjectType::UserCustom) {
        try {
          IddObjectType iddObjectType(iddObject.name());
          loadedIddObjectTypes.insert(iddObjectType);
        }
        catch (...) {
          LOG(Debug,"Unable to convert IddObject name '" << iddObject.name() << "' "
              << "to IddObjectType.");
        }
      }
      else {
        loadedIddObjectTypes.insert(iddObject.type());
      }
    }
    std::set_difference(epIddObjectTypes.begin(),epIddObjectTypes.end(),
                        loadedIddObjectTypes.begin(),loadedIddObjectTypes.end(),
                        std::inserter(diff,diff.begin()));
    std::stringstream ss;
    for (const IddObjectType& iddType : diff) {
      ss << "  " << iddType << std::endl;
    }
    diff.clear();
    LOG(Debug,"The following object types are in epIddFile, but are not in loadedIddFile: "
        << std::endl << ss.str());
    ss.str("");
    std::set_difference(loadedIddObjectTypes.begin(),loadedIddObjectTypes.end(),
                        epIddObjectTypes.begin(),epIddObjectTypes.end(),
                        std::inserter(diff,diff.begin()));
    for (const IddObjectType& iddType : diff) {
      ss << "  " << iddType << std::endl;
    }
    LOG(Debug,"The following object types are in loadedIddFile, but are not in epIddFile: "
        << std::endl << ss.str());
  }

  testIddFile(*loadedIddFile);
}

// test idd file
TEST_F(IddFixture, OSIddFile)
{
  // DLM: it is necessary to build openstudio_model_resources to update the OpenStudio.idd that this test works on
  // I did not want to make utilities tests depend on openstudio_model_resources in cmake but that is an option

  StringStreamLogSink ss;
  ss.setLogLevel(Debug);

  // from file
  path iddPath = resourcesPath() / toPath("model/OpenStudio.idd");
  openstudio::filesystem::ifstream inFile(iddPath); ASSERT_TRUE(inFile ? true : false);
  OptionalIddFile loadedIddFile = IddFile::load(inFile);
  ASSERT_TRUE(loadedIddFile); inFile.close();

  for (auto iddObject : loadedIddFile->objects()){
    for (auto iddField : iddObject.nonextensibleFields()){
      iddField.properties();
    }
    for (auto iddField : iddObject.extensibleGroup()){
      iddField.properties();
    }
  }

  EXPECT_EQ(0, ss.logMessages().size());
  for (auto logMessage : ss.logMessages()){
    EXPECT_EQ("", logMessage.logMessage());
  }

  EXPECT_EQ(openStudioVersion(), loadedIddFile->version());
  EXPECT_EQ(osIddFile.objects().size(), loadedIddFile->objects().size());
  if (osIddFile.objects().size() != loadedIddFile->objects().size()) {
    // get sets of IddObjectType
    IddObjectTypeSet osIddObjectTypes, loadedIddObjectTypes, diff;
    for (const IddObject& iddObject : osIddFile.objects()) {
      EXPECT_TRUE(iddObject.type() != IddObjectType::UserCustom);
      osIddObjectTypes.insert(iddObject.type());
    }
    for (const IddObject& iddObject : loadedIddFile->objects()) {
      if (iddObject.type() == IddObjectType::UserCustom) {
        try {
          IddObjectType iddObjectType(iddObject.name());
          loadedIddObjectTypes.insert(iddObjectType);
        } catch (...) {
          LOG(Debug, "Unable to convert IddObject name '" << iddObject.name() << "' "
              << "to IddObjectType.");
        }
      } else {
        loadedIddObjectTypes.insert(iddObject.type());
      }
    }
    std::set_difference(osIddObjectTypes.begin(), osIddObjectTypes.end(),
                        loadedIddObjectTypes.begin(), loadedIddObjectTypes.end(),
                        std::inserter(diff, diff.begin()));
    std::stringstream ss;
    for (const IddObjectType& iddType : diff) {
      ss << "  " << iddType << std::endl;
    }
    diff.clear();
    LOG(Debug, "The following object types are in osIddFile, but are not in loadedIddFile: "
        << std::endl << ss.str());
    ss.str("");
    std::set_difference(loadedIddObjectTypes.begin(), loadedIddObjectTypes.end(),
                        osIddObjectTypes.begin(), osIddObjectTypes.end(),
                        std::inserter(diff, diff.begin()));
    for (const IddObjectType& iddType : diff) {
      ss << "  " << iddType << std::endl;
    }
    LOG(Debug, "The following object types are in loadedIddFile, but are not in osIddFile: "
        << std::endl << ss.str());
  }
}

// test single object, no fields
void IddFile_LeadInput(const IddFile& iddFile)
{
  string objectName("Lead Input");
  OptionalIddObject object = iddFile.getObject(objectName);
  ASSERT_TRUE(object);
  EXPECT_EQ(object->name(), objectName);
  EXPECT_TRUE(object->nonextensibleFields().empty() );
}

// test single object, one alpha field, no keys
void IddFile_Version(const IddFile& iddFile)
{
  string objectName("Version");
  OptionalIddObject object = iddFile.getObject(objectName);
  ASSERT_TRUE(object);
  EXPECT_EQ(object->name(), objectName);
  EXPECT_EQ(static_cast<unsigned int>(1),object->nonextensibleFields().size());

  string fieldName("Version Identifier");
  OptionalIddField field = object->getField(fieldName);
  ASSERT_TRUE(field);
  EXPECT_EQ(field->name(), fieldName);
  EXPECT_TRUE(field->keys().empty());
}

// test single object, 7 fields, 5 keys on field 3
void IddFile_Building(const IddFile& iddFile)
{
  string objectName("Building");
  OptionalIddObject object = iddFile.getObject(objectName);
  ASSERT_TRUE(object);
  EXPECT_EQ(object->name(), objectName);
  EXPECT_EQ(static_cast<unsigned int>(8),object->nonextensibleFields().size());
  EXPECT_TRUE(object->properties().unique);
  EXPECT_TRUE(object->properties().required);

  string fieldName("Terrain");
  OptionalIddField field = object->getField(fieldName);
  ASSERT_TRUE(field);
  EXPECT_EQ(field->name(), fieldName);
  EXPECT_EQ(static_cast<unsigned int>(5),field->keys().size());
  ASSERT_TRUE(field->properties().stringDefault);
  EXPECT_EQ(*(field->properties().stringDefault), "Suburbs");
  EXPECT_FALSE(field->properties().numericDefault);

  string keyName("Suburbs");
  OptionalIddKey key = field->getKey(keyName);
  EXPECT_TRUE(key);
  EXPECT_EQ(key->name(), keyName);
}

// case insensitive test
void IddFile_bUiLdInG(const IddFile& iddFile)
{
  string objectName("bUiLdInG");
  OptionalIddObject object = iddFile.getObject(objectName);
  ASSERT_TRUE(object);
  EXPECT_TRUE(iequals(object->name(), objectName));
  EXPECT_EQ(static_cast<unsigned int>(8),object->nonextensibleFields().size());
  EXPECT_TRUE(object->properties().unique);
  EXPECT_TRUE(object->properties().required);

  string fieldName("TeRrAiN");
  OptionalIddField field = object->getField(fieldName);
  ASSERT_TRUE(field);
  EXPECT_TRUE(iequals(field->name(), fieldName));
  EXPECT_EQ(field->keys().size(), static_cast<unsigned int>(5) );
  EXPECT_TRUE(field->properties().stringDefault);
  EXPECT_EQ(*(field->properties().stringDefault), "Suburbs");
  EXPECT_FALSE(field->properties().numericDefault);

  string keyName("SuBuRbS");
  OptionalIddKey key = field->getKey(keyName);
  ASSERT_TRUE(key);
  EXPECT_FALSE(equals(key->name(), keyName));
  EXPECT_TRUE(iequals(key->name(), keyName));
}

// extensible object test, BuildingSurface:Detailed
void IddFile_BuildingSurfaceDetailed(const IddFile& iddFile)
{
  string objectName("BuildingSurface:Detailed");
  OptionalIddObject object = iddFile.getObject(objectName);
  ASSERT_TRUE(object);
  EXPECT_TRUE(iequals(object->name(), objectName));
  ASSERT_TRUE(object->nonextensibleFields().size() > 0);
  EXPECT_EQ(object->nonextensibleFields().size(), static_cast<unsigned int>(10));
  EXPECT_FALSE(object->properties().format.empty());
  EXPECT_TRUE(iequals("vertices", object->properties().format));

  EXPECT_TRUE(object->properties().extensible);
  EXPECT_TRUE(object->properties().numExtensible);
  ASSERT_EQ(static_cast<unsigned int>(3), object->properties().numExtensible);

  EXPECT_FALSE(object->getField("Vertex 1 X-coordinate"));
  EXPECT_FALSE(object->getField("Vertex 1 X-coordinate"));

  ASSERT_TRUE(object->extensibleGroup().size() > 0);
  string fieldName("Vertex X-coordinate");
  EXPECT_TRUE(iequals(fieldName, object->extensibleGroup().front().name() ));
  OptionalIddField field = object->getField(fieldName);
  ASSERT_TRUE(field);
  EXPECT_TRUE(iequals(fieldName, field->name()));
  EXPECT_TRUE(field->properties().beginExtensible);

  fieldName = "Vertex Z-coordinate";
  EXPECT_TRUE(iequals(fieldName, object->extensibleGroup().back().name() ));
  field = object->getField(fieldName);
  ASSERT_TRUE(field);
  EXPECT_TRUE(iequals(fieldName, field->name()));
  EXPECT_FALSE(field->properties().beginExtensible);
}

// call all test functions, keep as last function
void testIddFile(const IddFile& iddFile)
{
  IddFile_LeadInput(iddFile);
  IddFile_Version(iddFile);
  IddFile_Building(iddFile);
  IddFile_bUiLdInG(iddFile);
  IddFile_BuildingSurfaceDetailed(iddFile);
}

TEST_F(IddFixture, IddFile_EpAllReferencesHaveNames)
{
  for (const IddObject& object : epIddFile.objects()){
    if (!object.references().empty()){
      if (object.nonextensibleFields().size() == 0) {
        LOG(Debug,"IddObject " << object.name() << " has references, but no fields.");
      }
      if (object.nonextensibleFields().size() > 0) {
        // ETH@20100319 Would be nice if \references conformed to this convention, but not getting
        // fixed in EnergyPlus 5.0. Removing failed tests, but retaining log messages.
        // EXPECT_EQ("Name", object.fields().front().name());
        if (object.nonextensibleFields().front().name() != "Name") {
          LOG(Debug,"IddObject " << object.name() << " has a reference field, but its first field is "
              << object.nonextensibleFields().front().name() << ".");
        }
      }
    }
  }
}

TEST_F(IddFixture, IddFile_EpMinFields) {
  for (const IddObject& object : epIddFile.objects()) {
    if (object.properties().minFields == 0) {
      IddFieldVector fields = object.nonextensibleFields();
      unsigned setTo = 0;
      for (unsigned i = 0, n = fields.size(); i < n; ++i) {
        if (fields[i].properties().required) { setTo = i; }
      }
      if (setTo > 0) {
          LOG(Debug,"IddObject " << object.name() << " has no min-fields property, but its last "
              << "required field is " << fields[setTo].name() << ".");
      }
      fields = object.extensibleGroup();
      for (const IddField& field : fields) {
        if (field.properties().required) {
          LOG(Debug,"IddObject " << object.name() << " has a required extensible field.");
        }
        break;
      }
    }
  }
}

TEST_F(IddFixture, IddFile_EpGroups) {
  StringVector groups = epIddFile.groups();
  EXPECT_TRUE(groups.size() > 0);
  EXPECT_EQ("",groups[0]);
  std::stringstream ss;
  // uniqueness
  for (StringVector::const_iterator it = groups.begin(), itEnd = groups.end(); it != itEnd; ++it) {
    auto loc = std::find_if(it+1,itEnd,std::bind(istringEqual,*it,std::placeholders::_1));
    EXPECT_TRUE(loc == itEnd);
    if (loc != itEnd) {
      LOG(Debug,"The group name '" << *it << "' is repeated in epIddFile.");
    }
    ss << "  " << *it << std::endl;
  }
  // log groups
  LOG(Info,"The current EnergyPlus IddFile contains the following " << groups.size()
      << " object groups, including the first, unnamed group: " << std::endl << ss.str());
}

